#ifndef BIBLIOWINDOW_H
#define BIBLIOWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class BiblioWindow;
}

class BiblioWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BiblioWindow(QWidget *parent = nullptr);
    ~BiblioWindow();

    void loadImagesFromDirectory(const QString &directoryPath); // Charger les images depuis un répertoire

private:
    Ui::BiblioWindow *ui;
    QGraphicsScene *scene; // Scène pour afficher les images
};

#endif // BIBLIOWINDOW_H
