#ifndef TRAITEMENT_HPP
#define TRAITEMENT_HPP

#include <opencv2/opencv.hpp>

class Traitement {
public:
    Traitement();
    cv::Mat  detectionContours();

    ~Traitement();

private:
    cv::Mat image_Originale;
    cv::Mat image_Courante;


};

#endif // TRAITEMENT_HPP
