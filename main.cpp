#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>



#include "Serveur.cpp"
#include "Emplacement.cpp"
#include "Groupe.cpp"
#define MAP
using namespace std;

vector<Serveur*> servOrdreInit, servOrdreRend;
vector<vector<Emplacement> > tab;

vector<Groupe> listGroupes;

int R, S, U, P, M;

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
    
    string line;
    
    if (!file.good())
    {
        cout << "ERREUR LECTURE" << endl;
        return;
    }
    
    // Premiere ligne
    file >> line;
    R = stoi(line);
    file >> line;
    S = stoi(line);
    file >> line;
    U = stoi(line);
    file >> line;
    P = stoi(line);
    file >> line;
    M = stoi(line);
    
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
    string line2;
    int r, e;
    for (int i = 0; i < U; ++i)
    {
        file >> line;
        file >> line2;
        r = stoi(line);
        e = stoi(line2);
        tab[r][e].etat = Emplacement::INDISPO;
    }
    
    // Lecture des serveurs (remplissage des tableaux par ordre d'ajout)
    for(int i = 0; i < M; ++i)
    {
       file >> line;
       file >> line2;
       servOrdreInit.push_back(new Serveur(stoi(line), stoi(line2)));
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

int verifRangee(vector<Emplacement>& rangee, int taille_s);
int rechercheCase(vector<Emplacement> barre, Serveur* serv);

bool rechercheEmplacement(vector<vector<Emplacement>> & barres, Serveur* servIns)
{
	vector<Emplacement> b_min = barres[0];
	int taille_s = servIns->taille;
	int val_min = 1000;//verifRangee(b_min, taille_s);
	int val_tmp = 0;
        cout << "Val tmp " << val_tmp << endl;
	for(int it_col = 0; it_col < barres.size(); it_col++)
	{
		val_tmp = verifRangee(barres[it_col], taille_s);
                cout << "Val tmp " << val_tmp << endl;
                cout << "Val min " << val_min << endl;
		if(val_tmp < val_min && val_tmp >= 0) //si barre ok et plus petite capacite
		{
                    cout << "RENTRE" << endl;
			val_min = val_tmp;
			b_min = barres[it_col];
                        
                        servIns->rangee = it_col;
                        
		}
	}
            #ifdef MAP
                cout << "insertion en rangee " << servIns->rangee << endl;
                #endif
	if(val_min < 0)
	{
		#ifdef MAP
			cout << "pas d'emplacement dispo" << endl;
		#endif
		return false;
	}
	int indice = rechercheCase(b_min, servIns);
	#ifdef MAP
		cout << "insertion en empl " << indice << endl;
	#endif
	b_min[indice].serv = servIns;
        servIns->emplacement = indice;
        
	for(int i = 0; i < servIns->taille; i++)
	{
		b_min[indice+i].etat = Emplacement::PRIS;
                b_min[indice+i].serv = servIns;
	}
	return true;
}



int rechercheCase(vector<Emplacement> barre, Serveur* serv)
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
                        if (taille_tmp == serv->taille)
                        {
                            return i - serv->taille + 1;
		
                        }        
                }
		else
		{
                    taille_tmp = 0;
                    /*
                    if(t_min < 0 && taille_tmp >= serv->taille)
                    {
                            t_min = serv->taille;
                            ind_min = i - serv->taille;
                    }
                    else
                    {
                        if(taille_tmp >= serv->taille && taille_tmp < t_min)
                        {
                                t_min = serv->taille;
                                ind_min = i - serv->taille;	
                        }
                    }*/
		}
	}

	//si la dernière case est disponible on doit mettre à jour les valeurs quand même
	/*if(t_min < 0 && taille_tmp >= serv->taille)
	{
		t_min = serv->taille;
		ind_min = i - serv->taille;
	}
	else
	{
		if(taille_tmp >= serv->taille && taille_tmp < t_min)
		{
			t_min = serv->taille;
			ind_min = i - serv->taille;	
		}
	}
*/
	return ind_min;

}


//renvoie la capacité ou -1 si insertion pas possible
int verifRangee(vector<Emplacement>& rangee, int taille_s)
{
	int capacite=0;
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
				taille_tmp = 0;	
			}
		}
	}
	if(taille_max < taille_tmp)
	{
		taille_max = taille_tmp;
		taille_tmp = 0;	
	}

	if(taille_max < taille_s)
	{
		return -1; //pas bon
	}

	//Capacité
	for(int i = 0; i < rangee.size();)
	{
		if(rangee[i].serv != NULL) //serveur dispo
		{
			capacite += rangee[i].serv->capacite;
			i+=rangee[i].serv->taille;
		}
		else
		{
			i++;
		}
	}

	return capacite;


}


// Renvoie le num du groupe avec le min sur cette ligne
int groupeMinLigne(int ligne)
{
	int groupeMin = 0;
	int minActuel = listGroupes[0].capacitesSurLigne[ligne];

	for (int i = 1; i < P; ++i)//sur tout les groupes
	{
		int min = listGroupes[i].capacitesSurLigne[ligne];
		if (min < minActuel)
		{
			//on met le nouveau min
			minActuel = min;
			groupeMin = i;
		}
	}
	return groupeMin;
}


void repartitionGroupe()
{
	//pour chaque serv
	//for (vector<Serveur*>::iterator i =servOrdreInit != servOrdreInit.end(); ++i)
            for (int i = 0; i < servOrdreInit.size(); ++i)
        {
		if (servOrdreInit[i]->rangee != -1)
		{
			// Quelle est le groupe avec le min sur cette ligne ?
			int legroupe = groupeMinLigne(servOrdreInit[i]->rangee);
			// Ajoute à celui-ci
			servOrdreInit[i]->groupe = legroupe;
			listGroupes[legroupe].capacitesSurLigne[servOrdreInit[i]->rangee] += servOrdreInit[i]->capacite;
		}	
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







int main(int argc, char** argv)
{
    init(argv[1]);
    
    for (unsigned int i = 0; i < servOrdreRend.size(); ++i)
    {
        //cout << servOrdreRend[i]->rendement <<"\t" <<servOrdreRend[i]->taille <<  endl;
        rechercheEmplacement(tab, servOrdreRend[i]);
    }
    
    
    repartitionGroupe();
    printOutFile();
    
    
    return 0;
}

