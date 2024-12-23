#include "Traitement.hpp"
#include <opencv2/opencv.hpp>

 Traitement::Traitement(){

}
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
