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

    // Ajuster l'image pour qu'elle remplisse toute la vue
    view->fitInView(pixmapItem, Qt::KeepAspectRatioByExpanding);

    // Supprimer les bordures pour maximiser l'espace de l'image
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Appliquer des options de rendu pour la qualité
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view->setRenderHint(QPainter::Antialiasing, true);
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
    QString login = ui->Login->text();

    if (login == "ad-01-ao" || login == "us-02-al") {
        if (!biblioWindow) {
            biblioWindow = std::make_unique<BiblioWindow>(nullptr);
            biblioWindow->setUserLogin(login); // Définir le login utilisateur
        }

        biblioWindow->show();
        biblioWindow->loadDefaultFile(login); // Charge le fichier par défaut avec les droits utilisateur
        this->close();
    } else {
        QMessageBox::warning(this, "Erreur", "Identification échouée, essayez encore.");
        ui->Login->clear();
    }
}
