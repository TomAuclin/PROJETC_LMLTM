// Inclusion des fichiers d'en-tête
#include "Image.hpp"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <iostream>  // Pour l'affichage sur la console
#include <fstream>   // Pour la gestion des fichiers
#include <algorithm> // Pour des fonctions d'algorithmes
#include <cctype>    // Pour les fonctions de manipulation de caractères
#include <string>    // Pour la gestion des chaînes de caractères
#include <sstream>   // Pour la manipulation de flux de chaînes
#include <QDebug>    // Pour l'affichage de débogage dans Qt


// Constructeur initialisant les attributs de la classe Image
Image::Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide)
    : source(src), titre(tit), numero(num), prix(prx), acces(acc), type(typ), nbTraitementPossible(nTp), identite(ide) {}

// Getter pour récupérer le chemin de l'image
std::string Image::getChemin() const {
    return source;
}

// Setter pour définir un nouveau chemin de l'image
void Image::setChemin(const std::string& chemin) {
    source = chemin;
}

// Retourne un descripteur détaillé de l'image
std::string Image::getDescripteur() const {
    std::ostringstream descripteur;
    descripteur << "Source: " << source << "\n"
                << "Titre: " << titre << "\n"
                << "Numero: " << numero << "\n"
                << "Prix: " << prix << "\n"
                << "Acces: " << acces << "\n"
                << "Type: " << type << "\n"
                << "Nombre de traitement possible : " << nbTraitementPossible << "\n";
    return descripteur.str();
}

// Compte le nombre de lignes dans le descripteur d'une image
int Image::nbrcarac() {
    int cpt = 0;
    Image image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur", 5, 7);
    std::string nbrc = image.getDescripteur();

    for (int i = 0; i < nbrc.size(); i++) {
        if (nbrc[i] == '\n') {
            cpt++;
        }
    }
    return cpt;
}

// Retourne un descripteur simple de l'image
std::string Image::getDescripteurSimple() const {
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre << ", "
                << numero << ", "
                << prix << ", "
                << acces << ", "
                << type << ", "
                << nbTraitementPossible << ", "
                << identite;
    return descripteur.str();
}

// Retourne un descripteur limité pour les images avec moins de 10 traitements possibles
std::string Image::getDescripteursmoins10() const {
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre << ", "
                << type;
    return descripteur.str();
}

// Retourne un descripteur pour les images avec plus de 10 traitements possibles
std::string Image::getDescripteursplus10() const {
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre << ", "
                << type << " , "
                << nbTraitementPossible;
    return descripteur.str();
}

// Retourne un descripteur limité aux images gratuites
std::string Image::getDescripteurGratuit() const {
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre;
    return descripteur.str();
}

// Getters pour accéder aux autres attributs
int Image::getNumero() const { return numero; }
int Image::getPrix() const { return prix; }
std::string Image::getSource() const { return source; }
std::string Image::getTitre() const { return titre; }
char Image::getAccess() const { return acces; }
std::string Image::getType() const { return type; }
int Image::getnbTraitementPossible() const { return nbTraitementPossible; }

// Setters pour modifier les attributs de l'image
void Image::setNumero(int num) { numero = num; }
void Image::setPrix(int _prix) { prix = _prix; }
void Image::setTitre(std::string _Titre) { titre = _Titre; }
void Image::setAccess(char _Access) { acces = _Access; }
void Image::setType(std::string _Type) { type = _Type; }
void Image::setnbTraitementPossible(int _nbTraitementPossible) { nbTraitementPossible = _nbTraitementPossible; }
void Image::setSource(const std::string& src) { source = src; }

// Fonction pour supprimer les espaces en début et fin de chaîne
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return ""; // Si la chaîne est vide
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Fonction pour convertir une chaîne de caractères en minuscules
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Associe un descripteur à l'image en lisant un fichier
void Image::associerDescripteur(const std::string& fichierDescripteurs) {
    std::ifstream fichier(fichierDescripteurs);
    if (!fichier.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << fichierDescripteurs << std::endl;
        return;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string champ;

        // Extraction des champs
        std::getline(ss, champ, ','); std::string fichierSource = champ;
        std::getline(ss, champ, ','); std::string fichierTitre = champ;
        std::getline(ss, champ, ','); int fichierNumero = std::stoi(champ);
        std::getline(ss, champ, ','); double fichierPrix = std::stod(champ);
        std::getline(ss, champ, ','); std::string fichierAcces = trim(champ);
        std::getline(ss, champ, ','); std::string fichierType = champ;
        std::getline(ss, champ, ','); int fichierNbTraitement = std::stoi(champ);

        // Vérification des titres en supprimant les espaces et en ignorant la casse
        if (toLowerCase(trim(fichierTitre)) == toLowerCase(trim(this->getTitre()))) {
            this->setSource(fichierSource);
            this->setTitre(fichierTitre);
            this->setNumero(fichierNumero);
            this->setPrix(fichierPrix);
            this->setAccess(fichierAcces[0]);
            this->setType(fichierType);
            this->setnbTraitementPossible(fichierNbTraitement);
        }
    }

    fichier.close();
}

// Recherche le prix d'une image par numéro dans un fichier
bool Image::rechercherPrix(int numeroImage, const std::string& fichierDescripteurs) {
    std::ifstream fichier(fichierDescripteurs);
    if (!fichier.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << fichierDescripteurs << std::endl;
        return false;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string champ;

        // Extraction des champs
        std::getline(ss, champ, ','); // Source
        std::getline(ss, champ, ','); // Titre
        std::getline(ss, champ, ','); int fichierNumero = std::stoi(champ);
        std::getline(ss, champ, ','); double fichierPrix = std::stod(champ);

        // Comparaison
        if (fichierNumero == numeroImage) {
            this->setNumero(fichierNumero);
            this->setPrix(fichierPrix);
            fichier.close();
            return true;
        }
    }

    fichier.close();
    return false;
}

// Destructeur
Image::~Image() {}
