
#include "Library.hpp"
#include "Library.cpp"
#include "Image.hpp"
#include "Image.cpp"
#include <iostream>

int main() {
    Library liste;

    /// Gratuites :
    liste.ajouterDescripteurs(Image("Open Sources", "Baboon_couleur.png", 1, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Open Sources", "Baboon_gris.pgm", 2, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Barbara_couleur.png", 3, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Open Sources", "Barbara_gris.pgm", 4, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Bird_couleur.png", 5, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Open Sources", "Bird_gris.pgm", 6, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_gris.pgm", 8, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Geek_couleur.CR2", 9, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Geek_gris.pgm", 10, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "ImageAerienne_couleur.png", 11, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Open Sources", "ImageAerienne_gris.pgm", 12, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Lena_couleur.png", 13, 0, 'O', "couleur"));
    liste.ajouterDescripteurs(Image("Open Sources", "Lena_gris.pgm", 14, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Micro1_gris.png", 15, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Cameraman_gris.pgm", 16, 0, 'O', "gris"));
    liste.ajouterDescripteurs(Image("Open Sources", "Facade_gris.pgm", 17, 0, 'O', "gris"));


    // <9.99€ :
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_couleur.CR2", 18, 8, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_gris.pgm", 19, 7, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_couleur.CR2", 20, 8, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_gris.pgm", 21, 6, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_couleur.CR2", 22, 5, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_gris.pgm", 23, 2, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_couleur.CR2", 24, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_gris.pgm", 25, 7, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_couleur.CR2", 3, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_gris.pgm", 26, 9, 'L', "gris"));    
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_couleur.CR2", 27, 8, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_gris.pgm", 28, 3, 'L', "gris")); 
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_couleur.CR2", 29, 3, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_gris.pgm", 30, 1, 'L', "gris")); 
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_couleur.CR2", 31, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_gris.pgm", 32, 6, 'L', "gris")); 
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_couleur.CR2", 33, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_gris.pgm", 34, 7, 'L', "gris")); 
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_couleur.CR2", 35, 3, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_gris.pgm", 36, 1, 'L', "gris")); 
    liste.ajouterDescripteurs(Image("Tom", "Pieton_couleur.CR2", 37, 5, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Pieton_gris.pgm", 38, 3, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_couleur.CR2", 39, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_gris.pgm", 40, 9, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_couleur.CR2", 41, 5, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_gris.pgm", 42, 2, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_couleur.CR2", 43, 4, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_gris.pgm", 44, 2, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_couleur.CR2", 45, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_gris.pgm", 46, 8, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_couleur.CR2", 47, 9, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_gris.pgm", 48, 5, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_couleur.CR2", 49, 6, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_gris.pgm", 50, 4, 'L', "gris"));


    // <99.99€ :
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_couleur.CR2", 51, 99, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_gris.pgm", 52, 60, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_couleur.CR2", 53, 99, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_gris.pgm", 54, 60, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_couleur.CR2", 3, 81, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_gris.pgm", 55, 20, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_couleur.CR2", 56, 22, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_gris.pgm", 57, 10, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_couleur.CR2", 58, 15, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_gris.pgm", 59, 10, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_couleur.CR2", 60, 41, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_gris.pgm", 61, 20, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_couleur.CR2", 62, 90, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_gris.pgm", 63, 50, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_couleur.CR2", 64, 99, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_gris.pgm", 65, 80, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_couleur.CR2", 66, 60, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_gris.pgm", 67, 30, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_couleur.CR2", 68, 83, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_gris.pgm", 69, 51, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_couleur.CR2", 70, 98, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_gris.pgm", 71, 43, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_couleur.CR2", 72, 72, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_gris.pgm", 73, 48, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_couleur.CR2", 74, 93, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_gris.pgm", 75, 24, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_couleur.CR2", 76, 92, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_gris.pgm", 77, 36, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_couleur.CR2", 78, 46, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_gris.pgm", 79, 27, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_couleur.CR2", 80, 58, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_gris.pgm", 81, 34, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_couleur.CR2", 82, 72, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_gris.pgm", 83, 56, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_couleur.CR2", 84, 89, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_gris.pgm", 85, 33, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_couleur.CR2", 86, 82, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_gris.pgm", 87, 74, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Ville_couleur.CR2", 88, 78, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Ville_gris.pgm", 89, 66, 'L', "gris"));

    // >100€ :
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_couleur.CR2", 90, 110, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_gris.pgm", 91, 105, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_couleur.CR2", 92, 10000, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_gris.pgm", 93, 5000, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_couleur.CR2", 94, 153, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_gris.pgm", 95, 120, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_couleur.CR2", 96, 243, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_gris.pgm", 97, 203, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "Dome_couleur.CR2", 98, 225, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "Dome_gris.pgm", 99, 193, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_couleur.CR2", 100, 546, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_gris.pgm", 101, 348, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_couleur.CR2", 102, 166, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_gris.pgm", 103, 124, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_couleur.CR2", 104, 211, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_gris.pgm", 105, 158, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_couleur.CR2", 106, 213, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_gris.pgm", 107, 155, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_couleur.CR2", 108, 321, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_gris.pgm", 109, 298, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_couleur.CR2", 110, 483, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_gris.pgm", 111, 249, 'L', "gris"));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_couleur.CR2", 112, 647, 'L', "couleur"));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_gris.pgm", 113, 573, 'L', "gris"));


    // Recherche d'une image par numéro
    int numeroRecherche;
    std::cout << "Entrez le numero de l'image a rechercher : ";
    std::cin >> numeroRecherche;

    liste.tricroissant(liste);
    
    liste.afficher();
    std::cout << "La taille de la liste est de : " << liste.tailleListe() << std::endl;
    std::string resultat = liste.rechercherImageParNumero(numeroRecherche);
    std::cout << resultat << std::endl;
    //liste.afficher();
    liste.supprimerDescripteurs(113);
    liste.afficher();

    return 0;
}
