#include "Library.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>

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

void Library::supprimerDescripteurs(int numero)
{
    //int cpt = 1;
    auto current = head;
    auto previous = head;
    while (current) {
        //current->data.setNumero(cpt);
        if (current->data.getNumero() == numero) {
            if (current == head) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }
            return;
        }
        //cpt++;
        previous = current;
        current = current->next;
    }
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



// Destructeur de la classe ListeChainee
Library::~Library() {
    while (head) { // Tant qu'il y a des nœuds
        auto temp = head; // Sauvegarde le nœud courant
        head = head->next; // Passe au nœud suivant
    }

}

