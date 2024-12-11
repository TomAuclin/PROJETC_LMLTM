#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <sstream>

/*
 * Classe Image
 * 
 * Représente une image avec son Descripteur : ses caractéristiques.
 * Cette classe stocke les informations relatives à une image, telles que son 
 * source, son titre, son numéro d'identification, son prix, son statut d'accès 
 * et son type
 */
class Image {
    private:
        std::string source;
        std::string titre;
        int numero;
        double prix;
        char acces;
        std::string type;
        int nbTraitementPossible;
        int identite;

    public:
        Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide);
        std::string getDescripteur() const;

        /**
         * Retourne le numéro d'identification de l'image.
         * 
         *  Le numéro de l'image.
         */
        int getNumero() const;

        /**
         * Destructeur de la classe Image.
         * 
         * Ce destructeur est responsable de la libération des ressources
         * allouées par  Image. 
         */
        ~Image();
};

#endif
