#ifndef TRAITEMENT_HPP
#define TRAITEMENT_HPP

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
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
// Classe de base abstraite pour l'image, permettant la gestion des histogrammes des images en niveaux de gris ou en couleur
class Image_color {
public:
    // Fonction virtuelle pure pour calculer l'histogramme. La classe dérivée doit implémenter cette méthode.
    virtual void calculerHistogramme(int histogramme[256], int canal = -1) const = 0; // Fonction virtuelle pure
};

// Classe dérivée pour les images en niveaux de gris
class ImageGris : public Image_color {
public:
    // Constructeur prenant une image en niveaux de gris représentée par une matrice de pixels
    ImageGris(const std::vector<std::vector<uint8_t>>& imageGris)
        : m_imageGris(imageGris) {}

    // Accesseur pour obtenir l'image en niveaux de gris
    const std::vector<std::vector<uint8_t>>& getImageGris() const {
        return m_imageGris;
    }

    // Implémentation de la méthode pour calculer l'histogramme
    void calculerHistogramme(int histogramme[256], int canal = -1) const override;

private:
    std::vector<std::vector<uint8_t>> m_imageGris;  // Représentation de l'image en niveaux de gris
};

// Classe dérivée pour les images couleur
class ImageCouleur : public Image_color {
public:
    // Constructeur prenant une image couleur représentée par une matrice de pixels (chaque pixel est un tableau de 3 valeurs pour R, G, B)
    ImageCouleur(const std::vector<std::vector<std::array<uint8_t, 3>>>& imageCouleur)
        : m_imageCouleur(imageCouleur) {}

    // Accesseur pour obtenir l'image couleur
    const std::vector<std::vector<std::array<uint8_t, 3>>>& getImageCouleur() const {
        return m_imageCouleur;
    }

    // Implémentation de la méthode pour calculer l'histogramme
    void calculerHistogramme(int histogramme[256], int canal = -1) const override;

private:
    std::vector<std::vector<std::array<uint8_t, 3>>> m_imageCouleur;  // Représentation de l'image couleur
};

// Classe statique pour la gestion des histogrammes d'images
class Histogramme {
public:
    // Fonction statique permettant de calculer l'histogramme d'une image (gris ou couleur)
    static void calculerHistogramme(const Image_color& image, int histogramme[256], int canal = -1);
};



// ----------------------------------------------------------------------------------------------

// ************************ Traitements d'images ************************
// ******
// Traitements comme la détection de contours, la transformation de Hough, la segmentation par couleur, et le rehaussement des contours
// ******

// ----------------------------------------------------------------------------------------------

// Classe permettant de gérer les traitements sur les images (détection de contours, Hough, etc.)
class Traitement {
private:
    cv::Mat image_Originale;  // Image d'origine, avant tout traitement
    cv::Mat image_Courante;   // Image après modification (traitement)

public:
    // Constructeur
    Traitement();

    cv::Mat detectionContours(const cv::Mat &image); // Méthode pour effectuer la détection des contours sur une image
    cv::Mat HoughDroite(const cv::Mat &image); // Méthode pour appliquer la transformation de Hough pour détecter des droites sur l'image
    cv::Mat rehaussementContours(const cv::Mat &image); // Méthode pour effectuer le rehaussement des contours sur l'image
    cv::Mat separationParCouleur(const cv::Mat& image, const std::vector<int>& canaux); // Méthode pour effectuer la segmentation par couleur en fonction des canaux sélectionnés
    cv::Mat convolution(const cv::Mat& image); // Méthode pour appliquer une convolution sur l'image
};


#endif

