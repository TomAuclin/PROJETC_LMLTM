#include "../Traitement.hpp"
#include <opencv2/opencv.hpp>


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
    double seuil = 100;
    cv::threshold(imgF, contourImage, seuil, 255, cv::THRESH_BINARY);

    return contourImage; // Retourne l'image des contours
}

cv::Mat Traitement::HoughDroite(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat();
    }

    // Détection des contours
    cv::Mat contours = detectionContours(image);
    

    // Dimensions et paramètres
    int largeur = contours.cols;
    int hauteur = contours.rows;
    int maxRho = static_cast<int>(sqrt(largeur * largeur + hauteur * hauteur));
    int angleResolution = 180;
    int rhoResolution = 1;

    // Création de l'accumulateur
    cv::Mat accumulateur = cv::Mat::zeros(2 * maxRho, angleResolution, CV_32SC1);

    // Parcours des pixels de contour
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (contours.at<uchar>(y, x) > 0) {
                for (int theta = 0; theta < angleResolution; theta++) {
                    double angle = CV_PI * theta / angleResolution;
                    int rho = static_cast<int>(x * cos(angle) + y * sin(angle));
                    if (rho >= -maxRho && rho < maxRho) {
                        accumulateur.at<int>(rho + maxRho, theta)++;
                    }
                }
            }
        }
    }

    // Détection des pics dans l'accumulateur
    int seuil = 150;
    std::vector<std::pair<int, int>> lignes;
    for (int rho = 0; rho < 2 * maxRho; rho++) {
        for (int theta = 0; theta < angleResolution; theta++) {
            if (accumulateur.at<int>(rho, theta) > seuil) {
                lignes.emplace_back(rho - maxRho, theta);
            }
        }
    }

    // Dessin des droites
    cv::Mat imgDroites = cv::Mat::zeros(image.size(), CV_8UC3);
    for (const auto &ligne : lignes) {
        int rho = ligne.first;
        int theta = ligne.second;
        double angle = CV_PI * theta / angleResolution;
        double a = cos(angle), b = sin(angle);
        double x0 = a * rho, y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
        cv::line(imgDroites, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    // Affichage de l'accumulateur
    double minVal, maxVal;
    cv::minMaxLoc(accumulateur, &minVal, &maxVal);
    cv::Mat heatmap;
    accumulateur.convertTo(heatmap, CV_8U, 255.0 / maxVal);
    cv::applyColorMap(heatmap, heatmap, cv::COLORMAP_JET);
    cv::imshow("Accumulateur de Hough", heatmap);
    cv::waitKey(0);

    return imgDroites;
}

cv::Mat Traitement::rehaussementContours(const cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Erreur : L'image fournie est vide." << std::endl;
        return cv::Mat();
    }

    // Étape 1 : Convertir en niveaux de gris (si nécessaire)
    cv::Mat imgGris;
    if (image.channels() == 3) {
        cv::cvtColor(image, imgGris, cv::COLOR_BGR2GRAY);
    } else {
        imgGris = image.clone();
    }

    // Étape 2 : Détecter les contours avec Sobel
    cv::Mat gradX, gradY, magnitude;
    cv::Sobel(imgGris, gradX, CV_16S, 1, 0, 3); // Gradient horizontal
    cv::Sobel(imgGris, gradY, CV_16S, 0, 1, 3); // Gradient vertical
    cv::convertScaleAbs(gradX, gradX);
    cv::convertScaleAbs(gradY, gradY);
    cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, magnitude);

    // Étape 3 : Accentuer les contours en combinant avec l'image originale
    cv::Mat imgContours;
    cv::addWeighted(imgGris, 0.8, magnitude, 0.5, 0, imgContours);

    // Étape 4 : Retourner l'image rehaussée
    return imgContours;
}
