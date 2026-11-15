/* Composante Bouton
 */

#ifndef BOUTONCLASS
#define BOUTONCLASS

class Bouton : public Composant
{
private:
	int	b_type;	/* Type de bouton */
	int	b_etat;	/* Etat du bouton */
	int	b_width;	/* Largeur */
	int	b_height;	/* Hauteur */
	char	b_title[24];	/* Intitulé du bouton */
	Coord	b_position;	/* Position du bouton */
	void Dessine_Bouton(void);
	void Dessine_InvertBouton(void);
	void (*Action)(Composant *C);	/* Action */

public:
	Bouton(Fenetre *fen, Coord *coords, const char *txt, int typ);
	~Bouton();
	void Rafraichir(void);
	int OnClic(Evenement *evt);
	int OnMove(Evenement *evt);
	int OnKey(Evenement *evt);
	void ChgAction(void (*fct)(Composant *C));
	int ChgTexte(const char *txt);
};

#endif
