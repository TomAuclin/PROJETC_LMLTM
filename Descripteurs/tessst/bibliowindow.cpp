#include "bibliowindow.h"
#include "ui_bibliowindow.h"
#include "Image.hpp"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QPixmap>
#include <QIcon>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QMouseEvent>
#include <QScreen>


BiblioWindow::BiblioWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BiblioWindow)
{
    ui->setupUi(this);

    setWindowTitle("Bibliotheque");
    resize(1000, 600);

    // Centrer la fenêtre sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }


    // Configurer le QListWidget pour afficher en mosaïque
    ui->AffichageBiblio->setViewMode(QListWidget::IconMode);
    ui->AffichageBiblio->setResizeMode(QListWidget::Adjust);
    ui->AffichageBiblio->setMovement(QListWidget::Static);
    ui->AffichageBiblio->setSpacing(10);

    ui->TraitementButton->setVisible(false);
    ui->DetailsButton->setVisible(false);


    // Connecter le clic sur une image
    connect(ui->AffichageBiblio, &QListWidget::itemClicked, this, &BiblioWindow::on_AffichageBiblio_itemClicked);
}

BiblioWindow::~BiblioWindow()
{
    delete ui;
}

void BiblioWindow::on_ChargerBiblioButton_clicked()
{
    QString directoryPath = QFileDialog::getExistingDirectory(this, "Sélectionnez un dossier d'images");

    if (directoryPath.isEmpty()) {
        qDebug() << "Aucun dossier sélectionné.";
        return;
    }

    loadImagesIntoList(directoryPath);
}

void BiblioWindow::loadImagesIntoList(const QString &directoryPath)
{
    ui->AffichageBiblio->clear();

    QDir dir(directoryPath);
    QStringList imageFiles = dir.entryList(QStringList() << "*.png" << "*.CR2" << "*.pgm", QDir::Files);

    if (imageFiles.isEmpty()) {
        QMessageBox::information(this, "Aucune image", "Le dossier sélectionné ne contient aucune image.");
        return;
    }

    const QSize iconSize(150, 150);

    foreach (const QString &fileName, imageFiles) {
        QString filePath = directoryPath + "/" + fileName;
        QPixmap pixmap(filePath);

        if (!pixmap.isNull()) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setIcon(QIcon(pixmap.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            item->setData(Qt::UserRole, filePath);
            item->setSizeHint(iconSize);

            ui->AffichageBiblio->addItem(item);
        } else {
            qDebug() << "Impossible de charger l'image : " << filePath;
        }
    }

    ui->AffichageBiblio->setIconSize(iconSize);
}

void BiblioWindow::on_AffichageBiblio_itemClicked(QListWidgetItem *item)
{
    QString filePath = item->data(Qt::UserRole).toString();
    qDebug() << "Image cliquée : " << filePath;

    selectedImagePath = filePath; // Stocker le chemin de l'image sélectionnée

    // Rendre le bouton de traitement visible
    ui->TraitementButton->setVisible(true);
    ui->DetailsButton->setVisible(true);
}

void BiblioWindow::on_TraitementButton_clicked()
{
    if (!selectedImagePath.isEmpty()) {
        qDebug() << "Traitement de l'image : " << selectedImagePath;

        // Créer une instance de MainWindow avec le chemin de l'image
        if (!mainWindow) {
            mainWindow = std::make_unique<MainWindow>(selectedImagePath, nullptr); // Passer le chemin de l'image
        }

        // Afficher MainWindow
        mainWindow->show();

        // Fermer la fenêtre actuelle
        this->close();
    } else {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
    }
}


void BiblioWindow::mousePressEvent(QMouseEvent *event)
{
    // Si le clic est en dehors de QListWidget, désélectionner l'image
    if (!ui->AffichageBiblio->geometry().contains(event->pos())) {
        ui->AffichageBiblio->clearSelection();
        selectedImagePath.clear();
        ui->TraitementButton->setVisible(false); // Masquer le bouton traitement
        ui->DetailsButton->setVisible(false); // Masquer le bouton details
    }

    QMainWindow::mousePressEvent(event);
}

/*void BiblioWindow::on_DetailsButton_clicked()
{
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }

    // Chemin du fichier descripteurs (à adapter selon ton organisation)
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

    // Création de l'instance Image
    Image image;
    image.titre = QFileInfo(selectedImagePath).baseName().toStdString();

    // Associer les descripteurs
    image.associerDescripteur(cheminDescripteurs);

    // Afficher les détails associés
    QString details = QString::fromStdString(
        "Titre : " + image.titre + "\n" +
        "Source : " + image.source + "\n" +
        "Numéro : " + std::to_string(image.numero) + "\n" +
        "Prix : " + std::to_string(image.prix) + "\n" +
        "Accès : " + image.acces + "\n" +
        "Type : " + image.type + "\n" +
        "Nombre de traitements possibles : " + std::to_string(image.nbTraitementPossible) + "\n" +
        "Identité : " + std::to_string(image.identite)
        );

    QMessageBox::information(this, "Détails de l'image", details);
}*/

void BiblioWindow::on_DetailsButton_clicked()
{
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }

    // Chemin du fichier descripteurs (à adapter selon votre organisation)
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

    // Création de l'instance Image
    Image image;
    image.titre = QFileInfo(selectedImagePath).fileName().toStdString(); // Inclut l'extension

    // Associer les descripteurs
    image.associerDescripteur(cheminDescripteurs);

    // Afficher les détails associés en utilisant les getters
    QString details = QString::fromStdString(
        "Titre : " + image.getTitre() + "\n" +
        "Source : " + image.getSource() + "\n" +
        "Numéro : " + std::to_string(image.getNumero()) + "\n" +
        "Prix : " + std::to_string(image.getPrix()) + "\n" +
        "Accès : " + std::string(1, image.getAccess()) + "\n" +
        "Type : " + image.getType() + "\n" +
        "Nombre de traitements possibles : " + std::to_string(image.getnbTraitementPossible()) + "\n"

        );

    QMessageBox::information(this, "Détails de l'image", details);
}


