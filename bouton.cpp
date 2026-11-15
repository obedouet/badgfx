
#include <badgfx.hpp>

#define HAUTEUR_BOUTON 20

Bouton::Bouton(Fenetre *fen, Coord *coords, char *txt, int typ)
{
	SAUVE;
	INSTALL("Bouton::Bouton()");

	f = fen;

	// Construit le bouton 
	b_position.x = coords->x;
	b_position.y = coords->y;

	// Calcul la largeur en fct du texte
	b_width = strlen(txt) * 10;
	b_height = HAUTEUR_BOUTON;
	strcpy(b_title, txt);

	// Si c'est une 'CHECKBOX'
	b_type = typ;
	if (typ == CHECKBOX)
	{
		// Par defaut, non cochee
		b_etat = DISABLED;
	}
	else
	{
		// C'est un bouton normal
		b_etat = ENABLED;
	}

	// Par defaut ne fait rien
	Action=NULL;

	RESTORE;
}

/* Le destructeur ne fait rien içi
 */
Bouton::~Bouton()
{
}

/*
 * Fonction de dessin du bouton
 * Entree : pointeur sur un objet Fenetre
 * Sortie : sur l'écran banane
 */
void Bouton::Dessine_Bouton()
{
	SAUVE;
	INSTALL("Bouton::Dessine_Bouton()");

	switch (b_type)
	{
	    case NORMAL:
		// Contour
		XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("white"));
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y,
			DEC + b_position.x + b_width, DEC + b_position.y);
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y,
			DEC + b_position.x, DEC + b_position.y + b_height);
		XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("black"));
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x + b_width, DEC + b_position.y,
			DEC + b_position.x + b_width, DEC + b_position.y + b_height);
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y + b_height,
			DEC + b_position.x + b_width, DEC + b_position.y + b_height);
		
		// Remet la couleur d'origine
		XSetForeground(f->display, f->contexte_graphique, f->avant_plan);
	
		// Ecrit le message
		XDrawString(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x + (b_width/2 - (strlen(b_title) * 3)),
			DEC + b_position.y + (b_height/4 * 3),
			b_title, strlen(b_title));
		break;

	    case CHECKBOX:
	    	// Contour 
	    	XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("black"));
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y,
			DEC + b_position.x + b_width, DEC + b_position.y);
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y,
			DEC + b_position.x, DEC + b_position.y + b_height);
		XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("white"));
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x + b_width, DEC + b_position.y,
			DEC + b_position.x + b_width, DEC + b_position.y + b_height);
		XDrawLine(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y + b_height,
			DEC + b_position.x + b_width, DEC + b_position.y + b_height);
		
		//XFlush(f->display);
		
		// Fond blanc
		XFillRectangle(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x + 1, DEC + b_position.y + 1,
			b_width - 2, b_height - 2);
			
		// Remet la couleur d'origine 
		XSetForeground(f->display, f->contexte_graphique, f->avant_plan);
		
		// Affiche la croix 
		if (b_etat == ENABLED)
		{
			XDrawLine(f->display, f->fenetre, f->contexte_graphique,
				DEC + b_position.x + 1, DEC + b_position.y + 1,
				DEC + b_position.x + b_width - 2,
				DEC + b_position.y + b_height - 2);
			XDrawLine(f->display, f->fenetre, f->contexte_graphique,
				DEC + b_position.x + b_width - 1,
				DEC + b_position.y + 1,
				DEC + b_position.x + 1,
				DEC + b_position.y + b_height - 2);
		}
		
		// Ecrit le message
		XDrawString(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x + 20,
			DEC + b_position.y + 12,
			b_title, strlen(b_title));
		
	    	break;
	}

	// Force la mise a jour
	XFlush(f->display);
	RESTORE;
}

/*
 * Fonction de dessin d'un bouton
 * simule l'appui (hors 'CHECKBOX')
 * Entree : objet Fenetre
 */
void Bouton::Dessine_InvertBouton()
{
	SAUVE;
	INSTALL("Bouton::Dessine_InvertBouton()");

	// Contour
	XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("white"));
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		DEC + b_position.x + b_width, DEC + b_position.y,
		DEC + b_position.x + b_width, DEC + b_position.y + b_height);
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		DEC + b_position.x, DEC + b_position.y + b_height,
		DEC + b_position.x + b_width, DEC + b_position.y + b_height);
	XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("black"));
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		DEC + b_position.x, DEC + b_position.y,
		DEC + b_position.x + b_width, DEC + b_position.y);
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		DEC + b_position.x, DEC + b_position.y,
		DEC + b_position.x, DEC + b_position.y + b_height);
		
	XFlush(f->display);
		
	// Remet la couleur d'origine
	XSetForeground(f->display, f->contexte_graphique, f->avant_plan);
	
	RESTORE;
}

void Bouton::Rafraichir()
{
	SAUVE;
	INSTALL("Bouton::Rafraichir()");

	if (b_type == NORMAL && b_etat == DISABLED)
	{
		Dessine_InvertBouton();
	}
	else
		Dessine_Bouton();

	RESTORE;
}

/*
 * Fonction de gestion d'un clic de souris
 * Entree : pointeur sur l'évènement
 * Sortie : booleen
 */
int Bouton::OnClic(Evenement *evt)
{
	SAUVE;
	INSTALL("Bouton::OnClic()");

	if ((evt->x_coord - DEC >= b_position.x) &&
		(evt->x_coord - DEC <= b_position.x + b_width) &&
		(evt->y_coord - DEC >= b_position.y) &&
		(evt->y_coord - DEC <= b_position.y + b_height))
	{
		// Le bon bouton a été trouvé
		if (b_type == NORMAL && evt->bouton_souris == GAUCHE && evt->etat_souris == ButtonPress)
		{
			// Dessine un bouton enfoncé
			Dessine_InvertBouton();
			b_etat = DISABLED;
		}
		else if (b_type == NORMAL && evt->bouton_souris == GAUCHE && evt->etat_souris == ButtonRelease && b_etat == DISABLED)
		{
			// Relache le bouton
			Dessine_Bouton();
			b_etat = ENABLED;

			// Action 
			if (Action != NULL)
			{
				(*Action)(this);
			}
		}
		// Ajouter içi le code pour gerer une CHECKBOX

		RESTORE;
		return 1;
	}
	else
	{
		// Retour en arrière, action annulée
		if (b_type == NORMAL && b_etat == STANDBY)
			b_etat = ENABLED;
	}

	RESTORE;
	return 0;
}

/* Fonction de gestion d'un mouvement de la souris
 * Entree : pointeur sur l'évènement
 * Sortie : booleen
 */
int Bouton::OnMove(Evenement *evt)
{
	SAUVE;
	INSTALL("Bouton::OnMove()");

	if (! ((evt->x_coord - DEC >= b_position.x) &&
		(evt->x_coord - DEC <= b_position.x + b_width) &&
		(evt->y_coord - DEC >= b_position.y) &&
		(evt->y_coord - DEC <= b_position.y + b_height)))
	{
		// On n'est pas au-dessus du bouton
		if (b_type == NORMAL && b_etat == DISABLED)
		{
			// Relache le bouton car on etait
			// dessus précédement
			b_etat = STANDBY;
			Dessine_Bouton();

			RESTORE;
			return 1;
		}
	}
	else
	{
		// On est au-dessus du bouton

		// Gere le cas ou l'on revient dessus
		if (b_type == NORMAL && b_etat == STANDBY)
		{
			// On revient donc
			b_etat = DISABLED;
			Dessine_InvertBouton();
		}

		RESTORE;
		return 1;
	}

	RESTORE;
	return 0;
}

/* Fonction de gestion d'un évènement clavier
 * içi, on ne s'en préoccupe pas
 */
int Bouton::OnKey(Evenement *evt)
{
	return 0;
}

/* Fonction de paramétrage du comportement
 * d'un bouton sur un clic
 * Entree : pointeur sur fonction
 * Sortie : aucune
 */
void Bouton::ChgAction(void (*fct)(Composant *C))
{
	Action = fct;
}

/*
 * Fonction de changement du contenu d'un bouton
 * Entree : pointeur sur une chaine
 * Sortie : resultat si changement accepté
 */
int Bouton::ChgTexte(char *txt)
{
	if (strlen(txt) * 10 <= b_width)
	{
		// Changement accepté
		strcpy(b_title, txt);

		// Efface l'ancien titre
		XSetForeground(f->display, f->contexte_graphique, f->arr_plan);
		XFillRectangle(f->display, f->fenetre, f->contexte_graphique,
			DEC + b_position.x, DEC + b_position.y, b_width, b_height);
		XSetForeground(f->display, f->contexte_graphique, f->avant_plan);

		// OK
		return 1;
	}

	// Texte trop grand
	return 0;
}
