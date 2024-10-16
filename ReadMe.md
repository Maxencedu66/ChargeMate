
# Projet : Transition vers une mobilité plus respectueuse de l'environnement

## Description du projet

Ce projet pluridisciplinaire d’informatique intégrative, réalisé durant le second semestre, vise à accompagner la transition vers une mobilité plus respectueuse de l'environnement, en particulier dans le contexte de l'interdiction de la vente de voitures à moteur thermique en Europe à partir de 2035. Dans cette perspective, les véhicules électriques apparaissent comme une solution prometteuse, et notre projet a pour objectif de faciliter leur utilisation.

Le projet se divise en deux grandes parties :

### 1. Planification de trajets en voiture électrique

La première partie de notre projet consiste à développer un outil permettant à un utilisateur de parcourir un trajet en voiture électrique d'un point A à un point B en France. Ce système inclut :
- L'affichage des stations de recharge disponibles sur le trajet.
- La proposition d'arrêts nécessaires pour recharger le véhicule afin de ne pas tomber en panne de batterie.
- Des fonctionnalités supplémentaires telles que la gestion de la batterie au départ et la définition d'un seuil minimal de batterie pour assurer la longévité de celle-ci.

### 2. Simulation de la charge des bornes de recharge

La seconde partie est dédiée à l'implémentation d'un module de simulation. Ce simulateur permet de :
- Gérer un ensemble d’usagers fictifs possédant des véhicules électriques.
- Évaluer la charge du réseau de bornes de recharge en fonction du nombre d’usagers, de leurs parcours, et des caractéristiques de leurs véhicules.
- Identifier les bornes surchargées et celles où des files d’attente se forment, et proposer des chemins alternatifs aux véhicules concernés.

Ce simulateur est un outil précieux pour les investisseurs et les autorités, leur permettant d'évaluer la charge des infrastructures de recharge et de prendre des décisions éclairées sur leur expansion ou leur gestion.

## Fonctionnalités principales

- **Planification de trajet** : Détermine le meilleur itinéraire pour un véhicule électrique tout en optimisant les arrêts de recharge.
- **Simulation de charge du réseau** : Simule le comportement de multiples usagers de véhicules électriques sur un réseau de bornes de recharge.
- **Gestion des files d'attente** : Identifie les bornes saturées et propose des alternatives pour éviter les files d’attente.
- **Paramètres personnalisables** : Prise en compte de l'état de la batterie, de la vitesse des véhicules et des caractéristiques de recharge.

## Objectif

Ce projet a pour but de faciliter l'adoption des véhicules électriques en France en offrant un outil de planification et de simulation qui aide à optimiser l'expérience utilisateur et à prévoir les besoins en infrastructures de recharge. Il s'adresse tant aux utilisateurs finaux qu'aux décideurs dans le domaine des infrastructures énergétiques.

## Guide de lancement

L’utilisation de notre application est très simple. Voici les étapes à suivre pour la lancer :

1. Ouvrez un terminal et placez-vous dans le répertoire `/src` de notre dépôt GitLab :
   ```bash
   cd src
   ```

2. Lancez l’application en utilisant la commande suivante :
   ```bash
   make ChargeMate
   ```

3. Un menu textuel apparaîtra, vous offrant plusieurs options à sélectionner en fonction des fonctionnalités que vous souhaitez utiliser :

    - **Taper 1** : Lance l’itinéraire classique. Vous devrez entrer les informations suivantes :
        - Ville de départ
        - Ville d’arrivée
        - Modèle de voiture
        - Batterie initiale
        - Seuil de batterie minimal à ne pas dépasser
        - Actualisation ou non des étapes du trajet
        - Temps d’attente maximum dans une station de recharge
    
    - **Taper 2** : Lance une simulation permettant d’afficher les trajets. Vous devrez entrer le nombre d'entités à simuler.

    - **Taper 3** : Lance une simulation pour évaluer les files d'attente dans les différentes stations de recharge. Vous devrez entrer :
        - Le nombre d’entités
        - La durée pendant laquelle évaluer les files d’attente
        - L'heure de début de l'évaluation

    - **Taper 4** : Ferme l'application.

## Technologies utilisées

- Python (gestion des données, simulation)
- APIs de géolocalisation (pour les trajets et les bornes de recharge)
- Librairies de simulation et de gestion de flux

## Installation et exécution

1. Clonez ce dépôt :
   ```bash
   git clone https://github.com/votre-utilisateur/votre-repo.git
   ```

2. Installez les dépendances :
   ```bash
   pip install -r requirements.txt
   ```

3. Exécutez le programme principal pour la planification de trajet ou la simulation :
   ```bash
   python main.py
   ```

