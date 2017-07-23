# Projet Bts

## Présentationdu projet
Voici Mon projet Bts "Réservation parking" que j'ai réalisé avec deux élèves de ma classe.

"Le système détecte les emplacements vides dans les deux étages du parking.
Les lumières au-dessus des places s’allument en conséquence. Rouge pour
une place occupée et vert pour une place disponible.
Un afficheur à chaque étage permet au conducteur de connaitre l’état de
chaque niveau pour rechercher une place.
A l’entrée du parking, un afficheur général permet de connaitre l’état du
parking avant de rentrer dedans.
Le superviseur du parking peut aussi connaitre en permanence l’état du
parking place par place jusqu’au général.
Une zone spécifique du parking est disponible uniquement sur réservation.
L’utilisateur prévoit son arrivée sur le site web du parking et choisit son
créneau horaire. Le conducteur s’authentifie avec un badge RFID, si son
arrivée correspond au créneau réservé, une barrière se lève pour lui
permettre d’accéder à la zone prévue."

**On a eu 3 tâches à répartir:**

- Capteurs et afficheurs unitaires à chaque étage (Mbed).

- Site, base de données, module de communication entre la base de données (Lamp et c/c++).

- Barrière et afficheur général (Raspberry).

Je fus en charge de la création du site web, de la modélisation et de la gestion de la base de données et de la création
module de communication entre la base de données.

## Norme de la trame

Afin de communiquer nous avons créé une trame sur 6 octets, avec 2 octets pour l'identification, et 4 octets pour l'information. Avec 
parfois une réponse du module.

**Nous avions besoin de créér 3 trames:**

- Une trame pour changer l'état (occupé/libre) d'une place dans la base de données. A chaque changement d'état la trame était envoyée.
Pas de réponse.

Identification (2 octets): E0 ; Place (2 octets) ; Etage (1 octet) ; Etat (1 octet).

Exemple: 

E00511 (La place 5 de l'étage 1 est occupée).
E00510 (La place 5 de l'étage 1 est libre).

- Une trame pour demander le nombre de places libres sur un étage.

Identification (2 octets): A0 ; Etage (1 octet) ; Octets de bourage (3 octets). La réponse est le nombre de places disponibles
pour l'étage demandé.

Exemple:

A001 (Nombre de places libres au premier étage)

- Une trame pour vérifier si une réservation a été éffectuée avec un badge. La réponse est l'autorisation de l'ouverture de la barrière
ou l'interdiction.

Identification (2 octets): B0 ; RFID (4 octets).

Exemple:

B005656790
