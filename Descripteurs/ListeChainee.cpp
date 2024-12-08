#include "ListeChainee.hpp"
      

ListeChainee::ListeChainee() : head(nullptr) {}


void ListeChainee::ajouterImage(const Image& img) {
    auto nouveau = std::make_shared<Node>(img);
    nouveau->next = head;
    head = nouveau;
}


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


ListeChainee::~ListeChainee() {
    while (head) {
        auto temp = head;
        head = head->next;
    }
}
