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
    ui->SousListes->hide();
    ui->Tris->hide();
    ui->SousListesButton->hide();    // si le user normal est connecter il ne peut pas ajouter, modifier ou supprimer un descripteur
    // Pour "us-02-al"
    if (LoginUtilisateur == "us-02-al") {
        ui->actionAjouterDescripteur->setVisible(false);
        ui->actionModifierDescripteur->setVisible(false);
        ui->actionSupprimer_un_descripteur->setVisible(false);
         ui->menubar->setVisible(false);
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
    ui->SousListesButton->show();
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
        ui->SousListesButton->hide();
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
    QString cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst";
    QString cheminDescripteur;
    
    QStringList options = {
        "Ajouter dans un fichier existant",
        "Créer un nouveau fichier"
    };
    
    bool ok;
    QString choix = QInputDialog::getItem(this, "Choix d'ajout", "Que voulez-vous faire ?", options, 0, false, &ok);
    if (!ok) return;
    
    if (choix == "Ajouter dans un fichier existant") {
        cheminDescripteur = QFileDialog::getOpenFileName(this, "Sélectionnez un fichier", cheminDescripteurs, "Text Files (*.txt);;All Files (*)");
        if (cheminDescripteur.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné.");
            return;
        }
        if (QFileInfo(cheminDescripteur).fileName() == "Biblio_init.txt") {
            QString cheminModifie = cheminDescripteurs + "/Biblio_init_change.txt";
            if (!QFile::exists(cheminModifie)) {
                if (!QFile::copy(cheminDescripteur, cheminModifie)) {
                    QMessageBox::warning(this, "Erreur", "Impossible de copier Biblio_init.txt vers Biblio_init_change.txt !");
                    return;
                }
            }
            cheminDescripteur = cheminModifie;
        }
    } else {
        QString nomFichier = QInputDialog::getText(this, "Créer un fichier", "Entrez le nom du fichier (sans extension) :");
        if (nomFichier.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom du fichier est invalide.");
            return;
        }
        cheminDescripteur = cheminDescripteurs + "/" + nomFichier + ".txt";
        QFile fichier(cheminDescripteur);
        if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
            return;
        }
        fichier.close();
        QMessageBox::information(this, "Fichier créé", "Le fichier a été créé avec succès !");
    }

    QString cheminImageSource = QFileDialog::getOpenFileName(this, "Sélectionnez une image à ajouter", "", "Images (*.png *.jpg *.jpeg *.bmp *.pgm *.CR2);;Tous les fichiers (*)");
    if (cheminImageSource.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucun fichier sélectionné !");
        return;
    }

    QString cheminBibliotheque = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Bibliotheque";
    QString cheminImageDestination = cheminBibliotheque + "/" + QFileInfo(cheminImageSource).fileName();

    if (!QFile::copy(cheminImageSource, cheminImageDestination)) {
        QMessageBox::warning(this, "Erreur", "La copie de l'image dans la bibliothèque a échoué !");
        return;
    }

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

    QFile fichierDescripteur(cheminDescripteur);
    if (fichierDescripteur.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream fluxLecture(&fichierDescripteur);
        fichierDescripteur.seek(fichierDescripteur.size());  // Aller à la fin du fichier
        
        // Vérifier si le fichier se termine par une nouvelle ligne
        fichierDescripteur.seek(qMax(0LL, fichierDescripteur.size() - 1));
        QChar dernierCaractere;
        fluxLecture >> dernierCaractere;

        // Ajouter une nouvelle ligne seulement si nécessaire
        if (dernierCaractere != '\n') {
            fluxLecture << "\n";
        }

        // Ajouter le nouveau descripteur sur une nouvelle ligne
        fluxLecture << titre << ", " << QFileInfo(cheminImageSource).fileName() << ", " 
                    << numero << ", " << prix << ", " << acces << ", " << type << ", " 
                    << nbTraitement << ", " << numero << "\n";
        
        fichierDescripteur.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier cible !");
        return;
    }

    QMessageBox::information(this, "Succès", "Le descripteur et l'image ont été ajoutés avec succès !");
}




// Modifier
void BiblioWindow::on_actionModifierDescripteur_triggered()
{
    // Définir le chemin fixe des descripteurs
    QString cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst";

    // Si cheminBiblio est vide, on considère qu'on veut modifier Biblio_init.txt
    // Sinon, on récupère simplement le nom du fichier indiqué par cheminBiblio
    QString cheminDescripteur = cheminBiblio.isEmpty() 
            ? "Biblio_init.txt" 
            : QFileInfo(cheminBiblio).fileName();

    QString cheminDescripteurComplet = cheminDescripteurs + "/" + cheminDescripteur;

    // ----- GESTION DU BIBLIO_INIT.TXT -----
    // Si l'utilisateur cherche à modifier Biblio_init.txt,
    // on bascule vers Biblio_init_change.txt (en le créant s'il n'existe pas)
    if (cheminDescripteur == "Biblio_init.txt") {
        // Nom du fichier "change"
        QString cheminModifie = cheminDescripteurs + "/Biblio_init_change.txt";

        // S'il n'existe pas, on essaie de le créer en copiant l'original
        if (!QFile::exists(cheminModifie)) {
            if (!QFile::copy(cheminDescripteurComplet, cheminModifie)) {
                QMessageBox::warning(
                    this,
                    "Erreur",
                    "Impossible de copier Biblio_init.txt vers Biblio_init_change.txt !\n"
                    "La modification est annulée."
                );
                return;
            }
        }

        // Désormais, on travaille sur le fichier "change" plutôt que sur l'init
        cheminDescripteurComplet = cheminModifie;
    }

    // ----- OUVERTURE DU FICHIER CIBLE (INIT_CHANGE OU AUTRE FICHIER) -----

    // Charger en lecture pour récupérer la liste des descripteurs existants
    QFile fichierDescripteur(cheminDescripteurComplet);
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", 
                             "Impossible d'ouvrir le fichier descripteurs :\n" 
                             + cheminDescripteurComplet);
        return;
    }

    QStringList listeDescripteurs;
    QTextStream fluxLecture(&fichierDescripteur);
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine().trimmed();
        if (!ligne.isEmpty()) {
            QStringList parties = ligne.split(",");
            if (parties.size() > 1) {
                listeDescripteurs.append(parties[1].trimmed());
            }
        }
    }
    fichierDescripteur.close();

    // Vérifier si on a bien des descripteurs disponibles
    if (listeDescripteurs.isEmpty()) {
        QMessageBox::information(this, "Aucun descripteur", 
                                 "Il n'y a aucun descripteur disponible à modifier.");
        return;
    }

    // Demander à l'utilisateur quel descripteur il souhaite modifier
    bool ok = false;
    QString nomImageRecherche = QInputDialog::getItem(
                this,
                "Modifier un descripteur",
                "Sélectionnez un descripteur à modifier :",
                listeDescripteurs,
                0,
                false,
                &ok
    );
    if (!ok || nomImageRecherche.isEmpty()) {
        QMessageBox::information(this, "Annulé", "Modification annulée.");
        return;
    }

    // ----- RECHARGER LE FICHIER POUR IDENTIFIER LA LIGNE A MODIFIER -----
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", 
                             "Impossible d'ouvrir le fichier descripteurs :\n" 
                             + cheminDescripteurComplet);
        return;
    }

    QString contenu;
    QString ligneAModifier;
    fluxLecture.setDevice(&fichierDescripteur);

    // On va relire ligne par ligne : on stocke tout sauf la ligne à modifier
    while (!fluxLecture.atEnd()) {
        QString ligne = fluxLecture.readLine();
        if (ligne.contains(nomImageRecherche)) {
            ligneAModifier = ligne;
        } else {
            contenu += ligne + "\n";
        }
    }
    fichierDescripteur.close();

    if (ligneAModifier.isEmpty()) {
        QMessageBox::warning(this, "Erreur", 
                             "Aucun descripteur trouvé avec ce nom d'image !");
        return;
    }

    // ----- EXTRAIRE LES CHAMPS ET DEMANDER LES NOUVELLES VALEURS -----
    QStringList elements = ligneAModifier.split(", ");
    if (elements.size() < 8) {
        QMessageBox::warning(this, "Erreur", 
                             "Le descripteur sélectionné est invalide ou mal formé !");
        return;
    }

    // Champ : Titre
    QString nouveauTitre = QInputDialog::getText(
                this, "Modifier un descripteur", 
                "Nouveau titre :", 
                QLineEdit::Normal, 
                elements[0], 
                &ok
    );
    if (!ok || nouveauTitre.isEmpty()) 
        return;

    // Champ : NumUnique
    int nouveauNumero = QInputDialog::getInt(
                this, "Modifier un descripteur", 
                "Nouveau numéro unique :", 
                elements[2].toInt(), // valeur initiale
                0,      // min
                100000, // max
                1,      // pas
                &ok
    );
    if (!ok) 
        return;

    // Champ : Prix
    double nouveauPrix = QInputDialog::getDouble(
                this, "Modifier un descripteur", 
                "Nouveau prix :", 
                elements[3].toDouble(), // valeur initiale
                0,       // min
                100000,  // max
                2,       // décimales
                &ok
    );
    if (!ok) 
        return;

    // Champ : Accès
    QString nouvelAcces = QInputDialog::getText(
                this, "Modifier un descripteur", 
                "Nouvel accès (O/L) :", 
                QLineEdit::Normal, 
                elements[4], 
                &ok
    );
    if (!ok || nouvelAcces.isEmpty()) 
        return;

    // Champ : Type
    QString nouveauType = QInputDialog::getText(
                this, "Modifier un descripteur", 
                "Nouveau type (couleur, gris) :", 
                QLineEdit::Normal, 
                elements[5], 
                &ok
    );
    if (!ok || nouveauType.isEmpty()) 
        return;

    // Champ : NbTraitements
    int nouveauNbTraitement = QInputDialog::getInt(
                this, "Modifier un descripteur", 
                "Nouveau nombre de traitements possibles :", 
                elements[6].toInt(), 
                1, 100, 1, &ok
    );
    if (!ok) 
        return;

    QString nouvelleLigne =
            nouveauTitre + ", " +
            elements[1] + ", " + 
            QString::number(nouveauNumero) + ", " +
            QString::number(nouveauPrix) + ", " +
            nouvelAcces + ", " +
            nouveauType + ", " +
            QString::number(nouveauNbTraitement) + ", " +
            QString::number(nouveauNumero) + // ou tout autre champ 
            "\n";

    // Ajout de la ligne modifiée au reste du fichier
    contenu += nouvelleLigne;

    // ----- ÉCRITURE DANS LE FICHIER CIBLE -----
    if (!fichierDescripteur.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", 
                             "Impossible d'écrire dans le fichier descripteurs :\n" 
                             + cheminDescripteurComplet);
        return;
    }

    QTextStream fluxEcriture(&fichierDescripteur);
    fluxEcriture << contenu;
    fichierDescripteur.close();

    QMessageBox::information(this,
                             "Succès",
                             "Le descripteur a été modifié avec succès dans :\n"
                             + cheminDescripteurComplet);
}



// Supprimer
void BiblioWindow::on_actionSupprimerDescripteur_triggered()
{
    // 1) Récupérer le chemin actuel du fichier chargé
    if (cheminBiblio.isEmpty()) {
        QMessageBox::warning(this, "Erreur", 
                             "Aucun fichier n'a été chargé. Impossible de supprimer.");
        return;
    }
    QString fileToModify = cheminBiblio; 

    // 2) Récupérer tous les items du QListWidget
    QList<QListWidgetItem*> items = ui->AffichageBiblio->findItems("*", Qt::MatchWildcard);
    if (items.isEmpty()) {
        QMessageBox::information(this, "Information", 
                                 "Aucun élément trouvé dans la bibliothèque.");
        return;
    }

    // 3) Construire un vecteur d'objets Image depuis l'affichage
    std::vector<Image> images;           
    std::vector<QString> cheminsImages;  

    for (QListWidgetItem* item : items) {
        // Récupère le chemin de l'image depuis le UserRole
        QVariant userData = item->data(Qt::UserRole);
        QString selectedImagePath = userData.toString();

        // Vérifier si l'image existe
        if (selectedImagePath.isEmpty() || !QFile::exists(selectedImagePath)) {
            qDebug() << "Chemin invalide ou inexistant : " << selectedImagePath;
            continue;
        }

        // Créer un objet Image et définir son titre (nom de fichier)
        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();

        // Associer les descripteurs (pour remplir getNumero(), getPrix(), etc.)
        try {
            image.associerDescripteur(fileToModify.toStdString());
        } catch (const std::exception& e) {
            qDebug() << "Erreur lors de l'association des descripteurs : " << e.what();
            continue;
        }

        images.push_back(image);
        cheminsImages.push_back(selectedImagePath);
    }

    // 4) Vérifier qu'on a bien des images en mémoire
    if (images.empty()) {
        QMessageBox::information(this, "Information", 
                                 "Aucune image exploitable n'a été détectée.");
        return;
    }

    // 5) Demander à l'utilisateur le numéro à supprimer (1..images.size())
    bool ok;
    int nbImages = (int)images.size();
    int numeroSup = QInputDialog::getInt(
                        this,
                        "Supprimer un descripteur",
                        QString("Entrez le numéro (1 à %1) de l'image à supprimer :")
                                .arg(nbImages),
                        1,   // Valeur par défaut
                        1,   // Min
                        nbImages,  // Max
                        1,   // Step
                        &ok
                    );
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Suppression annulée.");
        return;
    }

    // 6) Récupérer l'image correspondante (index = numeroSup-1)
    int index = numeroSup - 1;   // entre 0 et nbImages-1
    Image &imgASupprimer = images[index];
    QString cheminImage = cheminsImages[index];

     if (QFileInfo(fileToModify).fileName() == "Biblio_init.txt") {
         QMessageBox::warning(this, "Interdit",
                              "Vous n'avez pas le droit de supprimer directement dans Biblio_init.txt !");
         return;
    }

    // 8) Supprimer la ligne correspondante dans le fichier .txt
    //    - On lit toutes les lignes
    //    - On compare titre & numéro au descripteur qu'on veut supprimer
    //    - On recopie toutes les autres lignes
    QFile fichierDescripteur(fileToModify);
    if (!fichierDescripteur.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible d'ouvrir le fichier pour la suppression :\n"
                             + fileToModify);
        return;
    }

    QString contenuFinal;
    QTextStream fluxLecture(&fichierDescripteur);

    QString titreASupprimer  = QString::fromStdString(imgASupprimer.titre).trimmed();
    int numeroASupprimer     = imgASupprimer.getNumero();

    // Lecture ligne à ligne
    while (!fluxLecture.atEnd()) {
        QString line = fluxLecture.readLine();
        QStringList fields = line.split(",");
        if (fields.size() < 3) {
            contenuFinal += line + "\n";
            continue;
        }

        QString titre  = fields[0].trimmed();
        int numero     = fields[2].trimmed().toInt();

        // Comparaison : si titre & numéro correspondent, on ne recopie pas
        if (titre == titreASupprimer && numero == numeroASupprimer) {
            // => C'est la ligne à supprimer => on la skip
            qDebug() << "Suppression de la ligne :" << line;
            // Ne rien ajouter à contenuFinal
        } else {
            // Sinon, on recopie
            contenuFinal += line + "\n";
        }
    }
    fichierDescripteur.close();

    // On réécrit le fichier
    if (!fichierDescripteur.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible de rouvrir le fichier en écriture :\n"
                             + fileToModify);
        return;
    }
    QTextStream fluxEcriture(&fichierDescripteur);
    fluxEcriture << contenuFinal;
    fichierDescripteur.close();

    // 9) Supprimer l'image dans le dossier s'il existe
    if (QFile::exists(cheminImage)) {
        if (!QFile::remove(cheminImage)) {
            QMessageBox::warning(this, "Erreur", 
                                 "Impossible de supprimer le fichier image :\n" + cheminImage);
        }
    }

    // 10) Mettre à jour l'affichage => on enlève l'élément supprimé
    // On vide la liste
    ui->AffichageBiblio->clear();
    // On enlève l'image du vecteur
    images.erase(images.begin() + index);
    cheminsImages.erase(cheminsImages.begin() + index);

    int compteur = 1;
    for (size_t i = 0; i < images.size(); ++i) {
        QString path = cheminsImages[i];
        QString itemText = QString::number(compteur) + ". " +
                           QString::fromStdString(images[i].titre);

        QListWidgetItem* newItem = new QListWidgetItem(QIcon(path), itemText);
        newItem->setData(Qt::UserRole, path);

        ui->AffichageBiblio->addItem(newItem);
        compteur++;
    }

    QMessageBox::information(this, "Succès",
                             "Le descripteur et l'image associée ont été supprimés avec succès !");
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
    ui->SousListes->hide();

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
    ui->Tris->hide();
}







void BiblioWindow::on_trinbtraitements_clicked()
{
    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";
    // On récupère toutes les images présentes dans le widget
    QList<QListWidgetItem*> items = ui->AffichageBiblio->findItems("*", Qt::MatchWildcard);

    if (items.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun élément trouvé dans le widget.");
        return;
    }

    // On crée la liste pour stocker les images
    std::vector<Image> images;

    // On crée les listes pour stocker les titres, nb de traitements et chemins des images
    std::vector<std::string> titres;
    std::vector<int> nbTraitements;
    std::vector<QString> cheminsImages;

    // On récupère le chemin du dossier des images chargées dans le widget
    QString dossierImage = QFileInfo(items[0]->data(Qt::UserRole).toString()).absolutePath();
    qDebug() << "Dossier d'images : " << dossierImage;

    for (QListWidgetItem* item : items) {
        QVariant userData = item->data(Qt::UserRole);
        QString selectedImagePath = userData.toString();


        if (selectedImagePath.isEmpty() || !QFile::exists(selectedImagePath)) {
            qDebug() << "Chemin de fichier invalide ou fichier inexistant : " << selectedImagePath;
            continue;
        }

        Image image;
        image.titre = QFileInfo(selectedImagePath).fileName().toStdString();
        qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

        try {
            // On associe les descripteurs des images pour pouvoir récupérer les valeurs de nb de traitements
            image.associerDescripteur(cheminDescripteurs);
        } catch (const std::exception& e) {
            qDebug() << "Erreur lors de l'association des descripteurs pour : " << selectedImagePath
                     << "Erreur : " << e.what();
            continue;
        }

        images.push_back(image);

        // Ajouter le titre, le nombre de traitements possibles et le chemin à leurs vecteurs respectifs
        titres.push_back(image.getTitre());
        nbTraitements.push_back(image.getnbTraitementPossible());
        cheminsImages.push_back(selectedImagePath);
    }

    // On effectue le tri du nb de traitements décroissant
    for (int i = 0; i < nbTraitements.size() - 1; i++) {
        for (int j = 0; j < nbTraitements.size() - i - 1; j++) {
            if (nbTraitements[j] < nbTraitements[j + 1]) {

                std::swap(nbTraitements[j], nbTraitements[j + 1]);

                std::swap(titres[j], titres[j + 1]);

                std::swap(cheminsImages[j], cheminsImages[j + 1]);
            }
        }
    }

    // On efface l'affichage actuelle pour pouvoir afficher celui du tri
    ui->AffichageBiblio->clear();

    const QSize iconSize(150, 150);

    // Réafficher les images dans l'ordre trié

    for (int i = 0; i < titres.size(); ++i) {
        QString imagePath = cheminsImages[i];
        QString title = QString::fromStdString(titres[i]);

        // Créer un nouvel item avec le titre de l'image
        QListWidgetItem* newItem = new QListWidgetItem(title);

        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {

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
    ui->Tris->hide();
}


void BiblioWindow::on_souslistetype_clicked()
{
    if (selectedImagePath.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucune image sélectionnée !");
        return;
    }
    qDebug() << "Chemin de l'image sélectionnée : " << selectedImagePath;

    std::string cheminDescripteurs = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/Biblio_init.txt";


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
    ui->SousListes->hide();
}


void BiblioWindow::on_SousListesButton_clicked()
{
    ui->SousListes->show();
}


void BiblioWindow::on_Trisbutton_clicked()
{
    ui->Tris->show();
}


void BiblioWindow::on_Retourbutton_clicked()
{
    ui->Tris->hide();
}

