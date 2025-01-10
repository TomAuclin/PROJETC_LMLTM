#include "connexionwindow.h"
#include "ui_connexionwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QScreen>

ConnexionWindow::ConnexionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnexionWindow)
    , biblioWindow(nullptr) // Initialisation du pointeur unique
{
    ui->setupUi(this);
    setWindowTitle("Connexion");
    resize(350, 200);

    // Centrer la fenêtre sur l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

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


void ConnexionWindow::on_Logo_ups_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{
    // Charger l'image logo de la fac
    QPixmap pixmap("/media/sf_PROJETC_LMLTM/logo_fac.png");

    // Vérifier si l'image est chargée
    if (pixmap.isNull()) {
        qWarning("Impossible de charger l'image logo");
        return;
    }

    // Créer une scène
    QGraphicsScene *scene = new QGraphicsScene(this);

    // Ajouter l'image à la scène
    QGraphicsPixmapItem *item = scene->addPixmap(pixmap);

    // Associer la scène à la QGraphicsView
    ui->Logo_ups->setScene(scene);

    // Adapter l'image à la taille de la QGraphicsView
    ui->Logo_ups->setSceneRect(pixmap.rect()); // Définir la scène en fonction de l'image
    ui->Logo_ups->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

}

