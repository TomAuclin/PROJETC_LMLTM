// Inclusion des fichiers d'en-tête
#include "Traitement.hpp"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <opencv2/opencv.hpp>


// Constructeur par défaut de la classe Traitement.
Traitement::Traitement() {}

// ----------------------------------------------------------------------------------------------

// ************************ Histogramme ************************

// ----------------------------------------------------------------------------------------------

// Calcul de l'histogramme d'une image en niveaux de gris
void ImageGris::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins de l'histogramme à zéro
    for (int i = 0; i < 256; ++i) {
        histogramme[i] = 0; // On initialise chaque valeur de l'histogramme à zéro
    }

    // Parcourir chaque pixel de l'image en niveaux de gris
    for (size_t ligne = 0; ligne < m_imageGris.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageGris[ligne].size(); ++colonne) {
            uint8_t valeurPixel = m_imageGris[ligne][colonne]; // Récupère la valeur du pixel à la position donnée
            histogramme[valeurPixel]++; // Incrémente le bin correspondant à la valeur du pixel
        }
    }
}

// Calcul de l'histogramme d'une image couleur
void ImageCouleur::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins des trois canaux (Rouge, Vert, Bleu) à zéro
    int histogrammeRouge[256] = {0};
    int histogrammeVert[256] = {0};
    int histogrammeBleu[256] = {0};

    // Parcourir chaque pixel de l'image couleur
    for (size_t ligne = 0; ligne < m_imageCouleur.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageCouleur[ligne].size(); ++colonne) {
            uint8_t rouge = m_imageCouleur[ligne][colonne][0]; // Récupère la valeur du composant Rouge du pixel
            uint8_t vert = m_imageCouleur[ligne][colonne][1];  // Récupère la valeur du composant Vert du pixel
            uint8_t bleu = m_imageCouleur[ligne][colonne][2];  // Récupère la valeur du composant Bleu du pixel

            histogrammeRouge[rouge]++; // Incrémente le bin correspondant à la valeur du composant Rouge
            histogrammeVert[vert]++;   // Incrémente le bin correspondant à la valeur du composant Vert
            histogrammeBleu[bleu]++;   // Incrémente le bin correspondant à la valeur du composant Bleu
        }
    }

    // Si un canal spécifique est demandé, on l'affiche
    if (canal == 0) { // Canal Rouge
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i]; // Remplir l'histogramme avec les valeurs du canal Rouge
        }
    } else if (canal == 1) { // Canal Vert
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeVert[i]; // Remplir l'histogramme avec les valeurs du canal Vert
        }
    } else if (canal == 2) { // Canal Bleu
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeBleu[i]; // Remplir l'histogramme avec les valeurs du canal Bleu
        }
    } else { // Afficher l'histogramme combiné de tous les canaux
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i] + histogrammeVert[i] + histogrammeBleu[i]; // Somme des valeurs des trois canaux
        }
    }
}

// Fonction qui permet de calculer l'histogramme d'une image, qu'elle soit en gris ou en couleur
void Histogramme::calculerHistogramme(const Image_color& image, int histogramme[256], int canal) {
    image.calculerHistogramme(histogramme, canal); // Appelle la méthode de l'objet Image_color pour calculer l'histogramme
}

// ----------------------------------------------------------------------------------------------

// ************************ Filtrage par convolution ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::convolution(const cv::Mat& image) {
   // Creation du filtre (gaussien ou moyenneur )
    //image.convertTo(image, CV_32F);
    cv::Mat filter = (cv::Mat_<float>(3, 3) << 
        1.0f/16, 2.0f/16, 1.0f/16, 
        2.0f/16, 4.0f/16, 2.0f/16, 
        1.0f/16, 2.0f/16, 1.0f/16);

    int rows = image.rows;
    int cols = image.cols;
    int filterSize = filter.rows; 
    int pad = filterSize/2;
    //image.convertTo(image, CV_32F);
    // On differencie les images couleurs des images en nuances de gris
    cv::Mat result;
    if (image.channels() == 3) {
   
        std::vector<cv::Mat> channels(3);
        cv::split(image, channels); 

        for (int c = 0; c < 3; ++c) {
            cv::Mat temp = cv::Mat::zeros(rows, cols, CV_32F);
            for (int i = pad; i < rows - pad; ++i) {
                for (int j = pad; j < cols - pad; ++j) {
                    float sum = 0.0f;
                    for (int k = -pad; k <= pad; ++k) {
                        for (int l = -pad; l <= pad; ++l) {
                            sum += channels[c].at<uchar>(i + k, j + l) * filter.at<float>(pad + k, pad + l);
                        }
                    }
                    temp.at<float>(i, j) = sum;
                }
            }
            cv::normalize(channels[c], channels[c], 0, 255, cv::NORM_MINMAX);
            temp.convertTo(channels[c], CV_8U);

        }

        cv::merge(channels, result); 
    } else if (image.channels() == 1) {
        result = cv::Mat::zeros(rows, cols, CV_32F);
        for (int i = pad; i < rows - pad; ++i) {
            for (int j = pad; j < cols - pad; ++j) {
                float sum = 0.0f;
                for (int k = -pad; k <= pad; ++k) {
                    for (int l = -pad; l <= pad; ++l) {
                        sum += image.at<uchar>(i + k, j + l) * filter.at<float>(pad + k, pad + l);
                    }
                }
                result.at<float>(i, j) = sum;
            }
        }
        cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);
        result.convertTo(result, CV_8U);
    } else {
        std::cerr << "Erreur : Type d'image non pris en charge." << std::endl;
        return cv::Mat();
    }

    return result;
}
// ----------------------------------------------------------------------------------------------

// ************************ Detection de contours ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::detectionContours(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat(); // Retourne une image vide en cas d'erreur
    }

    // Conversion en niveaux de gris
    cv::Mat img_nvg;
    if (image.channels() == 3) {
        cv::cvtColor(image, img_nvg, cv::COLOR_BGR2GRAY);
    } else {
        img_nvg = image;
    }

    // Application d'un flou pour réduire le bruit
    cv::Mat img_filtree;
    cv::GaussianBlur(img_nvg, img_filtree, cv::Size(3, 3), 0);

    // Application du filtre Sobel
    cv::Mat sobelX = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat sobelY = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    cv::Mat gradX, gradY;
    cv::filter2D(img_filtree, gradX, CV_32F, sobelX);
    cv::filter2D(img_filtree, gradY, CV_32F, sobelY);

    // Calcul de la norme du gradient
    cv::Mat normeGradient;
    cv::magnitude(gradX, gradY, normeGradient);

    // Conversion en image 8 bits
    cv::Mat imgF;
    normeGradient.convertTo(imgF, CV_8U);

    // Seuillage pour extraire les contours
    cv::Mat contourImage;
    double seuil = 30;
    cv::threshold(imgF, contourImage, seuil, 255, cv::THRESH_BINARY);

    return contourImage; // Retourne l'image des contours
}


// ----------------------------------------------------------------------------------------------

// ************************ Réhaussement de contours ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::rehaussementContours(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat();
    }

    //Détecter les contours avec la fonction implementée
    cv::Mat contours = detectionContours(image);
    if (contours.empty()) {
        std::cerr << "Erreur : Détection des contours échouée." << std::endl;
        return cv::Mat();
    }

    //Initialisation de l'image de sortie
    cv::Mat imgRehaussee = image.clone();

    //On appliquer les contours à l'image originale (couelru ou nvg)
    if (image.channels() == 3) {
        // Si l'image est en couleur on applique les contours à chaque canal
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                cv::Vec3b& pixel = imgRehaussee.at<cv::Vec3b>(i, j);
                uchar valeurContour = contours.at<uchar>(i, j);

                // Ajouter les contours à chaque canal (R, G, B)
                for (int k = 0; k < 3; ++k) {
                    pixel[k] = cv::saturate_cast<uchar>(pixel[k] + 0.15 * valeurContour);
                }
            }
        }
    } else {
        // Si l'image est en niveaux de gris
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                uchar& pixel = imgRehaussee.at<uchar>(i, j);
                uchar valeurContour = contours.at<uchar>(i, j);

                // Ajouter les contours au pixel en niveaux de gris
                pixel = cv::saturate_cast<uchar>(pixel + 0.15 * valeurContour);
            }
        }
    }

    return imgRehaussee;
}

// ----------------------------------------------------------------------------------------------

// ************************ Détection de Droite ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::HoughDroite(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat();
    }

    // Conversion en niveaux de gris pour la détection des contours
    cv::Mat grayImage;
    if (image.channels() > 1) {  // Vérification si l'image est en couleur
        // Si l'image a un canal alpha, on le supprime et on convertit en niveaux de gris
        cv::Mat temp;
        if (image.channels() == 4) {
            cv::cvtColor(image, temp, cv::COLOR_BGRA2BGR);  // Conversion BGRA -> BGR
        } else {
            temp = image.clone();  // Pour les autres images avec 3 canaux (BGR)
        }
        cv::cvtColor(temp, grayImage, cv::COLOR_BGR2GRAY);  // Conversion BGR -> Grayscale
    } else {
        grayImage = image.clone();  // Si l'image est déjà en niveaux de gris, on la garde telle quelle
    }

    // Détection des contours
    cv::Mat contours = detectionContours(grayImage);
    if (contours.empty()) {
        std::cerr << "Erreur : La détection des contours a échoué." << std::endl;
        return cv::Mat();
    }

    // Dimensions de l'image
    int largeur = contours.cols;
    int hauteur = contours.rows;
    int maxRho = static_cast<int>(sqrt(largeur * largeur + hauteur * hauteur));
    int angleResolution = 180;  // La résolution angulaire de l'espace de Hough

    // Création de l'espace de Hough
    cv::Mat espaceHough = cv::Mat::zeros(2 * maxRho, angleResolution, CV_32SC1);

    // Parcours des pixels de contour
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (contours.at<uchar>(y, x) > 0) {  // Si le pixel fait partie d'un contour
                for (int theta = 0; theta < angleResolution; theta++) {
                    double angle = CV_PI * theta / angleResolution;
                    int rho = static_cast<int>(x * cos(angle) + y * sin(angle));
                    if (rho + maxRho >= 0 && rho + maxRho < 2 * maxRho) {
                        espaceHough.at<int>(rho + maxRho, theta)++;
                    }
                }
            }
        }
    }

    // Détection des droites les plus probables dans l'espace de Hough
    double minVal, maxVal;
    cv::minMaxLoc(espaceHough, &minVal, &maxVal);
    int seuil = static_cast<int>(0.80 * maxVal);  // seuil pour la détection des lignes
    std::vector<std::pair<int, int>> stockage;
    for (int rho = 0; rho < 2 * maxRho; rho++) {
        for (int theta = 0; theta < angleResolution; theta++) {
            if (espaceHough.at<int>(rho, theta) > seuil) {
                stockage.emplace_back(rho - maxRho, theta);
            }
        }
    }

    // Filtrage des droites proches
    std::vector<std::pair<int, int>> stockageFiltré;
    const int seuilRho = 20, seuilTheta = 200;
    for (const auto &ligne : stockage) {
        bool tropProche = false;
        for (const auto &ref : stockageFiltré) {
            if (std::abs(ligne.first - ref.first) < seuilRho && std::abs(ligne.second - ref.second) < seuilTheta) {
                tropProche = true;
                break;
            }
        }
        if (!tropProche) {
            stockageFiltré.emplace_back(ligne);
        }
    }

    // Filtrage des droites diagonales (on exclut un angle entre 40 et 50 degrés)
    std::vector<std::pair<int, int>> stockageFiltréSansDiagonales;
    const float angleMin = 40.0, angleMax = 50.0;
    for (const auto &ligne : stockageFiltré) {
        float thetaDeg = ligne.second * 180.0 / CV_PI;  // Conversion theta en degrés

        // Filtrage des droites de 45°
        if (thetaDeg > angleMin && thetaDeg < angleMax) {
            continue;  // on exclut les droites diagonales
        }

        stockageFiltréSansDiagonales.push_back(ligne);
    }

    // Dessin des droites sur l'image d'origine
    cv::Mat imgDroites = image.clone();
    for (const auto &ligne : stockageFiltréSansDiagonales) {
        int rho = ligne.first;
        int theta = ligne.second;

        double angle = CV_PI * theta / angleResolution;
        double a = cos(angle), b = sin(angle);
        double x0 = a * rho, y0 = b * rho;

        // Points sur les bords de l'image pour dessiner la droite
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
        cv::line(imgDroites, pt1, pt2, cv::Scalar(250), 2, cv::LINE_AA);
    }

    return imgDroites;
}

// ----------------------------------------------------------------------------------------------

// ************************ Segmentation couleur ************************

// -----------------------------------------------------------------------------------------------
cv::Mat Traitement::separationParCouleur(const cv::Mat& image, const std::vector<int>& canaux) {
    if (image.empty() || image.channels() != 3) {
        std::cerr << "Image invalide ou non en couleur." << std::endl;
        return cv::Mat();
    }

    // Conversion de l'image en espace HSV
    cv::Mat imageHSV;
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);

    // Création des masques pour chaque canal
    cv::Mat masqueFinal = cv::Mat::zeros(image.size(), CV_8U);

    for (int canal : canaux) {
        cv::Mat masque;
        if (canal == 0) { // Rouge
            cv::Mat masque1, masque2;
            cv::inRange(imageHSV, cv::Scalar(100, 20, 50), cv::Scalar(180, 255, 255), masque); // Rouge sombre
        } else if (canal == 1) { // Vert
            cv::inRange(imageHSV, cv::Scalar(45, 20, 50), cv::Scalar(90, 255, 255), masque); // Vert
        } else if (canal == 2) { // Bleu
            cv::inRange(imageHSV, cv::Scalar(0, 20, 50), cv::Scalar(45, 255, 255), masque); // Bleu
        }

        masqueFinal |= masque;
    }
    // Masque final sur l'image originale
    cv::Mat resultat;
    image.copyTo(resultat, masqueFinal);

    return resultat;
    }






