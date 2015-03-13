#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Serveur.cpp"
#include "Emplacement.cpp"
#include "Groupe.cpp"
#include <stdlib.h>
#include <time.h> 
#define MAP
using namespace std;
vector<Serveur*> servOrdreInit, servOrdreRend;
vector<vector<Emplacement> > tab;
vector<Groupe> listGroupes;
vector<int> capacite;
int capaciteTotale = 0;
int R, S, U, P, M;

void afficheBaies(vector<vector<Emplacement> > & baies)
{
//cout << "affiche" <<endl;
//cout << baies.size() << endl;
	for(int i =0; i < baies.size(); i ++)
	{
	cout << "ligne " << i <<": ";
		for(int j = 0; j < baies[i].size(); j ++)
		{
			if(baies[i][j].serv != NULL)
			{
				cout << baies[i][j].serv->indice <<" ";
			}
			else if(baies[i][j].etat == Emplacement::LIBRE)
			{
				cout << "v ";
			}
			else
			{
				cout <<"x ";
			}
		}
		cout << endl;
	}


}

void afficherGroupe()
{
	for(int i = 0; i < P; i ++)
	{
	cout << "Groupe "<< i <<endl;
		for(int j=0; j < R; j++)
		{
			cout <<"capacité rangée "<<j<<": "<<listGroupes[i].capacitesSurLigne[j] <<endl;
			cout <<"nb serv :"<<listGroupes[i].nbserv[j]<<endl;
		}
	}


}

void chargeMin()
{
 int GlobalMin = 10000;
 //sur tous les groupes
 for(int i = 0; i < P; i ++)
 {
  cout << "GROUPE "<< i ;
  //on enleve le max du total
  int total = 0;
  int max = 0;
  for(int j=0; j < R; j++)
  {
   total+=listGroupes[i].capacitesSurLigne[j];
   if(listGroupes[i].capacitesSurLigne[j] > max)
    max = listGroupes[i].capacitesSurLigne[j];
  }
  //la ligne max crash
  int capacMin = total-max;
  if (capacMin < GlobalMin)
  {
   GlobalMin = capacMin;
  }
  cout << " : "<< capacMin << endl;;
 }
 cout <<"Score : " << GlobalMin;
}

// FN SORT des serveurs par rendement
bool sortByRend(Serveur* s1, Serveur* s2)
{
//return true si il faut S1 passe avant S2
if(s1->rendement > s2->rendement)
{
return true;
}
else if (s1->rendement == s2->rendement)
// si rendement egal on prend le plus petit
{
if(s1->taille < s2->taille)
{
return true;
}
else
{
return false;
}
}
else
{
return false;
}
}
// INIT tous les var globaux => lecture du fichier
void init(const string& s)
{
ifstream file;
file.open(s);
int line;
if (!file.good())
{
cout << "ERREUR LECTURE" << endl;
return;
}
// Premiere ligne
file >> R;
//R = stoi(line);
file >> S;
//S = stoi(line);
file >> U;
//U = stoi(line);
file >> P;
//P = stoi(line);
file >> M;
//M = stoi(line);
// Création tableaux
for (int i = 0; i < R; ++i)
{
vector<Emplacement> v;
tab.push_back(v);
for (int j = 0; j<S; ++j)
{
Emplacement e;
tab[i].push_back(e);

}
}
// Emplacements indispo
int line2;
int r, e;
for (int i = 0; i < U; ++i)
{
file >> r;
file >> e;
//r = stoi(line);
//e = stoi(line2);
tab[r][e].etat = Emplacement::INDISPO;
}
// Lecture des serveurs (remplissage des tableaux par ordre d'ajout)
for(int i = 0; i < M; ++i)
{
file >> line;
file >> line2;
servOrdreInit.push_back(new Serveur(line,line2,i));
}
file.close();
// Remplissage des tableaux triés (copie & tri)
servOrdreRend.resize(M);
copy(servOrdreInit.begin(), servOrdreInit.end(), servOrdreRend.begin());
sort(servOrdreRend.begin(), servOrdreRend.end(), sortByRend);
for (int i = 0; i < P; ++i)
{
Groupe gr(R);
listGroupes.push_back(gr);
}
}
bool verifRangee(vector<Emplacement>& rangee, int taille_s);
int rechercheCase(vector<Emplacement>& barre, Serveur* serv);
int insererServ( vector<Emplacement>& barre,int emplacement, Serveur* serv)
{
	
	for(int i = 0; i < serv->taille; i++)
	{
		barre[emplacement+i].etat = Emplacement::PRIS;
		barre[emplacement+i].serv = serv;
		
	}
	capaciteTotale += serv->capacite;
	serv->emplacement = emplacement;
	return 0;
}
int rechercheEmplacement(vector<vector<Emplacement>> & barres, Serveur* servIns)
{
vector<Emplacement> b_min = barres[0];
int taille_s = servIns->taille;
int val_min = *min_element(capacite.begin(),capacite.end());
bool trouve = false;
int inser;
for(int i = 0; i < barres.size(); i++)
{
//cout <<"rangee " << i <<endl;
	if(capacite[i] == val_min && verifRangee(barres[i],taille_s))
	{
		inser = i;
		trouve = true;
		break;
	}
}

if(trouve == true )
{
	int emplacement= rechercheCase(barres[inser],servIns);
	//cout <<"emplacement " << emplacement <<endl;
	insererServ(barres[inser],emplacement,servIns);
	servIns->rangee = inser;
	capacite[inser] += servIns->capacite;
	
}
//cout <<endl;
return 0;
}
int rechercheCase(vector<Emplacement>& barre, Serveur* serv)
{
int taille_tmp = 0;
//int t_ret = 0;
int t_min=-1;
int ind_min=-1;
int i;
for(i = 0; i < barre.size(); i++)
{
	if(barre[i].etat == Emplacement::LIBRE) //trou
	{
		taille_tmp++;
		if (taille_tmp >= serv->taille)
		{
			return i - serv->taille + 1;
		}
	}
	else
	{
		taille_tmp = 0;
	}
}

return ind_min;
}
//renvoie la capacité ou -1 si insertion pas possible
bool verifRangee(vector<Emplacement>& rangee, int taille_s)
{
int taille_max=0;
int taille_tmp =0;
//calcul de taille maximum ou on peut inserer
for(int i = 0; i < rangee.size(); i++)
{
if(rangee[i].etat == Emplacement::LIBRE) //trou
{
	taille_tmp ++;
}
else
{
	if(taille_max < taille_tmp)
	{
		taille_max = taille_tmp;
	}
	taille_tmp = 0;
}
}
if(taille_max < taille_tmp)
{
	taille_max = taille_tmp;
}
if(taille_max < taille_s)
{
	//cout << "insertion pas ok" <<endl;
	return false; //pas bon
}
else
{
//cout <<"insertion ok" <<endl;
return true;
}
}
int nbServDuGroup(int groupe)
{
	int sum = 0;
	for (std::vector<int>::iterator it = listGroupes[groupe].nbserv.begin(); it != listGroupes[groupe].nbserv.end(); ++it)
	{
		sum+=*it;
	}
	return sum;
}


float rendGroup(int groupe)
{
	int nbserv;
	if((nbserv=nbServDuGroup(groupe)) == 0)
	{
		return 0.f;
	}
	float total = 0;
	for(int j=0; j < R; j++)
	{
		total+=listGroupes[groupe].capacitesSurLigne[j];
	}
	return total/float(nbServDuGroup(groupe));
}
// Renvoie le num du groupe avec le min sur cette ligne
int groupeMinLigne(int ligne, int servDejaGrouppe)
{
	int groupeMin = 0;
	//int minActuel = listGroupes[0].capacitesSurLigne[ligne];
	float minActuel = rendGroup(0);
	float min;
	for (int i = 0; i < P; ++i)//sur tout les groupes
	{
		//min = listGroupes[i].capacitesSurLigne[ligne];
		min = rendGroup(i);
		cout << "groupe " << i <<" nbserv " << float(nbServDuGroup(i)) << " rendement: "<<min<<endl;
		/*if (float(nbServDuGroup(i)) <= float(servDejaGrouppe)/P)//si inferieur a la moyenne insertion prioritaire
		{*/
			
			if (min < minActuel)
			{
				//on met le nouveau min
				minActuel = min;
				groupeMin = i;
			}
		//}
	//cout << "min "<<minActuel << endl;
		
	}
	cout << "gr min " << groupeMin <<endl;
	//cout <<"capacite :" << listGroupes[groupeMin].capacitesSurLigne[ligne]<<endl;
	system("sleep 10");
	return groupeMin;
}
void repartitionGroupe()
{
//pour chaque serv
//for (vector<Serveur*>::iterator i =servOrdreInit != servOrdreInit.end(); ++i)

int cpt = 0;
int k = 0;
//ATTENTION: on considere que le nombre de serveur positionné est > au double du nombre de groupe
// sinon la capacité minimale est 0 car les groupes n'auront que 0 ou 1 serv
// BOUCLE INFINIE MAGGLE
while(cpt < 2*P)
{
	if (servOrdreRend[k]->rangee != -1 && listGroupes[cpt%P].nbserv[ servOrdreRend[k]->rangee ] == 0)
	{
		servOrdreRend[k]->groupe = cpt%P;
		listGroupes[cpt%P].capacitesSurLigne[servOrdreRend[k]->rangee] += servOrdreRend[k]->capacite;
		listGroupes[cpt%P].nbserv[servOrdreRend[k]->rangee]++;
		cpt++;
	}
	k++;
	k = k%M;
}
for (int i = 2*P; i < servOrdreInit.size(); ++i)
{
	//cout <<"serveur "<< servOrdreInit[i]->indice << endl;
//afficherGroupe();
	if (servOrdreRend[i]->rangee != -1)
	{
		// Quelle est le groupe avec le min sur cette ligne ?
		int legroupe = groupeMinLigne(servOrdreInit[i]->rangee, i);
		// Ajoute à celui-ci
		servOrdreRend[i]->groupe = legroupe;
		listGroupes[legroupe].capacitesSurLigne[servOrdreRend[i]->rangee] += servOrdreRend[i]->capacite;
		listGroupes[legroupe].nbserv[servOrdreRend[i]->rangee]++;
		//cout <<"capacite ajoutee "<< servOrdreInit[i]->capacite <<" en rangee "<< servOrdreInit[i]->rangee  <<endl;
	}
	//cout << endl <<endl;
}
}
void printOutFile()
{
ofstream file;
file.open("result.out.txt", ios::out | ios::trunc);
for(unsigned int i = 0; i < servOrdreInit.size(); ++i)
{
if (servOrdreInit[i]->groupe == -1)
{
file << "x" << endl;
}
else
{
file << servOrdreInit[i]->rangee << " " << servOrdreInit[i]->emplacement << " " << servOrdreInit[i]->groupe << endl;
}
}
file.close();
}

void repartitionRand()
{
int gr = 0;
int flag = 1;
for (int i = 0; i < servOrdreInit.size(); ++i)
{
	//cout <<"serveur "<< servOrdreInit[i]->indice << endl;
//afficherGroupe();
	if (servOrdreRend[i]->rangee != -1)
	{
		// Quelle est le groupe avec le min sur cette ligne ?
		// Ajoute à celui-ci
		servOrdreRend[i]->groupe = gr;
		listGroupes[gr].capacitesSurLigne[servOrdreRend[i]->rangee] += servOrdreRend[i]->capacite;
		listGroupes[gr].nbserv[servOrdreRend[i]->rangee]++;
		//cout <<"capacite ajoutee "<< servOrdreInit[i]->capacite <<" en rangee "<< servOrdreInit[i]->rangee  <<endl;
		gr+=flag;
		gr = gr%P;
	}
	//cout << endl <<endl;
}


}


int main(int argc, char** argv)
{
init(argv[1]);
capacite = vector<int> (tab.size(),0);
//for(int i = 0; i < capacite.size(); i++)
//cout << capacite[i] << endl;
afficheBaies(tab);
for (unsigned int i = 0; i < servOrdreRend.size(); ++i)
{
//cout << servOrdreRend[i]->rendement <<"\t" <<servOrdreRend[i]->taille << endl;
//cout << "serveur " <<servOrdreRend[i]->indice << "de taille" << servOrdreRend[i]->taille <<endl;
rechercheEmplacement(tab, servOrdreRend[i]);

//afficheBaies(tab);

//cout << endl;
}
//repartitionGroupe();
repartitionRand();
cout << endl;
afficherGroupe();
afficheBaies(tab);
chargeMin();
cout << endl << "capacitetotale "<<capaciteTotale;
printOutFile();
return 0;
}