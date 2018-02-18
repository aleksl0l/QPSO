#ifndef SWARM_HPSO_H
#define SWARM_HPSOT_H
#include <random>
#include <algorithm>
#include "particle.h"

using namespace std;

class Swarm_HPSO
{
private:
	vector<Particle> swarm;
	const int num_swarm;
	const int dim;
	double w, fp, fg;	//	
						//vector <double> top;
	vector<vector<double>> lbest; //the best vector
	vector < vector <int> > num_branch;
	//double val_fun_g;
	double x_min, x_max;
	int d = 4;

public:
	Swarm_HPSO(double, double, double, int, int, double, double);
	void next_step();
	vector<double> getBest() { return lbest[0]; }
	double get_val_best() { return f(lbest[0]); }

};

#endif // SWARM_GBEST_H