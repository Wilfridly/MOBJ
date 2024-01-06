LYAUTEY Wilfrid M1 SESI

                        --TME 8 - 10 :-- 

Mise en place : 
-------------------------------
    Répertoire	            Contenu
    <top>/src/	            Les fichiers sources (.h, .cpp, cmake)
    <top>/build/	        La compilation générée par cmake.
    <top>/install/include/	Les fichiers .h installés
    <top>/install/lib/	    Les éventuelles librairies
    <top>/install/bin/	    Les binaires.
    <top>/work/

Compilation : 
-------------------------------

    cd build/
    cmake ../src
    cd ../work
    ../install/bin/TME810

Implémentation des classes :
-------------------------------

    -CellViewer
    -SaveCellDialog
    -OpenCellDialog
    -InstancesWidget
    -InstancesModel
    -CellsLib
    -CellsModel
    -CellWidget

Possibilités de l'application :
-------------------------------
    +Ouverture des xml pour la visualisation des Netlist.
        -Affichage des Cells        -> (Ctrl+C) puis load
        -Affichage des Instances    -> (Ctrl+V) puis load
    +Sauvegarde des fichiers 
        -Sauvegarde du fichier      -> (Ctrl+S)
    +Chargement des fichiers non présents dans la liste
        -Chargement d'un fichier à partir de son nom sans 
        extension                   -> (Ctrl+O) puis ok
        Le chargement nécessite deux appuis.
        La première fois est pour charger le fichier,
        la deuxième fois est pour ouvrir le fichier.
    +Quitter 
        -Quitter l'application       -> (Ctrl+Q)

Points à revoir :
-------------------------------
    +Gestion de la mémoire
    +Affichage de la Netlist
        -noms des instances non centrés
    +Le code de query peut être fragmenté en plusieurs
    sous-fonctions pour une meilleure lisibilité.
    +Polygone pour les ExternalTerms

-------------------------------
-------------------------------
Avec l'aide précieuse d'Emile Pirard et d'Owen Liu qui 
m'ont permis de progresser au mieux. 








