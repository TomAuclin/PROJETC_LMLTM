#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Inclusion des fichiers d'en-tête
#include "Traitement.hpp"
#include "Library.hpp"
#include "bibliowindow.h"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>

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
    // Constructeur prenant le login, le chemin de l'image et un pointeur vers la fenêtre BiblioWindow en paramètre.
    explicit MainWindow(const QString &login, const QString &imagePath, BiblioWindow *parentBiblio = nullptr, QWidget *parent = nullptr);

    // Méthode pour charger et afficher une image à partir d'un fichier
    void loadAndDisplayImage(const QString &fileName);

    // Constructeur par défaut
    MainWindow(QWidget *parent = nullptr);

    // Destructeur
    ~MainWindow();

private slots:
    // Méthode pour calculer l'histogramme de l'image
    void on_CalculerHisto_clicked();

    // Méthodes pour gérer les changements d'état des canaux R, V, et B
    void on_Canal_R_stateChanged(int arg1);
    void on_Canal_V_stateChanged(int arg1);
    void on_Canal_B_stateChanged(int arg1);

    // Méthodes pour effectuer des traitements d'image
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

    //Ajout du menu Descripteur
    void on_actionAjouterDescripteur_triggered();
    void on_actionModifierDescripteur_triggered();
    void on_actionSupprimerDescripteur_triggered();

    //Afficher l'image
    void on_actionRechercherImage_triggered();


    // Méthode pour revenir à la fenêtre de la bibliothèque
    void on_RetourVersBiblio_clicked();

    cv::Mat  on_BruiterImage_clicked();

private:
    Ui::MainWindow *ui;               // Pointeur vers l'interface utilisateur de la fenêtre
    QGraphicsScene *sceneImage;       // Scène pour afficher l'image
    QGraphicsScene *seceneResultat;   // Scène pour afficher l'histogramme
    QImage image;                     // Image chargée
    Image_color* imageObj;            // Pointeur vers une image (en niveaux de gris ou couleur)
    QString LoginUtilisateur;        // Login de l'utilisateur
    QString selectedImagePath;       // Chemin de l'image sélectionnée

    // Méthode pour afficher l'histogramme de l'image
    void afficherHistogramme(int histogramme[256]);

    // Méthode pour afficher l'histogramme d'un canal spécifique
    void afficherHistogrammeCanal(int histogramme[256], int canal);

    // Méthode pour obtenir les canaux sélectionnés pour la segmentation
    std::vector<int> getSelectedSegmentationCanaux();

    int seuilUtilisateur = 0.8;  // Seuil pour certains traitements

    BiblioWindow *parentBiblio;                  // Pointeur vers la fenêtre BiblioWindow (utilisé pour la navigation)
    std::unique_ptr<BiblioWindow> biblioWindow;  // Pointeur unique pour gérer la fenêtre BiblioWindow de manière automatique
};


#endif // MAINWINDOW_H
