# Fonctionnement

## Algorithme

Habitant initialisé à un site aléatoire
Maintenance initialisé au dépot

Les habitants se déplacent à un site aléatoire en prenant un velo
Une fois arrivé ils déponsent le vélo

S'il n'y a plus de vélo
	Vont entrer dans la queue pour savoir prendre un vélo

S'il n'y a plus de borne libre
	Vont entrer dans une autre queue pour déposer leur vélo


Les habitants sont la classes habitant.h

La maintenance est la class maintenance.h

Les sites sont représenté par une structure Site dans algothread.h
Structure décrite dans le programme


La queue est représentée par une QQueue, Il y 2 conditions pour ne réveillé seulement les habitants concerné.
-> Si on rajoute des velos, on réveille seulement ceux qui attendent pour un vélo

La maintenance et les habitants sont dans une boucle infinies comme précisé dans le PDF.

## Interface Graphique

Un widget à été créé pour demandé à l'utilisateurs les divers argument -> initwidget.h
Tout ce qui était dans MainWindow à la base a été déplacé dans CityWidget
MainWindow n'est plus qu'un QWidget avec un stacklayout qui va gérer la transition des données de InitWidget à CityWidget

## Tests

Le programme a été testé en affichant toutes les modifications dans les sites et en vérifiant que le total de vélo à chaque site et les déplacement de habitants de site à site était correcte. Nous avons également vérifié que les queues fonctionnement bien correctement que les personnes "sorte" de l'attente dans l'ordre.

Il est possible d'afficher les déplacement et modification de total de vélo à chaque site en changeant le booléen displayQDebug dans le constructeur en le mettant à true


