#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <string>
#include <sstream>
#include <vector>


/*
 * Classe Image
 *
 * Représente une image avec son Descripteur : ses caractéristiques.
 */
class Image {
public:
    std::string source; ///< Source de l'image ou son Auteur
    std::string titre; ///< Titre de l'image
    int numero; ///< Numéro d'identification de l'image
    double prix; ///< Prix de l'image
    char acces; ///< Statut d'accès de l'image ('O' pour ouvert, 'L' pour limité)
    std::string type; ///< Type ou catégorie de l'image (couleur ou niveaux de gris)
    int nbTraitementPossible; ///< Nombre de traitements possibles sur l'image
    int identite; ///< Identifiant unique de l'image

public:
    // Constructeur complet
    Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide);

    // Constructeur par défaut
    Image()
        : source(""), titre(""), numero(0), prix(0.0), acces('O'), type("inconnu"), nbTraitementPossible(0), identite(0) {}

    // Méthodes existantes
    std::string getDescripteur() const;
    int nbrcarac();
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
    void setPrix(int _prix);
    void setTitre(std::string _Titre);
    void setAccess(char _Access);
    void setType(std::string _Type);
    void setnbTraitementPossible(int _nbTraitementPossible);
    void associerDescripteur(const std::string& fichierDescripteurs);
    void setSource(const std::string& src);
    bool rechercherPrix(int numero, const std::string& cheminDescripteurs);

    // Destructeur
    ~Image();
};

#endif
