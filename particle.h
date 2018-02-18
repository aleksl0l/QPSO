#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <iostream>
#include <math.h>

using std::vector;
using std::cout;

inline double f(const vector<double> &x);

class Particle
{
private:
	vector<double> x;
	vector<double> p;
	double val_p;
	vector<double> v;
public:
	Particle(vector<double> _x);
	vector<double> getp();
	double get_val_p();
	vector<double> getx();
	void setx(vector<double> _x);
	void setv(vector<double> _v) { v = _v; }
	void update_v(double w,
		double fp,
		double fg,
		vector<double> &rp,
		vector<double> &rg,
		vector<double> &g);

	void move();
};


#endif // PARTICLE_H
