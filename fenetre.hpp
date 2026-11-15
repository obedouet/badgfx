// Header pour fenetre.cpp

#ifndef CLASSFENETRE
#define CLASSFENETRE

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Composant;

class Fenetre
{
private:
	/*** Variables globales ***/
	Composant	*First_Comp;
	int	pas_fini;
	int	largeur,
		hauteur;
	const char	*f_titre;
	Atom	wm_protocols,
		wm_delete;

	// Fonctions privées
	int OuvreFenetre(void);
	void FermeFenetre(void);
	void AttenteEvenement(Evenement *evt);
	void Rafraichir(void);
	void TraiteEvenement(Evenement *evt);

public:
	/* Celles-ci sont reservees a X-Window */
	Display 	*display;
	Window 		fenetre;
	GC 		contexte_graphique;
	XEvent 		evenement;
	KeySym 		touche;
	XSizeHints 	infos;
	int 		ecran;
	unsigned long 	avant_plan,
			arr_plan;
	/* Fin des variables X-Window */

	/*** Prototypes ***/
	Fenetre();
	~Fenetre();
	Fenetre(int l, int h);
	Fenetre(int l, int h, const char *f_title);
	void MainLoop(void);
	void FinLoop(void);
	unsigned long ChargeCouleur(const char *str);
	void AjouteComposant(Composant *comp);
	int CheckEvenement(Evenement *evt);
	void Taille(Coord *f_taille);
	void ChgTitre(const char *f_title);
};

#endif
