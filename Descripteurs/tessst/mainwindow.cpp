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


