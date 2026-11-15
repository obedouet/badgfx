/* Routines d'affichage
 * Cree le : 14/12/1999
 */

#include <badgfx.hpp>

/* Constructeur
 */
Fenetre::Fenetre(void)
{
	First_Comp = NULL;

	largeur = 640;
	hauteur = 480;

	if (!OuvreFenetre())
	{
		fprintf(stderr, "Erreur d'instanciation de la classe !\n");
	}

}

/* Constructeur
 * Entree : largeur hauteur
 */
Fenetre::Fenetre(int l, int h)
{
	First_Comp = NULL;

	largeur = l;
	hauteur = h;

	if (!OuvreFenetre())
	{
		fprintf(stderr, "Erreur d'instanciation de la classe !\n");
	}

}

/* Encore un autre constructeur
 * Entree : largeur, hauteur, titre
 */
Fenetre::Fenetre(int l, int h, char *f_title)
{
	First_Comp = NULL;

	largeur = l;
	hauteur = h;
	f_titre = f_title;

	if (!OuvreFenetre())
	{
		fprintf(stderr, "Erreur d'instanciation de la classe !\n");
	}
}

/* Destructeur */
Fenetre::~Fenetre(void)
{
	FermeFenetre();
}

/*
 * Fonction d'ouverture de la fenetre graphique
 * Entree : aucune
 * Sortie : un booleen indiquant la reussite
 *	de la fonction (1 = reussite, 0 = echec)
 */
int Fenetre::OuvreFenetre(void)
{
	SAUVE;
	INSTALL("Fenetre::OuvreFenetre()");

	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "ERREUR: OuvreFenetre() : impossible d'ouvrir le display.\n");
		RESTORE;
		return 0;
	}

        /*
 	 * selection de l'ecran par defaut, dans le cas ou il y en a plusieurs
	 * et des couleurs.
	 */
	ecran = DefaultScreen(display);
	arr_plan = ChargeCouleur("grey");
	avant_plan = BlackPixel(display, ecran);

	/*
	 * Initialisation des dimensions.
	 * Les flags servent a indiquer les champs de la structure qui
	 * contienne des informations.
	 */

	infos.x = 100; infos.y = 100;                   /* positions */
	infos.width = largeur; infos.height = hauteur;		/* dimensions */
	infos.flags = PPosition|PSize;

	/* Creation de la fenetre */
	fenetre = XCreateSimpleWindow(display, DefaultRootWindow(display),
			infos.x, infos.y, infos.width, infos.height,
			0, 0, arr_plan);

	/*
	 * Choix des proprietes qui seront donnees au WindowsManager.
	 * Une autre procedure peut-etre utilisee a cet effet : XSetWMProperties()
	 */
	XSetStandardProperties(display, fenetre, f_titre == NULL ? "Bad Gfx": f_titre, f_titre == NULL ? "Bad Gfx" : f_titre,
				None, NULL, 0, &infos);

	/*
	 * Demande au Xserveur de nous creer un nouveau contexte graphique
	 * utilise pour les affichages de textes ou de graphiques.
	 */
	contexte_graphique = XCreateGC(display, fenetre, 0, NULL);

	XSetBackground(display, contexte_graphique, arr_plan);
	XSetForeground(display, contexte_graphique, avant_plan);

	/* Selection des evenements */
	XSelectInput(display, fenetre, 
		ButtonPressMask|ButtonReleaseMask|\
		KeyPressMask|KeyReleaseMask|\
		PointerMotionMask|ExposureMask);
		

        /* Rendre la fenetre visible */
	XMapRaised(display, fenetre);

	RESTORE;
	return 1;
}

/* Fonction de fermeture de la fenetre graphique
 * Entree : aucune
 * Sortie : aucune
 */
void Fenetre::FermeFenetre(void)
{
	XFreeGC(display, contexte_graphique);
	XDestroyWindow(display, fenetre);
	XCloseDisplay(display);
}

/* Fonction d'attente d'un evenement 
 * => bloquante
 * Entree : aucune
 * Sortie : une structure Evenement
 */
void Fenetre::AttenteEvenement(Evenement *evt)
{
	SAUVE;
	INSTALL("Fenetre::AttenteEvenement()");
	
	// Attente de l'evenement X
	XNextEvent(display, &evenement);

	// Traitement de l'evenement
	TraiteEvenement(evt);

	RESTORE;
}

/* Fonction de traitement d'un evenement
 * Entree : variable globale evenement
 * Sortie : structure Evenement
 */
void Fenetre::TraiteEvenement(Evenement *evt)
{
	Composant	*courant;
	SAUVE;
	INSTALL("Fenetre::TraiteEvenement()");

	switch(evenement.type)
	{
		case Expose:
			// Il faut rafraichir la fenetre
			if (evenement.xexpose.count == 0)
			{
				evt->type = AUTRE;
				Rafraichir();
			}
			break;

		case MappingNotify:
			// Mapping du clavier modifie
			evt->type = AUTRE;
			XRefreshKeyboardMapping((XMappingEvent *) &evenement);
			break;

		case MotionNotify:
			// La souris a bougée 
			evt->type = SOURIS;

			// Inscrit les coordonnées
			evt->x_coord = evenement.xbutton.x;
			evt->y_coord = evenement.xbutton.y;

			// Appel les composants
			courant = (Composant *) First_Comp;
			while (courant)
			{
				courant->OnMove(evt);
				courant = courant->suivant;
			}
			break;

		case ButtonPress:
		case ButtonRelease:
			// Bouton souris 
			evt->type = SOURIS;
			evt->etat_souris = evenement.type;

			// Inscrit les coordonnées
			evt->x_coord = evenement.xbutton.x;
			evt->y_coord = evenement.xbutton.y;

			// Indique le bouton
			switch(evenement.xbutton.button)
			{
				case 1:
					evt->bouton_souris = GAUCHE;
					break;
				case 2:
					evt->bouton_souris = MILIEU;
					break;
				case 3:
					evt->bouton_souris = DROITE;
					break;
				default:
					evt->bouton_souris = GAUCHE;
					break;
			}

			/* Appel les composants */
			courant = (Composant *) First_Comp;
			while (courant)
			{
				courant->OnClic(evt);
				courant = courant->suivant;
			}
			
			break;

		case KeyPress:
			/* Clavier */
			evt->type = CLAVIER;
			evt->lu_clavier = XLookupString((XKeyEvent *) &evenement, evt->buf_clavier, 15, &touche, NULL);

			/* Appel les composants */
			courant = (Composant *) First_Comp;
			while (courant)
			{
				courant->OnKey(evt);
				courant = courant->suivant;
			}

			break;

	}
	RESTORE;
}

/* Fonction d'attente d'un evenement 
 * => non bloquante
 * Entree : aucune
 * Sortie : une structure Evenement
 */
int Fenetre::CheckEvenement(Evenement *evt)
{
	SAUVE;
	INSTALL("Fenetre::CheckEvenement()");

	// Verifie la file d'attente
	if (XCheckWindowEvent(display, fenetre, ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|ExposureMask,&evenement))
	{
		// Traite l'evenement
		TraiteEvenement(evt);

		RESTORE;
		return 1;
	}

	RESTORE;
	return 0;
}

/* Fonction de rafraichissement de l'ecran
 * Entree : aucune
 * Sortie : aucune
 */
void Fenetre::Rafraichir(void)
{
	Composant *courant;

	SAUVE;
	INSTALL("Fenetre::Rafraichir()");

	/* Efface la fenetre */
	XClearWindow(display, fenetre);

	/* Re-dessine les composants */
	courant = (Composant *) First_Comp;
	while (courant)
	{
		courant->Rafraichir();
		courant = courant->suivant;
	}

	XFlush(display);

	RESTORE;
}

/* Fonction de recherche de la valeur d'une couleur
 * designee par son nom en anglais
 * Entree : nom de la couleur en anglais
 * Sortie : la valeur
 */
int Fenetre::ChargeCouleur(char *str)
{
	/* Variable temporaire */
	XGCValues xgcvalues;
	XColor xcolour;

	/* On recherche la couleur la plus proche */
	if (XParseColor (display, DefaultColormap(display, DefaultScreen(display)), str, &xcolour))
	{
		/* Et on l'alloue */
		if (XAllocColor (display, DefaultColormap(display, DefaultScreen(display)), &xcolour))
		{
			xgcvalues.foreground = xcolour.pixel;
		}
	}

	return xgcvalues.foreground;
}

/* Fonction de bouclage principale
 */
void Fenetre::MainLoop()
{
	Evenement event;

	SAUVE;
	INSTALL("Fenetre::MainLoop()");

	pas_fini = 1;
	while (pas_fini)
	{
		AttenteEvenement(&event);
	}

	RESTORE;
}

/* Fonction d'arret de la boucle principale
 *
 */
void Fenetre::FinLoop()
{
	pas_fini = 0;
}

/* Fonction d'ajout d'un composant graphique à la liste
 * Entree : référence sur le composant
 * Sortie : aucune
 */
void Fenetre::AjouteComposant(void *comp)
{
	SAUVE;
	INSTALL("Fenetre::AjouteComposant()");

	if (First_Comp == NULL)
	{
		/* Création de la liste */
		First_Comp = comp;
		((Composant *)comp)->suivant = NULL;
	}
	else
	{
		/* Insertion en tete */
		((Composant *)comp)->suivant = (Composant *) First_Comp;
		First_Comp = comp;
	}

	RESTORE;
}

/* Retourne la taille de la fenetre
 * Entree : référence sur une structure Coord
 * Sortie : dans la structure Coord
 */
void Fenetre::Taille(Coord *f_taille)
{
	f_taille->x = largeur;
	f_taille->y = hauteur;
}

/* Fonction de changement du titre dynamiquement
 * Entree : nouveau titre
 */
void Fenetre::ChgTitre(char *f_title)
{
	f_titre = f_title;
	XStoreName(display, fenetre, f_titre);
}
