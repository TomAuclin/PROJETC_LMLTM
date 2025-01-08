#include "BiblioWindow.h"
#include "ui_BiblioWindow.h"
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>

BiblioWindow::BiblioWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BiblioWindow),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->AffichageBiblio->setScene(scene);
    qDebug() << "BiblioWindow créé";
}

BiblioWindow::~BiblioWindow()
{
    delete ui;
    qDebug() << "BiblioWindow détruit";
}

void BiblioWindow::loadImagesFromDirectory(const QString &directoryPath)
{
    QDir dir(directoryPath);

    if (!dir.exists()) {
        qDebug() << "Le répertoire spécifié n'existe pas : " << directoryPath;
        return;
    }

    QStringList filters{"*.png", "*.jpg", "*.CR2", "*.pgm"};
    dir.setNameFilters(filters);

    const int imageWidth = 150;
    const int imageHeight = 150;
    const int spacing = 10;

    int viewWidth = ui->AffichageBiblio->viewport()->width();
    int imagesPerLine = qMax(1, (viewWidth + spacing) / (imageWidth + spacing));

    int x = 0, y = 0, currentColumn = 0;
    QFileInfoList fileList = dir.entryInfoList();

    for (const QFileInfo &fileInfo : fileList) {
        QPixmap pixmap(fileInfo.absoluteFilePath());
        if (!pixmap.isNull()) {
            QPixmap resizedPixmap = pixmap.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(resizedPixmap);

            item->setPos(x, y);
            scene->addItem(item);

            x += imageWidth + spacing;
            currentColumn++;
            if (currentColumn >= imagesPerLine) {
                currentColumn = 0;
                x = 0;
                y += imageHeight + spacing;
            }
        } else {
            qDebug() << "Impossible de charger l'image : " << fileInfo.absoluteFilePath();
        }
    }

    scene->setSceneRect(0, 0, viewWidth, y + imageHeight);
}
