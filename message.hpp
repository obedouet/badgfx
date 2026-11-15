/* Composant boite à messages
 */

#ifndef CLASSMESSAGEBOX
#define CLASSMESSAGEBOX

class MessageBox : public Composant
{
private:
	int	size_msg,	/* Taille des messages affiches */
		hist;		/* Taille de l'historique */
	char	**old_str;
	int	num,
		zone_msg_x,	/* Position de la fenetre */
		zone_msg_y,
		zone_msg_h,
		zone_msg_w;
	void Init();	// Fonction d'initialisation

public:
	~MessageBox();
	MessageBox(Fenetre *fen);
	MessageBox(Fenetre *fen, int msg_hist);
	void EcritMessage(const char *str);
	void Rafraichir();
	int OnClic(Evenement *evt);
	int OnMove(Evenement *evt);
	int OnKey(Evenement *evt);

};

#endif

