#include "Traitement.hpp"
#include <opencv2/opencv.hpp>


Traitement ::Traitement (){}
// ----------------------------------------------------------------------------------------------

// ************************ Filtrage par convolution ************************

// ----------------------------------------------------------------------------------------------

cv::Mat Traitement::convolution(const cv::Mat& image) {

    cv::Mat filter = (cv::Mat_<float>(3, 3) << 
        1.0f/16, 2.0f/16, 1.0f/16, 
        2.0f/16, 4.0f/16, 2.0f/16, 
        1.0f/16, 2.0f/16, 1.0f/16);

    int rows = image.rows;
    int cols = image.cols;
    int filterSize = filter.rows; 
    int pad = filterSize/2;   

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
    // om mets à 0 ce qui est <30 et à 255 ce qui est >=30
    cv::Mat contourImage;
    double seuil = 30;
    cv::threshold(imgF, contourImage, seuil, 255, cv::THRESH_BINARY);

    return contourImage; // Retourne l'image des contours
}

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

// Calcul de l'histogramme d'une image couleur
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
void Histogramme::calculerHistogramme(const Image& image, int histogramme[256], int canal) {
    image.calculerHistogramme(histogramme, canal);
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
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        cv::Point pt2(cvRound(x0 - 1000* (-b)), cvRound(y0 - 1000* a));

        // Dessiner la ligne sur l'image
        cv::line(imgDroites, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    return imgDroites;  // Retourner l'image avec les droites dessinées
}

// ----------------------------------------------------------------------------------------------

// ************************ Réhaussement de contour ************************

// ----------------------------------------------------------------------------------------------






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
