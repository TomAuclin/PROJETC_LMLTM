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
//#include "Library.hpp"
#include <QInputDialog>



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
    ui->pushButtonRechercherp->setVisible(true);



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

void BiblioWindow::on_ChargeBoutton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionnez un fichier", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        library.chargerDepuisFichier(filePath.toStdString());
        QMessageBox::information(this, "Chargement", "Bibliothèque chargée depuis le fichier.");
    }
}

void BiblioWindow::on_SaveBoutton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        library.sauvegarderDansFichier(filePath.toStdString());
        QMessageBox::information(this, "Sauvegarde", "Bibliothèque sauvegardée dans le fichier.");
    }
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

void BiblioWindow::on_DetailsButton_clicked() {
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }

    // Chemin du fichier descripteurs
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

    // Création de l'instance Image
    Image image;
    image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
    // Association des descripteurs
    image.associerDescripteur(cheminDescripteurs);

    // Formatage du prix avec le symbole €
    std::string prixAvecEuro = std::to_string(image.getPrix()) + " €";

    // Affichage des détails associés en utilisant les getters
    QString details = QString::fromStdString(
        "Titre : " + image.getTitre() + "\n" +
        "Source : " + image.getSource() + "\n" +
        "Numéro : " + std::to_string(image.getNumero()) + "\n" +
        "Prix : " + prixAvecEuro + "\n" +  // Affichage avec le symbole euro
        "Accès : " + std::string(1, image.getAccess()) + "\n" +
        "Type : " + image.getType() + "\n" +
        "Nombre de traitements possibles : " + std::to_string(image.getnbTraitementPossible()) + "\n"
        );

    // Affichage des détails dans une boîte de message
    QMessageBox::information(this, "Détails de l'image", details);
}


void BiblioWindow::on_pushButtonRechercherp_clicked() {
    bool ok;
    int numeroImage = QInputDialog::getInt(this, tr("Recherche de Prix"),
                                           tr("Saisissez le numéro de l'image pour avoir son prix :"),
                                           0, 0, 9999, 1, &ok);
    if (ok) {
        Image image;
        std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

        if (image.rechercherPrix(numeroImage, cheminDescripteurs)) {
            double prix = image.getPrix();
            QString details = QString("Le prix de l'image numéro %1 est : %2 €")
                                  .arg(numeroImage)
                                  .arg(prix);
            QMessageBox::information(this, "Résultat", details);
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune image avec ce numéro n'a été trouvée !");
        }
    } else {
        qDebug() << "L'utilisateur a annulé la saisie.";
    }
}
