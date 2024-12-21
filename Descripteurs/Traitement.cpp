#include "Traitement.hpp"
#include <iostream>


Traitement::Traitement() {
    image_Originale = cv::imread("lena.jpg", cv::IMREAD_COLOR);
    image_Courante = image_Originale.clone();
}



void Traitement::detectionContours() {

   // CONVERTIR EN NIVEAUX DE GRIS c'est plus
    cv::Mat img_nvg;
    if (image_Courante.channels() == 3) {
        cv::cvtColor(image_Courante, img_nvg, cv::COLOR_BGR2GRAY);
    } else {
       img_nvg = image_Courante;
    }

    // on filtre pour avoir un meilleur resultat ( en attendant la fonction de filtrage)
    cv::Mat img_filtree;
    cv::GaussianBlur(img_nvg, img_filtree, cv::Size(3, 3), 0);

    // Filtrage de Sobel
    cv::Mat sobelX = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat sobelY = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

   //Application par convolution à l'image
    cv::Mat gradX, gradY;
    cv::filter2D(img_filtree, gradX, CV_32F, sobelX);
    cv::filter2D(img_filtree, gradY, CV_32F, sobelY);

   // Calcul de la norme du gradient
    cv::Mat normeGradient;
    cv::magnitude(gradX, gradY, normeGradient);

    // Convertion de float  de 32 bits  à un type image de  8 bits
    cv::Mat imgF;
   normeGradient.convertTo(imgF, CV_8U);

    // Affichage des resultats 
    cv::imshow("Gradient Magnitude",imgF);
    cv::waitKey(0);

    cv::Mat contourImage;
    double seuil = 100;  // Définir un seuil pour les contours
    cv::threshold(imgF, contourImage, seuil, 255, cv::THRESH_BINARY);

    //cv::imshow("Contours Detected", contourImage);  // Affichage de l'image binaire des contours
    //cv::waitKey(0);
    cv::Mat imgContoursSuperposes;
    cv::cvtColor(image_Courante, imgContoursSuperposes, cv::COLOR_BGR2RGB);  // Convertir en RGB
    cv::cvtColor(contourImage, contourImage, cv::COLOR_GRAY2BGR);  // Convertir en image couleur
    cv::addWeighted(imgContoursSuperposes, 0.7, contourImage, 0.3, 0, imgContoursSuperposes);
    cv::imshow("Contours Superposes", imgContoursSuperposes);
    cv::waitKey(0);



}
