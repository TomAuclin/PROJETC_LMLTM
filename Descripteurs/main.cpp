#include "ListeChainee.hpp"
#include "ListeChainee.cpp"
#include "Image.hpp"
#include "Image.cpp"
#include <iostream>

int main() {
    ListeChainee liste;

    /// Gratuites :
    liste.ajouterImage(Image("Open Sources", "Baboon_couleur.png", 1, 0, 'O', "couleur", 0, 1));
    liste.ajouterImage(Image("Open Sources", "Baboon_gris.pgm", 2, 0, 'O', "gris", 0, 2));
    liste.ajouterImage(Image("Open Sources", "Barbara_couleur.png", 3, 0, 'O', "couleur", 0, 3));
    liste.ajouterImage(Image("Open Sources", "Barbara_gris.pgm", 4, 0, 'O', "gris", 0, 4));
    liste.ajouterImage(Image("Open Sources", "Bird_couleur.png", 5, 0, 'O', "couleur", 0, 5));
    liste.ajouterImage(Image("Open Sources", "Bird_gris.pgm", 6, 0, 'O', "gris", 0, 6));
    liste.ajouterImage(Image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur", 0, 7));
    liste.ajouterImage(Image("Tom", "EauToitPont_gris.pgm", 8, 0, 'O', "gris", 0, 8));
    liste.ajouterImage(Image("Tom", "Geek_couleur.CR2", 9, 0, 'O', "couleur", 0, 9));
    liste.ajouterImage(Image("Tom", "Geek_gris.pgm", 10, 0, 'O', "gris", 0, 10));
    liste.ajouterImage(Image("Open Sources", "ImageAerienne_couleur.png", 11, 0, 'O', "couleur", 0, 11));
    liste.ajouterImage(Image("Open Sources", "ImageAerienne_gris.pgm", 12, 0, 'O', "gris", 0, 12));
    liste.ajouterImage(Image("Open Sources", "Lena_couleur.png", 13, 0, 'O', "couleur", 0, 13));
    liste.ajouterImage(Image("Open Sources", "Lena_gris.pgm", 14, 0, 'O', "gris", 0, 14));
    liste.ajouterImage(Image("Open Sources", "Micro1_gris.png", 15, 0, 'O', "gris", 0, 15));
    liste.ajouterImage(Image("Open Sources", "Cameraman_gris.pgm", 16, 0, 'O', "gris", 0, 16));
    liste.ajouterImage(Image("Open Sources", "Facade_gris.pgm", 17, 0, 'O', "gris", 0, 17));


    // <9.99€ :
    liste.ajouterImage(Image("Tom", "ArbreMultiBranche_couleur.CR2", 18, 8, 'L', "couleur", 0, 18));
    liste.ajouterImage(Image("Tom", "ArbreMultiBranche_gris.pgm", 19, 7, 'L', "gris", 0, 19));
    liste.ajouterImage(Image("Tom", "Banc_02_couleur.CR2", 20, 8, 'L', "couleur", 0, 20));
    liste.ajouterImage(Image("Tom", "Banc_02_gris.pgm", 21, 6, 'L', "gris", 0, 21));
    liste.ajouterImage(Image("Tom", "BandeSol_couleur.CR2", 22, 5, 'L', "couleur", 0, 22));
    liste.ajouterImage(Image("Tom", "BandeSol_gris.pgm", 23, 2, 'L', "gris", 0, 23));
    liste.ajouterImage(Image("Tom", "Bateau01_couleur.CR2", 24, 9, 'L', "couleur", 0, 24));
    liste.ajouterImage(Image("Tom", "Bateau01_gris.pgm", 25, 7, 'L', "gris", 0, 25));
    liste.ajouterImage(Image("Lydia", "BateauCote_couleur.CR2", 26, 9, 'L', "couleur", 0, 26));
    liste.ajouterImage(Image("Lydia", "BateauCote_gris.pgm", 27, 9, 'L', "gris", 0, 27));    
    liste.ajouterImage(Image("Tom", "BriqueMur_couleur.CR2", 28, 8, 'L', "couleur", 0, 28));
    liste.ajouterImage(Image("Tom", "BriqueMur_gris.pgm", 29, 3, 'L', "gris", 0, 29)); 
    liste.ajouterImage(Image("Tom", "CedezPassage01_couleur.CR2", 30, 3, 'L', "couleur", 0, 30));
    liste.ajouterImage(Image("Tom", "CedezPassage01_gris.pgm", 31, 1, 'L', "gris", 0, 31)); 
    liste.ajouterImage(Image("Lydia", "GaletSol_couleur.CR2", 32, 9, 'L', "couleur", 0, 32));
    liste.ajouterImage(Image("Lydia", "GaletSol_gris.pgm", 33, 6, 'L', "gris", 0, 33)); 
    liste.ajouterImage(Image("Tom", "LimaceVelo_couleur.CR2", 34, 9, 'L', "couleur", 0, 34));
    liste.ajouterImage(Image("Tom", "LimaceVelo_gris.pgm", 35, 7, 'L', "gris", 0, 35)); 
    liste.ajouterImage(Image("Tom", "PanneauPieton_couleur.CR2", 36, 3, 'L', "couleur", 0, 36));
    liste.ajouterImage(Image("Tom", "PanneauPieton_gris.pgm", 37, 1, 'L', "gris", 0, 37)); 
    liste.ajouterImage(Image("Tom", "Pieton_couleur.CR2", 38, 5, 'L', "couleur", 0, 38));
    liste.ajouterImage(Image("Tom", "Pieton_gris.pgm", 39, 3, 'L', "gris", 0, 39));
    liste.ajouterImage(Image("Lydia", "PlanteEau01_couleur.CR2", 40, 9, 'L', "couleur", 0, 40));
    liste.ajouterImage(Image("Lydia", "PlanteEau01_gris.pgm", 41, 9, 'L', "gris", 0, 41));
    liste.ajouterImage(Image("Lydia", "PontGauche01_couleur.CR2", 42, 5, 'L', "couleur", 0, 42));
    liste.ajouterImage(Image("Lydia", "PontGauche01_gris.pgm", 43, 2, 'L', "gris", 0, 43));
    liste.ajouterImage(Image("Lydia", "SolBois01_couleur.CR2", 44, 4, 'L', "couleur", 0, 44));
    liste.ajouterImage(Image("Lydia", "SolBois01_gris.pgm", 45, 2, 'L', "gris", 0, 45));
    liste.ajouterImage(Image("Lydia", "Statu_couleur.CR2", 46, 9, 'L', "couleur", 0, 46));
    liste.ajouterImage(Image("Lydia", "Statu_gris.pgm", 47, 8, 'L', "gris", 0, 47));
    liste.ajouterImage(Image("Lydia", "Ver_couleur.CR2", 48, 9, 'L', "couleur", 0, 48));
    liste.ajouterImage(Image("Lydia", "Ver_gris.pgm", 49, 5, 'L', "gris", 0, 49));
    liste.ajouterImage(Image("Tom", "Vitraille_couleur.CR2", 50, 6, 'L', "couleur", 0, 50));
    liste.ajouterImage(Image("Tom", "Vitraille_gris.pgm", 51, 4, 'L', "gris", 0, 51));


    // <99.99€ :
    liste.ajouterImage(Image("Lydia", "BateauEau01_couleur.CR2", 52, 99, 'L', "couleur", 0, 52));
    liste.ajouterImage(Image("Lydia", "BateauEau01_gris.pgm", 53, 60, 'L', "gris", 0, 53));
    liste.ajouterImage(Image("Lydia", "BateauEau04_couleur.CR2", 54, 99, 'L', "couleur", 0, 54));
    liste.ajouterImage(Image("Lydia", "BateauEau04_gris.pgm", 55, 60, 'L', "gris", 0, 55));
    liste.ajouterImage(Image("Tom", "BeaucoupVer_couleur.CR2", 56, 81, 'L', "couleur", 0, 56));
    liste.ajouterImage(Image("Tom", "BeaucoupVer_gris.pgm", 57, 20, 'L', "gris", 0, 57));
    liste.ajouterImage(Image("Tom", "BriqueSol01_couleur.CR2", 58, 22, 'L', "couleur", 0, 58));
    liste.ajouterImage(Image("Tom", "BriqueSol01_gris.pgm", 59, 10, 'L', "gris", 0, 59));
    liste.ajouterImage(Image("Lydia", "BriqueSol02_couleur.CR2", 60, 15, 'L', "couleur", 0, 60));
    liste.ajouterImage(Image("Lydia", "BriqueSol02_gris.pgm", 61, 10, 'L', "gris", 0, 61));
    liste.ajouterImage(Image("Tom", "CedezPassage02_couleur.CR2", 62, 41, 'L', "couleur", 0, 62));
    liste.ajouterImage(Image("Tom", "CedezPassage02_gris.pgm", 63, 20, 'L', "gris", 0, 63));
    liste.ajouterImage(Image("Tom", "DessousPont01_couleur.CR2", 64, 90, 'L', "couleur", 0, 64));
    liste.ajouterImage(Image("Tom", "DessousPont01_gris.pgm", 65, 50, 'L', "gris", 0, 65));
    liste.ajouterImage(Image("Tom", "DomePont_couleur.CR2", 66, 99, 'L', "couleur", 0, 66));
    liste.ajouterImage(Image("Tom", "DomePont_gris.pgm", 67, 80, 'L', "gris", 0, 67));
    liste.ajouterImage(Image("Lydia", "Grille01_couleur.CR2", 68, 60, 'L', "couleur", 0, 68));
    liste.ajouterImage(Image("Lydia", "Grille01_gris.pgm", 69, 30, 'L', "gris", 0, 69));
    liste.ajouterImage(Image("Tom", "GrosArbre01_couleur.CR2", 70, 83, 'L', "couleur", 0, 70));
    liste.ajouterImage(Image("Tom", "GrosArbre01_gris.pgm", 71, 51, 'L', "gris", 0, 71));
    liste.ajouterImage(Image("Tom", "LaboPhonix_couleur.CR2", 72, 98, 'L', "couleur", 0, 72));
    liste.ajouterImage(Image("Tom", "LaboPhonix_gris.pgm", 73, 43, 'L', "gris", 0, 73));
    liste.ajouterImage(Image("Tom", "MagnifiquePontPret_couleur.CR2", 74, 72, 'L', "couleur", 0, 74));
    liste.ajouterImage(Image("Tom", "MagnifiquePontPret_gris.pgm", 75, 48, 'L', "gris", 0, 75));
    liste.ajouterImage(Image("Lydia", "Mario01_couleur.CR2", 76, 93, 'L', "couleur", 0, 76));
    liste.ajouterImage(Image("Lydia", "Mario01_gris.pgm", 77, 24, 'L', "gris", 0, 77));
    liste.ajouterImage(Image("Tom", "PlanteBeaucoup_couleur.CR2", 78, 92, 'L', "couleur", 0, 78));
    liste.ajouterImage(Image("Tom", "PlanteBeaucoup_gris.pgm", 79, 36, 'L', "gris", 0, 79));
    liste.ajouterImage(Image("Lydia", "PontPlante01_couleur.CR2", 80, 46, 'L', "couleur", 0, 80));
    liste.ajouterImage(Image("Lydia", "PontPlante01_gris.pgm", 81, 27, 'L', "gris", 0, 81));
    liste.ajouterImage(Image("Tom", "PontGens_couleur.CR2", 82, 58, 'L', "couleur", 0, 82));
    liste.ajouterImage(Image("Tom", "PontGens_gris.pgm", 83, 34, 'L', "gris", 0, 83));
    liste.ajouterImage(Image("Tom", "Rugby_couleur.CR2", 84, 72, 'L', "couleur", 0, 84));
    liste.ajouterImage(Image("Tom", "Rugby_gris.pgm", 85, 56, 'L', "gris", 0, 85));
    liste.ajouterImage(Image("Tom", "SousPontLumineux_couleur.CR2", 86, 89, 'L', "couleur", 0, 86));
    liste.ajouterImage(Image("Tom", "SousPontLumineux_gris.pgm", 87, 33, 'L', "gris", 0, 87));
    liste.ajouterImage(Image("Tom", "StatuLoin_couleur.CR2", 88, 82, 'L', "couleur", 0, 88));
    liste.ajouterImage(Image("Tom", "StatuLoin_gris.pgm", 89, 74, 'L', "gris", 0, 89));
    liste.ajouterImage(Image("Tom", "Ville_couleur.CR2", 90, 78, 'L', "couleur", 0, 90));
    liste.ajouterImage(Image("Tom", "Ville_gris.pgm", 91, 66, 'L', "gris", 0, 91));

    // >100€ :
    liste.ajouterImage(Image("Lydia", "BriqueSolLoin_couleur.CR2", 92, 110, 'L', "couleur", 0, 92));
    liste.ajouterImage(Image("Lydia", "BriqueSolLoin_gris.pgm", 93, 105, 'L', "gris", 0, 93));
    liste.ajouterImage(Image("Tom", "ChaussureLydia_couleur.CR2", 94, 10000, 'L', "couleur", 0, 94));
    liste.ajouterImage(Image("Tom", "ChaussureLydia_gris.pgm", 95, 5000, 'L', "gris", 0, 95));
    liste.ajouterImage(Image("Tom", "CotePont_couleur.CR2", 96, 153, 'L', "couleur", 0, 96));
    liste.ajouterImage(Image("Tom", "CotePont_gris.pgm", 97, 120, 'L', "gris", 0, 97));
    liste.ajouterImage(Image("Tom", "DessusPont_couleur.CR2", 98, 243, 'L', "couleur", 0, 98));
    liste.ajouterImage(Image("Tom", "DessusPont_gris.pgm", 99, 203, 'L', "gris", 0, 99));
    liste.ajouterImage(Image("Tom", "Dome_couleur.CR2", 100, 225, 'L', "couleur", 0, 100));
    liste.ajouterImage(Image("Tom", "Dome_gris.pgm", 101, 193, 'L', "gris", 0, 101));
    liste.ajouterImage(Image("Tom", "EauBord_couleur.CR2", 102, 546, 'L', "couleur", 0, 102));
    liste.ajouterImage(Image("Tom", "EauBord_gris.pgm", 103, 348, 'L', "gris", 0, 103));
    liste.ajouterImage(Image("Tom", "GruArbre_couleur.CR2", 104, 166, 'L', "couleur", 0, 104));
    liste.ajouterImage(Image("Tom", "GruArbre_gris.pgm", 105, 124, 'L', "gris", 0, 105));
    liste.ajouterImage(Image("Lydia", "MurArcEnCiel01_couleur.CR2", 106, 211, 'L', "couleur", 0, 106));
    liste.ajouterImage(Image("Lydia", "MurArcEnCiel01_gris.pgm", 107, 158, 'L', "gris", 0, 107));
    liste.ajouterImage(Image("Tom", "MurArcEnCiel02_couleur.CR2", 108, 213, 'L', "couleur", 0, 108));
    liste.ajouterImage(Image("Tom", "MurArcEnCiel02_gris.pgm", 109, 155, 'L', "gris", 0, 109));
    liste.ajouterImage(Image("Tom", "MurEauGens_couleur.CR2", 110, 321, 'L', "couleur", 0, 110));
    liste.ajouterImage(Image("Tom", "MurEauGens_gris.pgm", 111, 298, 'L', "gris", 0, 111));
    liste.ajouterImage(Image("Lydia", "PlantePont02_couleur.CR2", 112, 483, 'L', "couleur", 0, 112));
    liste.ajouterImage(Image("Lydia", "PlantePont02_gris.pgm", 113, 249, 'L', "gris", 0, 113));
    liste.ajouterImage(Image("Tom", "StatuPorte_couleur.CR2", 114, 647, 'L', "couleur", 0, 114));
    liste.ajouterImage(Image("Tom", "StatuPorte_gris.pgm", 115, 573, 'L', "gris", 0, 115));


    // Recherche d'une image par numéro
    int numeroRecherche;
    std::cout << "Entrez le numero de l'image a rechercher : ";
    std::cin >> numeroRecherche;

    std::string resultat = liste.rechercherImageParNumero(numeroRecherche);
    std::cout << resultat << std::endl;

    return 0;
}
