#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QInputDialog>


// ----------------------------------------------------------------------------------------------

// ************************ mise en place de la fenêtre ************************

// ----------------------------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sceneImage(new QGraphicsScene(this)),
    sceneHisto(new QGraphicsScene(this)),
    imageObj(nullptr) // Initialisation du pointeur image à null
{
    ui->setupUi(this);
    ui->AfficherImage->setScene(sceneImage);
    ui->AffichageResultat->setScene(sceneHisto);

    // Cacher les boutons de sélection des canaux par défaut
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageObj;
}


// ----------------------------------------------------------------------------------------------

// ************************ Chargement d'un image  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_ChargerImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Charger une image"), "", tr("Images (*.png *.jpg *.pgm *.CR2)"));

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    // Reset de l'ecran d'affichage
    if (ui->AffichageResultat->scene()) {
        ui->AffichageResultat->scene()->clear();
    }



    if (!fileName.isEmpty()) {
        // Charger l'image dans l'objet imageObj
        try {
            delete imageObj; // Libérer l'ancien objet imageObj, s'il existe
            QImage img(fileName);

            if (img.isNull()) {
                throw std::runtime_error("L'image n'a pas pu être chargée.");
            }

            // Vérifier si l'image est en niveaux de gris ou en couleur
            if (img.format() == QImage::Format_Grayscale8) {
                std::vector<std::vector<uint8_t>> imageGris;
                for (int y = 0; y < img.height(); ++y) {
                    std::vector<uint8_t> row;
                    for (int x = 0; x < img.width(); ++x) {
                        row.push_back(qGray(img.pixel(x, y)));
                    }
                    imageGris.push_back(row);
                }
                imageObj = new ImageGris(imageGris);  // Charger comme image en niveaux de gris
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
                imageObj = new ImageCouleur(imageCouleur);  // Charger comme image couleur
            }

            // Charger et afficher l'image dans l'interface
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                QSize viewSize = ui->AfficherImage->viewport()->size();
                pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                sceneImage->clear();
                sceneImage->addPixmap(pixmap);
                ui->AfficherImage->setScene(sceneImage);
            }
        } catch (const std::exception& e) {
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

    // Vérifier si l'image est en niveaux de gris
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
    sceneHisto->clear();
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

    sceneHisto->addLine(margin, height - margin, width, height - margin, QPen(Qt::black));
    sceneHisto->addLine(margin, 0, margin, height - margin, QPen(Qt::black));

    for (int i = 0; i < 256; i += 32) {
        sceneHisto->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        sceneHisto->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    for (int i = 0; i < 256; ++i) {
        int barHeight = static_cast<int>(histogramme[i] * scaleFactor);
        sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::black));
    }

    ui->AffichageResultat->setScene(sceneHisto);
}

// Afichage d'une couleur pour chaque canneau
void MainWindow::afficherHistogrammeCanal(int histogramme[256], int canal)
{
    sceneHisto->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;

    // Trouver la valeur maximale dans l'histogramme pour le redimensionnement
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

    // Dessiner les axes de l'histogramme
    sceneHisto->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    sceneHisto->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // Dessiner les graduations pour les axes
    for (int i = 0; i < 256; i += 32) {
        sceneHisto->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        sceneHisto->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    // Dessiner les histogrammes pour chaque canal en fonction des cases cochées
    if (ui->Canal_R->isChecked()) {
        int histoRouge[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoRouge, 0); // Canal rouge
        QColor couleurRouge = Qt::red;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoRouge[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurRouge), QBrush(couleurRouge));
        }
    }

    if (ui->Canal_V->isChecked()) {
        int histoVert[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoVert, 1); // Canal vert
        QColor couleurVert = Qt::green;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoVert[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurVert), QBrush(couleurVert));
        }
    }

    if (ui->Canal_B->isChecked()) {
        int histoBleu[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoBleu, 2); // Canal bleu
        QColor couleurBleu = Qt::blue;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoBleu[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurBleu), QBrush(couleurBleu));
        }
    }

    // Appliquer la scène mise à jour à l'affichage graphique
    ui->AffichageResultat->setScene(sceneHisto);
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
    // Vérifier si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    // Créer une cv::Mat pour stocker l'image en OpenCV
    cv::Mat imageMat;

    // Si l'image est en couleur (ImageCouleur)
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        // Accéder aux pixels de l'image couleur via l'accesseur
        const auto& imageCouleur = img->getImageCouleur(); // tableau des pixels (BGR)

        // Créer une Mat de OpenCV avec les dimensions de l'image
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);

        // Remplir le cv::Mat avec les données des pixels (BGR)
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); // BGR
            }
        }
    }
    // Si l'image est en niveaux de gris (ImageGris)
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        // Accéder aux pixels de l'image en niveaux de gris via l'accesseur
        const auto& imageGris = img->getImageGris(); // tableau des pixels en niveaux de gris

        // Créer une Mat de OpenCV avec les dimensions de l'image
        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);

        // Remplir le cv::Mat avec les données des pixels (niveaux de gris)
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    // Appliquer la détection des contours en utilisant la classe Traitement
    Traitement traitement;
    cv::Mat contours = traitement.detectionContours(imageMat); // Image des contours

    if (contours.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des contours a échoué."));
        return;
    }

    // Convertir l'image des contours en QImage pour l'afficher
    QImage imgContours(contours.data, contours.cols, contours.rows, contours.step, QImage::Format_Grayscale8);
    QPixmap pixmap = QPixmap::fromImage(imgContours);

    // Redimensionner l'image selon la taille de la vue
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Créer une nouvelle scène pour AffichageResultat
    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);

    // Afficher l'image des contours dans la vue AffichageResultat
    ui->AffichageResultat->setScene(sceneResultat);
}



// ----------------------------------------------------------------------------------------------

// ************************ Détection de Droite ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_DetectionDroite_clicked()
{
    // Vérifier si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    // Créer une cv::Mat pour stocker l'image en OpenCV
    cv::Mat imageMat;

    // Si l'image est en couleur (ImageCouleur)
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur(); // tableau des pixels (BGR)
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); // BGR
            }
        }
    }
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        const auto& imageGris = img->getImageGris(); // tableau des pixels en niveaux de gris
        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    // Appliquer la détection des droites
    Traitement traitement;
    cv::Mat imageAvecDroites = traitement.HoughDroite(imageMat);

    if (imageAvecDroites.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des droites a échoué."));
        return;
    }

    QImage imgDroites(imageAvecDroites.data, imageAvecDroites.cols, imageAvecDroites.rows, imageAvecDroites.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(imgDroites);

    // Redimensionner l'image selon la taille de la vue
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
