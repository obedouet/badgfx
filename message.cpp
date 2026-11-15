/* Composant boite à messages
 */

#include <badgfx.hpp>

MessageBox::MessageBox(Fenetre *fen)
{

	f = fen;

	// Taille par défaut des messages
	size_msg = 45;

	// Taille de l'historique par défaut 
	hist = 4;

	Init();

}

MessageBox::MessageBox(Fenetre *fen, int msg_hist)
{

	f = fen;

	hist = msg_hist;

	// Taille par défaut des messages
	size_msg = 45;

	Init();

}

MessageBox::~MessageBox()
{
	int	i;

	/* Libere l'historique */
	if (old_str != NULL)
	{
		for (i = 0 ; i < hist ; i++)
		{
			free(old_str[i]);
		}
		free(old_str);
	}

}

void MessageBox::Init(void)
{
	int i;

	Coord taille_fen;

	if (hist <= 0)
	{
		hist = 4;
	}

	if (size_msg <= 0)
	{
		size_msg = 45;
	}

	f->Taille(&taille_fen);

	// Est collé au rebord
	zone_msg_x = DEC;

	// Hauteur de la boite a message
	zone_msg_h = hist * 18;

	// Positionne la boite a message
	zone_msg_y = taille_fen.y - (zone_msg_h + DEC);

	// Prend la largeur de la fenetre
	zone_msg_w = taille_fen.x - 2*DEC;

	/* Construction de l'historique */
	old_str = (char **) malloc(hist * sizeof(char *));
	if (old_str == NULL)
	{
		hist = 0;
		num = 0;
		return;
	}

	for (i = 0 ; i < hist ; i++)
	{
		old_str[i] = (char *) calloc(size_msg, sizeof(char));
		if (old_str[i] == NULL)
		{
			hist = i;
			break;
		}
	}

	num = 0;
}

void MessageBox::EcritMessage(const char *str)
{
	int		i,
			j;

	if (str == NULL || hist <= 0 || old_str == NULL)
	{
		return;
	}

	// Efface la zone 
	XClearArea(f->display, f->fenetre, zone_msg_x + 3, zone_msg_y + 3, 
		zone_msg_w - 6, zone_msg_h - 6, 0);

	if (num < hist)
	{
		// Alloue une nouvelle chaine 
		CopieChaine(old_str[num], str, size_msg);

		// Se place sur la prochaine 
		num++;
	}
	else
	{
		/* On libere la premiere */
		free(old_str[0]);

		/* Et on recopie les autres */
		j = 0;
		for (i = 1 ; i < hist ; i++)
		{
			old_str[j++] = old_str[i];
		}

		/* Alloue un nouveau */
		old_str[hist - 1] = (char *) calloc(size_msg, sizeof(char));
		if (old_str[hist - 1] != NULL)
		{
			CopieChaine(old_str[hist - 1], str, size_msg);
		}
	}

	/* Ecrit le texte */
	for (i = 0 ; i < num ; i++)
	{
		XDrawString(f->display, f->fenetre, f->contexte_graphique,
			zone_msg_x + 5, zone_msg_y + 20 + (15 * i), old_str[i], strlen(old_str[i]));
	}

	/* Synchro avec l'affichage */
	XFlush(f->display);

}

void MessageBox::Rafraichir()
{
	int		i;


	/* Efface la zone */
	XClearArea(f->display, f->fenetre, zone_msg_x + 3, zone_msg_y + 3, 
		zone_msg_w - 6, zone_msg_h - 6, 0);


	/* Dessine la zone de messagerie */
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		zone_msg_x, zone_msg_y, 
		zone_msg_x + zone_msg_w, zone_msg_y);
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		zone_msg_x, zone_msg_y, 
		zone_msg_x, zone_msg_y + zone_msg_h);

	XSetForeground(f->display, f->contexte_graphique, f->ChargeCouleur("white"));
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		zone_msg_x, zone_msg_y + zone_msg_h, 
		zone_msg_x + zone_msg_w, zone_msg_y + zone_msg_h);
	XDrawLine(f->display, f->fenetre, f->contexte_graphique,
		zone_msg_x + zone_msg_w, zone_msg_y + zone_msg_h, 
		zone_msg_x + zone_msg_w, zone_msg_y);
	XSetForeground(f->display, f->contexte_graphique, f->avant_plan);


	/* Ecrit le texte */
	for (i = 0 ; i < num ; i++)
	{
		XDrawString(f->display, f->fenetre, f->contexte_graphique,
			zone_msg_x + 5, zone_msg_y + 20 + (15 * i), old_str[i], strlen(old_str[i]));
	}

	/* Synchro avec l'affichage */
	XFlush(f->display);

}

int MessageBox::OnClic(Evenement *evt)
{
	(void) evt;
	return 0;
}

int MessageBox::OnMove(Evenement *evt)
{
	(void) evt;
	return 0;
}

int MessageBox::OnKey(Evenement *evt)
{
	(void) evt;
	return 0;
}
