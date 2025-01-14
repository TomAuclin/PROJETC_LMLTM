#ifndef LISTECHAINEE_HPP
#define LISTECHAINEE_HPP

#include "Image.hpp"
#include <memory>

/**
 * @class ListeChainee
 * @brief Représente une liste chaînée contenant des objets de type Image.
 *
 * Cette classe permet d'ajouter des images, de rechercher une image
 * par son numéro, et de gérer la mémoire dynamiquement à l'aide de 
 * pointeurs intelligents.
 */
class ListeChainee {
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
    
    ListeChainee();

    /**
     * Ajoute une image à la liste chaînée.
     * img L'image à ajouter au début de la liste.
     *
     */
    void ajouterImage(const Image& img);

    /**
     * Recherche une image dans la liste par son numéro.
     *  numero Le numéro de l'image à rechercher.
     * retourne Une chaîne contenant les informations de l'image si elle est trouvée,
     * ou un message indiquant que l'image n'existe pas.
     */
    std::string rechercherImageParNumero(int numero) const;

    /**
     *Destructeur de la classe ListeChainee.
     *
     * pour libérer la mémoire 
     *
     */
    ~ListeChainee();
};

#endif

