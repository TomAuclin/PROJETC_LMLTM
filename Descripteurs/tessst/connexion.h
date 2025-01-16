#ifndef CONNEXION_H
#define CONNEXION_H

#include <QDialog>
#include <memory>
#include "GestionUtilisateur.hpp"
#include "BiblioWindow.h" // Inclusion de la fenêtre BiblioWindow

namespace Ui {
class Connexion;
}

class Connexion : public QDialog
{
    Q_OBJECT

public:
    explicit Connexion(QWidget *parent = nullptr);
    ~Connexion();

private slots:
    void on_connexionButton_clicked(); // Slot pour gérer le clic sur le bouton de connexion

private:
    Ui::Connexion *ui;
    GestionUtilisateur gestionUtilisateur; // Instance de GestionUtilisateur
    std::unique_ptr<BiblioWindow> biblioWindow; // Gestion unique de la fenêtre BiblioWindow
};

#endif // CONNEXION_H
