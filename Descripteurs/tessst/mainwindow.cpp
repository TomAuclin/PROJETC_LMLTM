#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <stdexcept>
#include <QInputDialog>

Library library;


// ----------------------------------------------------------------------------------------------

// ************************ mise en place de la fenêtre ************************

// ----------------------------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sceneImage(new QGraphicsScene(this)),
    sceneHisto(new QGraphicsScene(this)),
    imageObj(nullptr) // Initialisation du pointeur image à null
{
    ui->setupUi(this);
    ui->AfficherImage->setScene(sceneImage);
    ui->AffichageResultat->setScene(sceneHisto);
    listWidget = ui->listWidget;  // Assurez-vous que listWidget est le bon nom d'élément dans le fichier .ui

    // Connecter le signal du clic droit au slot de gestion du menu contextuel
    connect(listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenuRequested);

    // Charger les images et descripteurs dans la liste (par exemple)
    // Assurez-vous de charger les images et de les associer avec des descripteurs
    // Exemple : chargerImagesAvecDescripteurs("chemin/vers/fichierDescripteurs.txt");
    for (const Image& image : images) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(image.getTitre()));
        item->setData(Qt::UserRole, QVariant::fromValue(image)); // Associe l'image à l'élément
        listWidget->addItem(item);

    // Cacher les boutons de sélection des canaux par défaut
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageObj;
}


// ----------------------------------------------------------------------------------------------

// ************************ Chargement d'un image  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_ChargerImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Charger une image"), "", tr("Images (*.png *.jpg *.pgm *.CR2)"));

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    // Reset de l'ecran d'affichage
    if (ui->AffichageResultat->scene()) {
        ui->AffichageResultat->scene()->clear();
    }



    if (!fileName.isEmpty()) {
        // Charger l'image dans l'objet imageObj
        try {
            delete imageObj; // Libérer l'ancien objet imageObj, s'il existe
            QImage img(fileName);

            if (img.isNull()) {
                throw std::runtime_error("L'image n'a pas pu être chargée.");
            }

            // Vérifier si l'image est en niveaux de gris ou en couleur
            if (img.format() == QImage::Format_Grayscale8) {
                std::vector<std::vector<uint8_t>> imageGris;
                for (int y = 0; y < img.height(); ++y) {
                    std::vector<uint8_t> row;
                    for (int x = 0; x < img.width(); ++x) {
                        row.push_back(qGray(img.pixel(x, y)));
                    }
                    imageGris.push_back(row);
                }
                imageObj = new ImageGris(imageGris);  // Charger comme image en niveaux de gris
            } else {
                std::vector<std::vector<std::array<uint8_t, 3>>> imageCouleur;
                for (int y = 0; y < img.height(); ++y) {
                    std::vector<std::array<uint8_t, 3>> row;
                    for (int x = 0; x < img.width(); ++x) {
                        QRgb pixel = img.pixel(x, y);
                        row.push_back({static_cast<uint8_t>(qRed(pixel)), static_cast<uint8_t>(qGreen(pixel)), static_cast<uint8_t>(qBlue(pixel))});
                    }
                    imageCouleur.push_back(row);
                }
                imageObj = new ImageCouleur(imageCouleur);  // Charger comme image couleur
            }

            // Charger et afficher l'image dans l'interface
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                QSize viewSize = ui->AfficherImage->viewport()->size();
                pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                sceneImage->clear();
                sceneImage->addPixmap(pixmap);
                ui->AfficherImage->setScene(sceneImage);
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(this, tr("Erreur"), tr(e.what()));
        }
    }
}


// ----------------------------------------------------------------------------------------------

// ************************ Calucle de l'histogramme et affichage  ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_CalculerHisto_clicked()
{
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Vérification si l'image est en niveaux de gris
    bool isGrayscale = dynamic_cast<ImageGris*>(imageObj) != nullptr;

    // Calculer l'histogramme
    try {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme);

        afficherHistogramme(histogramme); // Afficher l'histogramme combiné

        if (isGrayscale) {
            // Désactiver et masquer les boutons pour les canaux
            ui->Canal_R->setVisible(false);
            ui->Canal_V->setVisible(false);
            ui->Canal_B->setVisible(false);
        } else {
            // Activer et afficher les boutons pour les canaux
            ui->Canal_R->setVisible(true);
            ui->Canal_V->setVisible(true);
            ui->Canal_B->setVisible(true);
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Erreur"), tr(e.what()));
    }
}


void MainWindow::afficherHistogramme(int histogramme[256])
{
    sceneHisto->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    sceneHisto->addLine(margin, height - margin, width, height - margin, QPen(Qt::black));
    sceneHisto->addLine(margin, 0, margin, height - margin, QPen(Qt::black));

    for (int i = 0; i < 256; i += 32) {
        sceneHisto->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        sceneHisto->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    for (int i = 0; i < 256; ++i) {
        int barHeight = static_cast<int>(histogramme[i] * scaleFactor);
        sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::black));
    }

    ui->AffichageResultat->setScene(sceneHisto);
}

// Afichage d'une couleur pour chaque canneau
void MainWindow::afficherHistogrammeCanal(int histogramme[256], int canal)
{
    sceneHisto->clear();
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    int width = viewSize.width();
    int height = viewSize.height();

    int margin = 30;
    int maxVal = 0;

    // valeur maximale dans l'histogramme pour le redimensionnement
    for (int i = 0; i < 256; ++i) {
        if (histogramme[i] > maxVal) {
            maxVal = histogramme[i];
        }
    }

    int barWidth = width / 256;
    if (barWidth < 1) {
        barWidth = 1;
    }

    int maxBarHeight = height - 2 * margin;
    double scaleFactor = static_cast<double>(maxBarHeight) / maxVal;

    // axes de l'histogramme
    sceneHisto->addLine(margin, height - margin, width, height - margin, QPen(Qt::black)); // Axe X
    sceneHisto->addLine(margin, 0, margin, height - margin, QPen(Qt::black));             // Axe Y

    // graduations pour les axes
    for (int i = 0; i < 256; i += 32) {
        sceneHisto->addLine(i * barWidth + margin, height - margin, i * barWidth + margin, height - margin - 5, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i));
        textItem->setPos(i * barWidth + margin - 10, height - margin + 5);
    }

    int tickInterval = maxVal / 5;
    for (int i = 0; i <= 5; ++i) {
        int yPos = height - margin - (i * (height - 2 * margin) / 5);
        sceneHisto->addLine(margin - 5, yPos, margin, yPos, QPen(Qt::black));
        QGraphicsTextItem* textItem = sceneHisto->addText(QString::number(i * tickInterval));
        textItem->setPos(margin - 30, yPos - 5);
    }

    //  les histogrammes pour chaque canal en fonction des cases cochées
    if (ui->Canal_R->isChecked()) {
        int histoRouge[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoRouge, 0); // Canal rouge
        QColor couleurRouge = Qt::red;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoRouge[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurRouge), QBrush(couleurRouge));
        }
    }

    if (ui->Canal_V->isChecked()) {
        int histoVert[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoVert, 1); // Canal vert
        QColor couleurVert = Qt::green;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoVert[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurVert), QBrush(couleurVert));
        }
    }

    if (ui->Canal_B->isChecked()) {
        int histoBleu[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histoBleu, 2); // Canal bleu
        QColor couleurBleu = Qt::blue;
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(histoBleu[i] * scaleFactor);
            sceneHisto->addRect(i * barWidth + margin, height - margin - barHeight, barWidth, barHeight, QPen(couleurBleu), QBrush(couleurBleu));
        }
    }


    ui->AffichageResultat->setScene(sceneHisto);
}

void MainWindow::on_Canal_R_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 0);
        afficherHistogrammeCanal(histogramme, 0);
    }
}

void MainWindow::on_Canal_V_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 1);
        afficherHistogrammeCanal(histogramme, 1);
    }
}

void MainWindow::on_Canal_B_stateChanged(int arg1)
{
    if (imageObj) {
        int histogramme[256] = {0};
        Histogramme::calculerHistogramme(*imageObj, histogramme, 2);
        afficherHistogrammeCanal(histogramme, 2);
    }
}



// ----------------------------------------------------------------------------------------------

// ************************ Détection de contour ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_DetectionContour_clicked()
{
    // Vérification si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // masquer les bouton a cocher
    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);


    cv::Mat imageMat;

    // Si l'image est en couleur
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {

        const auto& imageCouleur = img->getImageCouleur();

        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);

        // Remplissage de la matrice
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); //RGB
            }
        }
    }
    // Si l'image est en niveaux de gris
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {

        const auto& imageGris = img->getImageGris(); // tableau des pixels en niveaux de gris

        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    //Détection des contours
    Traitement traitement;
    cv::Mat contours = traitement.detectionContours(imageMat);

    if (contours.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des contours a échoué."));
        return;
    }

    // Conversion de l'image des contours pour l'afficher
    QImage imgContours(contours.data, contours.cols, contours.rows, contours.step, QImage::Format_Grayscale8);
    QPixmap pixmap = QPixmap::fromImage(imgContours);

    // Redimensionnement de l'image
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Création d' une nouvelle scène pour Affichage le Resultat
    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);

    // Affichage l'image des contours dans la vue AffichageResultat
    ui->AffichageResultat->setScene(sceneResultat);
}



// ----------------------------------------------------------------------------------------------

// ************************ Détection de Droite ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_DetectionDroite_clicked()
{
    // Vérification si une image a été chargée
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    cv::Mat imageMat;

    // Si l'image est en couleur
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]);
            }
        }
    }

    // Si l'image est en niveaux de gris
    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        const auto& imageGris = img->getImageGris();
        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    // détection des droites
    Traitement traitement;
    cv::Mat imageAvecDroites = traitement.HoughDroite(imageMat);

    // si l'image des droites est vide
    if (imageAvecDroites.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La détection des droites a échoué."));
        return;
    }


    QImage imgDroites;

    if (imageAvecDroites.channels() == 3) {
        // Conversion  pour les images en couleur
        cv::Mat imageRGB;
        cv::cvtColor(imageAvecDroites, imageRGB, cv::COLOR_BGR2RGB);
        imgDroites = QImage(imageRGB.data, imageRGB.cols, imageRGB.rows, imageRGB.step, QImage::Format_RGB888);
    }
    else if (imageAvecDroites.channels() == 1) {
        // Pour les images en niveaux de gris
        imgDroites = QImage(imageAvecDroites.data, imageAvecDroites.cols, imageAvecDroites.rows, imageAvecDroites.step, QImage::Format_Grayscale8);
    }


    QPixmap pixmap = QPixmap::fromImage(imgDroites);

    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);
    ui->AffichageResultat->setScene(sceneResultat);
}



// ----------------------------------------------------------------------------------------------

// ************************ Séparation par couleur ************************

// ----------------------------------------------------------------------------------------------
std::vector<int> MainWindow::getSelectedSegmentationCanaux() {
    std::vector<int> canaux;
    if (ui->Segmentation_Canal_R->isChecked()) {
        canaux.push_back(0); // Rouge
    }
    if (ui->Segmentation_Canal_V->isChecked()) {
        canaux.push_back(1); // Vert
    }
    if (ui->Segmentation_Canal_B->isChecked()) {
        canaux.push_back(2); // Bleu
    }
    return canaux;
}

void MainWindow::on_SegmenterCouleur_clicked() {
    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    // Récupération des canaux sélectionnés
    std::vector<int> canaux = getSelectedSegmentationCanaux();

    if (canaux.empty() || canaux.size() > 2) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner un ou deux canaux."));
        return;
    }

    // Conversion de l'image en cv::Mat
    cv::Mat imageMat;
    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();
        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);
        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]);
            }
        }
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("L'image doit être en couleur."));
        return;
    }

    // Appliquer la segmentation
    Traitement traitement;
    cv::Mat resultat = traitement.separationParCouleur(imageMat, canaux);

    if (resultat.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La segmentation a échoué."));
        return;
    }

    // Afficher le résultat
    QImage imgResultat(resultat.data, resultat.cols, resultat.rows, resultat.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(imgResultat);
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);
    ui->AffichageResultat->setScene(sceneResultat);

    ui->statusbar->showMessage("Segmentation par couleur appliquée.");
}

// ----------------------------------------------------------------------------------------------

// ************************ Rehaussement de contours ************************

// ----------------------------------------------------------------------------------------------

void MainWindow::on_RehaussementContours_clicked()
{

    if (!imageObj) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image chargée."));
        return;
    }

    ui->Canal_R->setVisible(false);
    ui->Canal_V->setVisible(false);
    ui->Canal_B->setVisible(false);

    cv::Mat imageMat;


    if (ImageCouleur* img = dynamic_cast<ImageCouleur*>(imageObj)) {
        const auto& imageCouleur = img->getImageCouleur();

        imageMat = cv::Mat(imageCouleur.size(), imageCouleur[0].size(), CV_8UC3);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                const auto& pixel = imageCouleur[y][x];
                imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel[0], pixel[1], pixel[2]); // BGR
            }
        }
    }

    else if (ImageGris* img = dynamic_cast<ImageGris*>(imageObj)) {
        const auto& imageGris = img->getImageGris();


        imageMat = cv::Mat(imageGris.size(), imageGris[0].size(), CV_8U);

        for (int y = 0; y < imageMat.rows; ++y) {
            for (int x = 0; x < imageMat.cols; ++x) {
                imageMat.at<uchar>(y, x) = imageGris[y][x];
            }
        }
    }

    //  rehaussement des contours
    Traitement traitement;
    cv::Mat imageRehaussee = traitement.rehaussementContours(imageMat);
    if (imageRehaussee.empty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le rehaussement des contours a échoué."));
        return;
    }


    QImage imgRehaussee(imageRehaussee.data, imageRehaussee.cols, imageRehaussee.rows, imageRehaussee.step, QImage::Format_Grayscale8);

    // Si l'image est en RGB
    if (imageRehaussee.channels() == 3) {
        imgRehaussee = QImage(imageRehaussee.data, imageRehaussee.cols, imageRehaussee.rows, imageRehaussee.step, QImage::Format_RGB888);
    }

    QPixmap pixmap = QPixmap::fromImage(imgRehaussee);
    QSize viewSize = ui->AffichageResultat->viewport()->size();
    pixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* sceneResultat = new QGraphicsScene(this);
    sceneResultat->addPixmap(pixmap);

    ui->AffichageResultat->setScene(sceneResultat);
}


// -----------------------------------------------------------------------------------------

// *************************** Ajout Menu Descripteur *************************************

// -----------------------------------------------------------------------------------------

// Ajouter

void MainWindow::on_actionAjouterDescripteur_triggered() {
    QString cheminImage = QFileDialog::getOpenFileName(this, "Sélectionnez une image à ajouter", "", "Images (*.png *.jpg *.jpeg *.bmp);;Tous les fichiers (*)");
    if (cheminImage.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Aucun fichier sélectionné !");
        return;
    }
    Library library ;
    // Demander les informations de l'utilisateur
    bool ok;
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

    // Ajouter le descripteur à la bibliothèque
    Image nouvelleImage(cheminImage.toStdString(), titre.toStdString(), numero, prix, acces.toStdString()[0], type.toStdString(), nbTraitement, 0);
    library.ajouterDescripteurs(nouvelleImage);

    QMessageBox::information(this, "Succès", "Le descripteur a été ajouté avec succès !");
}

// Modifier

void MainWindow::on_actionModifierDescripteur_triggered() {
<<<<<<< HEAD
    // Demander le numéro du descripteur à modifier
    bool ok;
    int numero = QInputDialog::getInt(this, "Modifier un descripteur",
                                      "Entrez le numéro unique du descripteur :", 0, 0, 10000, 1, &ok);
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Modification annulée.");
        return;
    }

    // Parcourir la liste pour trouver le descripteur correspondant
    auto current = library.head; // Accès à la liste chaînée depuis votre bibliothèque
    bool descripteurTrouve = false;

    while (current) {
        if (current->data.getNumero() == numero) {
            descripteurTrouve = true;

            // Parcourir les attributs à modifier
            QStringList options = {"Titre", "Numéro", "Prix", "Accès", "Type", "Nombre de traitements"};
            for (int i = 0; i < options.size(); i++) {
                QString choix = QInputDialog::getItem(this, "Modifier un attribut",
                                                      "Choisissez un attribut à modifier :", options, i, false, &ok);
                if (!ok) break;

                if (choix == "Titre") {
                    QString titre = QInputDialog::getText(this, "Modifier le titre",
                                                          "Entrez le nouveau titre :", QLineEdit::Normal, "", &ok);
                    if (ok && !titre.isEmpty()) {
                        // Vérifiez l'unicité avec titrecheck
                        while (library.titrecheck(titre.toStdString()) != 0) {
                            titre = QInputDialog::getText(this, "Erreur",
                                                          "Le titre existe déjà, entrez un autre titre :", QLineEdit::Normal, "", &ok);
                            if (!ok) break;
                        }
                        current->data.setTitre(titre.toStdString());
                    }
                } else if (choix == "Numéro") {
                    int num = QInputDialog::getInt(this, "Modifier le numéro",
                                                   "Entrez le nouveau numéro :", 0, 0, 10000, 1, &ok);
                    if (ok) {
                        // Vérifiez l'unicité avec numerocheck
                        while (library.numerocheck(num) != 0) {
                            num = QInputDialog::getInt(this, "Erreur",
                                                       "Le numéro existe déjà, entrez un autre numéro :", 0, 0, 10000, 1, &ok);
                            if (!ok) break;
                        }
                        current->data.setNumero(num);
                    }
                } else if (choix == "Prix") {
                    double prix = QInputDialog::getDouble(this, "Modifier le prix",
                                                          "Entrez le nouveau prix :", 0, 0, 10000, 2, &ok);
                    if (ok) {
                        current->data.setPrix(prix);
                    }
                } else if (choix == "Accès") {
                    QString acces = QInputDialog::getText(this, "Modifier l'accès",
                                                          "Entrez le nouvel accès (O ou L) :", QLineEdit::Normal, "O", &ok);
                    if (ok && (acces == "O" || acces == "L")) {
                        current->data.setAccess(acces.toStdString()[0]);
                    }
                } else if (choix == "Type") {
                    QString type = QInputDialog::getText(this, "Modifier le type",
                                                         "Entrez le nouveau type (couleur ou gris) :", QLineEdit::Normal, "couleur", &ok);
                    if (ok && !type.isEmpty()) {
                        current->data.setType(type.toStdString());
                    }
                } else if (choix == "Nombre de traitements") {
                    int nbTraitement = QInputDialog::getInt(this, "Modifier le nombre de traitements",
                                                            "Entrez le nouveau nombre de traitements possibles :", 1, 1, 100, 1, &ok);
                    if (ok) {
                        current->data.setnbTraitementPossible(nbTraitement);
                    }
                }
            }

            break; // Modification terminée pour ce descripteur
        }
        current = current->next;
    }

    if (!descripteurTrouve) {
        QMessageBox::warning(this, "Erreur", "Aucun descripteur trouvé avec ce numéro.");
    } else {
        QMessageBox::information(this, "Succès", "Le descripteur a été modifié avec succès !");
    }
=======
    Library library;
    int numero = QInputDialog::getInt(this, "Modifier un descripteur", "Entrez le numéro unique de l'image :", 0);
    library.modifdescripteurs(numero, library);
    QMessageBox::information(this, "Succès", "Le descripteur a été modifié avec succès !");
>>>>>>> 776d77f2fc9415e59db9c1dd5ab5fd89c5c1b1ba
}

// Supprimer

void MainWindow::on_actionSupprimerDescripteur_triggered() {
    Library library ;
    int numero = QInputDialog::getInt(this, "Supprimer un descripteur", "Entrez le numéro unique de l'image :", 0);
    library.supprimerDescripteurs(numero);
    QMessageBox::information(this, "Succès", "Le descripteur a été supprimé avec succès !");
}

/**************************************************************************************************/

<<<<<<< HEAD
void MainWindow::afficherDetailsDescripteur(const Image& image) {
    // Associer le descripteur à l'image en utilisant la fonction associée
    Image imageAvecDescripteur = image;  // Crée une copie de l'image
    imageAvecDescripteur.associerDescripteur("/media/sf_PROJETC_LMLTM1/Descripteurs/Biblio_init.txt");  // Associe le descripteur depuis le fichier
=======
// --------------------------------------------------------------------------------------------------

// ***************************** Afficher image **************************

// ----------------------------------------------------------------

void MainWindow::on_actionRechercherImage_triggered() {
    // Demander le numéro de l'image
    bool ok;
    int numero = QInputDialog::getInt(this, "Rechercher une image",
                                      "Entrez le numéro de l'image :", 0, 0, 10000, 1, &ok);
    if (!ok) {
        QMessageBox::information(this, "Annulé", "Recherche annulée.");
        return;
    }

    // Appeler la méthode de recherche dans Library
    std::string resultat = library.rechercherImageParNumero(numero);

    // Afficher le résultat
    if (resultat == "Image non trouvée.") {
        QMessageBox::warning(this, "Résultat", "Aucune image trouvée avec ce numéro.");
    } else {
        QMessageBox::information(this, "Résultat", QString::fromStdString(resultat));
    }
}


>>>>>>> d03298699e6c4b88fd119b892ef8800401cbd465

    // Récupère le descripteur associé à l'image
    Descripteur descripteur = imageAvecDescripteur.getDescripteur();

    // Affiche les détails dans une boîte de dialogue
    QString details = QString("Titre: %1\nPrix: %2\nAccès: %3\nType: %4\nNombre de traitements possibles: %5\nIdentité: %6")
                          .arg(QString::fromStdString(descripteur.getTitre()))
                          .arg(descripteur.getPrix())
                          .arg(descripteur.getAcces())
                          .arg(QString::fromStdString(descripteur.getType()))
                          .arg(descripteur.getNbTraitementPossible())
                          .arg(descripteur.getIdentite());

    QMessageBox::information(this, "Détails de l'image", details);
}

void MainWindow::afficherMenuContextuel(QListWidgetItem* item, const QPoint &pos) {
    // Récupérer l'image associée à l'élément
    Image image = item->data(Qt::UserRole).value<Image>();

    // Créer un menu contextuel
    QMenu contextMenu(tr("Menu contextuel"), this);

    // Ajouter l'option pour afficher les détails
    QAction* detailsAction = new QAction(tr("Voir les détails"), this);

    // Connecter l'action à la fonction qui affiche les détails
    connect(detailsAction, &QAction::triggered, this, [this, image]() {
        afficherDetailsDescripteur(image); // Appelle la fonction pour afficher les détails
    });

    contextMenu.addAction(detailsAction);
    contextMenu.exec(pos);
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos) {
    // Récupérer l'élément à la position du clic
    QListWidgetItem* item = listWidget->itemAt(pos);
    if (item) {
        afficherMenuContextuel(item, pos); // Ouvre le menu contextuel
    }
}

