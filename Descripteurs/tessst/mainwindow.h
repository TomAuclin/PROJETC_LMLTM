#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include "Traitement.hpp"
#include "Library.hpp"

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
    void on_RehaussementContours_clicked();
    void on_SegmenterCouleur_clicked();

    //Ajout du menu Descripteur
    void on_actionAjouterDescripteur_triggered();
    void on_actionModifierDescripteur_triggered();
    void on_actionSupprimerDescripteur_triggered();

    //Ajout des descripteurs
    void afficherDetailsDescripteur(const Image& image);  // Afficher les détails du descripteur
    void afficherMenuContextuel(QListWidgetItem* item, const QPoint &pos);  // Afficher le menu contextuel
    void onCustomContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *sceneImage;   // Scène pour afficher l'image
    QGraphicsScene *sceneHisto;   // Scène pour afficher l'histogramme
    QImage image;                 // Image chargée
    Image_color* imageObj;              // Pointeur vers une image (gris ou couleur)

    void afficherHistogramme(int histogramme[256]);   // Méthode pour afficher l'histogramme
    void afficherHistogrammeCanal(int histogramme[256], int canal); // Affichage des histogrammes par canal
    std::vector<int> getSelectedSegmentationCanaux();
    QListWidget *listWidget;
    int seuilUtilisateur = 0.8;
};

#endif // MAINWINDOW_H
