// Inclusion des fichiers d'en-tête
#include "mainwindow.h" // Inclut la fenêtre principale
#include "connexionwindow.h" // Inclut la fenêtre de connexion

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include "bibliowindow.h"
#include "ui_bibliowindow.h"
#include "Image.hpp" // Inclut la classe Image pour gérer les images
#include <QFileDialog> // Pour ouvrir des dialogues de fichier
#include <QListWidgetItem> // Pour les éléments de la liste dans la fenêtre
#include <QPixmap> // Pour charger et afficher des images
#include <QIcon> // Pour gérer les icônes dans l'interface
#include <QMessageBox> // Pour afficher des boîtes de message
#include <QDir> // Pour manipuler les répertoires et fichiers
#include <QDebug> // Pour afficher des messages de débogage
#include <QMouseEvent> // Pour gérer les événements de la souris
#include <QScreen> // Pour obtenir des informations sur l'écran
#include "Library.hpp" // Inclut la classe Library pour la gestion des images
#include <QInputDialog> // Pour afficher des boîtes de dialogue d'entrée

#include <QMessageBox> // Pour afficher des boîtes de message
#include <QGraphicsPixmapItem> // Pour afficher des images dans une scène graphique
#include <stdexcept> // Pour gérer les exceptions
#include <QTextStream> // Pour gérer le flux de texte pour les fichiers

#include <fstream> // Pour manipuler les fichiers
#include <sstream> // Pour convertir des chaînes de caractères
#include <string> // Pour manipuler des chaînes
#include <vector> // Pour utiliser des vecteurs

// Déclaration de la constante représentant le chemin du fichier par défaut
const QString BiblioWindow::DEFAULT_FILE_PATH = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";

// Constructeur de la classe BiblioWindow
BiblioWindow::BiblioWindow(const QString &login, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BiblioWindow), LoginUtilisateur(login)
{
    // Initialisation de l'interface utilisateur
    ui->setupUi(this);

    // Paramétrage de la fenêtre principale
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

    // Afficher le login de l'utilisateur connecté dans les logs
    qDebug() << "Utilisateur connecté : " << LoginUtilisateur;

    // Configuration du QListWidget pour afficher les images en mode mosaïque
    ui->AffichageBiblio->setViewMode(QListWidget::IconMode);
    ui->AffichageBiblio->setResizeMode(QListWidget::Adjust);
    ui->AffichageBiblio->setMovement(QListWidget::Static);
    ui->AffichageBiblio->setSpacing(10);

    // Rendre certains boutons invisibles
    ui->TraitementButton->setVisible(false);
    ui->DetailsButton->setVisible(false);
    ui->pushButtonRechercherp->setVisible(true);

    // Initialisation du texte pour le nombre d'images
    ui->labelImageCount->setText("Nombre d'images : 0");

    // si le user normal est connecter il ne peut pas ajouter, modifier ou supprimer un descripteur
    // Pour "us-02-al"
    if (LoginUtilisateur == "us-02-al") {
        ui->actionAjouterDescripteur->setVisible(false);
        ui->actionModifierDescripteur->setVisible(false);
        ui->actionSupprimer_un_descripteur->setVisible(false);
    }

    // Connexions des slots aux signaux des widgets
    connect(ui->AffichageBiblio, &QListWidget::itemClicked, this, &BiblioWindow::on_AffichageBiblio_itemClicked);
    connect(ui->actionSupprimer_un_descripteur, &QAction::triggered, this, &BiblioWindow::on_actionSupprimerDescripteur_triggered);
    connect(ui->Deco, &QPushButton::clicked, this, &BiblioWindow::on_Deco_clicked);
    connect(ui->pushButtonSousListePrix, &QPushButton::clicked, this, &BiblioWindow::on_pushButtonSousListePrix_clicked);
    connect(ui->souslistetype, &QPushButton::clicked, this, &BiblioWindow::on_souslistetype_clicked);
    connect(ui->triprix, &QPushButton::clicked, this, &BiblioWindow::on_triprix_clicked);
    connect(ui->trinbtraitements, &QPushButton::clicked, this, &BiblioWindow::on_trinbtraitements_clicked);
}

// Destructeur de la classe
BiblioWindow::~BiblioWindow()
{
    delete ui;
}

// Fonction pour gérer la déconnexion de l'utilisateur
void BiblioWindow::on_Deco_clicked()
{
    gestionUtilisateur.deconnexion();
    this->close();

    // Ouvre la fenêtre de connexion
    connexionWindow = std::make_unique<ConnexionWindow>();
    connexionWindow->show();
}

// Fonction pour charger la bibliothèque d'images à partir d'un fichier
void BiblioWindow::on_ChargeBoutton_clicked()
{
    // Ouvre une boîte de dialogue pour sélectionner un fichier
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionnez un fichier", "", "Text Files (*.txt);;All Files (*)");

    if (!filePath.isEmpty()) {
        cheminBiblio = filePath;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
            return;
        }

        // Répertoire des images
        QString imageDirectory = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Bibliotheque";
        ui->AffichageBiblio->clear();
        QStringList missingImages;

        QTextStream in(&file);
        int compteur = 1;

        // Lire chaque ligne du fichier
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');

            // Vérifier qu'il y a assez de données
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

        // Fermeture du fichier
        file.close();

        // Affichage d'un message si certaines images sont manquantes
        if (!missingImages.isEmpty()) {
            QString missingMessage = "Les images suivantes n'ont pas été trouvées dans le répertoire :\n" + missingImages.join("\n");
            QMessageBox::warning(this, "Images Manquantes", missingMessage);
        }

        // Message de confirmation après le chargement
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

// Fonction pour charger le fichier par défaut
void BiblioWindow::loadDefaultFile(const QString &userLogin)
{
    // Définir le login de l'utilisateur
    setUserLogin(userLogin);

    // Chemin du fichier par défaut
    QString filePath = DEFAULT_FILE_PATH;
    QFile file(filePath);

    // Vérifier si le fichier s'ouvre correctement
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier par défaut.");
        return;
    }

    // Répertoire des images
    QString imageDirectory = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Bibliotheque";
    ui->AffichageBiblio->clear();
    QStringList missingImages;

    QTextStream in(&file);
    int compteur = 1;

    // Lire chaque ligne du fichier
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Ignorer les lignes vides
        if (line.isEmpty()) {
            continue;
        }

        QStringList fields = line.split(',');
        if (fields.size() < 5) {
            continue; // Ignorer les lignes mal formatées
        }

        // Extraire les informations de l'image
        QString imageName = fields[1].trimmed();
        QString accessType = fields[4].trimmed();
        QString imagePath = imageDirectory + "/" + imageName;

        // Vérifier les droits d'accès en fonction du login
        bool hasAccess = (userLogin == "us-02-al" && accessType == "O") ||
                         (userLogin == "ad-01-ao" && (accessType == "O" || accessType == "L"));

        if (hasAccess) {
            if (QFileInfo::exists(imagePath)) {
                // Ajouter l'image à la liste
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

    file.close();

    // Affichage d'un message si des images sont manquantes
    if (!missingImages.isEmpty()) {
        QString missingMessage = "Les images suivantes n'ont pas été trouvées dans le répertoire :\n" + missingImages.join("\n");
        QMessageBox::warning(this, "Images Manquantes", missingMessage);
    }

    // Affichage d'une confirmation après chargement
    QMessageBox::information(this, "Chargement", "Bibliothèque chargée et images affichées.");
    mettreAJourCompteurImages();
}

// Fonction pour gérer le clic sur un élément de la bibliothèque
void BiblioWindow::on_AffichageBiblio_itemClicked(QListWidgetItem *item)
{
    // Récupérer le chemin de l'image sélectionnée
    QString filePath = item->data(Qt::UserRole).toString();
    qDebug() << "Image cliquée : " << filePath;

    selectedImagePath = filePath;

    // Afficher les boutons de traitement et détails
    ui->TraitementButton->setVisible(true);
    ui->DetailsButton->setVisible(true);
}

// Fonction pour gérer le clic sur le bouton de traitement
void BiblioWindow::on_TraitementButton_clicked()
{
    if (!selectedImagePath.isEmpty()) {
        qDebug() << "Traitement de l'image : " << selectedImagePath;

        // Créer et afficher une fenêtre de traitement pour l'image
        if (!mainWindow) {
            mainWindow = std::make_unique<MainWindow>(LoginUtilisateur, selectedImagePath, this);
            mainWindow->show();
            mainWindow->raise();
            mainWindow->activateWindow();
            mainWindow->loadAndDisplayImage(selectedImagePath);
        } else {
            mainWindow->loadAndDisplayImage(selectedImagePath);
            mainWindow->show();
        }

        // Masquer la bibliothèque
        this->hide();
    } else {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
    }
}

// Fonction pour gérer les événements de clic souris
void BiblioWindow::mousePressEvent(QMouseEvent *event)
{
    // Désélectionner l'image si le clic est en dehors de la bibliothèque
    if (!ui->AffichageBiblio->geometry().contains(event->pos())) {
        ui->AffichageBiblio->clearSelection();
        selectedImagePath.clear();
        ui->TraitementButton->setVisible(false);
        ui->DetailsButton->setVisible(false);
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

        image.titre = titreCherche.toStdString();
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


// Fonction pour définir le login de l'utilisateur
void BiblioWindow::setUserLogin(const QString &login) {
    userLogin = login;  // Attribuer la valeur du login à la variable membre userLogin
}


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
    if (cheminBiblio.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier n'a été chargé.");
        return;
    }

    // Lire le fichier chargé
    QFile fichierDescripteur(cheminBiblio);
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier descripteurs.");
        return;
    }

    // Créer une liste pour contenir les descripteurs disponibles
    QStringList listeDescripteurs;
    QTextStream fluxLecture(&fichierDescripteur);

    QStringList lignesFichier; // Pour stocker toutes les lignes du fichier
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine().trimmed();
        if (!ligne.isEmpty()) {
            lignesFichier.append(ligne);

            // Ajouter les titres et numéros à la liste pour l'utilisateur
            QStringList champs = ligne.split(",");
            if (champs.size() >= 2) {
                QString titre = champs[0].trimmed();
                int numero = champs[2].trimmed().toInt();
                listeDescripteurs.append(titre + " (Numéro: " + QString::number(numero) + ")");
            }
        }
    }
    fichierDescripteur.close();

    // Vérifier si la liste est vide
    if (listeDescripteurs.isEmpty()) {
        QMessageBox::information(this, "Aucun descripteur", "Aucun descripteur disponible à supprimer.");
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

    // Supprimer la ligne correspondante dans le fichier
    QString contenuModifie;
    bool descripteurTrouve = false;

    for (const QString &ligne : lignesFichier) {
        QStringList champs = ligne.split(",");
        if (champs.size() >= 2) {
            QString titre = champs[0].trimmed();
            int numero = champs[2].trimmed().toInt();

            // Identifier la ligne à supprimer
            if (titre == titreChoisi && numero == numeroChoisi) {
                descripteurTrouve = true;

                // Supprimer l'image associée
                QString cheminImage = "/media/sf_PROJETC_LMLTM/Bibliotheque/" + champs[1].trimmed();
                if (QFile::exists(cheminImage)) {
                    if (!QFile::remove(cheminImage)) {
                        QMessageBox::warning(this, "Erreur", "Impossible de supprimer l'image associée.");
                    }
                }
                continue; // Ne pas ajouter cette ligne au contenu modifié
            }
        }
        contenuModifie += ligne + "\n"; // Ajouter les autres lignes
    }

    // Si le descripteur a été trouvé et supprimé
    if (descripteurTrouve) {
        // Réécrire le fichier avec les modifications
        if (fichierDescripteur.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream fluxEcriture(&fichierDescripteur);
            fluxEcriture << contenuModifie;
            fichierDescripteur.close();

            QMessageBox::information(this, "Succès", "Le descripteur et son image associée ont été supprimés avec succès !");

        } else {
            QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier descripteurs.");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce titre et numéro.");
    }
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
        QString title = QString("%1. %2").arg(i + 1).arg(QString::fromStdString(titres[i]));

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
        QString title = QString("%1. %2").arg(i + 1).arg(QString::fromStdString(titres[i]));


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

