#include "Image.hpp"
#include <iostream>
#include <fstream>


Image::Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ, int nTp, int ide)
    : source(src), titre(tit), numero(num), prix(prx), acces(acc), type(typ), nbTraitementPossible(nTp), identite(ide){}


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
/*
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

        // Extraction des champs depuis la ligne
        std::getline(ss, champ, ',');
        std::string fichierSource = champ;

        std::getline(ss, champ, ',');
        std::string fichierTitre = champ;

        std::getline(ss, champ, ',');
        int fichierNumero = std::stoi(champ);

        std::getline(ss, champ, ',');
        double fichierPrix = std::stod(champ);

        std::getline(ss, champ, ',');
        char fichierAcces = champ[0];

        std::getline(ss, champ, ',');
        std::string fichierType = champ;

        std::getline(ss, champ, ',');
        int fichierNbTraitement = std::stoi(champ);

        std::getline(ss, champ, ',');
        int fichierIdentite = std::stoi(champ);

        // Vérification si le titre correspond
        if (fichierTitre == this->titre) {
            this->source = fichierSource;
            this->titre = fichierTitre;
            this->numero = fichierNumero;
            this->prix = fichierPrix;
            this->acces = fichierAcces;
            this->type = fichierType;
            this->nbTraitementPossible = fichierNbTraitement;
            this->identite = fichierIdentite;
            break;
        }
    }

    fichier.close();
}*/

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

        // Extraction des champs depuis la ligne
        std::getline(ss, champ, ',');
        std::string fichierSource = champ;

        std::getline(ss, champ, ',');
        std::string fichierTitre = champ;

        std::getline(ss, champ, ',');
        int fichierNumero = std::stoi(champ);

        std::getline(ss, champ, ',');
        double fichierPrix = std::stod(champ);

        std::getline(ss, champ, ',');
        char fichierAcces = champ[0];

        std::getline(ss, champ, ',');
        std::string fichierType = champ;

        std::getline(ss, champ, ',');
        int fichierNbTraitement = std::stoi(champ);



        // Vérification si le titre correspond
        if (fichierTitre == this->getTitre()) {  // Comparer avec le titre
            // Utilisation des setters pour affecter les valeurs
            this->setSource(fichierSource);
            this->setTitre(fichierTitre);
            this->setNumero(fichierNumero);
            this->setPrix(fichierPrix);
            this->setAccess(fichierAcces);
            this->setType(fichierType);
            this->setnbTraitementPossible(fichierNbTraitement);

        std::cout << "Source : " << this->source << std::endl;
        std::cout << "Titre : " << this->titre << std::endl;
        std::cout << "Numéro : " << this->numero << std::endl;
        std::cout << "Prix : " << this->prix << std::endl;
        std::cout << "Accès : " << this->acces << std::endl;
        std::cout << "Type : " << this->type << std::endl;
        std::cout << "Nb Traitements : " << this->nbTraitementPossible << std::endl;
    }

    fichier.close();
 }


Image::~Image() {}
