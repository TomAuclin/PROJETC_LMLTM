#ifndef BIBLIOWINDOW_H
#define BIBLIOWINDOW_H

// Inclusion des fichiers d'en-tête
#include "bibliowindow.h"
#include "Image.hpp"
#include "Library.hpp"
#include "bibliowindow.h"
#include "GestionUtilisateur.hpp"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QMainWindow>
#include <QListWidget>
#include <QString>
#include <QMouseEvent>
#include "Image.hpp"
#include "Library.hpp"
//#include "mainwindow.h"
//#include "connexionwindow.h"
#include <memory>
#include <QVBoxLayout>

// Déclaration anticipée de MainWindow
class MainWindow;
class ConnexionWindow;

namespace Ui {
class BiblioWindow;
}

class BiblioWindow : public QMainWindow
{
    Q_OBJECT

public:

    void loadImagesIntoList(const QString &directoryPath); // Charge les images à partir du répertoire spécifié et les ajoute à la liste
    explicit BiblioWindow(const QString &login, QWidget *parent = nullptr); // Constructeur de la fenêtre principale BiblioWindow, prend le login de l'utilisateur
    void loadDefaultFile(const QString &userLogin); // Charge un fichier par défaut pour l'utilisateur spécifié
    void setUserLogin(const QString &login); // Définit le login de l'utilisateur

    // Destructeur de la classe BiblioWindow
    ~BiblioWindow();

    // Instance de la classe Library pour gérer les bibliothèques d'images
    Library library;

protected:
// Gère l'événement de clic de souris dans la fenêtre principale
    void mousePressEvent(QMouseEvent *event) override;


private slots:

    void on_AffichageBiblio_itemClicked(QListWidgetItem *item);  // Réagit au clic sur un élément de la liste des images

    void on_TraitementButton_clicked(); // Réagit au clic sur le bouton de traitement des images
    void on_DetailsButton_clicked(); // Affiche les détails d'une image
    void on_SaveBoutton_clicked(); // Sauvegarde les informations actuelles de la bibliothèque
    void on_pushButtonRechercherp_clicked(); // Recherche une image ou un fichier dans la bibliothèque
    void on_Deco_clicked(); // Déconnecte l'utilisateur et ferme la fenêtre

    // Charge une bibliothèque d'images à partir d'un fichier .txt
    void on_ChargeBoutton_clicked();

    // Actions pour gérer les descripteurs d'images
    void on_actionAjouterDescripteur_triggered();
    void on_actionModifierDescripteur_triggered();
    void on_actionSupprimerDescripteur_triggered();

    void on_pushButtonSousListePrix_clicked();  // Filtre la liste d'images par prix
    void on_triprix_clicked(); // Trie les images par prix
    void on_trinbtraitements_clicked(); // Trie les images par nombre de traitements appliqués
    void on_souslistetype_clicked();// Affiche les images filtrées par type


    void on_SousListesButton_clicked();

    void on_Trisbutton_clicked();

    void on_Retourbutton_clicked();

private:

    Ui::BiblioWindow *ui; // Interface utilisateur générée par Qt
    QString LoginUtilisateur; // Login de l'utilisateur actuel
    QString selectedImagePath; // Chemin de l'image actuellement sélectionnée
    int currentImageNumber; // Numéro de l'image actuellement affichée

    // Prix de l'image actuellement sélectionnée
    double currentImagePrice;

    std::unique_ptr<MainWindow> mainWindow;// Pointeur unique pour gérer la fenêtre MainWindow
    QString cheminBiblio; // Chemin du fichier contenant la bibliothèque d'images
    GestionUtilisateur gestionUtilisateur; // Gestion de la déconnexion de l'utilisateur
    std::unique_ptr<ConnexionWindow> connexionWindow;// Pointeur unique pour rouvrir la fenêtre de connexion
    static const QString DEFAULT_FILE_PATH; // Chemin par défaut pour charger un fichier
    QString userLogin; // Login de l'utilisateur stocké dans la classe

    // Met à jour le compteur d'images
    void mettreAJourCompteurImages();
};

#endif // BIBLIOWINDOW_H
