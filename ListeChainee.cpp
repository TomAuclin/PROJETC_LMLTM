#include "ListeChainee.hpp"

// Constructeur de la liste chaînée
ListeChainee::ListeChainee() : head(nullptr) {}

// Ajoute une image à la liste
void ListeChainee::ajouterImage(const Image& img) {
    auto nouveau = std::make_shared<Node>(img);
    nouveau->next = head;
    head = nouveau;
}

// Recherche une image par numéro et retourne son descripteur
std::string ListeChainee::rechercherImageParNumero(int numero) const {
    auto current = head;
    while (current) {
        if (current->data.getNumero() == numero) {
            return current->data.getDescripteur();
        }
        current = current->next;
    }
    return "Image non trouvée.";
}
