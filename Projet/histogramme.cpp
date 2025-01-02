#include "histogramme.hpp"

// Calcul de l'histogramme d'une image en niveaux de gris
void ImageGris::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins de l'histogramme à zéro
    for (int i = 0; i < 256; ++i) {
        histogramme[i] = 0;
    }

    // Parcourir chaque pixel de l'image en niveaux de gris
    for (size_t ligne = 0; ligne < m_imageGris.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageGris[ligne].size(); ++colonne) {
            uint8_t valeurPixel = m_imageGris[ligne][colonne];
            histogramme[valeurPixel]++;
        }
    }
}

// Calcul de l'histogramme d'une image couleur
void ImageCouleur::calculerHistogramme(int histogramme[256], int canal) const {
    // Initialiser les bins des trois canaux à zéro
    int histogrammeRouge[256] = {0};
    int histogrammeVert[256] = {0};
    int histogrammeBleu[256] = {0};

    // Parcourir chaque pixel de l'image couleur
    for (size_t ligne = 0; ligne < m_imageCouleur.size(); ++ligne) {
        for (size_t colonne = 0; colonne < m_imageCouleur[ligne].size(); ++colonne) {
            uint8_t rouge = m_imageCouleur[ligne][colonne][0];
            uint8_t vert = m_imageCouleur[ligne][colonne][1];
            uint8_t bleu = m_imageCouleur[ligne][colonne][2];

            histogrammeRouge[rouge]++;
            histogrammeVert[vert]++;
            histogrammeBleu[bleu]++;
        }
    }

    // Si un canal spécifique est demandé, on l'affiche
    if (canal == 0) { // Canal Rouge
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i];
        }
    } else if (canal == 1) { // Canal Vert
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeVert[i];
        }
    } else if (canal == 2) { // Canal Bleu
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeBleu[i];
        }
    } else { // Afficher l'histogramme combiné de tous les canaux
        for (int i = 0; i < 256; ++i) {
            histogramme[i] = histogrammeRouge[i] + histogrammeVert[i] + histogrammeBleu[i];
        }
    }
}

// Fonction qui permet de calculer l'histogramme d'une image, qu'elle soit en gris ou en couleur
void Histogramme::calculerHistogramme(const Image& image, int histogramme[256], int canal) {
    image.calculerHistogramme(histogramme, canal);
}
