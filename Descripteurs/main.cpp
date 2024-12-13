#include "Library.hpp"
#include "Library.cpp"
#include "Image.hpp"
#include "Image.cpp"
#include <iostream>

int main() {
    Library liste;

     /// Gratuites :
    liste.ajouterDescripteurs(Image("Open Sources", "Baboon_couleur.png", 1, 0, 'O', "couleur", 0, 1));
    liste.ajouterDescripteurs(Image("Open Sources", "Baboon_gris.pgm", 2, 0, 'O', "gris", 0, 2));
    liste.ajouterDescripteurs(Image("Open Sources", "Barbara_couleur.png", 3, 0, 'O', "couleur", 0, 3));
    liste.ajouterDescripteurs(Image("Open Sources", "Barbara_gris.pgm", 4, 0, 'O', "gris", 0, 4));
    liste.ajouterDescripteurs(Image("Open Sources", "Bird_couleur.png", 5, 0, 'O', "couleur", 0, 5));
    liste.ajouterDescripteurs(Image("Open Sources", "Bird_gris.pgm", 6, 0, 'O', "gris", 0, 6));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur", 0, 7));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_gris.pgm", 8, 0, 'O', "gris", 0, 8));
    liste.ajouterDescripteurs(Image("Tom", "Geek_couleur.CR2", 9, 0, 'O', "couleur", 0, 9));
    liste.ajouterDescripteurs(Image("Tom", "Geek_gris.pgm", 10, 0, 'O', "gris", 0, 10));
    liste.ajouterDescripteurs(Image("Open Sources", "ImageAerienne_couleur.png", 11, 0, 'O', "couleur", 0, 11));
    liste.ajouterDescripteurs(Image("Open Sources", "ImageAerienne_gris.pgm", 12, 0, 'O', "gris", 0, 12));
    liste.ajouterDescripteurs(Image("Open Sources", "Lena_couleur.png", 13, 0, 'O', "couleur", 0, 13));
    liste.ajouterDescripteurs(Image("Open Sources", "Lena_gris.pgm", 14, 0, 'O', "gris", 0, 14));
    liste.ajouterDescripteurs(Image("Open Sources", "Micro1_gris.png", 15, 0, 'O', "gris", 0, 15));
    liste.ajouterDescripteurs(Image("Open Sources", "Cameraman_gris.pgm", 16, 0, 'O', "gris", 0, 16));
    liste.ajouterDescripteurs(Image("Open Sources", "Facade_gris.pgm", 17, 0, 'O', "gris", 0, 17));


    // <9.99€ :
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_couleur.CR2", 18, 8, 'L', "couleur", 4, 18));
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_gris.pgm", 19, 7, 'L', "gris", 4, 19));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_couleur.CR2", 20, 8, 'L', "couleur", 5, 20));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_gris.pgm", 21, 6, 'L', "gris", 5, 21));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_couleur.CR2", 22, 5, 'L', "couleur", 3, 22));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_gris.pgm", 23, 2, 'L', "gris", 3, 23));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_couleur.CR2", 24, 9, 'L', "couleur", 6, 24));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_gris.pgm", 25, 7, 'L', "gris", 5, 25));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_couleur.CR2", 26, 9, 'L', "couleur", 6, 26));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_gris.pgm", 27, 9, 'L', "gris", 5, 27));    
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_couleur.CR2", 28, 8, 'L', "couleur", 6, 28));
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_gris.pgm", 29, 3, 'L', "gris", 5, 29)); 
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_couleur.CR2", 30, 3, 'L', "couleur", 6, 30));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_gris.pgm", 31, 1, 'L', "gris", 5, 31)); 
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_couleur.CR2", 32, 9, 'L', "couleur", 4, 32));
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_gris.pgm", 33, 6, 'L', "gris", 4, 33)); 
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_couleur.CR2", 34, 9, 'L', "couleur", 4, 34));
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_gris.pgm", 35, 7, 'L', "gris", 3, 35)); 
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_couleur.CR2", 36, 3, 'L', "couleur", 6, 36));
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_gris.pgm", 37, 1, 'L', "gris", 5, 37)); 
    liste.ajouterDescripteurs(Image("Tom", "Pieton_couleur.CR2", 38, 5, 'L', "couleur", 4, 38));
    liste.ajouterDescripteurs(Image("Tom", "Pieton_gris.pgm", 39, 3, 'L', "gris", 3, 39));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_couleur.CR2", 40, 9, 'L', "couleur", 5, 40));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_gris.pgm", 41, 9, 'L', "gris", 4, 41));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_couleur.CR2", 42, 5, 'L', "couleur", 5, 42));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_gris.pgm", 43, 2, 'L', "gris", 4, 43));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_couleur.CR2", 44, 4, 'L', "couleur", 5, 44));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_gris.pgm", 45, 2, 'L', "gris", 4, 45));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_couleur.CR2", 46, 9, 'L', "couleur", 6, 46));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_gris.pgm", 47, 8, 'L', "gris", 5, 47));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_couleur.CR2", 48, 9, 'L', "couleur", 5, 48));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_gris.pgm", 49, 5, 'L', "gris", 4, 49));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_couleur.CR2", 50, 6, 'L', "couleur", 6, 50));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_gris.pgm", 51, 4, 'L', "gris", 5, 51));


    // <99.99€ :
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_couleur.CR2", 52, 99, 'L', "couleur", 0, 52));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_gris.pgm", 53, 60, 'L', "gris", 0, 53));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_couleur.CR2", 54, 99, 'L', "couleur", 0, 54));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_gris.pgm", 55, 60, 'L', "gris", 0, 55));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_couleur.CR2", 56, 81, 'L', "couleur", 0, 56));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_gris.pgm", 57, 20, 'L', "gris", 0, 57));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_couleur.CR2", 58, 22, 'L', "couleur", 0, 58));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_gris.pgm", 59, 10, 'L', "gris", 0, 59));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_couleur.CR2", 60, 15, 'L', "couleur", 0, 60));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_gris.pgm", 61, 10, 'L', "gris", 0, 61));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_couleur.CR2", 62, 41, 'L', "couleur", 0, 62));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_gris.pgm", 63, 20, 'L', "gris", 0, 63));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_couleur.CR2", 64, 90, 'L', "couleur", 0, 64));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_gris.pgm", 65, 50, 'L', "gris", 0, 65));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_couleur.CR2", 66, 99, 'L', "couleur", 0, 66));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_gris.pgm", 67, 80, 'L', "gris", 0, 67));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_couleur.CR2", 68, 60, 'L', "couleur", 0, 68));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_gris.pgm", 69, 30, 'L', "gris", 0, 69));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_couleur.CR2", 70, 83, 'L', "couleur", 0, 70));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_gris.pgm", 71, 51, 'L', "gris", 0, 71));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_couleur.CR2", 72, 98, 'L', "couleur", 0, 72));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_gris.pgm", 73, 43, 'L', "gris", 0, 73));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_couleur.CR2", 74, 72, 'L', "couleur", 0, 74));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_gris.pgm", 75, 48, 'L', "gris", 0, 75));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_couleur.CR2", 76, 93, 'L', "couleur", 0, 76));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_gris.pgm", 77, 24, 'L', "gris", 0, 77));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_couleur.CR2", 78, 92, 'L', "couleur", 0, 78));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_gris.pgm", 79, 36, 'L', "gris", 0, 79));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_couleur.CR2", 80, 46, 'L', "couleur", 0, 80));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_gris.pgm", 81, 27, 'L', "gris", 0, 81));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_couleur.CR2", 82, 58, 'L', "couleur", 0, 82));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_gris.pgm", 83, 34, 'L', "gris", 0, 83));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_couleur.CR2", 84, 72, 'L', "couleur", 0, 84));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_gris.pgm", 85, 56, 'L', "gris", 0, 85));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_couleur.CR2", 86, 89, 'L', "couleur", 0, 86));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_gris.pgm", 87, 33, 'L', "gris", 0, 87));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_couleur.CR2", 88, 82, 'L', "couleur", 0, 88));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_gris.pgm", 89, 74, 'L', "gris", 0, 89));
    liste.ajouterDescripteurs(Image("Tom", "Ville_couleur.CR2", 90, 78, 'L', "couleur", 0, 90));
    liste.ajouterDescripteurs(Image("Tom", "Ville_gris.pgm", 91, 66, 'L', "gris", 0, 91));

    // >100€ :
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_couleur.CR2", 92, 110, 'L', "couleur", 0, 92));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_gris.pgm", 93, 105, 'L', "gris", 0, 93));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_couleur.CR2", 94, 10000, 'L', "couleur", 0, 94));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_gris.pgm", 95, 5000, 'L', "gris", 0, 95));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_couleur.CR2", 96, 153, 'L', "couleur", 0, 96));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_gris.pgm", 97, 120, 'L', "gris", 0, 97));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_couleur.CR2", 98, 243, 'L', "couleur", 0, 98));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_gris.pgm", 99, 203, 'L', "gris", 0, 99));
    liste.ajouterDescripteurs(Image("Tom", "Dome_couleur.CR2", 100, 225, 'L', "couleur", 0, 100));
    liste.ajouterDescripteurs(Image("Tom", "Dome_gris.pgm", 101, 193, 'L', "gris", 0, 101));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_couleur.CR2", 102, 546, 'L', "couleur", 0, 102));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_gris.pgm", 103, 348, 'L', "gris", 0, 103));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_couleur.CR2", 104, 166, 'L', "couleur", 0, 104));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_gris.pgm", 105, 124, 'L', "gris", 0, 105));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_couleur.CR2", 106, 211, 'L', "couleur", 0, 106));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_gris.pgm", 107, 158, 'L', "gris", 0, 107));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_couleur.CR2", 108, 213, 'L', "couleur", 0, 108));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_gris.pgm", 109, 155, 'L', "gris", 0, 109));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_couleur.CR2", 110, 321, 'L', "couleur", 0, 110));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_gris.pgm", 111, 298, 'L', "gris", 0, 111));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_couleur.CR2", 112, 483, 'L', "couleur", 0, 112));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_gris.pgm", 113, 249, 'L', "gris", 0, 113));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_couleur.CR2", 114, 647, 'L', "couleur", 0, 114));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_gris.pgm", 115, 573, 'L', "gris", 0, 115));






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


    return 0;
}