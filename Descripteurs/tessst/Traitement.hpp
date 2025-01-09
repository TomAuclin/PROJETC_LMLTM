#ifndef TRAITEMENT_HPP
#define TRAITEMENT_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include <vector>
#include <array>
#include <cstdint>
#include <stdexcept>

// ----------------------------------------------------------------------------------------------
// ************************ Histogramme ************************
// ----------------------------------------------------------------------------------------------

class Image {
public:
    virtual void calculerHistogramme(int histogramme[256], int canal = -1) const = 0; // Fonction virtuelle pure
};

class ImageGris : public Image {
public:
    ImageGris(const std::vector<std::vector<uint8_t>>& imageGris)
        : m_imageGris(imageGris) {}

    // Accesseur pour l'image en niveaux de gris
    const std::vector<std::vector<uint8_t>>& getImageGris() const {
        return m_imageGris;
    }

    void calculerHistogramme(int histogramme[256], int canal = -1) const override;

private:
    std::vector<std::vector<uint8_t>> m_imageGris;
};

class ImageCouleur : public Image {
public:
    ImageCouleur(const std::vector<std::vector<std::array<uint8_t, 3>>>& imageCouleur)
        : m_imageCouleur(imageCouleur) {}

    // Accesseur pour l'image couleur
    const std::vector<std::vector<std::array<uint8_t, 3>>>& getImageCouleur() const {
        return m_imageCouleur;
    }

    void calculerHistogramme(int histogramme[256], int canal = -1) const override;

private:
    std::vector<std::vector<std::array<uint8_t, 3>>> m_imageCouleur;
};

class Histogramme {
public:
    static void calculerHistogramme(const Image& image, int histogramme[256], int canal = -1);
};

// ----------------------------------------------------------------------------------------------
// ************************ Traitement ************************
// ----------------------------------------------------------------------------------------------

class Traitement {
private:
    cv::Mat image_Originale; // Image d'origine
    cv::Mat image_Courante;  // Image modifiée

public:
    Traitement();
    cv::Mat detectionContours(const cv::Mat& image);
    void setImageOriginale(const cv::Mat& image);
    cv::Mat HoughDroite(const cv::Mat& image);
    
    // Méthode mise à jour pour gérer plusieurs canaux
    cv::Mat separationParCouleur(const cv::Mat& image, const std::vector<int>& canaux);
    cv::Mat convolution(const cv::Mat& image);

};



#endif // TRAITEMENT_HPP
