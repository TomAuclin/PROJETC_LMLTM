#include "Library.hpp"


#include "GestionUtilisateur.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
/**
 *
 * Initialise une liste chaînée vide en définissant la tête de la liste à nullptr.
 */
Library::Library() : head(nullptr) {}

/**
 * Ajoute   img  une image à la liste chaînée.
 * L'image est insérée au début de la liste en créant un nouveau nœud qui pointe
 * vers l'ancien premier nœud.
 */
void Library::ajouterDescripteurs(const Image& img) {
    auto nouveau = std::make_shared<Node>(img); // Crée un nouveau nœud
    nouveau->next = head; // L'ancien premier nœud devient le suivant
    head = nouveau; // La tête pointe maintenant vers le nouveau nœud
}
void Library::supprimerDescripteurs(int numero)
{
    // Si la liste est vide, il n'y a rien à supprimer
    // A voir
    //int cpt = 1;
    auto current = head;
    auto previous = head;
    while (current) {
        //current->data.setNumero(cpt);
        if (current->data.getNumero() == numero) {
            if (current == head) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }
            return;
        }
        //cpt++;
        previous = current;
        current = current->next;
    }
}


void  Library::tricroissant(Library liste)

{
    auto current = head;
    
    for (int i = 0; i <liste.tailleListe()-1; i ++ )
    {current = head;
    //std::cout << current->data.getDescripteur() << std::endl;
        for (int j = 0; j <liste.tailleListe()-1; j ++ )   
        {          
            if (current->data.getPrix() > current->next->data.getPrix())
            {
                //std::cout << "Prix 1 : " << current->data.getPrix() << std::endl;
                //std::cout << "Prix 2 : " << current->next->data.getPrix() << std::endl;
                auto temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                
            }
            current = current->next;        
        }      
    }   
}

void Library::trinbtraitement(Library liste)
{
    auto current = head;
    
    for (int i = 0; i <liste.tailleListe()-1; i ++ )
    {current = head;
        for (int j = 0; j <liste.tailleListe()-1; j ++ )   
        {          
            if (current->data.getnbTraitementPossible() > current->next->data.getnbTraitementPossible())
            {
                auto temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;   
            }
            current = current->next;        
        }      
    }
}

void Library::afficher() const {
    auto current = head;
    std::cout << "Liste des images : " << std::endl;
    //std::cout << current->data.getDescripteur() << std::endl;
    while (current ) {
        std::cout << current->data.getDescripteur() << std::endl;
        current = current->next;
    }

}

// fonction qui affiche seuleument les descripteur des images gratuites/ouverte pour le simple user
void Library::afficherGratuites() const {
    auto current = head;
    std::cout << "Liste des images gratuites/autorise : " << std::endl;
    while (current) {
        if (current->data.getAccess() == 'O') {
            std::cout << current->data.getDescripteur() << std::endl;
        }
        current = current->next;
    }
}

// permet de modifier l'acces de L a O et inversement d'une image choisi grace à son numéro
//void Library::modifierAcces(int numero, const std::string& nomFichier) {
//    auto current = head;
//    while (current) {
//        if (current->data.getNumero() == numero) {
//            if (current->data.getAcces() == 'O') {
//                current->data.setAcces('L');
//            } else {
//                current->data.setAcces('O');
//            }
//            sauvegarderDansFichier(nomFichier); // Sauvegarde après modification
//            std::cout << "Modification sauvegardée dans " << nomFichier << std::endl;
//            return;
//        }
//        current = current->next;
//    }
//    std::cerr << "Image non trouvée." << std::endl;
//}

void Library::modifierAcces(int numero) {
    auto current = head; // Commence à la tête de la liste
    while (current) { // Parcourt la liste chaînée
        if (current->data.getNumero() == numero) { // Vérifie si le numéro correspond
            // Bascule entre 'O' et 'L' pour l'accès
            if (current->data.getAccess() == 'O') {
                current->data.setAccess('L');
            } else {
                current->data.setAccess('O');
            }
            std::cout << "L'accès de l'image numéro " << numero << " a été modifié." << std::endl;
            return; // Arrête la recherche après la modification
        }
        current = current->next; // Passe au nœud suivant
    }
    std::cerr << "Image avec le numéro " << numero << " non trouvée dans la liste." << std::endl;
}



void Library::afficherImagesAvecAccesO(const std::string &nomFichier) const
{
    std::ifstream fichier(nomFichier);

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::istringstream iss(ligne);
        std::string source, titre, type;
        int numero, nbTraitementPossible, identite;
        double prix;
        char acces;

        if (std::getline(iss, source, ',') &&
            std::getline(iss, titre, ',') &&
            iss >> numero &&
            iss.ignore(1) &&
            iss >> prix &&
            iss.ignore(1) &&
            iss >> acces &&
            iss.ignore(1) &&
            std::getline(iss, type, ',') &&
            iss >> nbTraitementPossible &&
            iss.ignore(1) &&
            iss >> identite) {

            if (acces == 'O') {
                std::cout << ligne << std::endl;
            }
        } else {
            std::cerr << "Erreur lors du traitement de la ligne : " << ligne << std::endl;
        }
    }

    fichier.close();
}

int Library::tailleListe()
{
    int taille = 0;
    auto current = head;
    while (current) {
        taille++;
        current = current->next;
    }
    //std::cout << "La taille de la liste est de : " << taille << std::endl;
    return taille;
}

void Library::sousListePrix(Library liste)
{
    auto current = head;
    while (current) {
        if (current->data.getPrix() < 10 && current->data.getPrix() != 0)
        {
            std::cout << current->data.getDescripteursmoins10() << std::endl;
        }
        if (current->data.getPrix() > 10)
        {
            std::cout << current->data.getDescripteursplus10() << std::endl;
        }
        if (current->data.getPrix() == 0)
        {
            std::cout << current->data.getDescripteurGratuit() << std::endl;
        }
        current = current->next;
    }
}

void Library::sousListePrix(int numero)
{
    auto current = head;
    while (current) {
        if (current->data.getNumero() == numero)
        {
            if (current->data.getPrix() < 10 && current->data.getPrix() != 0)
            {
                std::cout << current->data.getDescripteursmoins10() << std::endl;
            }
            if (current->data.getPrix() > 10)
            {
                std::cout << current->data.getDescripteursplus10() << std::endl;
            }
            if (current->data.getPrix() == 0)
            {
                std::cout << current->data.getDescripteurGratuit() << std::endl;
            }
        }
        current = current->next;
    }
}

void Library::sousListetype(Library Liste)
{

    auto current = head;
    while (current) {
    
            if (current->data.getType() == "gris")
            {
                std::cout << current->data.getDescripteursmoins10() << std::endl;
            }
            else
            {
                std::cout << current->data.getDescripteursplus10() << std::endl;
            }
        
        current = current->next;
    }
}



void Library::sousListetype(int numero)
{
    auto current = head;
    while (current) {
        if (current->data.getNumero() == numero)
        {
               if (current->data.getType() == "gris")
            {
                std::cout << current->data.getDescripteursmoins10() << std::endl;
            }
            else
            {
                std::cout << current->data.getDescripteursplus10() << std::endl;
            }
        
        current = current->next;
        }
}
}

// !!!!!!!!!!!!!!!!!!!! Sousliste couleur et gris 

/**
 * Recherche une image dans la liste en fonction de son numéro.
 * 
 *retourne  Une chaîne contenant le descripteur de l'image si elle est trouvée,
 * ou un message indiquant que l'image n'existe pas.
 *
 * La méthode parcourt la liste chaînée à partir de la tête et compare
 * les numéros des images jusqu'à trouver une correspondance ou atteindre
 * la fin de la liste.
 */
std::string Library::rechercherImageParNumero(int numero) const {
    auto current = head; // Commence à la tête de la liste
    while (current) { // Parcourt la liste tant qu'il y a des nœuds
        if (current->data.getNumero() == numero) {
            return current->data.getDescripteur(); // Retourne le descripteur si trouvé
        }
        current = current->next; // Passe au nœud suivant
    }
    return "Image non trouvée."; // Retourne un message si aucune image ne correspond
}

void Library::modifdescripteurs(int numero, Library bibli)
{
    
    auto current = head;
    auto numcheck = head;
    int checkG = 3;
    int nbimages = 0;
    while (current) { // Parcourt la liste tant qu'il y a des nœuds
        if (current->data.getNumero() == numero) {
            
            for (int i = 0; i < current->data.nbrcarac(); i++)
            {
                if (i == 0)
                {   
                    std::cout << "Si vous voulez changer le Titre de l'image tapez Y sinon tapez N" << std::endl;
                    //current.titrecheck(current->data.getTitre());
                    
                    std::string titre;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")
                    {
                        std::cout << "Entrez le titre de l'image : ";
                        std::cin >> titre;
                        checkG = bibli.titrecheck(titre);
                        while (checkG != 0) {
                            std::cout << "Le titre existe deja dans la biblio, veuillez en rentrer un nouveau :" << std::endl;
                            std::cin >> titre;
                            checkG = bibli.titrecheck(titre);
                        }
                        current->data.setTitre(titre);                     
                    }
                }
                if (i == 1)
                {  
                    std::cout << "Si vous voulez changer le numero de l'image tapez Y sinon tapez N" << std::endl;                    
                    int num;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")   
                    
                    {
                        std::cout << "Entrez le numero de l'image : ";
                        std::cin >> num;
                        checkG = bibli.numerocheck(num);
                        while (checkG != 0) {
                            std::cout << "Le numero existe deja dans la biblio, veuillez en rentrer un nouveau :" << std::endl;
                            std::cout << " nous recommandons de choisir un numero superieur a " << bibli.tailleListe()<< std::endl;
                            std::cin >> num;
                            checkG = bibli.numerocheck(num);
                        }

                        current->data.setNumero(num);
                    }
                }
                if (i == 2)
                {   
                    std::cout << "Si vous voulez changer le prix de l'image tapez Y sinon tapez N" << std::endl;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")   
                    {
                        std::cout << "Entrez le prix de l'image : ";
                        int prix;
                        std::cin >> prix;
                        current->data.setPrix(prix);
                    }
                }
                if (i == 3)
                {   
                    std::cout << "Si vous voulez changer l'acces de l'image tapez Y sinon tapez N" << std::endl;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")   
                    {
                        std::cout << "Entrez l'acces de l'image : ";
                        char acces;
                        std::cin >> acces;
                        current->data.setAccess(acces);
                    }
                }
                if (i == 4)
                {
                    std::cout << "Si vous voulez changer le type de l'image tapez Y sinon tapez N" << std::endl;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")   
                    {
                        std::cout << "Entrez le type de l'image : ";
                        std::string type;
                        std::cin >> type;
                        current->data.setType(type);
                    }
                }
                if (i == 5)
                {
                    std::cout << "Si vous voulez changer le nombre de traitement possible de l'image tapez Y sinon tapez N" << std::endl;
                    std::string check;
                    std::cin >> check;
                    if (check == "Y")   
                    {
                        std::cout << "Entrez le nombre de traitement possible de l'image : ";
                        int nbTraitementPossible;
                        std::cin >> nbTraitementPossible;
                        current->data.setnbTraitementPossible(nbTraitementPossible);
                    }
                }

            }
                
        }
        current = current->next; // Passe au nœud suivant
    }
    

}

void Library::sauvegarderDansFichier(const std::string& nomFichier) const {
    std::ofstream fichier(nomFichier);

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return;
    }

    auto current = head;
    if (!current) {
        std::cerr << "La liste est vide, rien à sauvegarder." << std::endl;
        return;
    }

    while (current) {
        std::string descripteur = current->data.getDescripteurSimple();
        if (!descripteur.empty()) {
            fichier << descripteur << "\n"; // Enregistrement ligne par ligne
        }
        current = current->next;
    }

    fichier.close();
    std::cout << "Les descripteurs ont été sauvegardés dans " << nomFichier << std::endl;
}


void Library::chargerDepuisFichier(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::istringstream iss(ligne);
        std::string source, titre, type;
        int numero, nbTraitementPossible, identite;
        double prix;
        char acces;

        // Lire chaque champ séparé par une virgule
        if (std::getline(iss, source, ',') &&
            std::getline(iss, titre, ',') &&
            iss >> numero &&
            iss.ignore(1) && // Ignorer la virgule
            iss >> prix &&
            iss.ignore(1) &&
            iss >> acces &&
            iss.ignore(1) &&
            std::getline(iss, type, ',') &&
            iss >> nbTraitementPossible &&
            iss.ignore(1) &&
            iss >> identite) {

            // Ajouter le descripteur sans modification des espaces
            ajouterDescripteurs(Image(source, titre, numero, prix, acces, type, nbTraitementPossible, identite));
        } else {
            std::cerr << "Erreur lors du traitement de la ligne : " << ligne << std::endl;
        }
    }

    fichier.close();
    std::cout << "Les descripteurs ont été chargés depuis " << nomFichier << std::endl;
}

void Library::fusion(Library liste2)
{  
    auto current = liste2.head;
    while (current) {
        ajouterDescripteurs(current->data);
        current = current->next;
    }
}

int Library::titrecheck(std::string _titre) const
{
    int check = 0;
    auto _titrecheck = head;
    while (_titrecheck)

        {   
            if (_titrecheck->data.getTitre() == _titre){
                
                check = 1;
            }
            _titrecheck = _titrecheck->next;
        }
    return check;
}


int Library::numerocheck(int num) const {
    auto numcheck = head;
    int check = 0;
    while (numcheck){

        if (numcheck->data.getNumero()== num){
            check = 1;
            
        }
        numcheck = numcheck->next;
    }
    return check;
}

// Destructeur de la classe ListeChainee
Library::~Library() {
    while (head) { // Tant qu'il y a des nœuds
        auto temp = head; // Sauvegarde le nœud courant
        head = head->next; // Passe au nœud suivant
    }
}

