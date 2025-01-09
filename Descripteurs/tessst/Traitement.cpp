#include "Traitement.hpp"
#include <opencv2/opencv.hpp>


Traitement ::Traitement (){}


// ----------------------------------------------------------------------------------------------

// ************************ Filtrage par convolution ************************

// ----------------------------------------------------------------------------------------------







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
    // om mets à 0 ce qui est <30 et à 255 ce qui est >=30
    cv::Mat contourImage;
    double seuil = 25;
    cv::threshold(imgF, contourImage, seuil, 255, cv::THRESH_BINARY);

    return contourImage; // Retourne l'image des contours
}

// ----------------------------------------------------------------------------------------------

// ************************ Détection de Droite ************************

// ----------------------------------------------------------------------------------------------
cv::Mat Traitement::HoughDroite(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat();
    }

    // Détection des contours dans l'image pour reduire le nbre de pixel a anlyser
    cv::Mat contours = detectionContours(image);

    // Dimensions et paramètres
    int largeur = contours.cols;
    int hauteur = contours.rows;
    int maxRho = static_cast<int>(sqrt(largeur * largeur + hauteur * hauteur));//distance max possible pour rho (diagonale)
    int angleResolution = 180;

    // Création de l'espace de hough
    cv::Mat espaceHough = cv::Mat::zeros(2 * maxRho, angleResolution, CV_32SC1);

    // Parcours des pixels de contour
    //rho : Distance entre la droite et l'origine  de l'image.
    //theta : Angle de la normale à la droite avec l'axe des X.

   //Representation de chaque droite dans l'espace rho theta

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (contours.at<uchar>(y, x) > 0) {
                for (int theta = 0; theta < angleResolution; theta++) {
                    double angle = CV_PI * theta / angleResolution;
                    int rho = static_cast<int>(x * cos(angle) + y * sin(angle)); // Calcul de rho pour chaque angle
                    if (rho >= -maxRho && rho < maxRho) { // si rho dans les limites
                        espaceHough.at<int>(rho + maxRho, theta)++; //on incrémente de 1 la valeur de la matrice espaceHough à l'indice correspondant
                    }
                }
            }
        }
    }

    // Détection des droites les plus probables dans l'espace de Hough ou y a le plus de points

    double minVal, maxVal;
    cv::minMaxLoc(espaceHough, &minVal, &maxVal); // on cherche dans la matrice les indice qui ont un valeur elevé
    int seuil = 0.8* maxVal; //on fixe un seuil pour ne recuperer que les valeur importante et eviter d'avoir trop de droites detectée
    std::vector<std::pair<int, int>> stockage;
    for (int rho = 0; rho < 2 * maxRho; rho++) {
        for (int theta = 0; theta < angleResolution; theta++) {
            if (espaceHough.at<int>(rho, theta) > seuil) {
                stockage.emplace_back(rho - maxRho, theta); // stockage des theta et rho trouvé
            }
        }
    }

    std::vector<std::pair<int, int>> stockageFiltré;
    for (const auto &ligne : stockage) {
        int theta = ligne.second;
        if (theta < 10 || theta > 170) {     // Tolérance autour de 0° ou 180°
            stockageFiltré.emplace_back(ligne);
        }
    }
    stockage = stockageFiltré; // Remplacez stockage par la version filtrée


    // Dessin des droites sur l'image d'origine
    cv::Mat imgDroites = image.clone();  // Utilisation de l'image originale pour dessiner les lignes
    for (const auto &ligne : stockage) {
        int rho = ligne.first;
        int theta = ligne.second;

        double angle = CV_PI * theta / angleResolution;
        double a = cos(angle), b = sin(angle);
        double x0 = a * rho, y0 = b * rho;

        // Points sur les bords de l'image pour dessiner la droite
        cv::Point pt1(cvRound(x0 + 500 * (-b)), cvRound(y0 + 500 * a));
        cv::Point pt2(cvRound(x0 - 500* (-b)), cvRound(y0 - 500* a));

        // Dessiner la ligne sur l'image
        cv::line(imgDroites, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    return imgDroites;  // Retourner l'image avec les droites dessinées
}

// ----------------------------------------------------------------------------------------------

// ************************ Réhaussement de contour ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::rehaussementContours(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat(); // Retourne une image vide en cas d'erreur
    }

    // Étape 1 : Convertir en niveaux de gris si l'image est en couleur
    cv::Mat imgGris;
    if (image.channels() == 3) {
        cv::cvtColor(image, imgGris, cv::COLOR_BGR2GRAY);
    } else {
        imgGris = image.clone();
    }

    // Étape 2 : Appliquer un filtre Sobel pour détecter les contours
    cv::Mat gradX, gradY;
    cv::Sobel(imgGris, gradX, CV_16S, 1, 0, 3); // Gradient horizontal
    cv::Sobel(imgGris, gradY, CV_16S, 0, 1, 3); // Gradient vertical
    cv::convertScaleAbs(gradX, gradX);
    cv::convertScaleAbs(gradY, gradY);

    cv::Mat magnitude;
    cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, magnitude);

    // Étape 3 : Ajouter les contours détectés à l'image originale
    cv::Mat imgRehaussee;
    cv::addWeighted(imgGris, 0.8, magnitude, 0.5, 0, imgRehaussee);

    return imgRehaussee; // Retourne l'image rehaussée
}

// ----------------------------------------------------------------------------------------------

// ************************ Ségmentation ************************

// ----------------------------------------------------------------------------------------------

#include "Traitement.hpp"
#include <opencv2/opencv.hpp>


Traitement ::Traitement (){}



// ----------------------------------------------------------------------------------------------

// ************************ Histogramme ************************

// ----------------------------------------------------------------------------------------------

// Calcul de l'histogramme d'une image en niveaux de gris
void ImageGris::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins de l'histogramme à zéro
    for (int i = 0; i < 256; ++i) {
        histogramme[i] = 0;
    }

    // Parcourir chaque pixel de l'image en niveaux de gris
    for (size_t ligne = 0; ligne < m_imageGris.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageGris[ligne].size(); ++colonne) {
            uint8_t valeurPixel = m_imageGris[ligne][colonne];
            histogramme[valeurPixel]++;
        }
    }
}

// Calcul de lhistogramme d'une image couleur
void ImageCouleur::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins des trois canaux à zéro
    int histogrammeRouge[256] = {0};
    int histogrammeVert[256] = {0};
    int histogrammeBleu[256] = {0};

    // Parcourir chaque pixel de l'image couleur
    for (size_t ligne = 0; ligne < m_imageCouleur.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageCouleur[ligne].size(); ++colonne) {
            uint8_t rouge = m_imageCouleur[ligne][colonne][0];
            uint8_t vert = m_imageCouleur[ligne][colonne][1];
            uint8_t bleu = m_imageCouleur[ligne][colonne][2];

            histogrammeRouge[rouge]++;
            histogrammeVert[vert]++;
            histogrammeBleu[bleu]++;
        }
    }

    // Si un canal spécifique est demandé, on l'affiche
    if (canal == 0) { // Canal Rouge
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i];
        }
    } else if (canal == 1) { // Canal Vert
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeVert[i];
        }
    } else if (canal == 2) { // Canal Bleu
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeBleu[i];
        }
    } else { // Afficher l'histogramme combiné de tous les canaux
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i] + histogrammeVert[i] + histogrammeBleu[i];
        }
    }
}

// Fonction qui permet de calculer l'histogramme d'une image, qu'elle soit en gris ou en couleur
void Histogramme::calculerHistogramme(const Image_color& image, int histogramme[256], int canal) {
    image.calculerHistogramme(histogramme, canal);
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

// ----------------------------------------------------------------------------------------------
cv::Mat Traitement::separationParCouleur(const cv::Mat& image, const std::vector<int>& canaux) {
    if (image.empty() || image.channels() != 3) {
        std::cerr << "Image invalide ou non en couleur." << std::endl;
        return cv::Mat();
    }

    // Conversion de l'image OpenCV en ImageCouleur
    std::vector<std::vector<std::array<uint8_t, 3>>> pixels(image.rows, std::vector<std::array<uint8_t, 3>>(image.cols));
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            pixels[y][x] = {pixel[0], pixel[1], pixel[2]}; // Rouge, Vert, Bleu
        }
    }
    ImageCouleur imageCouleur(pixels);

    // Initialisation des histogrammes pour chaque canal
    int histogramme[256] = {0};
    int seuils[3] = {0};

    // Calcul des seuils pour chaque canal sélectionné
    for (int canal : canaux) {
        Histogramme::calculerHistogramme(imageCouleur, histogramme, canal);

        int totalPixels = image.rows * image.cols;
        int seuilPixels = static_cast<int>(totalPixels * 0.3); // 30% des pixels les plus intenses
        int cumulativeSum = 0;

        for (int i = 255; i >= 0; i--) {
            cumulativeSum += histogramme[i];
            if (cumulativeSum >= seuilPixels) {
                seuils[canal] = i;
                break;
            }
        }
    }

    // Création d'une image filtrée
    cv::Mat resultat = cv::Mat::zeros(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            const cv::Vec3b& pixel = image.at<cv::Vec3b>(y, x);
            uint8_t rouge = pixel[0];
            uint8_t vert = pixel[1];
            uint8_t bleu = pixel[2];

            // Vérification pour chaque combinaison de canaux
            if (std::find(canaux.begin(), canaux.end(), 0) != canaux.end() &&
                rouge >= seuils[0] && rouge > bleu * 1.5 && rouge > vert * 1.4) {
                resultat.at<cv::Vec3b>(y, x) = pixel; // Conserver les pixels rouges
            }
            if (std::find(canaux.begin(), canaux.end(), 1) != canaux.end() &&
                vert >= seuils[1] && vert > rouge * 0.9 && vert > bleu * 0.9) {
                resultat.at<cv::Vec3b>(y, x) = pixel; // Conserver les pixels verts
            }
            if (std::find(canaux.begin(), canaux.end(), 2) != canaux.end() &&
                bleu >= seuils[2] && bleu > rouge * 0.8 && bleu > vert * 0.8) {
                resultat.at<cv::Vec3b>(y, x) = pixel; // Conserver les pixels bleus
            }
        }
    }

    return resultat;
}
