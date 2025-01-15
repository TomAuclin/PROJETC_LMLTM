#ifndef CONNEXIONWINDOW_H
#define CONNEXIONWINDOW_H

#include <QDialog>
#include <memory>
#include "bibliowindow.h" // Inclusion de la classe BiblioWindow

class BiblioWindow;


namespace Ui {
class ConnexionWindow;
}

class ConnexionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnexionWindow(QWidget *parent = nullptr);
    ~ConnexionWindow();

    QString getLogin() const; // pour récupérer le login

private slots:
    void on_Login_cursorPositionChanged(int arg1, int arg2);
    void on_connexionButton_clicked();

private:
    Ui::ConnexionWindow *ui;
    std::unique_ptr<BiblioWindow> biblioWindow; // Pointeur unique pour gérer la fenêtre BiblioWindow
    QString LoginActuel; // Stocke le login actuel
    void setupLogo();
};

#endif // CONNEXIONWINDOW_H
