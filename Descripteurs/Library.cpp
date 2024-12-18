#include "Library.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <memory>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/**
 *
 * Initialise une liste chaînée vide en définissant la tête de la liste à nullptr.
 */
Library::Library() : head(nullptr) {}

/**
 * Ajoute   img  une image à la liste chaînée.
 * L'image est insérée au début de la liste en créant un nouveau nœud qui pointe
 * vers l'ancien premier nœud.
 */
void Library::ajouterDescripteurs(const Image& img) {
    auto nouveau = std::make_shared<Node>(img); // Crée un nouveau nœud
    nouveau->next = head; // L'ancien premier nœud devient le suivant
    head = nouveau; // La tête pointe maintenant vers le nouveau nœud
}

void  Library::tricroissant(Library liste)

{

    auto current = head;
    
    for (int i = 0; i <liste.tailleListe()-1; i ++ )
    
    {current = head;
    //std::cout << current->data.getDescripteur() << std::endl;
        for (int j = 0; j <liste.tailleListe()-1; j ++ )
        
        
        {
            
            if (current->data.getPrix() > current->next->data.getPrix())
            {
                std::cout << "Prix 1 : " << current->data.getPrix() << std::endl;
                std::cout << "Prix 2 : " << current->next->data.getPrix() << std::endl;
                auto temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                
            }
            current = current->next;
            
        }
        
       
    }
   
  
}

void Library::afficher() const {
    auto current = head;
    std::cout << "Liste des images : " << std::endl;
    //std::cout << current->data.getDescripteur() << std::endl;
    while (current ) {
        std::cout << current->data.getDescripteur() << std::endl;
        current = current->next;
    }

}

int Library::tailleListe()
{
    int taille = 0;
    auto current = head;
    while (current) {
        taille++;
        current = current->next;
    }
    //std::cout << "La taille de la liste est de : " << taille << std::endl;
    return taille;
}

/**
 * Recherche une image dans la liste en fonction de son numéro.
 * 
 *retourne  Une chaîne contenant le descripteur de l'image si elle est trouvée,
 * ou un message indiquant que l'image n'existe pas.
 *
 * La méthode parcourt la liste chaînée à partir de la tête et compare
 * les numéros des images jusqu'à trouver une correspondance ou atteindre
 * la fin de la liste.
 */
std::string Library::rechercherImageParNumero(int numero) const {
    auto current = head; // Commence à la tête de la liste
    while (current) { // Parcourt la liste tant qu'il y a des nœuds
        if (current->data.getNumero() == numero) {
            return current->data.getDescripteur(); // Retourne le descripteur si trouvé
        }
        current = current->next; // Passe au nœud suivant
    }
    return "Image non trouvée."; // Retourne un message si aucune image ne correspond
}



void Library::sauvegarderDansFichier(const std::string& nomFichier) const {
    std::ofstream fichier(nomFichier);

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return;
    }

    auto current = head;
    if (!current) {
        std::cerr << "La liste est vide, rien à sauvegarder." << std::endl;
        return;
    }

    while (current) {
        std::string descripteur = current->data.getDescripteurSimple();
        if (!descripteur.empty()) {
            fichier << descripteur << "\n"; // Enregistrement ligne par ligne
        }
        current = current->next;
    }

    fichier.close();
    std::cout << "Les descripteurs ont été sauvegardés dans " << nomFichier << std::endl;
}


void Library::chargerDepuisFichier(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::istringstream iss(ligne);
        std::string source, titre, type;
        int numero, nbTraitementPossible, identite;
        double prix;
        char acces;

        // Lire chaque champ séparé par une virgule
        if (std::getline(iss, source, ',') &&
            std::getline(iss, titre, ',') &&
            iss >> numero &&
            iss.ignore(1) && // Ignorer la virgule
            iss >> prix &&
            iss.ignore(1) &&
            iss >> acces &&
            iss.ignore(1) &&
            std::getline(iss, type, ',') &&
            iss >> nbTraitementPossible &&
            iss.ignore(1) &&
            iss >> identite) {

            // Ajouter le descripteur sans modification des espaces
            ajouterDescripteurs(Image(source, titre, numero, prix, acces, type, nbTraitementPossible, identite));
        } else {
            std::cerr << "Erreur lors du traitement de la ligne : " << ligne << std::endl;
        }
    }

    fichier.close();
    std::cout << "Les descripteurs ont été chargés depuis " << nomFichier << std::endl;
}

////////////////////////////////
// Fonction pour ouvrir une boîte de dialogue et sélectionner un fichier
std::string openFileDialog() {
    char fileName[MAX_PATH] = "";
    OPENFILENAME ofn = {0};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = nullptr; // Pas de fenêtre parent
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.jpeg;*.bmp;*.gif\0Tous les fichiers\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return std::string(fileName); // Retourne le chemin du fichier sélectionné
    }

    return ""; // Si l'utilisateur annule, retourne une chaîne vide
}

// Fonction pour copier un fichier dans un dossier de destination
void copyFile(const std::string& sourcePath, const std::string& destinationFolder) {
    fs::path sourceFile(sourcePath);

    // Vérifier si le fichier source existe
    if (!fs::exists(sourceFile)) {
        std::cerr << "Erreur : le fichier n'existe pas." << std::endl;
        return;
    }

    // Créer le dossier de destination s'il n'existe pas
    if (!fs::exists(destinationFolder)) {
        fs::create_directories(destinationFolder);
    }

    // Construire le chemin de destination
    fs::path destinationFile = fs::path(destinationFolder) / sourceFile.filename();

    // Copier le fichier
    try {
        fs::copy(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
        std::cout << "Fichier copié dans : " << destinationFile << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la copie du fichier : " << e.what() << std::endl;
    }
}

//////////////////////////////

/**
 * Permet de permuter deux images dans la bibliothèque en se basant sur leurs numéros (ils sont uniques)
 */
void Library::permuterImages(int numero1, int numero2) {
    if (numero1 == numero2) {
        std::cout << "Les deux numéros sont identiques, aucune permutation nécessaire" << std::endl;
        return;
    }

    // Pointeurs pour les nœuds contenant les images à permuter
    std::shared_ptr<Node> node1 = nullptr, node2 = nullptr;
    auto current = head;

    // Parcours de la liste pour trouver les nœuds correspondants
    while (current) {
        if (current->data.getNumero() == numero1) {
            node1 = current;
        } else if (current->data.getNumero() == numero2) {
            node2 = current;
        }
        current = current->next;
    }

    // Vérification si les deux images existent
    if (!node1 || !node2) {
        std::cerr << "Erreur : Une ou les deux images n'existent pas dans la bibliothèque." << std::endl;
        return;
    }

    // Permutation des données des deux nœuds
    Image temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;

    std::cout << "Les images avec les numéros " << numero1 << " et " << numero2 << " ont été permutées." << std::endl;
}



void Library::save() {

    /*Fonction save : demande a l'utilisateur de naviguer dans ces fichier pour clique sur une image 
    -> puis remplir a la main les descripteur (terminal ça va demander le prix etc...)
    -> et va enregistrer l'image dans la biblio et les descripteur a la suite de la liste .txt*/

    std::string cheminImage, source, titre, type;
    int numero, nbTraitementPossible, identite;
    double prix;
    char acces;
    /*
    std::cout << "Entrez le chemin complet de l'image à ajouter : ";
    std::cin.ignore();
    std::getline(std::cin, cheminImage);

    if (cheminImage.empty()){
        std::cerr << "chemin introuvable " << std::endl;    }*/

    std::cout << "Sélectionnez une image à ajouter.\n";
    cheminImage = openFileDialog();

    if (cheminImage.empty()) {
        std::cerr << "Aucun fichier sélectionné.\n";
        return;
    }

    //Renseigner les descripteurs de l'image à la main 
    std::cout << "Source : ";
    std::cin >> source;
    std::cin.ignore();
    std::cout << "Titre : ";
    std::getline(std::cin, titre);
    std::cout << "Numéro unique : ";
    std::cin >> numero;
    std::cout << "Prix (en €) : ";
    std::cin >> prix;
    std::cout << "Accès (O pour ouvert, L pour limité) : ";
    std::cin >> acces;
    std::cout << "Type (par ex. 'couleur', 'gris') : ";
    std::cin >> type;
    std::cout << "Nombre de traitements possibles : ";
    std::cin >> nbTraitementPossible;
    std::cout << "Identité (ID utilisateur) : ";
    std::cin >> identite;


    //determine la biblio cible en fonction du prix
    // Déterminer la bibliothèque cible en fonction du prix
    std::string basePath = "C:/Users/leona_u6zzw0q/M2SIA2/PROJET/PROJETC_LMLTM/Bibliotheque/";


    std::string destinationDir;
    if (prix == 0) {
        destinationDir = basePath + "Gratuites";
    } else if (prix < 9.99) {
        destinationDir = basePath + "_9.99";
    } else if (prix < 99.99) {
        destinationDir = basePath + "_99.99";
    } else {
        destinationDir = basePath + "_100";
    }

    // Créer le répertoire cible si nécessaire
    fs::create_directories(destinationDir);

    // Copier l'image dans la bibliothèque dediée
    std::string destination = destinationDir + "/" + titre;
    try {
        fs::copy_file(cheminImage, destination, fs::copy_options::overwrite_existing);
        std::cout << "Image copiée dans : " << destination << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la copie de l'image : " << e.what() << std::endl;
        return;
    }

    //Ajouter à la bibliothèque
    Image nouvelleImage(source, titre, numero, prix, acces, type, nbTraitementPossible, identite);
    ajouterDescripteurs(nouvelleImage);



    //sauver avec un nom unique de bibliotheque pour ne pas ecraser les anciennes versions 
    // (on genere un nom unique pour pas que l'utilisateur ait a rentrer un nom de biblio)

    auto now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "Bibliotheque_%Y%m%d_%H%M%S.txt", std::localtime(&now));
    std::string nomBibliotheque = buffer;

    //partie save de la biblio

    std::ofstream fichier(nomBibliotheque);
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomBibliotheque << ".\n";
        return;
    }

    auto current = head;
    while (current) {
        fichier << current->data.getDescripteurSimple() << "\n";
        current = current->next;
    }

    fichier.close();

    //(jsp si c'est utile de dire le nom mais pour tester si la fonction marche c'est pratique)
    std::cout << "La nouvelle bibliothèque a été sauvegardée sous le nom : " << nomBibliotheque << std::endl;

}

// Destructeur de la classe ListeChainee
Library::~Library() {
    while (head) { // Tant qu'il y a des nœuds
        auto temp = head; // Sauvegarde le nœud courant
        head = head->next; // Passe au nœud suivant
    }

}

