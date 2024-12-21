#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include "Traitement_image/Traitement.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadImageButton_clicked();  // Slot pour charger l'image
    void on_detectContoursButton_clicked();  // Slot pour détecter les contours

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;  // Label pour afficher l'image traitée
    Traitement traitement;  // Objet Traitement pour gérer l'image
    cv::Mat image_Courante;  // Image courante à traiter


};
#endif // MAINWINDOW_H
