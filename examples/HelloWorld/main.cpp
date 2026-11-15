/* Hello World avec la BadGFX
 * 
 * pour Login
 *
 * Par : Bad Max
 * Date: 24/03/2002
 */

#define MAIN
#include <badgfx.hpp>

// On declare la fenetre en premier
Fenetre Fen1(200, 80, "Hello World");

// Les fonctions appelées par les boutons
void HelloWorld(Composant *B);
void CliquezIci(Composant *B);
void FermerApp(Composant *B);

// Au travail
int main(void)
{
	// Cette structure est utilisée pour indiquer la position du bouton
	Coord	pos;
	pos.x = 5;
	pos.y = 5;

	// On créé le bouton intitulé "Cliquez içi"
	// Il référence la fenêtre créée auparavant
	Bouton Hello(&Fen1, &pos, "Cliquez içi", NORMAL);

	// On définit l'action du bouton
	Hello.ChgAction(HelloWorld);

	// On enregistre le composant auprès de la fenetre
	Fen1.AjouteComposant((void *) &Hello);

	// On créé un bouton pour quitter l'application
	pos.x = 5;
	pos.y = 30;
	Bouton Quitter(&Fen1, &pos, "Quitter", NORMAL);

	// On enregistre le composant
	Fen1.AjouteComposant((void *) &Quitter);

	// On définit son action
	Quitter.ChgAction(FermerApp);

	// Affichage du tout
	Fen1.MainLoop();

	return 1;
}

void HelloWorld(Composant *B)
{
	((Bouton *)B)->ChgTexte("Hello World");
	((Bouton *)B)->ChgAction(CliquezIci);
	B->Rafraichir();
}

void CliquezIci(Composant *B)
{
	((Bouton *)B)->ChgTexte("Cliquez içi");
	((Bouton *)B)->ChgAction(HelloWorld);
	B->Rafraichir();
}

void FermerApp(Composant *B)
{
	Fen1.FinLoop();
}
