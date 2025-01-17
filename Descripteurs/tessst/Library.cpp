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

#include <QDir>

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

#include <QCoreApplication>

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

/*------------------------------------------------------------------------------------------------*/

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

//void Library::modifierAcces(int numero) {
//    auto current = head; // Commence à la tête de la liste
//    while (current) { // Parcourt la liste chaînée
//        if (current->data.getNumero() == numero) { // Vérifie si le numéro correspond
//            // Bascule entre 'O' et 'L' pour l'accès
//            if (current->data.getAccess() == 'O') {
//                current->data.setAccess('L');
//            } else {
//                current->data.setAccess('O');
//            }
//            std::cout << "L'accès de l'image numéro " << numero << " a été modifié." << std::endl;
//            return; // Arrête la recherche après la modification
//        }
//        current = current->next; // Passe au nœud suivant
//    }
//    std::cerr << "Image avec le numéro " << numero << " non trouvée dans la liste." << std::endl;
//}




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

/*
// Fonction pour ouvrir une boîte de dialogue et sélectionner un fichier
std::string openFileDialog() {
    char fileName[MAX_PATH] = "";
    OPENFILENAME ofn = {0};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = nullptr; // Pas de fenêtre parent
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.jpeg;*.bmp;*.gif\0Tous les fichiers\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return std::string(fileName); // Retourne le chemin du fichier sélectionné
    }

    return ""; // Si l'utilisateur annule, retourne une chaîne vide
}

// Fonction pour copier un fichier dans un dossier de destination
void copyFile(const std::string& sourcePath, const std::string& destinationFolder) {
    fs::path sourceFile(sourcePath);

    // Vérifier si le fichier source existe
    if (!fs::exists(sourceFile)) {
        std::cerr << "Erreur : le fichier n'existe pas." << std::endl;
        return;
    }

    // Créer le dossier de destination s'il n'existe pas
    if (!fs::exists(destinationFolder)) {
        fs::create_directories(destinationFolder);
    }

    // Construire le chemin de destination
    fs::path destinationFile = fs::path(destinationFolder) / sourceFile.filename();

    // Copier le fichier
    try {
        fs::copy(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
        std::cout << "Fichier copié dans : " << destinationFile << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la copie du fichier : " << e.what() << std::endl;
    }
}
*/

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
    /*
    std::cout << "Entrez le chemin complet de l'image à ajouter : ";
    std::cin.ignore();
    std::getline(std::cin, cheminImage);

    if (cheminImage.empty()){
        std::cerr << "chemin introuvable " << std::endl;    }*/

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

    //(jsp si c'est utile de dire le nom mais pour tester si la fonction marche c'est pratique)
    std::cout << "La nouvelle bibliothèque a été sauvegardée sous le nom : " << nomBibliotheque << std::endl;

}void Library::fusion(Library liste2)
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

