#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "Image.hpp"
#include <string>
#include <filesystem>
#include <memory>


/**
 * @class ListeChainee
 * @brief Représente une liste chaînée contenant des objets de type Image.
 *
 * Cette classe permet d'ajouter des images, de rechercher une image
 * par son numéro, et de gérer la mémoire dynamiquement à l'aide de 
 * pointeurs intelligents.
 */
class Library {
private:
    /**
     * struct Node
     *  Un nœud de la liste chaînée.
     *
     * Chaque nœud represente une Image et un
     * pointeur partagé vers le nœud suivant.
     */
    struct INode {
        Image data; /**< L'image stockée dans le nœud. */
        std::shared_ptr<INode> next; /**< Pointeur vers le nœud suivant. */

        /**
         * Constructeur de la structure Node.
         * img L'image à stocker dans le nœud.
         */
        INode(const Image& img) : data(img), next(nullptr) {}
    };
 
    std::shared_ptr<INode> head; /**< Pointeur partagé vers le premier nœud de la liste. */

public:
    
    Library();

    /**
     * Ajoute une image à la liste chaînée.
     * img L'image à ajouter au début de la liste.
     *
     */
    void ajouterDescripteurs(const Image& img); // utilisateurs de niveau 1 peuvent ajouter
    void supprimerDescripteurs(int numero); // utilisateurs de niveua 1 peuvent supprimer
    void modifdescripteurs( int numero, Library bibli); // utilisateurs de niveau 1 peuvent modifier

    void tricroissant(Library liste); // tri par prix croissant
    void trinbtraitement(Library liste); // tri par nombre de traitement

    void afficher() const;  // Affichage des descripteurs 

    void afficherGratuites() const;  // Affichages des images gratuites 

    void modifierAcces(int numero, const std::string &nomFichier); // Pour donner l'acces au utilisateurs de niveau 2 

    void modifierAccesDansFichier(const std::string &nomFichier, int numero);

    int tailleListe(); 

    void sousListePrix(Library liste); // sous liste selon le prix 
    void sousListePrix(int numero);
    void sousListetype(Library Liste); // sous liste selon le type
    void sousListetype(int numero);

    
    void afficherImagesAvecAccesO(const std::string& nomFichier) const; // Affichage des image en accees Ouvert
    
    void sauvegarderDansFichier(const std::string& nomFichier) const; // sauvegarde des fichiers 
    void chargerDepuisFichier(const std::string& nomFichier); // chargement des fichiers 

    //////////////////////
    void save();
    void permuterImages(int numero1, int numero2); // permutation des images (utilisateurs de niveau 1)

    void copyFile(const std::string& sourcePath, const std::string& destinationFolder); 
    //////////////////////

    void fusion(Library liste2); // fusion de bibliothèque 

    int titrecheck(std::string _titre) const; 
    /**
     * Recherche une image dans la liste par son numéro.
     *  numero Le numéro de l'image à rechercher.
     * retourne Une chaîne contenant les informations de l'image si elle est trouvée,
     * ou un message indiquant que l'image n'existe pas.
     */
    std::string rechercherImageParNumero(int numero) const; // recherche d'une image par numéro 

    int numerocheck(int num) const;

    /**
     *Destructeur de la classe ListeChainee.
     *
     * pour libérer la mémoire
     *
     */
    ~Library();
};

#endif
