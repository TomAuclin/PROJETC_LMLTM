#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QInputDialog>
#include <QPixmap>
#include "Traitement.hpp"
#include <opencv2/opencv.hpp>

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
    sceneHisto->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    sceneHisto->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // graduations pour les axes
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

    //  les histogrammes pour chaque canal en fonction des cases cochées
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include "Traitement.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), sceneOriginal(new QGraphicsScene(this)), sceneResult(new QGraphicsScene(this)) {
    ui->setupUi(this);

    // Assigner les scènes aux QGraphicsView
    ui->AfficherImageOriginale->setScene(sceneOriginal);
    ui->AfficherImageResultat->setScene(sceneResult);

    // Connecter les boutons aux slots
    connect(ui->ChargerImage, &QPushButton::clicked, this, &MainWindow::on_ChargerImage_clicked);
    connect(ui->DetecterContours, &QPushButton::clicked, this, &MainWindow::on_DetecterContours_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_ChargerImage_clicked() {
    // Ouvre un QFileDialog pour sélectionner une image
    QString fileName = QFileDialog::getOpenFileName(this, tr("Charger une image"), "", tr("Images (*.png *.pgm *.CR2 *.jpeg)"));

    if (!fileName.isEmpty()) {
        // Charge l'image avec OpenCV
        image_Courante = cv::imread(fileName.toStdString());

        if (!image_Courante.empty()) {
            // Converti l'image pour l'afficher dans QGraphicsView
            cv::Mat imageRGB;
            cv::cvtColor(image_Courante, imageRGB, cv::COLOR_BGR2RGB);
            QImage imgQt(imageRGB.data, imageRGB.cols, imageRGB.rows, imageRGB.step, QImage::Format_RGB888);

            // Ajoute l'image originale à la scène
            sceneOriginal->clear();
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(imgQt));
            sceneOriginal->addItem(item);

            // Mets à jour l'affichage
            ui->AfficherImageOriginale->fitInView(item, Qt::KeepAspectRatio);
            ui->statusbar->showMessage("Image chargée avec succès.");
        } else {
            QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image."));
        }
    }
}

void MainWindow::on_DetecterContours_clicked() {
    if (image_Courante.empty()) {
        std::cerr << "Aucune image chargée !" << std::endl;
        ui->statusbar->showMessage("Veuillez charger une image avant de détecter les contours.");
        return;
    }

    // Applique la détection des contours sur l'image chargée
    Traitement traitement;
    cv::Mat imgContours = traitement.detectionContours(image_Courante);
    if (imgContours.empty()) {
        std::cerr << "Erreur lors de la détection des contours." << std::endl;
        ui->statusbar->showMessage("Erreur lors de la détection des contours.");
        return;
    }

    // Converti l'image des contours pour Qt
    QImage img(imgContours.data, imgContours.cols, imgContours.rows, imgContours.step, QImage::Format_Grayscale8);

    // Efface la scène avant d'ajouter une nouvelle image
    sceneResult->clear();
    sceneResult->addPixmap(QPixmap::fromImage(img));

    ui->AfficherImageResultat->setScene(sceneResult);

    // Ajuste le rect de la scène pour que l'image de contour soit bien cadrée
    ui->AfficherImageResultat->setSceneRect(0, 0, imgContours.cols, imgContours.rows);

    ui->AfficherImageResultat->fitInView(sceneResult->sceneRect(), Qt::KeepAspectRatio);


    ui->statusbar->showMessage("Contours détectés et affichés.");
}
void MainWindow::on_HoughDroite_clicked() {
    // Vérifie si une image a été chargée
    if (image_Courante.empty()) {
        std::cerr << "Aucune image chargée !" << std::endl;
        ui->statusbar->showMessage("Veuillez charger une image avant d'appliquer la transformation de Hough.");
        return;
    }

    // Applique la transformation de Hough pour détecter les droites
    Traitement traitement;
    cv::Mat imgHough = traitement.HoughDroite(image_Courante);

    if (imgHough.empty()) {
        std::cerr << "Erreur lors de l'application de la transformation de Hough." << std::endl;
        ui->statusbar->showMessage("Erreur lors de l'application de la transformation de Hough.");
        return;
    }

    // Convertit l'image contenant les droites détectées pour Qt
    QImage img(imgHough.data, imgHough.cols, imgHough.rows, imgHough.step, QImage::Format_RGB888);

    // Efface la scène avant d'ajouter une nouvelle image
    sceneResult->clear();
    sceneResult->addPixmap(QPixmap::fromImage(img));

    ui->AfficherImageResultat->setScene(sceneResult);

    // Ajuste le rect de la scène pour que l'image des droites soit bien cadrée
    ui->AfficherImageResultat->setSceneRect(0, 0, imgHough.cols, imgHough.rows);

    ui->AfficherImageResultat->fitInView(sceneResult->sceneRect(), Qt::KeepAspectRatio);

    ui->statusbar->showMessage("Transformation de Hough appliquée et résultat affiché.");
}

void MainWindow::on_RehaussementContours_clicked() {
    if (image_Courante.empty()) {
        ui->statusbar->showMessage("Veuillez charger une image avant d'appliquer le rehaussement des contours.");
        return;
    }

    // Appliquer la fonction de rehaussement des contours
    Traitement traitement;
    cv::Mat imgRehaussee = traitement.rehaussementContours(image_Courante);

    if (imgRehaussee.empty()) {
        ui->statusbar->showMessage("Erreur lors de l'application du rehaussement des contours.");
        return;
    }

    // Convertir l'image pour l'afficher dans l'interface Qt
    QImage imgQt(imgRehaussee.data, imgRehaussee.cols, imgRehaussee.rows, imgRehaussee.step, QImage::Format_Grayscale8);
    sceneResult->clear();
    sceneResult->addPixmap(QPixmap::fromImage(imgQt));
    ui->AfficherImageResultat->fitInView(sceneResult->sceneRect(), Qt::KeepAspectRatio);

    ui->statusbar->showMessage("Rehaussement des contours appliqué avec succès.");
}


