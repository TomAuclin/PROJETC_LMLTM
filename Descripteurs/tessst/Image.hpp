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
        std::string source; ///< Source de l'image ou son Auteur
        std::string titre; ///< Titre de l'image
        int numero; ///< Numéro d'identification de l'image
        double prix; ///< Prix de l'image
        char acces; ///< Statut d'accès de l'image ('O' pour ouvert, 'L' pour limité)
        std::string type; ///< Type ou catégorie de l'image couleur où gris pour niveaux de gris.
        int nbTraitementPossible; ///< Nombre de traitement possible sur l'image.
        int identite; ///< Identifiant unique de l'image.

    public:
        /**
         * Constructeur de la classe Image.
         * 
         * Initialise un objet Image avec :
         * 
         *  src La source de l'image.
         *  tit Le titre de l'image.
         *  num Le numéro d'identification de l'image.
         *  prx Le prix de l'image.
         *  acc Le statut d'accès de l'image.
         *  typ Le type  de l'image.
         */
        Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide);

        /**
         * Retourne une chaîne de caractères décrivant l'image.
         * 
         */
        std::string getDescripteur() const;

        int nbrcarac();
        
        /**
         * Retourne le numéro d'identification de l'image.
         * 
         *  Le numéro de l'image.
         */
        int getNumero() const;
        int getPrix() const;
        std::string getSource() const;
        std::string getTitre() const;
        char getAccess() const;
        std::string getType() const;
        int getnbTraitementPossible() const;
        std::string getDescripteurSimple() const;
        std::string getDescripteurGratuit() const;
        std::string getDescripteursmoins10() const;
        std::string getDescripteursplus10() const;

        void setNumero(int _numero);
        void setPrix (int _prix);
        void setTitre(std::string _Titre);
        void setAccess(char _Access);
        void setType(std::string _Type);
        void setnbTraitementPossible(int _nbTraitementPossible);
        void associerDescripteur(const std::string& fichierDescripteurs);

        
        

      
        /**
         * Destructeur de la classe Image.
         * 
         * Ce destructeur est responsable de la libération des ressources
         * allouées par  Image. 
         */
        ~Image();
};

#endif


