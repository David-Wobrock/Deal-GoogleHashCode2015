#ifndef EMPLACEMENT_H
#define	EMPLACEMENT_H
#include "Serveur.cpp"
#include <stdlib.h>
class Emplacement {
public:
static const int LIBRE = 0;
static const int INDISPO = 1;
static const int PRIS = 2;
Serveur* serv;
int etat;
Emplacement()
{
etat = LIBRE;
serv = NULL;
}
virtual ~Emplacement(){}
private:
};
#endif	/* EMPLACEMENT_H */
