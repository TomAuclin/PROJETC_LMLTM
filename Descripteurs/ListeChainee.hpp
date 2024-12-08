#ifndef LISTECHAINEE_HPP
#define LISTECHAINEE_HPP

#include "Image.hpp"
#include <memory>

class ListeChainee {
private:
    struct Node {
        Image data;
        std::shared_ptr<Node> next;

        Node(const Image& img) : data(img), next(nullptr) {}
    };

    std::shared_ptr<Node> head;

public:
    ListeChainee();
    void ajouterImage(const Image& img);
    std::string rechercherImageParNumero(int numero) const;
    ~ListeChainee();
};

#endif
