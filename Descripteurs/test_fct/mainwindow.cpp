#include "mainwindow.h"
#include "Traitement_image/Traitement.hpp"

#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageLabel = new QLabel(this);  // Création du label pour afficher l'image
    imageLabel->setGeometry(50, 50, 640, 480);  // Positionner le label sur la fenêtre
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageButton_clicked()
{
    // Ouvrir une boîte de dialogue pour choisir l'image
    QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir l'image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        // Charger l'image dans image_Courante
        image_Courante = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);

        // Afficher l'image sur le QLabel
        cv::cvtColor(image_Courante, image_Courante, cv::COLOR_BGR2RGB);  // Convertir en RGB pour l'affichage
        QImage img = QImage(image_Courante.data, image_Courante.cols, image_Courante.rows, image_Courante.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(img));
    }
}

void MainWindow::on_detectContoursButton_clicked()
{
    // Appliquer la détection de contours
    traitement.image_Courante = image_Courante.clone();  // Copier l'image courante
    traitement.detectionContours();  // Appeler la fonction de détection de contours

    // Mettre à jour l'image dans l'interface après traitement
    cv::cvtColor(traitement.image_Courante, traitement.image_Courante, cv::COLOR_BGR2RGB);  // Convertir en RGB pour l'affichage
    QImage img = QImage(traitement.image_Courante.data, traitement.image_Courante.cols, traitement.image_Courante.rows, traitement.image_Courante.step, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(img));
}
