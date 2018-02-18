#ifndef SWARM_GBEST_H
#define SWARM_GBEST_H
#include <random>
#include "particle.h"

using namespace std;


class Swarm_gbest
{
private:
	vector<Particle> swarm;
	const int num_swarm;
	const int dim;
	double w, fp, fg;	//	
	vector<double> g; //the best vector
	double val_fun_g;
	double x_min, x_max;

public:
	Swarm_gbest(double, double, double, int, int, double, double);
	void next_step();
	vector<double> getBest() { return g; }
	double get_val_best() { return val_fun_g; }

};


#endif // SWARM_GBEST_H
