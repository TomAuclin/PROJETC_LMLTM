#include "connexionwindow.h"
#include "ui_connexionwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QScreen>
#include <QGraphicsPixmapItem>


ConnexionWindow::ConnexionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnexionWindow)
    , biblioWindow(nullptr) // Initialisation du pointeur unique
{
    ui->setupUi(this);
    setWindowTitle("Connexion");
    resize(450, 300);

    // Centrer la fenêtre sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
    setupLogo();

}

void ConnexionWindow::setupLogo()
{
    // Chemin vers l'image
    QString imagePath = "/media/sf_PROJETC_LMLTM/Descripteurs/tessst/logo_fac.png";

    // Charger l'image
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qWarning() << "Erreur : Impossible de charger l'image à l'emplacement" << imagePath;
        return;
    }

    // Créer une scène et ajouter l'image
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);

    // Trouver le QGraphicsView
    QGraphicsView *view = ui->LogoUps; // Utilisez directement le pointeur généré par `setupUi`
    if (!view) {
        qWarning() << "Erreur : QGraphicsView introuvable.";
        return;
    }

    // Associer la scène au QGraphicsView
    view->setScene(scene);

    // Ajuster l'image pour qu'elle s'adapte à la taille de la vue
    QRectF pixmapRect = pixmapItem->boundingRect(); // Obtenir la taille de l'image
    view->fitInView(pixmapRect, Qt::KeepAspectRatio);
    view->setRenderHint(QPainter::Antialiasing);
}

ConnexionWindow::~ConnexionWindow()
{
    delete ui;
}

void ConnexionWindow::on_Login_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
}

void ConnexionWindow::on_connexionButton_clicked()
{
    QString login = ui->Login->text(); // Assurez-vous que "Login" est défini dans l'interface
    qDebug() << "Login saisi: " << login;

    if (login == "ad-01-ao" || login == "us-02-al" || login == "a") {
        qDebug() << "Connexion réussie";

        // Ouvrir la fenêtre BiblioWindow
        if (!biblioWindow) {
            biblioWindow = std::make_unique<BiblioWindow>(nullptr); // Crée une instance de BiblioWindow sans parent
            qDebug() << "BiblioWindow créée";
        }

        biblioWindow->show(); // Affiche la fenêtre
        this->close();        // Ferme la fenêtre de connexion
    } else {
        qDebug() << "Connexion échouée";
        QMessageBox::warning(this, "Erreur", "Identification échouée, essayez encore.");
        ui->Login->clear(); // Permet de saisir un nouveau mot de passe
    }
}
