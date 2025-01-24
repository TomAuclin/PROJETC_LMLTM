// Inclusion des fichiers d'en-tête
#include "Library.hpp"
#include "GestionUtilisateur.hpp"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <filesystem>
#include <string>
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>

namespace fs = std::filesystem;

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

/* -------------------------------------------------------------------------
 *
 * ***************Pour menu descripteur ********************
 *
 * ----------------------------------------------------------------------------*/



std::string Library::getDossierParPrix(double prix) const {
    QString basePath = QDir::currentPath(); // Récupère le répertoire courant
    QString dossier;

    if (prix == 0) {
        dossier = basePath + "/Bibliotheque/Gratuites";
    } else if (prix > 0 && prix <= 9.99) {
        dossier = basePath + "/Bibliotheque/_9.99";
    } else if (prix > 9.99 && prix <= 100) {
        dossier = basePath + "/Bibliotheque/_99.99";
    } else if (prix > 100) {
        dossier = basePath + "/Bibliotheque/_100";
    }

    return dossier.toStdString();
}



std::string Library::getCheminProjet() const {
    // Récupère le chemin absolu du répertoire du projet
    QString chemin = QCoreApplication::applicationDirPath();
    std::filesystem::path path = chemin.toStdString();
    // Remonte de quelques dossiers si nécessaire pour atteindre le répertoire projet
    path = path.parent_path(); // Niveau 1
    path = path.parent_path(); // Niveau 2
    return path.string();
}



bool Library::sauvegarderImage(const std::string &cheminImage, double prix, const std::string &titre) {
    // Définir le chemin de base du dossier Bibliotheque
    std::string cheminDossier = getCheminProjet() + "/Bibliotheque";

    // Créer le répertoire cible si nécessaire
    try {
        if (!std::filesystem::exists(cheminDossier)) {
            std::filesystem::create_directories(cheminDossier);
        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur lors de la création du dossier : " << e.what() << std::endl;
        return false;
    }

    // Conserver l'extension du fichier original
    std::filesystem::path sourcePath(cheminImage);
    std::string cheminDestination = cheminDossier + "/" + titre + sourcePath.extension().string();

    // Copier l'image
    try {
        std::filesystem::copy_file(cheminImage, cheminDestination, std::filesystem::copy_options::overwrite_existing);

        // Message confirmant l'emplacement du fichier
        QMessageBox::information(nullptr, "Sauvegarde réussie",
                                 QString::fromStdString("L'image a été enregistrée dans : " + cheminDestination));
    } catch (const std::exception &e) {
        std::cerr << "Erreur lors de la copie du fichier : " << e.what() << std::endl;
        return false;
    }

    return true;
}



QString Library::determineBibliotheque(double prix) const {
    if (prix == 0) return "Gratuites";
    else if (prix < 10) return "_9.99";
    else if (prix < 100) return "_99.99";
    else return "_100";
}


void Library::ajouterDescripteurs(const Image& img) {
    auto nouveau = std::make_shared<INode>(img); // Crée un nouveau nœud
    nouveau->next = head; // L'ancien premier nœud devient le suivant
    head = nouveau; // La tête pointe maintenant vers le nouveau nœud
}


void Library::supprimerDescripteurs(int numero)
{
    // Si la liste est vide, il n'y a rien à supprimer
    
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
        
        previous = current;
        current = current->next;
    }
}

/*------------------------------------------------------------------------------------------------*/

// Fonction qui permet de trier une bibliotheque selon un critere de prix
// Entree : Objet de type library qui represente notre bibliotheque
// Sortie : /
void  Library::tricroissant(Library liste)

{
    // on initialise la tete de notre liste chainee
    auto current = head;
    
    // on parcourt les elements de notre liste
    for (int i = 0; i <liste.tailleListe()-1; i ++ )
    {current = head;

        for (int j = 0; j <liste.tailleListe()-1; j ++ )   
        {   // Si le prix d'un element est plus grand alors cet element change de place dans la
            // liste avec l'element de prix moins eleve
            if (current->data.getPrix() > current->next->data.getPrix())
            {
               
                auto temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                
            }
            current = current->next;        
        }      
    }   
}

// Fonction qui permet de trier une bibliotheque selon un critere de nombre de traitements
// Entree : Objet de type library qui represente notre bibliotheque
// Sortie : /
void Library::trinbtraitement(Library liste)
{
    // Le principe est le meme que pour tricroissant()
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
void Library::modifierAcces(int numero, const std::string& nomFichier) {
    auto current = head;
    while (current) {
        if (current->data.getNumero() == numero) {
            if (current->data.getAccess() == 'O') {
                current->data.setAccess('L');
            } else {
                current->data.setAccess('O');
            }
            sauvegarderDansFichier(nomFichier); // Sauvegarde après modification
            std::cout << "Modification sauvegardée dans " << nomFichier << std::endl;
            return;
        }
        current = current->next;
    }
    std::cerr << "Image non trouvée." << std::endl;
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

// Cette fonction retourne la taille de la liste
// Entre : /
// Sortie : entier taille
int Library::tailleListe()
{
    int taille = 0;
    auto current = head;
    while (current) {
        taille++;
        current = current->next;
    }
   
    return taille;
}

// Fonction permettant d'afficher une sous-liste de descripteurs
// de toutes les images de la bibliotheque selon un critere de prix
// Entree : Objet de type Library representant notre bibliotheque
// Sortie : /
void Library::sousListePrix(Library liste)
{
    // Pour chaque image de la bibliothque on affiche une sous-liste
    // selon son prix est entre 0 et 10, 10 et 100, superieur a 100 ou gratuite
    auto current = head;
    while (current) {
        if (current->data.getPrix() <= 9.99 && current->data.getPrix() != 0)
        {
            std::cout << current->data.getDescripteursmoins10() << std::endl;
        }
        if (current->data.getPrix() > 9.99 && current->data.getPrix() <=99.99 )
        {
            std::cout << current->data.getDescripteursplus10() << std::endl;
        }
        if (current->data.getPrix() == 0)
        {
            std::cout << current->data.getDescripteurGratuit() << std::endl;
        }
        if (current->data.getPrix() > 99.99)
        {
            std::cout << current->data.getDescripteursplus100() << std::endl;
        }
        current = current->next;
    }
}

// Fonction permettant d'afficher une sous-liste de descripteurs
// d'une image de la bibliotheque selon un critere de prix
// Entree : int qui represente le numero de l'image
// Sortie : /
void Library::sousListePrix(int numero)
{
    auto current = head;
    while (current) {
        if (current->data.getNumero() == numero)
        {
            if (current->data.getPrix() <= 9.99 && current->data.getPrix() != 0)
            {
                std::cout << current->data.getDescripteursmoins10() << std::endl;
            }
            if (current->data.getPrix() > 9.99 && current->data.getPrix() <=99.99 )
            {
                std::cout << current->data.getDescripteursplus10() << std::endl;
            }
            if (current->data.getPrix() == 0)
            {
                std::cout << current->data.getDescripteurGratuit() << std::endl;
            }
            if (current->data.getPrix() > 99.99)
            {
                std::cout << current->data.getDescripteursplus100() << std::endl;
            }
        }
        current = current->next;
    }
}



// Fonction permettant d'afficher une sous-liste de descripteurs
// de toutes les images de la bibliotheque selon un critere de type
// Entree : Objet de type Library representant notre bibliotheque
// Sortie : /
void Library::sousListetype(Library Liste)
{
    // Meme principe que la fonction sousListePrix mais le critere est le type de l'image
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


// Fonction permettant d'afficher une sous-liste de descripteurs
// d'une image de la bibliotheque selon un critere de type
// Entree : int qui represente le numero de l'image
// Sortie : /
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

// Fonction permettant de modifier l'ensemble des descripteurs d'une image
// Entree : int qui est le numero de l'image, objet de type Library qui est notre bibliotheque
// Sortie : /
void Library::modifdescripteurs(int numero, Library bibli)
{
    
    auto current = head;
    auto numcheck = head;
    int checkG = 3;
    int nbimages = 0;
    while (current) { // Parcourt la liste tant qu'il y a des nœuds
        if (current->data.getNumero() == numero) {
            // On parcourt l'ensemble de nos descripteurs pour demander a l'utilisateur si il veut les modifier
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
    // Ouvre un fichier en mode écriture
    std::ofstream fichier(nomFichier);

    // Vérifie si le fichier a été correctement ouvert
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return; // Sort de la fonction si l'ouverture échoue
    }

    // Pointeur vers le début de la liste chaînée
    auto current = head;
    if (!current) {
        // Si la liste est vide, afficher un message et quitter
        std::cerr << "La liste est vide, rien à sauvegarder." << std::endl;
        return;
    }

    // Parcourt chaque élément de la liste chaînée
    while (current) {
        // Récupère le descripteur sous forme de chaîne de caractères
        std::string descripteur = current->data.getDescripteurSimple();

        // Si le descripteur n'est pas vide, l'écrit dans le fichier
        if (!descripteur.empty()) {
            fichier << descripteur << "\n"; // Ajoute un descripteur par ligne
        }

        // Passe à l'élément suivant de la liste chaînée
        current = current->next;
    }

    // Ferme le fichier après avoir terminé l'écriture
    fichier.close();
    std::cout << "Les descripteurs ont été sauvegardés dans " << nomFichier << std::endl;
}


void Library::chargerDepuisFichier(const std::string& nomFichier) {
    // Ouvre un fichier en mode lecture
    std::ifstream fichier(nomFichier);

    // Vérifie si le fichier a été correctement ouvert
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomFichier << std::endl;
        return; // Sort de la fonction si l'ouverture échoue
    }

    std::string ligne;

    // Parcourt chaque ligne du fichier
    while (std::getline(fichier, ligne)) {
        std::istringstream iss(ligne); // Permet de découper la ligne en champs
        std::string source, titre, type;
        int numero, nbTraitementPossible, identite;
        double prix;
        char acces;

        // Découpe la ligne en différents champs séparés par des virgules
        if (std::getline(iss, source, ',') &&
            std::getline(iss, titre, ',') &&
            iss >> numero &&
            iss.ignore(1) && // Ignore la virgule suivante
            iss >> prix &&
            iss.ignore(1) &&
            iss >> acces &&
            iss.ignore(1) &&
            std::getline(iss, type, ',') &&
            iss >> nbTraitementPossible &&
            iss.ignore(1) &&
            iss >> identite) {

            // Crée un nouvel objet Image avec les données extraites
            // et l'ajoute à la liste des descripteurs
            ajouterDescripteurs(Image(source, titre, numero, prix, acces, type, nbTraitementPossible, identite));
        } else {
            // Si une erreur survient lors du traitement de la ligne, affiche un message
            std::cerr << "Erreur lors du traitement de la ligne : " << ligne << std::endl;
        }
    }

    // Ferme le fichier après avoir terminé la lecture
    fichier.close();
    std::cout << "Les descripteurs ont été chargés depuis " << nomFichier << std::endl;
}


/**
 * Permet de permuter deux images dans la bibliothèque en se basant sur leurs numéros (ils sont uniques)
 */
void Library::permuterImages(int numero1, int numero2) {
    if (numero1 == numero2) {
        std::cout << "Les deux numéros sont identiques, aucune permutation nécessaire" << std::endl;
        return;
    }

    // Pointeurs pour les nœuds contenant les images à permuter
    std::shared_ptr<INode> node1 = nullptr, node2 = nullptr;
    auto current = head;

    // Parcours de la liste pour trouver les nœuds correspondants
    while (current) {
        if (current->data.getNumero() == numero1) {
            node1 = current;
        } else if (current->data.getNumero() == numero2) {
            node2 = current;
        }
        current = current->next;
    }

    // Vérification si les deux images existent
    if (!node1 || !node2) {
        std::cerr << "Erreur : Une ou les deux images n'existent pas dans la bibliothèque." << std::endl;
        return;
    }

    // Permutation des données des deux nœuds
    Image temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;

    std::cout << "Les images avec les numéros " << numero1 << " et " << numero2 << " ont été permutées." << std::endl;
}



void Library::save() {

    /*Fonction save : demande a l'utilisateur de naviguer dans ces fichier pour clique sur une image 
    -> puis remplir a la main les descripteur (terminal ça va demander le prix etc...)
    -> et va enregistrer l'image dans la biblio et les descripteur a la suite de la liste .txt*/

    std::string cheminImage, source, titre, type;
    int numero, nbTraitementPossible, identite;
    double prix;
    char acces;
   

    std::cout << "Sélectionnez une image à ajouter.\n";

    if (cheminImage.empty()) {
        std::cerr << "Aucun fichier sélectionné.\n";
        return;
    }

    //Renseigner les descripteurs de l'image à la main 
    std::cout << "Source : ";
    std::cin >> source;
    std::cin.ignore();
    std::cout << "Titre : ";
    std::getline(std::cin, titre);
    std::cout << "Numéro unique : ";
    std::cin >> numero;
    std::cout << "Prix (en €) : ";
    std::cin >> prix;
    std::cout << "Accès (O pour ouvert, L pour limité) : ";
    std::cin >> acces;
    std::cout << "Type (par ex. 'couleur', 'gris') : ";
    std::cin >> type;
    std::cout << "Nombre de traitements possibles : ";
    std::cin >> nbTraitementPossible;
    std::cout << "Identité (ID utilisateur) : ";
    std::cin >> identite;


    //determine la biblio cible en fonction du prix
    // Déterminer la bibliothèque cible en fonction du prix
    std::string basePath = "C:/Users/leona_u6zzw0q/M2SIA2/PROJET/PROJETC_LMLTM/Bibliotheque/";


    std::string destinationDir;
    if (prix == 0) {
        destinationDir = basePath + "Gratuites";
    } else if (prix < 9.99) {
        destinationDir = basePath + "_9.99";
    } else if (prix < 99.99) {
        destinationDir = basePath + "_99.99";
    } else {
        destinationDir = basePath + "_100";
    }

    // Créer le répertoire cible si nécessaire
    fs::create_directories(destinationDir);

    // Copier l'image dans la bibliothèque dediée
    std::string destination = destinationDir + "/" + titre;
    try {
        fs::copy_file(cheminImage, destination, fs::copy_options::overwrite_existing);
        std::cout << "Image copiée dans : " << destination << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la copie de l'image : " << e.what() << std::endl;
        return;
    }

    //Ajouter à la bibliothèque
    Image nouvelleImage(source, titre, numero, prix, acces, type, nbTraitementPossible, identite);
    ajouterDescripteurs(nouvelleImage);



    //sauver avec un nom unique de bibliotheque pour ne pas ecraser les anciennes versions 
    // (on genere un nom unique pour pas que l'utilisateur ait a rentrer un nom de biblio)

    auto now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "Bibliotheque_%Y%m%d_%H%M%S.txt", std::localtime(&now));
    std::string nomBibliotheque = buffer;

    //partie save de la biblio

    std::ofstream fichier(nomBibliotheque);
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier " << nomBibliotheque << ".\n";
        return;
    }

    auto current = head;
    while (current) {
        fichier << current->data.getDescripteurSimple() << "\n";
        current = current->next;
    }

    fichier.close();

    std::cout << "La nouvelle bibliothèque a été sauvegardée sous le nom : " << nomBibliotheque << std::endl;

}

// Fonction permettant de fusionner deux bibliotheque
// Entre : objet de type Library qui represente la seconde bibliotheque
void Library::fusion(Library liste2)
{
    // On ajoute a la premiere bibliotheque chaque element de la seconde
    auto current = liste2.head;
    while (current) {
        ajouterDescripteurs(current->data);
        current = current->next;
    }
}

// Fonction permettant de verifier si la chaine de charactere passée en argument coincide
// au titre d'une image
// Entree : string
// Sortie : int (1 si la chaine de caractere correspond a un titre, 0 sinon)
int Library::titrecheck(std::string _titre) const
{
    int check = 0;
    auto _titrecheck = head;
    // On parcourt tous les titres des images de notre bibliotheque et on teste
    // si il existe une egalite notre argument
    while (_titrecheck)

        {   
            if (_titrecheck->data.getTitre() == _titre){
                
                check = 1;
            }
            _titrecheck = _titrecheck->next;
        }
    return check;
}

// Fonction permettant de verifier si un entier passé en argument coincide
// au numero d'une image
// Entree : int
// Sortie : int (1 si la chaine de caractere correspond a un titre, 0 sinon)
int Library::numerocheck(int num) const {
    auto numcheck = head;
    int check = 0;
    // On parcourt tous les numeros des images de notre bibliotheque et on teste
    // si il existe une egalite avec notre argument
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

