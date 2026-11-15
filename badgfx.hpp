/* Bibliotheque BadGFX
 * 
 * headers
 *
 * Par : Bad Max
 * Date : 24/03/2002
 */

#ifndef BADGFX_HPP
#define BADGFX_HPP

// ==== Types graphiques ==== 
// type coordonnées graphiques
typedef struct
{
	int	x;
	int	y;
} Coord;

// type Evenement
typedef struct
{
	int	type;		/* Type d'evenement */
	char	buf_clavier[16];	/* Buffer clavier */
	char	lu_clavier;	/* Nombres de char lus */
	char	bouton_souris;	/* Indique le bouton de la souris */
	char	etat_souris;	/* Etat bouton souris */
	int	x_coord;	/* Coordonnees X */
	int	y_coord;	/* Coordonnees Y */
} Evenement;

// ==== Constantes ====
// Types d'evenements graphiques possibles 
#define CLAVIER 1
#define SOURIS 2
#define BOUTON 3
#define AUTRE 20 

// Boutons de la souris 
#define GAUCHE 1
#define MILIEU 2
#define DROITE 3

// Decalage graphique NE PAS TOUCHER
#define DEC 5

// Types de boutons graphiques
#define NORMAL	0
#define CHECKBOX	1

// Etats possibles
#define ENABLED 1
#define DISABLED 0
#define STANDBY 2

// ==== Inclus les headers necessaires ====
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// ==== Fonction utilitaire ====
// Copie une chaine dans un tampon fixe, toujours terminee par '\0'
static inline void CopieChaine(char *dest, const char *src, unsigned int n)
{
	strncpy(dest, src, n - 1);
	dest[n - 1] = '\0';
}

#include <fenetre.hpp>
#include <composant.hpp>
#include <bouton.hpp>
#include <message.hpp>

#endif
