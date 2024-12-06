---

# **Instructions pour travailler avec le repository**

Ce document explique les étapes pour configurer votre environnement local, cloner le repository, et contribuer au projet.

---

## **1. Prérequis**
Avant de commencer, assurez-vous que votre machine dispose de :
- **Git** installé.  
  (Pour installer Git : [Documentation Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git))
- Un compte GitHub configuré avec une clé SSH ou un accès HTTPS.

---

## **2. Cloner le repository**
Pour obtenir une copie locale du projet :

1. Accédez à l'URL du repository GitHub.
2. Copiez le lien HTTPS ou SSH.
3. Dans un terminal, exécutez :
   ```bash
   git clone <URL_DU_REPOSITORY>
   ```
   Par exemple :
   ```bash
   git clone https://github.com/votre_nom_utilisateur/Projet-C.git
   ```
4. Accédez au dossier cloné :
   ```bash
   cd Projet-C
   ```

---

## **3. Configuration initiale**
### **3.1 Configurer votre identité Git**
Ces informations seront associées à vos commits :
```bash
git config --global user.name "VotreNom"
git config --global user.email "VotreEmail@exemple.com"
```

### **3.2 Vérifier le lien avec le repository GitHub**
Pour vérifier que votre projet est bien connecté :
```bash
git remote -v
```
Vous devez voir une ligne indiquant l’URL du repository GitHub.

---

## **4. Travailler sur le projet**
### **4.1 Mettre à jour votre copie locale**
Avant de commencer à travailler, assurez-vous d’avoir la dernière version du code :
```bash
git pull
```

### **4.2 Ajouter de nouvelles modifications**
Après avoir fait des modifications dans vos fichiers :
1. Vérifiez les fichiers modifiés :
   ```bash
   git status
   ```
2. Ajoutez les fichiers au suivi :
   ```bash
   git add .
   ```
3. Faites un commit avec un message clair décrivant les changements :
   ```bash
   git commit -m "Description de vos modifications"
   ```

### **4.3 Envoyer les modifications sur GitHub**
Poussez vos changements vers le repository distant :
```bash
git push
```

---

## **5. Créer et utiliser des branches**
Pour travailler sur une nouvelle fonctionnalité ou correction de bug, créez une branche séparée :
1. Créez une nouvelle branche :
   ```bash
   git checkout -b nom_de_la_branche
   ```
2. Faites vos modifications, ajoutez, et commitez comme d’habitude.
3. Envoyez votre branche sur GitHub :
   ```bash
   git push -u origin nom_de_la_branche
   ```

Pour revenir à la branche principale (`main`) :
```bash
git checkout main
```

---

## **6. Résoudre les conflits**
En cas de conflit lors d’un `git pull` ou `git merge` :
1. Git indiquera les fichiers en conflit. Ouvrez ces fichiers pour résoudre les différences manuellement.
2. Une fois les conflits résolus, ajoutez les fichiers corrigés :
   ```bash
   git add fichier_conflit
   ```
3. Terminez le merge avec un commit :
   ```bash
   git commit -m "Conflit résolu"
   ```

---

## **7. Récapitulatif des commandes utiles**
| Action                        | Commande                                                   |
|-------------------------------|-----------------------------------------------------------|
| Cloner le repository          | `git clone <URL>`                                         |
| Vérifier l’état des fichiers  | `git status`                                              |
| Ajouter des fichiers au suivi | `git add .`                                               |
| Faire un commit               | `git commit -m "Message descriptif"`                     |
| Pousser les modifications     | `git push`                                               |
| Mettre à jour votre copie     | `git pull`                                               |
| Créer une branche             | `git checkout -b nom_de_la_branche`                      |
| Changer de branche            | `git checkout nom_de_la_branche`                         |
| Supprimer une branche         | `git branch -d nom_de_la_branche`                        |

---

## **8. En cas de problème**
Si vous rencontrez des difficultés :
1. Vérifiez les messages d’erreur affichés dans le terminal.
2. Consultez la [documentation officielle Git](https://git-scm.com/doc).
3. Contactez un membre de l'équipe pour assistance.

---
