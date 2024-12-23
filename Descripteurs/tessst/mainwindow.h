#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot pour charger une image
    void on_ChargerImage_clicked();

    // Slot pour détecter les contours
    void on_DetecterContours_clicked();

private:
    Ui::MainWindow *ui;                // Interface utilisateur
    QGraphicsScene *sceneOriginal;     // Scène pour afficher l'image originale
    QGraphicsScene *sceneResult;       // Scène pour afficher le résultat
    cv::Mat image_Courante;            // Image chargée en mémoire (OpenCV)
};

#endif // MAINWINDOW_H
