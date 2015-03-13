#ifndef SERVEUR_H
#define	SERVEUR_H
#include <iostream>
#include <stdlib.h>
using namespace std;
class Serveur {
public:
int taille;
int capacite;
int rangee;
int emplacement;
float rendement;
bool positionne;
int groupe;
int indice;
Serveur(int t, int cap, int i) : taille(t), capacite(cap)
{
indice = i;
rangee = -1;
emplacement = -1;
groupe = -1;
positionne = false;
rendement = (float)capacite / (float)taille;
cout << rendement << " = " << capacite << " / " << taille << endl;;
}
virtual ~Serveur(){}
private:
};
#endif	/* SERVEUR_H */