#include "Image.hpp"


Image::Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ)
    : source(src), titre(tit), numero(num), prix(prx), acces(acc), type(typ) {}


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


int Image::getNumero() const {
    return numero;
}

Image::~Image() {
   
}

