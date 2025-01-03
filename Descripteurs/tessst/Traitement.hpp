#ifndef TRAITEMENT_HPP
#define TRAITEMENT_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

class Traitement {
private:
    cv::Mat image_Originale; // Image d'origine
    cv::Mat image_Courante;  // Image modifiée

public:
    Traitement();
    cv::Mat detectionContours(const cv::Mat &image);
    void setImageOriginale(const cv::Mat &image);
    cv::Mat HoughDroite(const cv::Mat &image);
    cv::Mat rehaussementContours(const cv::Mat &image);

};

#endif // TRAITEMENT_HPP

