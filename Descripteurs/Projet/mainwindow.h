#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include "Traitement.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ChargerImage_clicked();
    void on_CalculerHisto_clicked();

    void on_Canal_R_stateChanged(int arg1);
    void on_Canal_V_stateChanged(int arg1);
    void on_Canal_B_stateChanged(int arg1); 

    void on_DetectionContour_clicked();

    void on_DetectionDroite_clicked();
    void on_SegmenterCouleur_clicked();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *sceneImage;   // Scène pour afficher l'image
    QGraphicsScene *sceneHisto;   // Scène pour afficher l'histogramme
    QImage image;                 // Image chargée
    Image* imageObj;              // Pointeur vers une image (gris ou couleur)

    void afficherHistogramme(int histogramme[256]);   // Méthode pour afficher l'histogramme
    void afficherHistogrammeCanal(int histogramme[256], int canal); // Affichage des histogrammes par canal
    std::vector<int> getSelectedSegmentationCanaux();

    int seuilUtilisateur = 0.8;
};

#endif // MAINWINDOW_H
