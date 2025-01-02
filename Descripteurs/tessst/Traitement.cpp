#include "../Traitement.hpp"
#include <opencv2/opencv.hpp>


Traitement ::Traitement (){}

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



