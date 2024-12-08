#include "ListeChainee.hpp"

/**
 *
 * Initialise une liste chaînée vide en définissant la tête de la liste à nullptr.
 */
ListeChainee::ListeChainee() : head(nullptr) {}

/**
 * Ajoute   img  une image à la liste chaînée.
 * L'image est insérée au début de la liste en créant un nouveau nœud qui pointe
 * vers l'ancien premier nœud.
 */
void ListeChainee::ajouterImage(const Image& img) {
    auto nouveau = std::make_shared<Node>(img); // Crée un nouveau nœud
    nouveau->next = head; // L'ancien premier nœud devient le suivant
    head = nouveau; // La tête pointe maintenant vers le nouveau nœud
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
std::string ListeChainee::rechercherImageParNumero(int numero) const {
    auto current = head; // Commence à la tête de la liste
    while (current) { // Parcourt la liste tant qu'il y a des nœuds
        if (current->data.getNumero() == numero) {
            return current->data.getDescripteur(); // Retourne le descripteur si trouvé
        }
        current = current->next; // Passe au nœud suivant
    }
    return "Image non trouvée."; // Retourne un message si aucune image ne correspond
}

// Destructeur de la classe ListeChainee
ListeChainee::~ListeChainee() {
    while (head) { // Tant qu'il y a des nœuds
        auto temp = head; // Sauvegarde le nœud courant
        head = head->next; // Passe au nœud suivant
    }

}
