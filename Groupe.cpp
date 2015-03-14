
#ifndef GROUPE_H
#define	GROUPE_H
#include <vector>
using namespace std;
class Groupe {
public:
	vector<int> capacitesSurLigne;
	vector<int> nbserv;
	Groupe(int nbR)
	{
		for (int i = 0; i < nbR; ++i)
		{
			capacitesSurLigne.push_back(0);
			nbserv.push_back(0);
		}
	}
	virtual ~Groupe(){}
private:
};
#endif	/* GROUPE_H */