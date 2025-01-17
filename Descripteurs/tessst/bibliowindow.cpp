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
#include "Library.hpp"
#include <QInputDialog>

#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QTextStream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

    // si le user normal est connecter il ne peut pas ajouter, modifier ou supprimer un descripteur
    // Pour "us-02-al"
    if (LoginUtilisateur == "us-02-al") {
        ui->actionAjouterDescripteur->setVisible(false);
        ui->actionModifierDescripteur->setVisible(false);
        ui->actionSupprimer_un_descripteur->setVisible(false);
    }
    // Connecter le clic sur une image
    connect(ui->AffichageBiblio, &QListWidget::itemClicked, this, &BiblioWindow::on_AffichageBiblio_itemClicked);
    connect(ui->Deco, &QPushButton::clicked, this, &BiblioWindow::on_Deco_clicked);

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
        cheminBiblio = filePath; // Met à jour le chemin actuel

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
            return;
        }

        QString imageDirectory = "/media/sf_PROJETC_LMLTM/Bibliotheque";
        ui->AffichageBiblio->clear(); // Nettoyer l'affichage avant de charger les nouvelles images
        QStringList missingImages;

        QTextStream in(&file);
        int compteur = 1;

        // Lecture du fichier ligne par ligne
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');

            if (fields.size() >= 5) {
                QString imageName = fields[1].trimmed();
                QString accessType = fields[4].trimmed();
                QString imagePath = imageDirectory + "/" + imageName;

                // Vérification des droits d'accès pour l'utilisateur
                if ((LoginUtilisateur == "us-02-al" && accessType == "O") ||
                    (LoginUtilisateur == "ad-01-ao" && (accessType == "O" || accessType == "L"))) {

                    if (QFileInfo::exists(imagePath)) {
                        // Ajouter l'image à l'affichage
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

        // Afficher un avertissement si des images sont manquantes
        if (!missingImages.isEmpty()) {
            QString missingMessage = "Les images suivantes n'ont pas été trouvées dans le répertoire :\n" + missingImages.join("\n");
            QMessageBox::warning(this, "Images Manquantes", missingMessage);
        }

        QMessageBox::information(this, "Chargement", "Bibliothèque chargée et images affichées.");
        mettreAJourCompteurImages(); // Met à jour le compteur d'images affichées
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
    //setUserLogin(userLogin); // Assurez-vous que le login est défini ici

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


/*void BiblioWindow::setUserLogin(const QString &login) {
    userLogin = login;
}*/

// -----------------------------------------------------------------------------------------

// *************************** Ajout Menu Descripteur *************************************

// -----------------------------------------------------------------------------------------

// Ajouter
void BiblioWindow::on_actionAjouterDescripteur_triggered() {
    // Menu pour proposer les différentes options d'ajout
    QStringList options = {
        "Ajouter au fichier déjà ouvert",
        "Ajouter dans un fichier existant",
        "Créer un nouveau fichier",
        "Annuler"
    };

    // Récupération du choix de l'utilisateur
    bool ok;
    QString choix = QInputDialog::getItem(this, "Choix d'ajout", "Que voulez-vous faire ?", options, 0, false, &ok);
    if (!ok || choix == "Annuler") {
        QMessageBox::information(this, "Annulé", "L'opération a été annulée.");
        return;
    }

    // Chemin du fichier où ajouter les données
    QString cheminFichier;
    QString cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst"; // Chemin où les fichiers sont enregistrés

    if (choix == "Ajouter au fichier déjà ouvert") {
        if (cheminBiblio.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun fichier ouvert actuellement.");
            return;
        }
        cheminFichier = cheminBiblio;
    } else if (choix == "Ajouter dans un fichier existant") {
        cheminFichier = QFileDialog::getOpenFileName(this, "Sélectionnez un fichier", cheminDescripteurs, "Text Files (*.txt);;All Files (*)");
        if (cheminFichier.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné.");
            return;
        }
    } else if (choix == "Créer un nouveau fichier") {
        // Demander le nom du nouveau fichier sans extension
        QString nomFichier = QInputDialog::getText(this, "Créer un fichier", "Entrez le nom du fichier (sans extension) :");
        if (nomFichier.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom du fichier est invalide.");
            return;
        }

        // Ajouter le chemin complet et l'extension .txt
        cheminFichier = cheminDescripteurs + "/" + nomFichier + ".txt";

        // Créer le fichier immédiatement
        QFile fichier(cheminFichier);
        if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
            return;
        }
        fichier.close(); // Fermer le fichier après sa création
        QMessageBox::information(this, "Fichier créé", "Le fichier a été créé avec succès !");
    }

    // Sélection de l'image source à ajouter
    QString cheminImageSource = QFileDialog::getOpenFileName(this,
                                                             "Sélectionnez une image à ajouter",
                                                             "",
                                                             "Images (*.png *.jpg *.jpeg *.bmp *.pgm *.CR2);;Tous les fichiers (*)");
    if (cheminImageSource.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucun fichier sélectionné !");
        return;
    }

    // Récupération des descripteurs pour l'image
    QString titre = QInputDialog::getText(this, "Ajouter un descripteur", "Titre de l'image :", QLineEdit::Normal, "", &ok);
    if (!ok || titre.isEmpty()) return;

    int numero = QInputDialog::getInt(this, "Ajouter un descripteur", "Numéro unique :", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    double prix = QInputDialog::getDouble(this, "Ajouter un descripteur", "Prix :", 0, 0, 10000, 2, &ok);
    if (!ok) return;

    QString acces = QInputDialog::getText(this, "Ajouter un descripteur", "Accès (O/L) :", QLineEdit::Normal, "O", &ok);
    if (!ok || acces.isEmpty()) return;

    QString type = QInputDialog::getText(this, "Ajouter un descripteur", "Type (couleur/gris) :", QLineEdit::Normal, "couleur", &ok);
    if (!ok || type.isEmpty()) return;

    int nbTraitement = QInputDialog::getInt(this, "Ajouter un descripteur", "Nombre de traitements possibles :", 1, 1, 100, 1, &ok);
    if (!ok) return;

    // Ajout des données dans le fichier sélectionné
    QFile fichierDescripteur(cheminFichier);
    if (fichierDescripteur.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream sortie(&fichierDescripteur);
        sortie << titre << ", " << QFileInfo(cheminImageSource).fileName() << ", " << numero << ", " << prix << ", "
               << acces << ", " << type << ", " << nbTraitement << ", " << numero << "\n";
        fichierDescripteur.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier cible !");
        return;
    }

    // Copier l'image dans le répertoire bibliothèque avec son nom d'origine
    QString cheminBibliotheque = "/media/sf_PROJETC_LMLTM/Bibliotheque";
    QString imageNomOriginal = QFileInfo(cheminImageSource).fileName();
    QString cheminImageDestination = cheminBibliotheque + "/" + imageNomOriginal;
    /*if (!QFile::copy(cheminImageSource, cheminImageDestination)) {
        QMessageBox::warning(this, "Erreur", "La copie de l'image dans la bibliothèque a échoué !");
        return;
    }*/

    // Confirmation de l'ajout
    QMessageBox::information(this, "Succès", "Le descripteur et l'image ont été ajoutés avec succès !");
}







// Modifier
void BiblioWindow::on_actionModifierDescripteur_triggered() {
    // Définir le chemin fixe des descripteurs
    QString cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst";
    QString cheminDescripteur = cheminBiblio.isEmpty() ? "Biblio_init.txt" : QFileInfo(cheminBiblio).fileName();
    QString cheminDescripteurComplet = cheminDescripteurs + "/" + cheminDescripteur;

    // Vérifier si on travaille sur Biblio_init
    if (cheminDescripteur == "Biblio_init.txt") {
        QString cheminModifie = cheminDescripteurs + "/Biblio_init_change.txt";
        if (!QFile::exists(cheminModifie)) {
            if (!QFile::copy(cheminDescripteurComplet, cheminModifie)) {
                QMessageBox::warning(this, "Erreur", "Impossible de copier Biblio_init.txt vers Biblio_init_change.txt !");
                return;
            }
        }
        cheminDescripteurComplet = cheminModifie;
    }

    // Charger les descripteurs du fichier
    QFile fichierDescripteur(cheminDescripteurComplet);
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier descripteurs !");
        return;
    }

    QStringList listeDescripteurs;
    QTextStream fluxLecture(&fichierDescripteur);
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine().trimmed();
        if (!ligne.isEmpty()) {
            listeDescripteurs.append(ligne.split(",")[1]); // Ajouter le nom de l'image
        }
    }
    fichierDescripteur.close();

    // Vérifier si la liste est vide
    if (listeDescripteurs.isEmpty()) {
        QMessageBox::information(this, "Aucun descripteur", "Il n'y a aucun descripteur disponible à modifier.");
        return;
    }

    // Afficher la liste déroulante pour choisir le descripteur à modifier
    bool ok;
    QString nomImageRecherche = QInputDialog::getItem(this, "Modifier un descripteur", 
                                                      "Sélectionnez un descripteur à modifier :", 
                                                      listeDescripteurs, 0, false, &ok);
    if (!ok || nomImageRecherche.isEmpty()) {
        QMessageBox::information(this, "Annulé", "Modification annulée.");
        return;
    }

    // Charger le fichier à nouveau pour trouver et modifier la ligne correspondante
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier descripteurs !");
        return;
    }

    QString contenu;
    QString ligneAModifier;
    fluxLecture.setDevice(&fichierDescripteur);
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine();
        if (ligne.contains(nomImageRecherche)) { // Rechercher la ligne contenant le nom de l'image
            ligneAModifier = ligne;
        } else {
            contenu += ligne + "\n";
        }
    }
    fichierDescripteur.close();

    if (ligneAModifier.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce nom d'image !");
        return;
    }

    // Récupérer les informations actuelles du descripteur pour modification
    QStringList elements = ligneAModifier.split(", ");
    if (elements.size() < 8) {
        QMessageBox::warning(this, "Erreur", "Le descripteur sélectionné est invalide !");
        return;
    }

    QString nouveauTitre = QInputDialog::getText(this, "Modifier un descripteur", "Nouveau titre :", QLineEdit::Normal, elements[0], &ok);
    if (!ok || nouveauTitre.isEmpty()) return;

    int nouveauNumero = QInputDialog::getInt(this, "Modifier un descripteur", "Nouveau numéro unique :", elements[2].toInt(), 0, 10000, 1, &ok);
    if (!ok) return;

    double nouveauPrix = QInputDialog::getDouble(this, "Modifier un descripteur", "Nouveau prix :", elements[3].toDouble(), 0, 10000, 2, &ok);
    if (!ok) return;

    QString nouvelAcces = QInputDialog::getText(this, "Modifier un descripteur", "Nouvel accès (O/L) :", QLineEdit::Normal, elements[4], &ok);
    if (!ok || nouvelAcces.isEmpty()) return;

    QString nouveauType = QInputDialog::getText(this, "Modifier un descripteur", "Nouveau type (couleur/gris) :", QLineEdit::Normal, elements[5], &ok);
    if (!ok || nouveauType.isEmpty()) return;

    int nouveauNbTraitement = QInputDialog::getInt(this, "Modifier un descripteur", "Nouveau nombre de traitements possibles :", elements[6].toInt(), 1, 100, 1, &ok);
    if (!ok) return;

    // Construire la nouvelle ligne du descripteur
    QString nouvelleLigne = nouveauTitre + ", " + elements[1] + ", " + QString::number(nouveauNumero) + ", "
                            + QString::number(nouveauPrix) + ", " + nouvelAcces + ", " + nouveauType + ", "
                            + QString::number(nouveauNbTraitement) + ", " + QString::number(nouveauNumero) + "\n";

    contenu += nouvelleLigne;

    // Écrire les modifications dans le fichier
    if (!fichierDescripteur.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier descripteurs !");
        return;
    }

    QTextStream fluxEcriture(&fichierDescripteur);
    fluxEcriture << contenu;
    fichierDescripteur.close();

    // Confirmation de la modification
    QMessageBox::information(this, "Succès", "Le descripteur a été modifié avec succès !");
}





// Supprimer
void BiblioWindow::on_actionSupprimerDescripteur_triggered() {
    // Créer une liste pour contenir les descripteurs disponibles
    QStringList listeDescripteurs;

    // Parcourir la liste des descripteurs pour les ajouter à la liste déroulante
    auto current = library.head;
    while (current) {
        QString descripteur = QString::fromStdString(current->data.getTitre()) +
                              " (Numéro: " + QString::number(current->data.getNumero()) + ")";
        listeDescripteurs.append(descripteur);
        current = current->next;
    }

    // Vérifier si la liste est vide
    if (listeDescripteurs.isEmpty()) {
        QMessageBox::information(this, "Aucun descripteur", "Il n'y a aucun descripteur disponible à supprimer.");
        return;
    }

    // Afficher la liste déroulante pour choisir le descripteur à supprimer
    bool ok;
    QString descripteurChoisi = QInputDialog::getItem(this, "Supprimer un descripteur", 
                                                      "Sélectionnez un descripteur à supprimer :", 
                                                      listeDescripteurs, 0, false, &ok);
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Suppression annulée.");
        return;
    }

    // Extraire le titre et le numéro du descripteur choisi
    QString titreChoisi = descripteurChoisi.section(" (", 0, 0);
    int numeroChoisi = descripteurChoisi.section("Numéro: ", 1, 1).remove(")").toInt();

    // Parcourir la liste pour trouver le descripteur correspondant
    current = library.head;
    std::shared_ptr<Library::INode> previous = nullptr;
    bool descripteurTrouve = false;

    while (current) {
        if (current->data.getNumero() == numeroChoisi) {
            descripteurTrouve = true;

            // Supprimer le fichier image associé
            std::string cheminImage = current->data.getSource();
            try {
                if (std::filesystem::exists(cheminImage)) {
                    std::filesystem::remove(cheminImage);
                } else {
                    QMessageBox::warning(this, "Avertissement", 
                                         "L'image associée au descripteur n'existe pas ou a déjà été supprimée.");
                }
            } catch (const std::exception &e) {
                QMessageBox::warning(this, "Erreur", 
                                     QString("Impossible de supprimer l'image associée : %1").arg(e.what()));
            }

            // Supprimer le descripteur de la liste chaînée
            if (previous) {
                previous->next = current->next;
            } else {
                library.head = current->next;
            }

            QMessageBox::information(this, "Succès", "Le descripteur et son image associée ont été supprimés !");
            return;
        }

        previous = current;
        current = current->next;
    }

    // Si aucun descripteur n'a été trouvé
    if (!descripteurTrouve) {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce numéro !");
    }
}


