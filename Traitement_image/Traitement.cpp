#include "Traitement.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

Traitement::Traitement() {
    
    image_Originale = cv::imread("image.jpg", cv::IMREAD_COLOR);
    image_Courante = image_Originale.clone();
}

