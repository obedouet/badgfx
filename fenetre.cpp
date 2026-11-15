/* Routines d'affichage
 * Cree le : 14/12/1999
 */

#include <badgfx.hpp>

/* Constructeur
 */
Fenetre::Fenetre(void)
{
	First_Comp = NULL;
	f_titre = NULL;

	largeur = 640;
	hauteur = 480;

	display = NULL;
	fenetre = 0;
	contexte_graphique = NULL;
	ecran = 0;
	avant_plan = 0;
	arr_plan = 0;
	wm_protocols = 0;
	wm_delete = 0;
	pas_fini = 1;
	touche = 0;

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
	f_titre = NULL;

	largeur = l;
	hauteur = h;

	display = NULL;
	fenetre = 0;
	contexte_graphique = NULL;
	ecran = 0;
	avant_plan = 0;
	arr_plan = 0;
	wm_protocols = 0;
	wm_delete = 0;
	pas_fini = 1;
	touche = 0;

	if (!OuvreFenetre())
	{
		fprintf(stderr, "Erreur d'instanciation de la classe !\n");
	}

}

/* Encore un autre constructeur
 * Entree : largeur, hauteur, titre
 */
Fenetre::Fenetre(int l, int h, const char *f_title)
{
	First_Comp = NULL;

	largeur = l;
	hauteur = h;
	f_titre = f_title;

	display = NULL;
	fenetre = 0;
	contexte_graphique = NULL;
	ecran = 0;
	avant_plan = 0;
	arr_plan = 0;
	wm_protocols = 0;
	wm_delete = 0;
	pas_fini = 1;
	touche = 0;

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
	const char	*titre;

	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "ERREUR: OuvreFenetre() : impossible d'ouvrir le display.\n");
		return 0;
	}

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
	if (fenetre == 0)
	{
		XCloseDisplay(display);
		display = NULL;
		return 0;
	}

	titre = (f_titre == NULL ? "Bad Gfx" : f_titre);
	XStoreName(display, fenetre, const_cast<char *>(titre));
	XSetWMNormalHints(display, fenetre, &infos);

	/*
	 * Demande au Xserveur de nous creer un nouveau contexte graphique
	 * utilise pour les affichages de textes ou de graphiques.
	 */
	contexte_graphique = XCreateGC(display, fenetre, 0, NULL);
	if (contexte_graphique == NULL)
	{
		XDestroyWindow(display, fenetre);
		XCloseDisplay(display);
		fenetre = 0;
		display = NULL;
		return 0;
	}

	XSetBackground(display, contexte_graphique, arr_plan);
	XSetForeground(display, contexte_graphique, avant_plan);

	/* Selection des evenements */
	XSelectInput(display, fenetre, 
		ButtonPressMask|ButtonReleaseMask|\
		KeyPressMask|KeyReleaseMask|\
		PointerMotionMask|ExposureMask|StructureNotifyMask);

	wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);
	wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, fenetre, &wm_delete, 1);

	/* Rendre la fenetre visible */
	XMapRaised(display, fenetre);
	XFlush(display);

	return 1;
}

/* Fonction de fermeture de la fenetre graphique
 * Entree : aucune
 * Sortie : aucune
 */
void Fenetre::FermeFenetre(void)
{
	if (display == NULL)
	{
		return;
	}

	if (contexte_graphique != NULL)
	{
		XFreeGC(display, contexte_graphique);
		contexte_graphique = NULL;
	}

	if (fenetre != 0)
	{
		XDestroyWindow(display, fenetre);
		fenetre = 0;
	}

	XCloseDisplay(display);
	display = NULL;
}

/* Fonction d'attente d'un evenement 
 * => bloquante
 * Entree : aucune
 * Sortie : une structure Evenement
 */
void Fenetre::AttenteEvenement(Evenement *evt)
{
	if (display == NULL)
	{
		if (evt != NULL)
		{
			evt->type = AUTRE;
		}

		return;
	}

	// Attente de l'evenement X
	XNextEvent(display, &evenement);

	// Traitement de l'evenement
	TraiteEvenement(evt);

}

/* Fonction de traitement d'un evenement
 * Entree : variable globale evenement
 * Sortie : structure Evenement
 */
void Fenetre::TraiteEvenement(Evenement *evt)
{
	Composant	*courant;

	if (evt == NULL)
	{
		return;
	}

	evt->type = AUTRE;

	switch(evenement.type)
	{
		case Expose:
			// Il faut rafraichir la fenetre
			if (evenement.xexpose.count == 0)
			{
				Rafraichir();
			}
			break;

		case MappingNotify:
			// Mapping du clavier modifie
			XRefreshKeyboardMapping((XMappingEvent *) &evenement);
			break;

		case MotionNotify:
			// La souris a bougée 
			evt->type = SOURIS;

			// Inscrit les coordonnées
			evt->x_coord = evenement.xbutton.x;
			evt->y_coord = evenement.xbutton.y;

			// Appel les composants
			courant = First_Comp;
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
			courant = First_Comp;
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
			courant = First_Comp;
			while (courant)
			{
				courant->OnKey(evt);
				courant = courant->suivant;
			}

			break;

		case ClientMessage:
			if ((Atom)evenement.xclient.data.l[0] == wm_delete)
			{
				FinLoop();
			}
			break;

		case DestroyNotify:
			FinLoop();
			break;

		default:
			break;

	}
}

/* Fonction d'attente d'un evenement 
 * => non bloquante
 * Entree : aucune
 * Sortie : une structure Evenement
 */
int Fenetre::CheckEvenement(Evenement *evt)
{
	if (display == NULL)
	{
		return 0;
	}

	// Verifie la file d'attente
	if (XCheckWindowEvent(display, fenetre, ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|ExposureMask|StructureNotifyMask,&evenement))
	{
		// Traite l'evenement
		TraiteEvenement(evt);

		return 1;
	}

	return 0;
}

/* Fonction de rafraichissement de l'ecran
 * Entree : aucune
 * Sortie : aucune
 */
void Fenetre::Rafraichir(void)
{
	Composant *courant;


	/* Efface la fenetre */
	XClearWindow(display, fenetre);

	/* Re-dessine les composants */
	courant = First_Comp;
	while (courant)
	{
		courant->Rafraichir();
		courant = courant->suivant;
	}

	XFlush(display);

}

/* Fonction de recherche de la valeur d'une couleur
 * designee par son nom en anglais
 * Entree : nom de la couleur en anglais
 * Sortie : la valeur
 */
unsigned long Fenetre::ChargeCouleur(const char *str)
{
	/* Variable temporaire */
	XColor	xcolour;

	if (display == NULL || str == NULL)
	{
		return 0;
	}

	/* On recherche la couleur la plus proche */
	if (XParseColor (display, DefaultColormap(display, ecran), str, &xcolour))
	{
		/* Et on l'alloue */
		if (XAllocColor (display, DefaultColormap(display, ecran), &xcolour))
		{
			return xcolour.pixel;
		}
	}

	return BlackPixel(display, ecran);
}

/* Fonction de bouclage principale
 */
void Fenetre::MainLoop()
{
	Evenement event;

	if (display == NULL)
	{
		return;
	}

	pas_fini = 1;
	while (pas_fini && display != NULL)
	{
		AttenteEvenement(&event);
	}

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
void Fenetre::AjouteComposant(Composant *comp)
{
	if (comp == NULL)
	{
		return;
	}

	if (First_Comp == NULL)
	{
		/* Création de la liste */
		First_Comp = comp;
		comp->suivant = NULL;
	}
	else
	{
		/* Insertion en tete */
		comp->suivant = First_Comp;
		First_Comp = comp;
	}

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
void Fenetre::ChgTitre(const char *f_title)
{
	f_titre = f_title;

	if (display != NULL && fenetre != 0 && f_titre != NULL)
	{
		XStoreName(display, fenetre, const_cast<char *>(f_titre));
	}
}
