#ifndef CONNEXIONWINDOW_H
#define CONNEXIONWINDOW_H

#include <QDialog>
#include <memory>
#include "bibliowindow.h" // Inclusion de la classe BiblioWindow

namespace Ui {
class ConnexionWindow;
}

class ConnexionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnexionWindow(QWidget *parent = nullptr);
    ~ConnexionWindow();

private slots:
    void on_Login_cursorPositionChanged(int arg1, int arg2);
    void on_connexionButton_clicked();

    // Chargement du logo de la fac
    void on_Logo_ups_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

private:
    Ui::ConnexionWindow *ui;
    std::unique_ptr<BiblioWindow> biblioWindow; // Pointeur unique pour gérer la fenêtre BiblioWindow
};

#endif // CONNEXIONWINDOW_H
