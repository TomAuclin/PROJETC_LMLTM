#ifndef CONNEXIONWINDOW_H
#define CONNEXIONWINDOW_H

// Inclusion des fichiers d'en-tête
#include "bibliowindow.h"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QDialog>
#include <memory>

class BiblioWindow; // Déclaration anticipée de la classe BiblioWindow

namespace Ui {
class ConnexionWindow; // Déclaration anticipée de l'interface utilisateur de ConnexionWindow
}

class ConnexionWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructeur de la fenêtre de connexion, prend un parent QWidget
    explicit ConnexionWindow(QWidget *parent = nullptr);

    // Destructeur de la classe ConnexionWindow
    ~ConnexionWindow();

    // Retourne le login de l'utilisateur actuellement entré
    QString getLogin() const;

private slots:
    // Gère les changements de position du curseur dans le champ de texte du login
    void on_Login_cursorPositionChanged(int arg1, int arg2);

    // Gère l'événement du clic sur le bouton de connexion
    void on_connexionButton_clicked();

private:

    Ui::ConnexionWindow *ui; // Interface utilisateur générée par Qt pour ConnexionWindow
    std::unique_ptr<BiblioWindow> biblioWindow;// Pointeur unique pour gérer la fenêtre BiblioWindow
    QString LoginActuel; // Stocke le login actuel saisi par l'utilisateur

    // Configure l'affichage du logo dans la fenêtre de connexion
    void setupLogo();
};

#endif // CONNEXIONWINDOW_H
