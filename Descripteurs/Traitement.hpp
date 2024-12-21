#ifndef TRAITEMENT_H
#define TRAITEMENT_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

class Traitement {
private:
    cv::Mat image_Originale; // Image d'origine 
    cv::Mat image_Courante;  // Image modifiée par des traitements

public:
    
    Traitement();





    void detectionContours();





    ~Traitement();
};

#endif 
