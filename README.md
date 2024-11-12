Application de Recherche de Fichiers en C
Ce projet implémente une application en C pour rechercher des fichiers dans un répertoire et ses sous-répertoires en utilisant des threads. Chaque thread explore un sous-répertoire et ajoute les chemins des fichiers trouvés à une liste partagée.

Fonctionnalités
Recherche récursive dans les répertoires et sous-répertoires spécifiés.
Exploration multi-threadée pour un traitement rapide et efficace.
Liste partagée des chemins de fichiers trouvés, protégée par un mutex pour garantir la sécurité des threads.
Documentation du code pour faciliter la compréhension et la maintenance.
Prérequis
Compilateur GCC avec support pour les threads (pthread).
Système compatible POSIX (Linux, macOS, etc.).
Instructions de Compilation
Pour compiler l'application, utilisez la commande suivante dans un terminal :

bash
Copier le code
gcc -pthread -o file_search file_search.c
Instructions d'Exécution
Pour exécuter l'application, spécifiez le répertoire de départ comme argument :

bash
Copier le code
./file_search /chemin/vers/repertoire
Exemple
bash
Copier le code
./file_search /home/utilisateur/documents
L'application affichera tous les fichiers trouvés dans le répertoire et ses sous-répertoires.

Structure du Projet
bash
Copier le code
file_search_application/
├── file_search.c          # Code principal de l'application
├── README.md              # Documentation du projet
Explication des Principales Fonctions
add_file_to_list : Ajoute un fichier trouvé à la liste partagée, avec une protection mutex pour éviter les conflits entre threads.
search_directory : Explore un répertoire, crée de nouveaux threads pour chaque sous-répertoire et ajoute les fichiers trouvés à la liste partagée.
print_file_list : Affiche tous les chemins de fichiers enregistrés dans la liste.
free_file_list : Libère toute la mémoire allouée pour la liste des fichiers.
Synchronisation et Gestion des Threads
Chaque sous-répertoire est exploré par un thread distinct, géré via les fonctions POSIX pthread. Un mutex (pthread_mutex_t) est utilisé pour sécuriser l'accès concurrent à la liste des fichiers.
