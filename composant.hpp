/*
 * Classe composant d'une classe fenetre
 *
 */

#ifndef CLASSCOMPOSANT
#define CLASSCOMPOSANT

class Fenetre;

class Composant
{
private:
public:
	Composant * suivant;
	Fenetre *f;

	/* Fonctions virtuelles */
	Composant();
	virtual ~Composant();
	void virtual Rafraichir(void)=0;
	int virtual OnClic(Evenement *evt)=0;
	int virtual OnMove(Evenement *evt)=0;
	int virtual OnKey(Evenement *evt)=0;
};

#endif

