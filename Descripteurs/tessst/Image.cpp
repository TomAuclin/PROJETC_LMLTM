#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <QDebug>


Image::Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide)
    : source(src), titre(tit), numero(num), prix(prx), acces(acc), type(typ), nbTraitementPossible(nTp), identite(ide){}


std::string Image::getChemin() const {
    return source;
}

void Image::setChemin(const std::string& chemin) {
    source = chemin;
}


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

int Image::nbrcarac()
{
    int cpt = 0;
    Image image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur", 5, 7);
    std::string nbrc;
    nbrc = image.getDescripteur();
    for (int i = 0; i < nbrc.size();i++){
        if (nbrc[i] == '\n'){
            cpt++;
        }

    }
    return cpt;

}

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

std::string Image::getDescripteursmoins10() const
{
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre << ", "
                << type;
               
    return descripteur.str();
}

std::string Image::getDescripteursplus10() const
{
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre << ", "
                << type << " , "
                << nbTraitementPossible;
               
    return descripteur.str();
}

std::string Image::getDescripteurGratuit() const
{
    std::ostringstream descripteur;
    descripteur << source << ", "
                << titre ;
               
    return descripteur.str();
}

int Image::getNumero() const {
    return numero;
}

int Image::getPrix() const {
    return prix;
}

std::string Image::getSource() const
{
    return source;
}

std::string Image::getTitre() const
{
    return titre;
}

char Image::getAccess() const
{
    return acces;
}

std::string Image::getType() const
{
    return type;
}

int Image::getnbTraitementPossible() const
{
    return nbTraitementPossible;
}

void Image::setNumero(int num)
{
    numero = num;
    
}

void Image::setPrix(int _prix)
{
    prix = _prix;
}

void Image::setTitre(std::string _Titre)
{
    titre = _Titre;
}

void Image::setAccess(char _Access)
{
    acces = _Access;
}

void Image::setType(std::string _Type)
{
    type = _Type;
}

void Image::setnbTraitementPossible(int _nbTraitementPossible)
{
    nbTraitementPossible = _nbTraitementPossible;
}

void Image::setSource(const std::string& src) {
    source = src;
}


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

        std::getline(ss, champ, ',');
        std::string fichierSource = champ;

        std::getline(ss, champ, ',');
        std::string fichierTitre = champ;

        std::getline(ss, champ, ',');
        int fichierNumero = std::stoi(champ);

        std::getline(ss, champ, ',');
        double fichierPrix = std::stod(champ);

        std::getline(ss, champ, ',');
        std::string fichierAcces = champ;

        // Vérification de la valeur d'accès
        fichierAcces = trim(fichierAcces); // Applique trim ici pour enlever les espaces
        if (fichierAcces.empty() || (fichierAcces != "O" && fichierAcces != "L")) {
            std::cerr << "Avertissement : valeur d'accès invalide '" << fichierAcces << "' dans le fichier descripteur." << std::endl;
            continue;  // Passe à la ligne suivante si l'accès est incorrect
        }

        std::getline(ss, champ, ',');
        std::string fichierType = champ;

        std::getline(ss, champ, ',');
        int fichierNbTraitement = std::stoi(champ);

        std::cout << "Titre fichier : " << fichierTitre << std::endl;
        std::cout << "Titre image : " << this->getTitre() << std::endl;

        // Comparaison en supprimant les espaces et en ignorant la casse
        std::string trimmedFichierTitre = toLowerCase(trim(fichierTitre));
        std::string trimmedImageTitre = toLowerCase(trim(this->getTitre()));

        std::cout << "Titre fichier (modifié) : " << trimmedFichierTitre << std::endl;
        std::cout << "Titre image (modifié) : " << trimmedImageTitre << std::endl;

        if (trimmedFichierTitre == trimmedImageTitre) {
            this->setSource(fichierSource);
            this->setTitre(fichierTitre);
            this->setNumero(fichierNumero);
            this->setPrix(fichierPrix);
            this->setAccess(fichierAcces[0]);
            this->setType(fichierType);
            this->setnbTraitementPossible(fichierNbTraitement);

            // Affichage des attributs mis à jour
            std::cout << "Descripteur mis à jour :\n";
            std::cout << "Source : " << this->getSource() << std::endl;
            std::cout << "Titre : " << this->getTitre() << std::endl;
            std::cout << "Numéro : " << this->getNumero() << std::endl;
            std::cout << "Prix : " << this->getPrix() << std::endl;
            std::cout << "Accès (caractère) : " << this->getAccess() << std::endl;
            std::cout << "Type : " << this->getType() << std::endl;
            std::cout << "Nb Traitements : " << this->getnbTraitementPossible() << std::endl;
        }
    }

    fichier.close();
}


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

        // Lecture des champs dans l'ordre attendu
        std::getline(ss, champ, ','); // Source
        std::getline(ss, champ, ','); // Titre
        std::getline(ss, champ, ',');
        int fichierNumero = std::stoi(champ); // Numéro

        std::getline(ss, champ, ',');
        double fichierPrix = std::stod(champ); // Prix

        // Comparaison avec le numéro recherché
        if (fichierNumero == numeroImage) {
            // Mise à jour des attributs pertinents
            this->setNumero(fichierNumero);
            this->setPrix(fichierPrix);

            // Facultatif : afficher les détails trouvés
            std::cout << "Numéro trouvé : " << fichierNumero << ", Prix : " << fichierPrix << std::endl;

            fichier.close();
            return true;
        }
    }

    fichier.close();
    std::cerr << "Aucune image trouvée avec le numéro : " << numeroImage << std::endl;
    return false;
}


Image::~Image() {}
