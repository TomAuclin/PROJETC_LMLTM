// Inclusion des fichiers nécessaires pour l'interface graphique et la gestion des fenêtres.
#include "connexionwindow.h"
#include "bibliowindow.h"
#include "ui_connexionwindow.h"
#include <QMessageBox> // Pour afficher des boîtes de message
#include <QDebug>      // Pour les messages de débogage
#include <QScreen>     // Pour récupérer les informations de l'écran
#include <QGraphicsPixmapItem> // Pour afficher des images dans des vues graphiques

// Constructeur de la fenêtre de connexion
ConnexionWindow::ConnexionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnexionWindow)
    , biblioWindow(nullptr) // Initialisation du pointeur unique vers la fenêtre bibliothèque
{
    ui->setupUi(this); // Configuration de l'interface utilisateur
    setWindowTitle("Connexion"); // Titre de la fenêtre
    resize(450, 300); // Taille de la fenêtre

    // Centrer la fenêtre de connexion sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen(); // Récupère l'écran principal
    if (screen) {
        QRect screenGeometry = screen->geometry(); // Obtient la géométrie de l'écran
        int x = (screenGeometry.width() - this->width()) / 2; // Calcul pour centrer horizontalement
        int y = (screenGeometry.height() - this->height()) / 2; // Calcul pour centrer verticalement
        this->move(x, y); // Déplace la fenêtre
    }
    setupLogo(); // Appel de la fonction pour configurer et afficher le logo
}

// Fonction pour configurer et afficher le logo dans la fenêtre de connexion
void ConnexionWindow::setupLogo()
{
    // Chemin vers l'image du logo
    QString imagePath = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/logo_fac.png";

    // Charger l'image en utilisant QPixmap
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) { // Vérifie si l'image a bien été chargée
        qWarning() << "Erreur : Impossible de charger l'image à l'emplacement" << imagePath;
        return;
    }

    // Créer une scène pour afficher l'image
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap); // Ajouter l'image à la scène

    // Récupérer la vue QGraphicsView où l'image sera affichée
    QGraphicsView *view = ui->LogoUps; // Utilise le QGraphicsView généré par `setupUi`
    if (!view) { // Vérifie si la vue est valide
        qWarning() << "Erreur : QGraphicsView introuvable.";
        return;
    }

    // Associer la scène à la vue
    view->setScene(scene);

    // Ajuster l'image pour qu'elle remplisse toute la vue en gardant les proportions
    view->fitInView(pixmapItem, Qt::KeepAspectRatioByExpanding);

    // Désactiver les barres de défilement pour maximiser l'espace de l'image
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Appliquer des options de rendu pour améliorer la qualité de l'image affichée
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view->setRenderHint(QPainter::Antialiasing, true);
}

// Destructeur de la fenêtre de connexion, libère l'interface utilisateur
ConnexionWindow::~ConnexionWindow()
{
    delete ui; // Libération des ressources allouées pour l'interface
}

// Retourne le login actuel de l'utilisateur
QString ConnexionWindow::getLogin() const
{
    return LoginActuel; // Retourne la valeur du login
}

// Fonction appelée lors du changement de position du curseur dans le champ de login (inutile ici)
void ConnexionWindow::on_Login_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1); // Argument inutilisé
    Q_UNUSED(arg2); // Argument inutilisé
}

// Fonction pour gérer la connexion de l'utilisateur lorsqu'il clique sur le bouton de connexion
void ConnexionWindow::on_connexionButton_clicked()
{
    QString login = ui->Login->text(); // Récupère le texte saisi dans le champ de login

    // Vérifie si le login est l'un des identifiants valides
    if (login == "ad-01-ao" || login == "us-02-al") {
        if (!biblioWindow) { // Si la fenêtre bibliothèque n'est pas déjà ouverte
            biblioWindow = std::make_unique<BiblioWindow>(nullptr); // Créer une nouvelle instance de BiblioWindow
            biblioWindow->setUserLogin(login); // Définir le login de l'utilisateur dans la fenêtre de la bibliothèque
        }

        biblioWindow->show(); // Affiche la fenêtre bibliothèque
        biblioWindow->loadDefaultFile(login); // Charge le fichier par défaut en fonction du login utilisateur
        this->close(); // Ferme la fenêtre de connexion
    } else { // Si le login n'est pas valide
        QMessageBox::warning(this, "Erreur", "Identification échouée, essayez encore."); // Affiche un message d'erreur
        ui->Login->clear(); // Vide le champ de login
    }
}
