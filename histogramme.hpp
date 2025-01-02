#ifndef HISTOGRAMME_HPP
#define HISTOGRAMME_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <stdexcept>

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

#endif // HISTOGRAMME_HPP
