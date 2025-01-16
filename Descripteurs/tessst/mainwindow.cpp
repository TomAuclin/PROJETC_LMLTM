#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bibliowindow.h"
#include "connexionwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QInputDialog>
#include <QScreen>
#include <QTextStream>


// ----------------------------------------------------------------------------------------------

// ************************ mise en place de la fenêtre ************************

// ----------------------------------------------------------------------------------------------


MainWindow::MainWindow(const QString &login, const QString &imagePath, BiblioWindow *parentBiblio, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sceneImage(new QGraphicsScene(this)),
    seceneResultat(new QGraphicsScene(this)),
    imageObj(nullptr),
    biblioWindow(parentBiblio),
    LoginUtilisateur(login),
    selectedImagePath(imagePath) // Initialisation du chemin d'image
{
    ui->setupUi(this);
    setWindowTitle("Fenetre de traitement");
    resize(1200, 600);

    // Centrer la fenêtre sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

    // Initialiser les scènes
    ui->AfficherImage->setScene(sceneImage);
    ui->AffichageResultat->setScene(seceneResultat);

    // Charger l'image au démarrage
    if (!selectedImagePath.isEmpty()) {
        loadAndDisplayImage(selectedImagePath);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    delete imageObj;
}

// ----------------------------------------------------------------------------------------------

// ************************ Chargement d'un image  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::loadAndDisplayImage(const QString &fileName)
{
    // Obtenir les dimensions de la fenêtre d'affichage (QGraphicsView)
    QSize viewSize = ui->AfficherImage->viewport()->size();

    if (!fileName.isEmpty()) {
        try {
            // Réinitialiser les scènes
            sceneImage->clear();
            seceneResultat->clear();

            // Libérer l'objet image précédent
            delete imageObj;
            imageObj = nullptr;

            QImage img(fileName);
            if (img.isNull()) {
                throw std::runtime_error("L'image n'a pas pu être chargée.");
            }

            // Charger et traiter l'image
            if (img.format() == QImage::Format_Grayscale8) {
                std::vector<std::vector<uint8_t>> imageGris;
                for (int y = 0; y < img.height(); ++y) {
                    std::vector<uint8_t> row;
                    for (int x = 0; x < img.width(); ++x) {
                        row.push_back(qGray(img.pixel(x, y)));
                    }
                    imageGris.push_back(row);
                }
                imageObj = new ImageGris(imageGris); // Charger comme image en niveaux de gris
            } else {
                std::vector<std::vector<std::array<uint8_t, 3>>> imageCouleur;
                for (int y = 0; y < img.height(); ++y) {
                    std::vector<std::array<uint8_t, 3>> row;
                    for (int x = 0; x < img.width(); ++x) {
                        QRgb pixel = img.pixel(x, y);
                        row.push_back({static_cast<uint8_t>(qRed(pixel)), static_cast<uint8_t>(qGreen(pixel)), static_cast<uint8_t>(qBlue(pixel))});
                    }
                    imageCouleur.push_back(row);
                }
                imageObj = new ImageCouleur(imageCouleur); // Charger comme image couleur
            }

            // Charger l'image dans sceneImage
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                // Définir la scène sur le QGraphicsView si ce n'est pas déjà fait
                ui->AfficherImage->setScene(sceneImage);

                // Redimensionner l'image pour correspondre à la taille de la fenêtre
                QPixmap scaledPixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Ajouter le pixmap redimensionné à la scène
                sceneImage->addPixmap(scaledPixmap);
                sceneImage->setSceneRect(scaledPixmap.rect()); // Met à jour le rectangle de la scène

                // Ajuster la vue pour s'assurer qu'elle correspond à la scène
                ui->AfficherImage->fitInView(sceneImage->sceneRect(), Qt::KeepAspectRatio);

                // Forcer une mise à jour immédiate de l'affichage
                ui->AfficherImage->update();
            }
        } catch (const std::exception &e) {
            QMessageBox::critical(this, tr("Erreur"), tr(e.what()));
        }
    }
}




// ----------------------------------------------------------------------------------------------

// ************************ Calucle de l'histogramme et affichage  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_CalculerHisto_clicked()
{
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Vérification si l'image est en niveaux de gris
    bool isGrayscale = dynamic_cast<ImageGris*>(imageObj) != nullptr;

    // Calculer l'histogramme
    try {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme);

        afficherHistogramme(histogramme); // Afficher l'histogramme combiné

        if (isGrayscale) {
            // Désactiver et masquer les boutons pour les canaux
            ui->Canal_R->setVisible(false);
            ui->Canal_V->setVisible(false);
            ui->Canal_B->setVisible(false);
        } else {
            // Activer et afficher les boutons pour les canaux
            ui->Canal_R->setVisible(true);
            ui->Canal_V->setVisible(true);
            ui->Canal_B->setVisible(true);
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Erreur"), tr(e.what()));
    }
}


void MainWindow::afficherHistogramme(int histogramme[256])
{
    seceneResultat->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    seceneResultat->addLine(margin, height - margin, width, height - margin, QPen(Qt::black));
    seceneResultat->addLine(margin, 0, margin, height - margin, QPen(Qt::black));

    for (int i = 0; i < 256; i += 32) {
        seceneResultat->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        seceneResultat->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    for (int i = 0; i < 256; ++i) {
        int barHeight = static_cast<int>(histogramme[i] * scaleFactor);
        seceneResultat->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::black));
    }

    ui->AffichageResultat->setScene(seceneResultat);
}

// Afichage d'une couleur pour chaque canneau
void MainWindow::afficherHistogrammeCanal(int histogramme[256], int canal)
{
    seceneResultat->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;

    // valeur maximale dans l'histogramme pour le redimensionnement
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    // axes de l'histogramme
    seceneResultat->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    seceneResultat->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // graduations pour les axes
    for (int i = 0; i < 256; i += 32) {
        seceneResultat->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        seceneResultat->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    //  les histogrammes pour chaque canal en fonction des cases cochées
    if (ui->Canal_R->isChecked()) {
        int histoRouge[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoRouge, 0); // Canal rouge
        QColor couleurRouge = Qt::red;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoRouge[i] * scaleFactor);
            seceneResultat->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurRouge), QBrush(couleurRouge));
        }
    }

    if (ui->Canal_V->isChecked()) {
        int histoVert[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoVert, 1); // Canal vert
        QColor couleurVert = Qt::green;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoVert[i] * scaleFactor);
            seceneResultat->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurVert), QBrush(couleurVert));
        }
    }

    if (ui->Canal_B->isChecked()) {
        int histoBleu[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoBleu, 2); // Canal bleu
        QColor couleurBleu = Qt::blue;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoBleu[i] * scaleFactor);
            seceneResultat->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurBleu), QBrush(couleurBleu));
        }
    }


    ui->AffichageResultat->setScene(seceneResultat);
}

void MainWindow::on_Canal_R_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 0);
        afficherHistogrammeCanal(histogramme, 0);
    }
}

void MainWindow::on_Canal_V_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 1);
        afficherHistogrammeCanal(histogramme, 1);
    }
}

void MainWindow::on_Canal_B_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 2);
        afficherHistogrammeCanal(histogramme, 2);
    }
}



// ----------------------------------------------------------------------------------------------

// ************************ Détection de contour ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_DetectionContour_clicked()
{
    // Vérification si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);


    cv::Mat imageMat;

    // Si l'image est en couleur
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {

        const auto& imageCouleur = img->getImageCouleur();

        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);

        // Remplissage de la matrice
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); //RGB
            }
        }
    }
    // Si l'image est en niveaux de gris
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {

        const auto& imageGris = img->getImageGris(); // tableau des pixels en niveaux de gris

        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    //Détection des contours
    Traitement traitement;
    cv::Mat contours = traitement.detectionContours(imageMat);

    if (contours.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des contours a échoué."));
        return;
    }

    // Conversion de l'image des contours pour l'afficher
    QImage imgContours(contours.data, contours.cols, contours.rows, contours.step, QImage::Format_Grayscale8);
    QPixmap pixmap = QPixmap::fromImage(imgContours);

    // Redimensionnement de l'image
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Création d' une nouvelle scène pour Affichage le Resultat
    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);

    // Affichage l'image des contours dans la vue AffichageResultat
    ui->AffichageResultat->setScene(sceneResultat);
}



// ----------------------------------------------------------------------------------------------

// ************************ Détection de Droite ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_DetectionDroite_clicked()
{
    // Vérification si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }
QMessageBox::information(this, tr("Information"), tr("Attention : Cette fonctionnalité est en cours de développement. Les résultats peuvent être imprécis."));
    cv::Mat imageMat;

    // Si l'image est en couleur
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]);
            }
        }
    }

    // Si l'image est en niveaux de gris
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        const auto& imageGris = img->getImageGris();
        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    // détection des droites
    Traitement traitement;
    cv::Mat imageAvecDroites = traitement.HoughDroite(imageMat);

    // si l'image des droites est vide
    if (imageAvecDroites.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des droites a échoué."));
        return;
    }


    QImage imgDroites;

    if (imageAvecDroites.channels() == 3) {
        // Conversion  pour les images en couleur
        cv::Mat imageRGB;
        cv::cvtColor(imageAvecDroites, imageRGB, cv::COLOR_BGR2RGB);
        imgDroites = QImage(imageRGB.data, imageRGB.cols, imageRGB.rows, imageRGB.step, QImage::Format_RGB888);
    }
    else if (imageAvecDroites.channels() == 1) {
        // Pour les images en niveaux de gris
        imgDroites = QImage(imageAvecDroites.data, imageAvecDroites.cols, imageAvecDroites.rows, imageAvecDroites.step, QImage::Format_Grayscale8);
    }


    QPixmap pixmap = QPixmap::fromImage(imgDroites);

    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);
    ui->AffichageResultat->setScene(sceneResultat);
}



// ----------------------------------------------------------------------------------------------

// ************************ Séparation par couleur ************************

// ----------------------------------------------------------------------------------------------
std::vector<int> MainWindow::getSelectedSegmentationCanaux() {
    std::vector<int> canaux;
    if (ui->Segmentation_Canal_R->isChecked()) {
        canaux.push_back(0); // Rouge
    }
    if (ui->Segmentation_Canal_V->isChecked()) {
        canaux.push_back(1); // Vert
    }
    if (ui->Segmentation_Canal_B->isChecked()) {
        canaux.push_back(2); // Bleu
    }
    return canaux;
}

void MainWindow::on_SegmenterCouleur_clicked() {
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Récupération des canaux sélectionnés
    std::vector<int> canaux = getSelectedSegmentationCanaux();

    if (canaux.empty() || canaux.size() > 2) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner un ou deux canaux."));
        return;
    }

    // Conversion de l'image en cv::Mat
    cv::Mat imageMat;
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]);
            }
        }
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("L'image doit être en couleur."));
        return;
    }

    // Appliquer la segmentation
    Traitement traitement;
    cv::Mat resultat = traitement.separationParCouleur(imageMat, canaux);

    if (resultat.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La segmentation a échoué."));
        return;
    }

    // Afficher le résultat
    QImage imgResultat(resultat.data, resultat.cols, resultat.rows, resultat.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(imgResultat);
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);
    ui->AffichageResultat->setScene(sceneResultat);

    ui->statusbar->showMessage("Segmentation par couleur appliquée.");
}

// ----------------------------------------------------------------------------------------------

// ************************ Rehaussement de contours ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_RehaussementContours_clicked()
{

    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    cv::Mat imageMat;


    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();

        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); // BGR
            }
        }
    }

    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        const auto& imageGris = img->getImageGris();


        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    //  rehaussement des contours
    Traitement traitement;
    cv::Mat imageRehaussee = traitement.rehaussementContours(imageMat);
    if (imageRehaussee.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le rehaussement des contours a échoué."));
        return;
    }


    QImage imgRehaussee(imageRehaussee.data, imageRehaussee.cols, imageRehaussee.rows, imageRehaussee.step, QImage::Format_Grayscale8);

    // Si l'image est en RGB
    if (imageRehaussee.channels() == 3) {
        imgRehaussee = QImage(imageRehaussee.data, imageRehaussee.cols, imageRehaussee.rows, imageRehaussee.step, QImage::Format_RGB888);
    }

    QPixmap pixmap = QPixmap::fromImage(imgRehaussee);
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);

    ui->AffichageResultat->setScene(sceneResultat);
}

// ----------------------------------------------------------------------------------------------

// ************************ Convolution ************************

// ----------------------------------------------------------------------------------------------
void MainWindow::on_AppliquerConvolution_clicked()
{
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Convertir l'image en cv::Mat
    cv::Mat imageMat;
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        // Image en couleur
        const auto& imageCouleur = img->getImageCouleur();
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[2], pixel[1], pixel[0]); // Convertir en BGR
            }
        }
    } else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        // Image en niveaux de gris
        const auto& imageGris = img->getImageGris();
        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Type d'image non pris en charge."));
        return;
    }

    // Appliquer la convolution
    Traitement traitement;
    cv::Mat resultat = traitement.convolution(imageMat);

    if (resultat.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La convolution a échoué."));
        return;
    }

    // Afficher le résultat
    QImage imgResultat;
    if (resultat.channels() == 3) {
        // Image en couleur
        imgResultat = QImage(resultat.data, resultat.cols, resultat.rows, resultat.step, QImage::Format_RGB888).rgbSwapped();
    } else if (resultat.channels() == 1) {
        // Image en niveaux de gris
        imgResultat = QImage(resultat.data, resultat.cols, resultat.rows, resultat.step, QImage::Format_Grayscale8);
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Type d'image non pris en charge pour l'affichage."));
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(imgResultat);
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);
    ui->AffichageResultat->setScene(sceneResultat);

    ui->statusbar->showMessage("Convolution appliquée.");
}

// -----------------------------------------------------------------------------------------

// *************************** Ajout Menu Descripteur *************************************

// -----------------------------------------------------------------------------------------
Library library;

// Ajouter

void MainWindow::on_actionAjouterDescripteur_triggered() {
    // 1. Ouvrir un QFileDialog pour sélectionner l'image d'origine
    QString cheminImageSource = QFileDialog::getOpenFileName(this,
                                                             "Sélectionnez une image à ajouter",
                                                             "",
                                                             "Images (*.png *.jpg *.jpeg *.bmp *.pgm *.CR2);;Tous les fichiers (*)");

    if (cheminImageSource.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucun fichier sélectionné !");
        return;
    }

    // 2. Demander les descripteurs à l'utilisateur
    bool ok;
    QString titre = QInputDialog::getText(this, "Ajouter un descripteur", "Titre de l'image :", QLineEdit::Normal, "", &ok);
    if (!ok || titre.isEmpty()) return;

    int numero = QInputDialog::getInt(this, "Ajouter un descripteur", "Numéro unique :", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    double prix = QInputDialog::getDouble(this, "Ajouter un descripteur", "Prix :", 0, 0, 10000, 2, &ok);
    if (!ok) return;

    QString acces = QInputDialog::getText(this, "Ajouter un descripteur", "Accès (O/L) :", QLineEdit::Normal, "O", &ok);
    if (!ok || acces.isEmpty()) return;

    QString type = QInputDialog::getText(this, "Ajouter un descripteur", "Type (couleur/gris) :", QLineEdit::Normal, "couleur", &ok);
    if (!ok || type.isEmpty()) return;

    int nbTraitement = QInputDialog::getInt(this, "Ajouter un descripteur", "Nombre de traitements possibles :", 1, 1, 100, 1, &ok);
    if (!ok) return;

    // 3. Vérifier l'unicité titre / numéro
    if (library.titrecheck(titre.toStdString()) != 0) {
        QMessageBox::warning(this, "Erreur", "Le titre existe déjà !");
        return;
    }
    if (library.numerocheck(numero) != 0) {
        QMessageBox::warning(this, "Erreur", "Le numéro existe déjà !");
        return;
    }

    // 4. Demander à l'utilisateur de choisir le nom du fichier de la nouvelle bibliothèque
    QString nouveauNomFichier = QInputDialog::getText(this, "Nom de la nouvelle bibliothèque", "Entrez le nom du nouveau fichier .txt :", QLineEdit::Normal, "nouvelle_bibliotheque.txt", &ok);
    if (!ok || nouveauNomFichier.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du fichier n'est pas valide !");
        return;
    }

    QString cheminBibliotheque = "/media/sf_PROJETC_LMLTM/Bibliotheque";
    QString cheminFichier = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst";
    QString cheminNouveauFichier = cheminFichier + "/" + nouveauNomFichier;

    // 5. Copier le fichier texte existant pour créer une nouvelle version
    QString cheminDescripteur = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    if (!QFile::copy(cheminDescripteur, cheminNouveauFichier)) {
        QMessageBox::warning(this, "Erreur", "La création de la nouvelle bibliothèque a échoué !");
        return;
    }

    // 6. Ajouter le nouveau descripteur au fichier texte
    QFile fichierDescripteur(cheminNouveauFichier);
    if (fichierDescripteur.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream sortie(&fichierDescripteur);
        sortie << titre << ", " << QFileInfo(cheminImageSource).fileName() << ", " << numero << ", " << prix << ", "
               << acces << ", " << type << ", " << nbTraitement << ", " << numero << "\n";
        fichierDescripteur.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier de la nouvelle bibliothèque !");
        return;
    }

    // 7. Copier l'image dans la bibliothèque avec le même nom que le titre
    QString extensionImage = QFileInfo(cheminImageSource).suffix();
    QString cheminImageDestination = cheminBibliotheque + "/" + titre + "." + extensionImage;
    if (!QFile::copy(cheminImageSource, cheminImageDestination)) {
        QMessageBox::warning(this, "Erreur", "La copie de l'image dans la bibliothèque a échoué !");
        return;
    }

    // 8. Ajouter l'objet Image à la bibliothèque
    Image nouvelleImage(cheminImageDestination.toStdString(),
                        titre.toStdString(),
                        numero,
                        prix,
                        acces.toStdString()[0],
                        type.toStdString(),
                        nbTraitement,
                        numero);

    library.ajouterDescripteurs(nouvelleImage);

    // 9. Confirmation
    QMessageBox::information(this, "Succès", "Le descripteur, l'image et la nouvelle bibliothèque ont été créés avec succès !");
}



// Modifier

void MainWindow::on_actionModifierDescripteur_triggered() {
    // 1. Définir le chemin fixe du dossier descripteurs
    QString cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst";

    // 2. Demander à l'utilisateur de choisir le fichier descripteurs à modifier
    QString cheminDescripteur = QInputDialog::getText(this, "Modifier un descripteur", "Entrez le nom du fichier descripteurs .txt :", QLineEdit::Normal, "nouvelle_bibliotheque.txt");
    QString cheminDescripteurComplet = cheminDescripteurs + "/" + cheminDescripteur;

    if (cheminDescripteur.isEmpty() || !QFile::exists(cheminDescripteurComplet)) {
        QMessageBox::warning(this, "Erreur", "Le fichier descripteurs spécifié est introuvable !");
        return;
    }

    // 3. Demander le titre du descripteur à modifier
    bool ok;
    QString titreRecherche = QInputDialog::getText(this, "Modifier un descripteur", "Entrez le titre du descripteur à modifier :", QLineEdit::Normal, "", &ok);
    if (!ok || titreRecherche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun titre spécifié !");
        return;
    }

    // 4. Charger le fichier et rechercher le descripteur à modifier
    QFile fichierDescripteur(cheminDescripteurComplet);
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier descripteurs.txt !");
        return;
    }

    QString contenu;
    QString ligneAModifier;
    QTextStream fluxLecture(&fichierDescripteur);
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine();
        if (ligne.startsWith(titreRecherche + ",")) {
            ligneAModifier = ligne;
        } else {
            contenu += ligne + "\n";
        }
    }
    fichierDescripteur.close();

    if (ligneAModifier.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce titre !");
        return;
    }

    // 5. Demander les nouvelles valeurs pour le descripteur
    QStringList elements = ligneAModifier.split(", ");
    if (elements.size() < 8) {
        QMessageBox::warning(this, "Erreur", "Le descripteur sélectionné est invalide !");
        return;
    }

    QString nouveauTitre = QInputDialog::getText(this, "Modifier un descripteur", "Nouveau titre :", QLineEdit::Normal, elements[0], &ok);
    if (!ok || nouveauTitre.isEmpty()) return;

    int nouveauNumero = QInputDialog::getInt(this, "Modifier un descripteur", "Nouveau numéro unique :", elements[2].toInt(), 0, 10000, 1, &ok);
    if (!ok) return;

    double nouveauPrix = QInputDialog::getDouble(this, "Modifier un descripteur", "Nouveau prix :", elements[3].toDouble(), 0, 10000, 2, &ok);
    if (!ok) return;

    QString nouvelAcces = QInputDialog::getText(this, "Modifier un descripteur", "Nouvel accès (O/L) :", QLineEdit::Normal, elements[4], &ok);
    if (!ok || nouvelAcces.isEmpty()) return;

    QString nouveauType = QInputDialog::getText(this, "Modifier un descripteur", "Nouveau type (couleur/gris) :", QLineEdit::Normal, elements[5], &ok);
    if (!ok || nouveauType.isEmpty()) return;

    int nouveauNbTraitement = QInputDialog::getInt(this, "Modifier un descripteur", "Nouveau nombre de traitements possibles :", elements[6].toInt(), 1, 100, 1, &ok);
    if (!ok) return;

    // 6. Construire la nouvelle ligne du descripteur
    QString nouvelleLigne = nouveauTitre + ", " + elements[1] + ", " + QString::number(nouveauNumero) + ", "
                            + QString::number(nouveauPrix) + ", " + nouvelAcces + ", " + nouveauType + ", "
                            + QString::number(nouveauNbTraitement) + ", " + QString::number(nouveauNumero) + "\n";

    contenu += nouvelleLigne;

    // 7. Écrire les modifications dans le fichier
    if (!fichierDescripteur.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier descripteurs.txt !");
        return;
    }

    QTextStream fluxEcriture(&fichierDescripteur);
    fluxEcriture << contenu;
    fichierDescripteur.close();

    // 8. Renommer l'image si le titre a été modifié
    QString cheminBibliotheque = "/media/sf_PROJETC_LMLTM/Bibliotheque";
    if (nouveauTitre != elements[0]) {
        QString extensionImage = QFileInfo(elements[1]).suffix();
        QString ancienCheminImage = cheminBibliotheque + "/" + elements[0] + "." + extensionImage;
        QString nouveauCheminImage = cheminBibliotheque + "/" + nouveauTitre + "." + extensionImage;

        if (QFile::exists(ancienCheminImage)) {
            if (!QFile::rename(ancienCheminImage, nouveauCheminImage)) {
                QMessageBox::warning(this, "Erreur", "Impossible de renommer l'image associée au descripteur !");
            }
        } else {
            QMessageBox::warning(this, "Erreur", "L'image associée au descripteur est introuvable !");
        }
    }

    // 9. Confirmation
    QMessageBox::information(this, "Succès", "Le descripteur a été modifié avec succès !");
}


// Supprimer

void MainWindow::on_actionSupprimerDescripteur_triggered() {
    // Demander le numéro du descripteur à supprimer
    bool ok;
    int numero = QInputDialog::getInt(this, "Supprimer un descripteur",
                                      "Entrez le numéro unique du descripteur :", 0, 0, 10000, 1, &ok);
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Suppression annulée.");
        return;
    }

    // Parcourir la liste pour trouver le descripteur correspondant
    auto current = library.head; // Accès à la liste chaînée depuis votre bibliothèque
    std::shared_ptr<Library::INode> previous = nullptr; // Pour maintenir une référence au noeud précédent
    bool descripteurTrouve = false;

    while (current) {
        if (current->data.getNumero() == numero) {
            descripteurTrouve = true;

            // Supprimer le fichier image associé
            std::string cheminImage = current->data.getSource(); // Récupérer le chemin de l'image sauvegardée
            try {
                if (std::filesystem::exists(cheminImage)) {
                    std::filesystem::remove(cheminImage);
                } else {
                    QMessageBox::warning(this, "Avertissement",
                                         "L'image associée au descripteur n'existe pas ou a déjà été supprimée.");
                }
            } catch (const std::exception &e) {
                QMessageBox::warning(this, "Erreur",
                                     QString("Impossible de supprimer l'image associée : %1").arg(e.what()));
            }

            // Supprimer le descripteur de la liste chaînée
            if (previous) {
                previous->next = current->next;
            } else {
                library.head = current->next;
            }

            QMessageBox::information(this, "Succès", "Le descripteur et son image associée ont été supprimés !");
            return;
        }

        previous = current;
        current = current->next;
    }

    if (!descripteurTrouve) {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce numéro !");
    }
}


// --------------------------------------------------------------------------------------------------

// ***************************** Afficher image **************************

// ----------------------------------------------------------------

void MainWindow::on_actionRechercherImage_triggered() {
    // Demander le numéro de l'image
    bool ok;
    int numero = QInputDialog::getInt(this, "Rechercher une image",
                                      "Entrez le numéro de l'image :", 0, 0, 10000, 1, &ok);
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Recherche annulée.");
        return;
    }

    // Appeler la méthode de recherche dans Library
    std::string resultat = library.rechercherImageParNumero(numero);

    // Afficher le résultat
    if (resultat == "Image non trouvée.") {
        QMessageBox::warning(this, "Résultat", "Aucune image trouvée avec ce numéro.");
    } else {
        QMessageBox::information(this, "Résultat", QString::fromStdString(resultat));
    }
}


void MainWindow::on_RetourVersBiblio_clicked()
{
    // Effacer les scènes pour préparer un nouvel affichage
    sceneImage->clear();
    seceneResultat->clear(); // Supprimer le contenu de AfficherResultat
    delete imageObj; // Libérer la mémoire utilisée pour l'image actuelle
    imageObj = nullptr;

    if (!biblioWindow) {
        // Créez une nouvelle instance si elle n'existe pas
        biblioWindow = std::make_unique<BiblioWindow>(LoginUtilisateur);
    }

    biblioWindow->show(); // Affiche la fenêtre Bibliothèque
    this->close();        // Ferme la fenêtre actuelle
}
