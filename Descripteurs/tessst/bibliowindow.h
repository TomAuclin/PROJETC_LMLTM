#ifndef BIBLIOWINDOW_H
#define BIBLIOWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>
#include <QMouseEvent>
#include "Image.hpp"

#include "mainwindow.h"
#include <memory>


namespace Ui {
class BiblioWindow;
}

class BiblioWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BiblioWindow(QWidget *parent = nullptr);
    ~BiblioWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override; // Gérer les clics dans la fenêtre principale

private slots:
    void on_ChargerBiblioButton_clicked(); // Charger les images
    void on_AffichageBiblio_itemClicked(QListWidgetItem *item); // Clic sur une image
    void on_TraitementButton_clicked(); // Clic sur le bouton "Traitement"

    //void on_DetailsButton_clicked();
    void on_DetailsButton_clicked();

private:
    Ui::BiblioWindow *ui;

    QString selectedImagePath; // Chemin de l'image sélectionnée
    void loadImagesIntoList(const QString &directoryPath); // Charger les images dans la liste

    std::unique_ptr<MainWindow> mainWindow; // Pointeur unique pour gérer la fenêtre MainWindow

};

#endif // BIBLIOWINDOW_H
