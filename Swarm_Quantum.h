#ifndef SWARM_QUANTUM_H
#define SWARM_QUANTUM_H
#include <random>
#include "particle.h"

using namespace std;


class Swarm_quantum
{
private:
	vector<Particle> swarm;
	const int num_swarm;
	const int dim;
	double w, fp, fg, b;	//	
	vector<double> g; //the best vector
	double val_fun_g;
	double x_min, x_max;
	int num_iter;
	int iter;

public:
	Swarm_quantum(double _w, double _fp, double _fg, double _b, int _dim, int _n_part, double _x_min, double _x_max, int n_iter);
	void next_step();
	vector<double> getBest() { return g; }
	double get_val_best() { return val_fun_g; }

};

#endif // SWARM_QUANTUM_H
