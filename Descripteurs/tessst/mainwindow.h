#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include "Traitement.hpp"
#include "Library.hpp"
#include "bibliowindow.h"

// Déclaration anticipée de la classe BiblioWindow
class BiblioWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Ajout d'un constructeur prenant le chemin de l'image en paramètre
    explicit MainWindow(const QString &login, const QString &imagePath, BiblioWindow *parentBiblio = nullptr, QWidget *parent = nullptr);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_ChargerImage_clicked();
    void on_CalculerHisto_clicked();

    void on_Canal_R_stateChanged(int arg1);
    void on_Canal_V_stateChanged(int arg1);
    void on_Canal_B_stateChanged(int arg1);

    void on_DetectionContour_clicked();

    void on_DetectionDroite_clicked();
    void on_RehaussementContours_clicked();
    void on_SegmenterCouleur_clicked();
    void on_AppliquerConvolution_clicked();

    //Ajout du menu Descripteur
    void on_actionAjouterDescripteur_triggered();
    void on_actionModifierDescripteur_triggered();
    void on_actionSupprimerDescripteur_triggered();

    //Afficher l'image
    void on_actionRechercherImage_triggered();


    void on_RetourVersBiblio_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *sceneImage;   // Scène pour afficher l'image
    QGraphicsScene *sceneHisto;   // Scène pour afficher l'histogramme
    QImage image;                 // Image chargée
    Image_color* imageObj;              // Pointeur vers une image (gris ou couleur)
    QString LoginUtilisateur;

    void afficherHistogramme(int histogramme[256]);   // Méthode pour afficher l'histogramme
    void afficherHistogrammeCanal(int histogramme[256], int canal); // Affichage des histogrammes par canal
    std::vector<int> getSelectedSegmentationCanaux();

    int seuilUtilisateur = 0.8;

    //Image *imageBiblio;
    BiblioWindow *parentBiblio;                 // Pointeur brut pour référence
    std::unique_ptr<BiblioWindow> biblioWindow; // Gestion automatique avec unique_ptr

    // Méthode pour charger et afficher une image directement
    void loadAndDisplayImage(const QString &fileName);
};

#endif // MAINWINDOW_H
