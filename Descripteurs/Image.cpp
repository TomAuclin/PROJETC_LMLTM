
#include "Image.hpp"

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






Image::~Image() {
   
}
