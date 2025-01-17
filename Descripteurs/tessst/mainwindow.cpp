// Inclusion des fichiers d'en-tête
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bibliowindow.h"
#include "connexionwindow.h"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QInputDialog>
#include <QScreen>
#include <QTextStream>
#include <QDebug>

// ----------------------------------------------------------------------------------------------

// ************************ mise en place de la fenêtre ************************

// ----------------------------------------------------------------------------------------------

// Constructeur de la classe MainWindow. Il initialise la fenêtre principale de l'application de traitement d'image.
MainWindow::MainWindow(const QString &login, const QString &imagePath, BiblioWindow *parentBiblio, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),                          // Initialisation de l'interface utilisateur
    sceneImage(new QGraphicsScene(this)),            // Création de la scène pour afficher l'image
    seceneResultat(new QGraphicsScene(this)),        // Création de la scène pour afficher les résultats du traitement
    imageObj(nullptr),                               // Initialisation du pointeur vers l'objet image (aucune image pour l'instant)
    biblioWindow(parentBiblio),                      // Initialisation du pointeur vers la fenêtre bibliothèque
    LoginUtilisateur(login),                        // Initialisation du login utilisateur
    selectedImagePath(imagePath)                     // Initialisation du chemin de l'image à afficher
{
    ui->setupUi(this);  // Setup de l'interface utilisateur avec le fichier .ui
    setWindowTitle("Fenetre de traitement");  // Définir le titre de la fenêtre
    resize(1200, 600);  // Définir la taille de la fenêtre principale

    // Centrage de la fenêtre sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

    // Initialisation des scènes pour afficher l'image et les résultats du traitement
    ui->AfficherImage->setScene(sceneImage);
    ui->AffichageResultat->setScene(seceneResultat);

    // Ajustement de la vue pour garantir une bonne mise en page de l'image
    sceneImage->setSceneRect(ui->AfficherImage->viewport()->rect());
    ui->AfficherImage->fitInView(sceneImage->sceneRect(), Qt::KeepAspectRatio);

    // Initialiser les canaux (R/V/B) comme invisibles au démarrage
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    // Si un chemin d'image a été fourni, charger et afficher l'image
    if (!selectedImagePath.isEmpty()) {
        loadAndDisplayImage(selectedImagePath);

        // Réajuster la vue après le chargement de la première image
        ui->AfficherImage->fitInView(sceneImage->sceneRect(), Qt::KeepAspectRatio);
    }
}

// Destructeur de la classe MainWindow. Il libère les ressources allouées pour l'interface et l'image.
MainWindow::~MainWindow()
{
    delete ui;           // Supprime l'interface utilisateur
    delete imageObj;     // Supprime l'objet image si nécessaire
}

// ----------------------------------------------------------------------------------------------

// ************************ Chargement d'un image  ************************

// ----------------------------------------------------------------------------------------------

// Fonction pour charger et afficher une image à partir d'un chemin de fichier donné.
void MainWindow::loadAndDisplayImage(const QString &fileName)
{
    // Si le nom de fichier est vide, ne rien faire et sortir de la fonction
    if (fileName.isEmpty()) {
        return;
    }

    try {
        // Nettoyage des scènes et objets existants pour préparer le nouvel affichage
        sceneImage->clear(); // Efface la scène d'image actuelle
        if (seceneResultat) {
            delete seceneResultat; // Libère l'ancienne scène de résultats
            seceneResultat = nullptr; // Assure que le pointeur est nul
        }
        seceneResultat = new QGraphicsScene(this); // Crée une nouvelle scène pour les résultats
        ui->AffichageResultat->setScene(seceneResultat); // Associe la nouvelle scène aux résultats

        if (imageObj) {
            delete imageObj; // Libère l'ancien objet image
            imageObj = nullptr; // Assure que le pointeur est nul
        }

        // Charger l'image à partir du fichier
        QImage img(fileName);
        if (img.isNull()) { // Si l'image ne peut pas être chargée, lever une exception
            throw std::runtime_error("L'image n'a pas pu être chargée.");
        }

        // Convertir l'image en fonction de son format
        if (img.format() == QImage::Format_Grayscale8) { // Si l'image est en niveaux de gris
            // Créer un vecteur bidimensionnel pour stocker l'image en niveaux de gris
            std::vector<std::vector<uint8_t>> imageGris;
            for (int y = 0; y < img.height(); ++y) {
                std::vector<uint8_t> row;
                for (int x = 0; x < img.width(); ++x) {
                    row.push_back(qGray(img.pixel(x, y))); // Récupérer la valeur en niveaux de gris du pixel
                }
                imageGris.push_back(row); // Ajouter la ligne à l'image en niveaux de gris
            }
            imageObj = new ImageGris(imageGris); // Créer un objet ImageGris
        } else { // Si l'image est en couleur
            // Créer un vecteur bidimensionnel pour stocker l'image couleur
            std::vector<std::vector<std::array<uint8_t, 3>>> imageCouleur;
            for (int y = 0; y < img.height(); ++y) {
                std::vector<std::array<uint8_t, 3>> row;
                for (int x = 0; x < img.width(); ++x) {
                    QRgb pixel = img.pixel(x, y);
                    row.push_back({static_cast<uint8_t>(qRed(pixel)), static_cast<uint8_t>(qGreen(pixel)), static_cast<uint8_t>(qBlue(pixel))}); // Extraire les composantes RGB
                }
                imageCouleur.push_back(row); // Ajouter la ligne de pixels à l'image couleur
            }
            imageObj = new ImageCouleur(imageCouleur); // Créer un objet ImageCouleur
        }

        // Charger le pixmap et ajuster la vue
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) { // Si le pixmap a bien été chargé
            ui->AfficherImage->setScene(sceneImage); // Associer la scène d'image à la vue
            sceneImage->clear(); // Effacer la scène d'image actuelle

            // Obtenir la taille de la vue et ajuster l'image en conséquence
            QSize viewSize = ui->AfficherImage->viewport()->size();
            QPixmap scaledPixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Mettre à l'échelle l'image pour s'ajuster à la vue

            // Ajouter l'image à la scène
            QGraphicsPixmapItem *item = sceneImage->addPixmap(scaledPixmap);
            item->setPos(0, 0); // Positionner l'image dans la scène
            sceneImage->setSceneRect(scaledPixmap.rect()); // Définir le rectangle de la scène

            // Ajuster la vue pour afficher l'image correctement
            ui->AfficherImage->fitInView(sceneImage->sceneRect(), Qt::KeepAspectRatio);
            ui->AfficherImage->update(); // Mettre à jour la vue
        }
    } catch (const std::exception &e) { // Si une exception est levée pendant le chargement
        QMessageBox::critical(this, tr("Erreur"), tr(e.what())); // Afficher un message d'erreur
    }
}

// ----------------------------------------------------------------------------------------------

// ************************ Calucle de l'histogramme et affichage  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_CalculerHisto_clicked()
{
    // Vérification de l'existence d'une image chargée
    if (!imageObj) {
        // Affichage d'un message d'erreur si aucune image n'est chargée
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Vérification si l'image est en niveaux de gris
    bool isGrayscale = dynamic_cast<ImageGris*>(imageObj) != nullptr;

    try {
        // Initialisation d'un tableau pour l'histogramme
        int histogramme[256] = {0};

        // Calcul de l'histogramme de l'image
        Histogramme::calculerHistogramme(*imageObj, histogramme);

        // Affichage de l'histogramme combiné
        afficherHistogramme(histogramme);

        // Si l'image est en niveaux de gris, désactivation des boutons pour les canaux
        if (isGrayscale) {
            ui->Canal_R->setVisible(false);
            ui->Canal_V->setVisible(false);
            ui->Canal_B->setVisible(false);
        } else {
            // Sinon, on rend les boutons pour les canaux visibles
            ui->Canal_R->setVisible(true);
            ui->Canal_V->setVisible(true);
            ui->Canal_B->setVisible(true);
        }
    } catch (const std::exception& e) {
        // En cas d'exception, affichage d'un message d'erreur
        QMessageBox::critical(this, tr("Erreur"), tr(e.what()));
    }
}

void MainWindow::afficherHistogramme(int histogramme[256])
{
    // Efface la scène avant de redessiner
    seceneResultat->clear();

    // Obtention de la taille de la vue pour l'affichage
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;

    // Recherche de la valeur maximale dans l'histogramme pour ajuster la taille des barres
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    // Largeur des barres de l'histogramme
    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    // Calcul du facteur d'échelle pour les hauteurs des barres
    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    // Ajout des axes à la scène
    seceneResultat->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    seceneResultat->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // Affichage des graduations sur l'axe X
    for (int i = 0; i < 256; i += 32) {
        seceneResultat->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    // Affichage des graduations sur l'axe Y
    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        seceneResultat->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = seceneResultat->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    // Dessin des barres de l'histogramme
    for (int i = 0; i < 256; ++i) {
        int barHeight = static_cast<int>(histogramme[i] * scaleFactor);
        seceneResultat->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::black));
    }

    // Affichage de la scène avec l'histogramme
    ui->AffichageResultat->setScene(seceneResultat);
}

void MainWindow::afficherHistogrammeCanal(int histogramme[256], int canal)
{
    seceneResultat->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;

    // Recherche de la valeur maximale dans l'histogramme pour le redimensionnement
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    // Largeur des barres de l'histogramme
    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    // Calcul du facteur d'échelle pour les hauteurs des barres
    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    // Ajout des axes
    seceneResultat->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    seceneResultat->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // Affichage des graduations
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

    // Affichage des barres de l'histogramme pour chaque canal
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
    // Vérification si une image est chargée
    if (imageObj) {
        // Initialisation d'un tableau pour stocker l'histogramme
        int histogramme[256] = {0};

        // Calcul de l'histogramme pour le canal rouge (0 pour le rouge)
        Histogramme::calculerHistogramme(*imageObj, histogramme, 0);

        // Affichage de l'histogramme du canal rouge
        afficherHistogrammeCanal(histogramme, 0);
    }
}

void MainWindow::on_Canal_V_stateChanged(int arg1)
{
    // Vérification si une image est chargée
    if (imageObj) {
        // Initialisation d'un tableau pour stocker l'histogramme
        int histogramme[256] = {0};

        // Calcul de l'histogramme pour le canal vert (1 pour le vert)
        Histogramme::calculerHistogramme(*imageObj, histogramme, 1);

        // Affichage de l'histogramme du canal vert
        afficherHistogrammeCanal(histogramme, 1);
    }
}

void MainWindow::on_Canal_B_stateChanged(int arg1)
{
    // Vérification si une image est chargée
    if (imageObj) {
        // Initialisation d'un tableau pour stocker l'histogramme
        int histogramme[256] = {0};

        // Calcul de l'histogramme pour le canal bleu (2 pour le bleu)
        Histogramme::calculerHistogramme(*imageObj, histogramme, 2);

        // Affichage de l'histogramme du canal bleu
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

//---------------------------------------------------------------------------

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
