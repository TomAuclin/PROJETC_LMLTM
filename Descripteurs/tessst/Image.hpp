#ifndef IMAGE_HPP
#define IMAGE_HPP

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
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
    std::string source; ///< Source de l'image ou son Auteur, peut être une référence à l'origine de l'image.
    std::string titre; ///< Titre de l'image, une description ou un nom donné à l'image.
    int numero; ///< Numéro d'identification unique pour chaque image, permet de différencier les images.
    double prix; ///< Prix de l'image, représente la valeur monétaire de l'image.
    char acces; ///< Statut d'accès de l'image, 'O' pour ouvert (accessible par tous) et 'L' pour limité (accessible sous conditions).
    std::string type; ///< Type ou catégorie de l'image, par exemple 'couleur' ou 'niveaux de gris'.
    int nbTraitementPossible; ///< Nombre de traitements possibles sur l'image, peut être utilisé pour savoir combien d'opérations l'image peut subir.
    int identite; ///< Identifiant unique de l'image, peut servir à l'identifier de manière unique dans une base de données ou un système.

public:
    // Constructeur complet qui initialise toutes les variables membres avec des valeurs spécifiques.
    Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide);

    // Constructeur par défaut, qui initialise les variables membres avec des valeurs par défaut (ex : une chaîne vide pour 'source' et 'titre').
    Image()
        : source(""), titre(""), numero(0), prix(0.0), acces('O'), type("inconnu"), nbTraitementPossible(0), identite(0) {}

    // Méthodes existantes
    std::string getDescripteur() const; ///< Retourne un descripteur complet de l'image.
    int nbrcarac(); ///< Retourne le nombre de caractères du descripteur de l'image.
    int getNumero() const; ///< Retourne le numéro d'identification de l'image.
    int getPrix() const; ///< Retourne le prix de l'image.
    std::string getSource() const; ///< Retourne la source (ou l'auteur) de l'image.
    std::string getTitre() const; ///< Retourne le titre de l'image.
    char getAccess() const; ///< Retourne le statut d'accès de l'image ('O' ou 'L').
    std::string getType() const; ///< Retourne le type de l'image ('couleur', 'niveaux de gris', etc.).
    int getnbTraitementPossible() const; ///< Retourne le nombre de traitements possibles sur l'image.
    std::string getDescripteurSimple() const; ///< Retourne un descripteur simplifié de l'image.
    std::string getDescripteurGratuit() const; ///< Retourne un descripteur pour les images gratuites.
    std::string getDescripteursmoins10() const; ///< Retourne un descripteur pour les images dont le prix est inférieur à 10.
    std::string getDescripteursplus10() const; ///< Retourne un descripteur pour les images dont le prix est supérieur à 10 mais inferieur a 100.
    std::string getDescripteursplus100() const; ///< Retourne un descripteur pour les images dont le prix est supérieur à 100.

    // Ajouter/modifier/supprimer descripteur
    std::string getChemin() const; ///< Retourne le chemin de l'image.
    void setChemin(const std::string& chemin); ///< Définit le chemin de l'image.

    void setNumero(int _numero); ///< Modifie le numéro d'identification de l'image.
    void setPrix(int _prix); ///< Modifie le prix de l'image.
    void setTitre(std::string _Titre); ///< Modifie le titre de l'image.
    void setAccess(char _Access); ///< Modifie le statut d'accès de l'image ('O' ou 'L').
    void setType(std::string _Type); ///< Modifie le type de l'image (ex: couleur ou niveaux de gris).
    void setnbTraitementPossible(int _nbTraitementPossible); ///< Modifie le nombre de traitements possibles sur l'image.
    void associerDescripteur(const std::string& fichierDescripteurs); ///< Associe un fichier descripteur à l'image.
    void setSource(const std::string& src); ///< Modifie la source (ou auteur) de l'image.
    bool rechercherPrix(int numero, const std::string& cheminDescripteurs); ///< Recherche le prix d'une image dans un fichier descripteur donné par le chemin.

    // Destructeur
    ~Image(); ///< Destructeur de la classe, utilisé pour nettoyer les ressources allouées pendant la durée de vie de l'objet.
};


#endif
