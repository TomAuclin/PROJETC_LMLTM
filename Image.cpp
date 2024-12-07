#include "Image.hpp"

// Constructeur de la classe Image
Image::Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ)
    : source(src), titre(tit), numero(num), prix(prx), acces(acc), type(typ) {}

// Retourne les informations d'une image sous forme de chaîne de caractères
std::string Image::getDescripteur() const {
    std::ostringstream descripteur;
    descripteur << "Source: " << source << "\n"
                << "Titre: " << titre << "\n"
                << "Numero: " << numero << "\n"
                << "Prix: " << prix << "\n"
                << "Acces: " << acces << "\n"
                << "Type: " << type << "\n";
    return descripteur.str();
}

// Retourne le numéro de l'image
int Image::getNumero() const {
    return numero;
}
