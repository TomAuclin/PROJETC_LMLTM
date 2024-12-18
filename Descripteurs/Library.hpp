#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "Image.hpp"
#include <string>
#include <filesystem>
#include <memory>

std::string openFileDialog();

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
    struct Node {
        Image data; /**< L'image stockée dans le nœud. */
        std::shared_ptr<Node> next; /**< Pointeur vers le nœud suivant. */

        /**
         * Constructeur de la structure Node.
         * img L'image à stocker dans le nœud.
         */
        Node(const Image& img) : data(img), next(nullptr) {}
    };

    std::shared_ptr<Node> head; /**< Pointeur partagé vers le premier nœud de la liste. */

public:
    
    Library();

    /**
     * Ajoute une image à la liste chaînée.
     * img L'image à ajouter au début de la liste.
     *
     */
    void ajouterDescripteurs(const Image& img);
    void supprimerDescripteurs(int numero);
    void modifdescripteurs( int numero, Library bibli);

    void tricroissant(Library liste);
    void trinbtraitement(Library liste);

    void afficher() const;

    void afficherGratuites() const;

    void modifierAcces(int numero, const std::string &nomFichier);

    //void modifierAcces(int numero);

    void modifierAccesDansFichier(const std::string &nomFichier, int numero);

    int tailleListe();

    void sousListePrix(Library liste);
    void sousListePrix(int numero);
    void sousListetype(Library Liste);
    void sousListetype(int numero);

    
    void afficherImagesAvecAccesO(const std::string& nomFichier) const;
    
    void sauvegarderDansFichier(const std::string& nomFichier) const;
    void chargerDepuisFichier(const std::string& nomFichier);

    //////////////////////
    void save();
    void permuterImages(int numero1, int numero2);

    void copyFile(const std::string& sourcePath, const std::string& destinationFolder); 
    //////////////////////

    void fusion(Library liste2);

    int titrecheck(std::string _titre) const; 
    /**
     * Recherche une image dans la liste par son numéro.
     *  numero Le numéro de l'image à rechercher.
     * retourne Une chaîne contenant les informations de l'image si elle est trouvée,
     * ou un message indiquant que l'image n'existe pas.
     */
    std::string rechercherImageParNumero(int numero) const;

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