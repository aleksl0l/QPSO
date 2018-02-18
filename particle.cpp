#include "particle.h"

Particle::Particle(vector<double> _x)
{
	x = p = _x;
	val_p = f(p);
}
vector<double> Particle::getp()
{
	return p;
}

double Particle::get_val_p()
{
	return val_p;
}

void Particle::setx(vector<double> _x)
{
	x = _x;
	if (f(x) < val_p)
	{
		p = x;
		val_p = f(p);
	}
}

void Particle::update_v(double w,
	double fp,
	double fg,
	vector<double> &rp,
	vector<double> &rg,
	vector<double> &g)
{
	for (int i = 0; i < x.size(); ++i)
		v[i] = w*v[i] +
		fp * rp[i] * (p[i] - x[i]) +
		fg * rg[i] * (g[i] - x[i]);

}

void Particle::move()
{
	for (int i = 0; i < x.size(); ++i)
		x[i] += v[i];
	if (f(x) < val_p)
	{
		p = x;
		val_p = f(p);
	}
}

vector<double> Particle::getx()
{
	return x;
}