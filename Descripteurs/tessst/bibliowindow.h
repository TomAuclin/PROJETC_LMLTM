#ifndef BIBLIOWINDOW_H
#define BIBLIOWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>
#include <QMouseEvent>
#include "Image.hpp"
#include "Library.hpp"
//#include "mainwindow.h"
//#include "connexionwindow.h"
#include "bibliowindow.h"
#include <memory>
#include <QVBoxLayout>

#include "GestionUtilisateur.hpp"

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
    void loadImagesIntoList(const QString &directoryPath);
    explicit BiblioWindow(const QString &login, QWidget *parent = nullptr);
    void loadDefaultFile(const QString &userLogin); // Charger les images dans la liste
    void setUserLogin(const QString &login); // Méthode pour définir le login utilisateur
    ~BiblioWindow();
    Library library;

protected:
    void mousePressEvent(QMouseEvent *event) override; // Gérer les clics dans la fenêtre principale

private slots:
    //void on_ChargerBiblioButton_clicked(); // Charger les images
    void on_AffichageBiblio_itemClicked(QListWidgetItem *item); // Clic sur une image
    void on_TraitementButton_clicked(); // Clic sur le bouton "Traitement"

    void on_DetailsButton_clicked();

    void on_SaveBoutton_clicked();
    void on_pushButtonRechercherp_clicked();
    void on_Deco_clicked();
    void on_ChargeBoutton_clicked();  // Charger la biblio .txt

    //menu descripteur
    void on_actionAjouterDescripteur_triggered();
    void on_actionModifierDescripteur_triggered();
    void on_actionSupprimerDescripteur_triggered();

private:
    Ui::BiblioWindow *ui;
    QString LoginUtilisateur;
    QString selectedImagePath; // Chemin de l'image sélectionnée
    int currentImageNumber;  // Pour stocker le numéro de l'image
    double currentImagePrice; // Pour stocker le prix de l'image
    std::unique_ptr<MainWindow> mainWindow; // Pointeur unique pour gérer la fenêtre MainWindow
    
    QString cheminBiblio;  // Variable membre pour stocker le chemin du fichier .txt

    GestionUtilisateur gestionUtilisateur; // Instance pour gérer la déconnexion
    std::unique_ptr<ConnexionWindow> connexionWindow; // Pointeur pour rouvrir la fenêtre de connexion
    static const QString DEFAULT_FILE_PATH;
    QString userLogin; // Attribut pour stocker le login utilisateur

    void mettreAJourCompteurImages() ;

};

#endif // BIBLIOWINDOW_H
