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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "mainwindow.h"
#include "connexionwindow.h"

#include "mainwindow.h"
#include "connexionwindow.h"

const QString BiblioWindow::DEFAULT_FILE_PATH = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";


BiblioWindow::BiblioWindow(const QString &login, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BiblioWindow), LoginUtilisateur(login)
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

    qDebug() << "Utilisateur connecté : " << LoginUtilisateur;
    // Configurer le QListWidget pour afficher en mosaïque
    ui->AffichageBiblio->setViewMode(QListWidget::IconMode);
    ui->AffichageBiblio->setResizeMode(QListWidget::Adjust);
    ui->AffichageBiblio->setMovement(QListWidget::Static);
    ui->AffichageBiblio->setSpacing(10);

    ui->TraitementButton->setVisible(false);
    ui->DetailsButton->setVisible(false);
    ui->pushButtonRechercherp->setVisible(true);
    ui->labelImageCount->setText("Nombre d'images : 0");


    // Connecter le clic sur une image
    connect(ui->AffichageBiblio, &QListWidget::itemClicked, this, &BiblioWindow::on_AffichageBiblio_itemClicked);
    connect(ui->Deco, &QPushButton::clicked, this, &BiblioWindow::on_Deco_clicked);

    connect(ui->pushButtonSousListePrix, &QPushButton::clicked, this, &BiblioWindow::on_pushButtonSousListePrix_clicked);
    connect(ui->souslistetype, &QPushButton::clicked, this, &BiblioWindow::on_souslistetype_clicked);
    connect(ui->triprix, &QPushButton::clicked, this, &BiblioWindow::on_triprix_clicked);
    connect(ui->trinbtraitements, &QPushButton::clicked, this, &BiblioWindow::on_trinbtraitements_clicked);



}

BiblioWindow::~BiblioWindow()
{
    delete ui;
}

void BiblioWindow::on_Deco_clicked()
{
    gestionUtilisateur.deconnexion();

    this->close();

    // Ouvre la fenêtre de connexion
    connexionWindow = std::make_unique<ConnexionWindow>();
    connexionWindow->show();
}


void BiblioWindow::on_ChargeBoutton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionnez un fichier", "", "Text Files (*.txt);;All Files (*)");

    if (!filePath.isEmpty()) {
        cheminBiblio = filePath;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
            return;
        }

        QString imageDirectory = "/media/sf_PROJETC_LMLTM/Bibliotheque";
        ui->AffichageBiblio->clear();
        QStringList missingImages;

        QTextStream in(&file);
        int compteur = 1;

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');

            if (fields.size() >= 5) {
                QString imageName = fields[1].trimmed();
                QString accessType = fields[4].trimmed();
                QString imagePath = imageDirectory + "/" + imageName;

                // Vérifier l'accès en fonction du login utilisateur
                if ((userLogin == "us-02-al" && accessType == "O") ||
                    (userLogin == "ad-01-ao" && (accessType == "O" || accessType == "L"))) {

                    if (QFileInfo::exists(imagePath)) {
                        QString itemText = QString::number(compteur) + ". " + imageName;
                        QListWidgetItem* item = new QListWidgetItem(QIcon(imagePath), itemText);
                        item->setData(Qt::UserRole, imagePath);
                        ui->AffichageBiblio->addItem(item);
                        compteur++;
                    } else {
                        missingImages.append(imageName);
                    }
                }
            }
        }

        file.close();

        if (!missingImages.isEmpty()) {
            QString missingMessage = "Les images suivantes n'ont pas été trouvées dans le répertoire :\n" + missingImages.join("\n");
            QMessageBox::warning(this, "Images Manquantes", missingMessage);
        }

        QMessageBox::information(this, "Chargement", "Bibliothèque chargée et images affichées.");
        mettreAJourCompteurImages();
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

void BiblioWindow::loadDefaultFile(const QString &userLogin)
{
    setUserLogin(userLogin); // Assurez-vous que le login est défini ici

    QString filePath = DEFAULT_FILE_PATH;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier par défaut.");
        return;
    }

    QString imageDirectory = "/media/sf_PROJETC_LMLTM/Bibliotheque";
    ui->AffichageBiblio->clear();
    QStringList missingImages;

    QTextStream in(&file);
    int compteur = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() >= 5) {
            QString imageName = fields[1].trimmed();
            QString accessType = fields[4].trimmed();
            QString imagePath = imageDirectory + "/" + imageName;

            // Vérifier l'accès en fonction du login utilisateur
            if ((userLogin == "us-02-al" && accessType == "O") ||
                (userLogin == "ad-01-ao" && (accessType == "O" || accessType == "L"))) {

                if (QFileInfo::exists(imagePath)) {
                    QString itemText = QString::number(compteur) + ". " + imageName;
                    QListWidgetItem* item = new QListWidgetItem(QIcon(imagePath), itemText);
                    item->setData(Qt::UserRole, imagePath);
                    ui->AffichageBiblio->addItem(item);
                    compteur++;
                } else {
                    missingImages.append(imageName);
                }
            }
        }
    }

    file.close();

    if (!missingImages.isEmpty()) {
        QString missingMessage = "Les images suivantes n'ont pas été trouvées dans le répertoire :\n" + missingImages.join("\n");
        QMessageBox::warning(this, "Images Manquantes", missingMessage);
    }

    QMessageBox::information(this, "Chargement", "Bibliothèque chargée et images affichées.");
    mettreAJourCompteurImages();
}

void BiblioWindow::on_AffichageBiblio_itemClicked(QListWidgetItem *item)
{
    QString filePath = item->data(Qt::UserRole).toString(); // Récupérer le chemin complet
    qDebug() << "Image cliquée : " << filePath;

    selectedImagePath = filePath; // Mettre à jour le chemin de l'image sélectionnée

    // Rendre les boutons visibles
    ui->TraitementButton->setVisible(true);
    ui->DetailsButton->setVisible(true);
}


void BiblioWindow::on_TraitementButton_clicked()
{
    if (!selectedImagePath.isEmpty()) {
        qDebug() << "Traitement de l'image : " << selectedImagePath;

        // Créer une instance de MainWindow et lui passer le chemin de l'image sélectionnée
        if (!mainWindow) {
            mainWindow = std::make_unique<MainWindow>(LoginUtilisateur, selectedImagePath, this);
        } else {
            // Mettre à jour l'image dans la fenêtre existante
            mainWindow->loadAndDisplayImage(selectedImagePath);
        }

        // Afficher la fenêtre de traitement
        mainWindow->show();

        // Masquer la bibliothèque
        this->hide();
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


/*void BiblioWindow::on_pushButtonRechercherp_clicked() {

    bool ok;
    int numeroImage = QInputDialog::getInt(this, tr("Recherche de Prix"),
                                           tr("Saisissez le numéro de l'image pour avoir son prix :"),
                                           0, 0, 9999, 1, &ok);

    if (ok) {
        Image image;

        // Vérifier si le chemin de la bibliothèque a été mémorisé
        if (!cheminBiblio.isEmpty()) {
            // Passer le chemin mémorisé à la méthode rechercherPrix
            if (image.rechercherPrix(numeroImage, cheminBiblio.toStdString())) {
                double prix = image.getPrix();
                QString details = QString("Le prix de l'image numéro %1 est : %2 €")
                                      .arg(numeroImage)
                                      .arg(prix);
                QMessageBox::information(this, "Résultat", details);
            } else {
                QMessageBox::warning(this, "Erreur", "Aucune image avec ce numéro n'a été trouvée !");
            }
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune bibliothèque n'a été chargée !");
        }
    } else {
        qDebug() << "L'utilisateur a annulé la saisie.";
    }
}
*/
/*
void BiblioWindow::on_pushButtonRechercherp_clicked() {
    bool ok;
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    int numeroImage = QInputDialog::getInt(this, tr("Recherche de Prix"),
                                           tr("Saisissez le numéro de l'image pour avoir son prix :"),
                                           1, 1, ui->AffichageBiblio->count(), 1, &ok);  // L'index commence à 1
    if (ok) {
        // Récupération de l'élément correspondant au numéro
        QListWidgetItem *item = ui->AffichageBiblio->item(numeroImage - 1);
        if (!item) {
            QMessageBox::warning(this, "Erreur", "Aucune image correspondant à ce numéro !");
            return;
        }

        // Extraction du titre de l'image depuis le texte de l'élément
        QString itemText = item->text();
        QString titreCherche = itemText.section('.', 1).trimmed(); // Extrait le texte après le numéro et le point
        qDebug() << "Titre extrait : " << titreCherche;

        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();

        // Associe le descripteur de l'image et récupère son prix
        image.associerDescripteur(cheminDescripteurs);

        // Formatage du prix avec le symbole €
        std::string prixAvecEuro = std::to_string(image.getPrix()) + " €";
        QString prix = QString::fromStdString(prixAvecEuro);

        // Affichage du prix de l'image
        QString details = QString("Le prix de l'image numéro %1 est : %2")
                              .arg(numeroImage)
                              .arg(prix);
        QMessageBox::information(this, "Résultat", details);
    } else {
        qDebug() << "L'utilisateur a annulé la saisie.";
    }
}*/

void BiblioWindow::on_pushButtonRechercherp_clicked() {
    bool ok;
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    int numeroImage = QInputDialog::getInt(this, tr("Recherche de Prix"),
                                           tr("Saisissez le numéro de l'image pour avoir son prix :"),
                                           1, 1, ui->AffichageBiblio->count(), 1, &ok);  // L'index commence à 1
    if (ok) {
        // Récupération de l'élément correspondant au numéro
        QListWidgetItem *item = ui->AffichageBiblio->item(numeroImage - 1);
        if (!item) {
            QMessageBox::warning(this, "Erreur", "Aucune image correspondant à ce numéro !");
            return;
        }

        // Extraction du titre de l'image depuis le texte de l'élément
        QString itemText = item->text();
        QString titreCherche = itemText.section('.', 1).trimmed(); // Extrait le texte après le numéro et le point
        qDebug() << "Titre extrait : " << titreCherche;

        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();

        // Association des descripteurs pour récupérer le prix
        image.associerDescripteur(cheminDescripteurs);

        // Formatage du prix avec le symbole €
        std::string prixAvecEuro = std::to_string(image.getPrix()) + " €";
        QString prix = QString::fromStdString(prixAvecEuro);

        // Affichage du prix de l'image
        QString details = QString("Le prix de l'image numéro %1 est : %2")
                              .arg(numeroImage)
                              .arg(prix);
        QMessageBox::information(this, "Résultat", details);
    } else {
        qDebug() << "L'utilisateur a annulé la saisie.";
    }
}



void BiblioWindow::mettreAJourCompteurImages() {
    int nombreImages = ui->AffichageBiblio->count(); // Obtenir le nombre d'images du QListWidget
    ui->labelImageCount->setText(QString("Nombre d'images : %1").arg(nombreImages));
}


void BiblioWindow::setUserLogin(const QString &login) {
    userLogin = login;
}

void BiblioWindow::on_pushButtonSousListePrix_clicked(){
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }
    qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

    // Chemin du fichier descripteurs
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

    // Création de l'instance Image
    Image image;
    image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
    // Association des descripteurs
    image.associerDescripteur(cheminDescripteurs);

    // Formatage du prix avec le symbole €
    std::string prixAvecEuro = std::to_string(image.getPrix()) + " €";



    double prix = image.getPrix();
    std::string source = image.getSource();
    std::string type = image.getType();
    std::string titre = image.getTitre();
    int nbTraitements = image.getnbTraitementPossible();

    if (prix < 10 && prix !=0) {
        // Formater et afficher le prix dans un message
        QString message = QString(
                              "Titre : %1\n"
                              "Source : %2\n"
                              "Type : %3\n"
                              ).arg(QString::fromStdString(titre))
                              .arg(QString::fromStdString(source))
                              .arg(QString::fromStdString(type));
        QMessageBox::information(this, "Prix de l'image", message);
        // Affichage des détails dans une boîte de message
    }
    else if (prix >=10 ){
        // Formater et afficher le prix dans un message
        QString message = QString(
                              "Titre : %1\n"
                              "Source : %2\n"
                              "Type : %3\n"
                              "Nombre de traitements possibles : %4"
                              ).arg(QString::fromStdString(titre))
                              .arg(QString::fromStdString(source))
                              .arg(QString::fromStdString(type))
                              .arg(nbTraitements);

        QMessageBox::information(this, "Détails de l'image", message);

    }
    else if (prix ==0 ){
        // Formater et afficher le prix dans un message
        QString message = QString(
                              "Titre : %1\n"
                              "Source : %2\n"

                              ).arg(QString::fromStdString(titre))
                              .arg(QString::fromStdString(source));

        QMessageBox::information(this, "Détails de l'image", message);

    }

}




void BiblioWindow::on_triprix_clicked()
{
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    // Récupérer tous les items du widget
    QList<QListWidgetItem*> items = ui->AffichageBiblio->findItems("*", Qt::MatchWildcard);

    if (items.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun élément trouvé dans le widget.");
        return;
    }

    // Liste pour stocker les objets Image créés
    std::vector<Image> images;

    // Vecteurs pour stocker les titres, prix et chemins des images
    std::vector<std::string> titres;
    std::vector<int> prix;
    std::vector<QString> cheminsImages;  // Ajouter un vecteur pour stocker les chemins des images

    // Récupérer le chemin du dossier à partir de la première image (le dossier est le même pour toutes)
    QString dossierImage = QFileInfo(items[0]->data(Qt::UserRole).toString()).absolutePath();
    qDebug() << "Dossier d'images : " << dossierImage;

    for (QListWidgetItem* item : items) {
        QVariant userData = item->data(Qt::UserRole);
        QString selectedImagePath = userData.toString(); // Récupérer le chemin du fichier

        // Vérifier si le chemin est valide
        if (selectedImagePath.isEmpty() || !QFile::exists(selectedImagePath)) {
            qDebug() << "Chemin de fichier invalide ou fichier inexistant : " << selectedImagePath;
            continue;
        }

        // Créer une instance de l'objet Image
        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
        qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

        try {
            // Associer les descripteurs à partir du chemin
            image.associerDescripteur(cheminDescripteurs);
        } catch (const std::exception& e) {
            qDebug() << "Erreur lors de l'association des descripteurs pour : " << selectedImagePath
                     << "Erreur : " << e.what();
            continue;
        }

        // Ajouter l'image à la liste
        images.push_back(image);

        // Ajouter le titre, le prix et le chemin à leurs vecteurs respectifs
        titres.push_back(image.getTitre());
        prix.push_back(image.getPrix());
        cheminsImages.push_back(selectedImagePath);  // Ajouter le chemin de l'image
    }

    // Tri à bulles sur les prix (et réarrangement des titres et chemins associés)
    for (int i = 0; i < prix.size() - 1; i++) {
        for (int j = 0; j < prix.size() - i - 1; j++) {
            if (prix[j] > prix[j + 1]) {
                // Échanger les prix
                std::swap(prix[j], prix[j + 1]);
                // Échanger les titres associés
                std::swap(titres[j], titres[j + 1]);
                // Échanger les chemins associés
                std::swap(cheminsImages[j], cheminsImages[j + 1]);
            }
        }
    }

    // Effacer l'affichage actuel du QListWidget
    ui->AffichageBiblio->clear();

    // Définir la taille des icônes comme dans la méthode `loadImagesIntoList`
    const QSize iconSize(150, 150);

    // Réafficher les images dans l'ordre trié
    for (int i = 0; i < titres.size(); ++i) {
        QString imagePath = cheminsImages[i];
        QString title = QString::fromStdString(titres[i]);

        // Créer un nouvel item avec le titre de l'image
        QListWidgetItem* newItem = new QListWidgetItem(title);

        // Charger l'image en tant qu'icône
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // Redimensionner l'image pour l'affichage
            pixmap = pixmap.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            newItem->setIcon(QIcon(pixmap));
            newItem->setSizeHint(iconSize);
        } else {
            qDebug() << "Impossible de charger l'image : " << imagePath;
        }

        // Associer à cet item le chemin de l'image dans UserRole
        newItem->setData(Qt::UserRole, imagePath);

        // Ajouter l'item au widget
        ui->AffichageBiblio->addItem(newItem);
    }

    // Afficher un résumé des images réarrangées
    QString summary = "Images réarrangées :\n";
    for (size_t i = 0; i < titres.size(); ++i) {
        summary += QString("Titre : %1, Prix : %2 €\n")
                       .arg(QString::fromStdString(titres[i]))
                       .arg(prix[i]);
    }

    QMessageBox::information(this, "Images réarrangées", summary);

    // Vérifier les titres et prix dans la console après réarrangement
    qDebug() << "Titres et prix après réarrangement :";
    for (size_t i = 0; i < titres.size(); ++i) {
        qDebug() << QString::fromStdString(titres[i]) << " - " << prix[i] << "€";
    }
}







void BiblioWindow::on_trinbtraitements_clicked()
{
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    // Récupérer tous les items du widget
    QList<QListWidgetItem*> items = ui->AffichageBiblio->findItems("*", Qt::MatchWildcard);

    if (items.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun élément trouvé dans le widget.");
        return;
    }

    // Liste pour stocker les objets Image créés
    std::vector<Image> images;

    // Vecteurs pour stocker les titres, nb de traitements et chemins des images
    std::vector<std::string> titres;
    std::vector<int> nbTraitements; // Vecteur pour stocker le nombre de traitements possibles
    std::vector<QString> cheminsImages;  // Ajouter un vecteur pour stocker les chemins des images

    // Récupérer le chemin du dossier à partir de la première image (le dossier est le même pour toutes)
    QString dossierImage = QFileInfo(items[0]->data(Qt::UserRole).toString()).absolutePath();
    qDebug() << "Dossier d'images : " << dossierImage;

    for (QListWidgetItem* item : items) {
        QVariant userData = item->data(Qt::UserRole);
        QString selectedImagePath = userData.toString(); // Récupérer le chemin du fichier

        // Vérifier si le chemin est valide
        if (selectedImagePath.isEmpty() || !QFile::exists(selectedImagePath)) {
            qDebug() << "Chemin de fichier invalide ou fichier inexistant : " << selectedImagePath;
            continue;
        }

        // Créer une instance de l'objet Image
        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
        qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

        try {
            // Associer les descripteurs à partir du chemin
            image.associerDescripteur(cheminDescripteurs);
        } catch (const std::exception& e) {
            qDebug() << "Erreur lors de l'association des descripteurs pour : " << selectedImagePath
                     << "Erreur : " << e.what();
            continue;
        }

        // Ajouter l'image à la liste
        images.push_back(image);

        // Ajouter le titre, le nombre de traitements possibles et le chemin à leurs vecteurs respectifs
        titres.push_back(image.getTitre());
        nbTraitements.push_back(image.getnbTraitementPossible()); // Remplacer le prix par nbTraitements
        cheminsImages.push_back(selectedImagePath);  // Ajouter le chemin de l'image
    }

    // Tri à bulles sur le nombre de traitements possibles (et réarrangement des titres et chemins associés)
    for (int i = 0; i < nbTraitements.size() - 1; i++) {
        for (int j = 0; j < nbTraitements.size() - i - 1; j++) {
            if (nbTraitements[j] > nbTraitements[j + 1]) {
                // Échanger les nombres de traitements possibles
                std::swap(nbTraitements[j], nbTraitements[j + 1]);
                // Échanger les titres associés
                std::swap(titres[j], titres[j + 1]);
                // Échanger les chemins associés
                std::swap(cheminsImages[j], cheminsImages[j + 1]);
            }
        }
    }

    // Effacer l'affichage actuel du QListWidget
    ui->AffichageBiblio->clear();

    // Définir la taille des icônes comme dans la méthode `loadImagesIntoList`
    const QSize iconSize(150, 150);

    // Réafficher les images dans l'ordre trié
    for (int i = 0; i < titres.size(); ++i) {
        QString imagePath = cheminsImages[i];
        QString title = QString::fromStdString(titres[i]);

        // Créer un nouvel item avec le titre de l'image
        QListWidgetItem* newItem = new QListWidgetItem(title);

        // Charger l'image en tant qu'icône
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // Redimensionner l'image pour l'affichage
            pixmap = pixmap.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            newItem->setIcon(QIcon(pixmap));
            newItem->setSizeHint(iconSize);
        } else {
            qDebug() << "Impossible de charger l'image : " << imagePath;
        }

        // Associer à cet item le chemin de l'image dans UserRole
        newItem->setData(Qt::UserRole, imagePath);

        // Ajouter l'item au widget
        ui->AffichageBiblio->addItem(newItem);
    }

    // Afficher un résumé des images réarrangées
    QString summary = "Images réarrangées :\n";
    for (size_t i = 0; i < titres.size(); ++i) {
        summary += QString("Titre : %1, Nombre de traitements : %2\n")
                       .arg(QString::fromStdString(titres[i]))
                       .arg(nbTraitements[i]);
    }

    QMessageBox::information(this, "Images réarrangées", summary);

    // Vérifier les titres et nb de traitements dans la console après réarrangement
    qDebug() << "Titres et nb de traitements après réarrangement :";
    for (size_t i = 0; i < titres.size(); ++i) {
        qDebug() << QString::fromStdString(titres[i]) << " - " << nbTraitements[i];
    }
}


void BiblioWindow::on_souslistetype_clicked()
{
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }
    qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

    // Chemin du fichier descripteurs
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

    // Création de l'instance Image
    Image image;
    image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
    // Association des descripteurs
    image.associerDescripteur(cheminDescripteurs);

    double prix = image.getPrix();
    std::string source = image.getSource();
    std::string type = image.getType();
    std::string titre = image.getTitre();
    int nbTraitements = image.getnbTraitementPossible();
    type.erase(0, type.find_first_not_of(' '));
    // Si le type est "gris", afficher uniquement titre, source, et type
    if (type == "gris") {
        // Formater et afficher le message avec titre, source, et type
        QString message = QString(
                              "Titre : %1\n"
                              "Source : %2\n"
                              "Type : %3\n"
                              ).arg(QString::fromStdString(titre))
                              .arg(QString::fromStdString(source))
                              .arg(QString::fromStdString(type));
        QMessageBox::information(this, "Détails de l'image", message);
    }
    else if (type == "couleur"){
        // Sinon, afficher aussi le nombre de traitements possibles
        QString message = QString(
                              "Titre : %1\n"
                              "Source : %2\n"
                              "Type : %3\n"
                              "Nombre de traitements possibles : %4"
                              ).arg(QString::fromStdString(titre))
                              .arg(QString::fromStdString(source))
                              .arg(QString::fromStdString(type))
                              .arg(nbTraitements);
        QMessageBox::information(this, "Détails de l'image", message);
    }
}


