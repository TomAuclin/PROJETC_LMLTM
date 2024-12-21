#ifndef TRAITEMENT_H
#define TRAITEMENT_H
#include <opencv2/opencv.hpp>

class Traitement {
private:
    cv::Mat image_Originale; // Image d'origine 
    cv::Mat image_Courante;  // Image modifiée par des traitements

public:
    
    Traitement();

    // 1. Calcul de l'histogramme 

    void calculerHistogramme();

    // 2. Filtrage d’une image par un filtre de convolution (matrice 3x3)

    void filtrageConvolution();

    // 3. Détection de contours avec filtres dérivatifs (gradient ou Laplacien)

    void detectionContours();

    // 4. Détection de droites par la transformée de Hough

    void detectionDroitesHough();

    // 5. Rehaussement de contours 

    void rehausserContours();

    // 6. Segmentation d'images par sélection de couleurs

    void segmentationCouleurs();

    
    void restaurerImageOriginale(); //Morgane
    // void sauvegarderImage();  a voir si fonction existe Lydia
    void afficherImage(); // a voir si fonction existe  Lydia


    ~Traitement();
};

#endif 
