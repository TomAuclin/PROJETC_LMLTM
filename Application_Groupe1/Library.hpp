#ifndef LIBRARY_HPP
#define LIBRARY_HPP

// Inclusion des fichiers d'en-tête
#include "Image.hpp"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QString>
#include <string>
#include <filesystem>
#include <memory>


/**
 * @class ListeChainee
 * @brief Représente une liste chaînée contenant des objets de type Image.
 *
 * Cette classe permet d'ajouter des images, de rechercher une image
 * par son numéro, et de gérer la mémoire dynamiquement à l'aide de 
 * pointeurs intelligents.
 */
class Library {
public:

    /**
     * struct INode
     * Un nœud de la liste chaînée, chaque nœud représente une image et un pointeur vers le nœud suivant.
     */
    struct INode {
        Image data; /**< L'image stockée dans le nœud. */
        std::shared_ptr<INode> next; /**< Pointeur vers le nœud suivant. */

        /**
         * Constructeur du nœud.
         * img L'image à stocker dans le nœud.
         */
        INode(const Image& img) : data(img), next(nullptr) {}
    };

    std::shared_ptr<INode> head; /**< Pointeur partagé vers le premier nœud de la liste chaînée. */

    Library(); /**< Constructeur de la bibliothèque, initialisation de la liste vide. */

    /**
     * Ajoute une image à la liste chaînée.
     * img L'image à ajouter au début de la liste.
     */
    void ajouterDescripteurs(const Image& img); /**< Ajoute un descripteur d'image. Utilisé par les utilisateurs de niveau 1. */
    void supprimerDescripteurs(int numero); /**< Supprime un descripteur d'image par numéro. Utilisé par les utilisateurs de niveau 1. */
    void modifdescripteurs(int numero, Library bibli); /**< Modifie un descripteur d'image. Utilisé par les utilisateurs de niveau 1. */

    QString determineBibliotheque(double prix) const; /**< Détermine la bibliothèque selon un prix donné. */

    std::string getDossierParPrix(double prix) const; /**< Retourne le dossier des images par prix. */
    bool sauvegarderImage(const std::string &cheminImage, double prix, const std::string &titre); /**< Sauvegarde une image avec son prix et titre dans la bibliothèque. */

    std::string getCheminProjet() const; /**< Retourne le chemin du projet. */

    void tricroissant(Library liste); /**< Trie la bibliothèque par prix croissant. */
    void trinbtraitement(Library liste); /**< Trie la bibliothèque par nombre de traitements. */
    void afficher() const; /**< Affiche tous les descripteurs des images. */
    void afficherGratuites() const; /**< Affiche les images gratuites. */
    void modifierAcces(int numero, const std::string &nomFichier); /**< Modifie l'accès d'une image pour les utilisateurs de niveau 2. */
    void modifierAccesDansFichier(const std::string &nomFichier, int numero); /**< Modifie l'accès d'une image dans un fichier spécifique. */
    int tailleListe(); /**< Retourne la taille de la liste chaînée. */
    void sousListePrix(Library liste); /**< Crée une sous-liste d'images selon leur prix. */
    void sousListePrix(int numero); /**< Crée une sous-liste d'images à partir d'un numéro donné. */
    void sousListetype(Library Liste); /**< Crée une sous-liste d'images selon leur type. */
    void sousListetype(int numero); /**< Crée une sous-liste d'images à partir d'un numéro de type. */
    void afficherImagesAvecAccesO(const std::string& nomFichier) const; /**< Affiche les images ayant un accès ouvert. */
    void sauvegarderDansFichier(const std::string& nomFichier) const; /**< Sauvegarde la bibliothèque dans un fichier. */
    void chargerDepuisFichier(const std::string& nomFichier); /**< Charge la bibliothèque depuis un fichier. */

    //////////////////////
    void save(); /**< Sauvegarde la bibliothèque actuelle. */
    void permuterImages(int numero1, int numero2); /**< Permute deux images de la bibliothèque (niveau 1). */
    void copyFile(const std::string& sourcePath, const std::string& destinationFolder); /**< Copie un fichier d'une source vers un dossier de destination. */
    //////////////////////

    void fusion(Library liste2); /**< Fusionne deux bibliothèques. */
    int titrecheck(std::string _titre) const; /**< Vérifie si un titre est valide. */

    /**
     * Recherche une image dans la liste par son numéro.
     * numero Le numéro de l'image à rechercher.
     * retourne Une chaîne contenant les informations de l'image si elle est trouvée,
     * ou un message indiquant que l'image n'existe pas.
     */
    std::string rechercherImageParNumero(int numero) const; /**< Recherche une image dans la bibliothèque par son numéro d'identification. */
    int numerocheck(int num) const; /**< Vérifie si un numéro est valide. */

    /**
     * Destructeur de la classe Library.
     * Libère la mémoire allouée pour la liste chaînée.
     */
    ~Library(); /**< Destructeur qui libère la mémoire de la bibliothèque. */
};

#endif
