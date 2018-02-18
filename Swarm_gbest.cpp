#include "Swarm_gbest.h"

Swarm_gbest::Swarm_gbest(double _w, 
	double _fp, 
	double _fg, 
	int _dim, 
	int _par, 
	double _x_min, 
	double _x_max) : w(_w), fp(_fp), fg(_fg), dim(_dim), num_swarm(_par), x_min(_x_min), x_max(_x_max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dist(x_min, x_max);
	uniform_real_distribution<double> vel(-fabs(x_max - x_min), fabs(x_max - x_min));

	for (int i = 0; i < num_swarm; ++i)
	{
		vector<double> tmp(dim);
		for (int j = 0; j < dim; ++j)
			tmp[j] = dist(gen);

		if (i == 0) // ничего лучше не придумал
		{
			g = tmp;
			val_fun_g = f(g);
		}
		else if (val_fun_g > f(tmp))
		{
			g = tmp;
			val_fun_g = f(g);
		}

		swarm.push_back(Particle(tmp));

		vector<double> v(dim);
		for (int j = 0; j < dim; ++j)
			v[j] = 0;
		swarm[i].setv(v);
	}
}

void Swarm_gbest::next_step()
{
	random_device rd;
	mt19937 gen(rd());
	for (int j = 0; j < num_swarm; ++j) //для всех частиц
	{
		vector<double> rp(dim), rg(dim);
		for (int l = 0; l < dim; ++l)
		{
			rp[l] = generate_canonical<double, 10>(gen);
			rg[l] = generate_canonical<double, 10>(gen);
		}
		swarm[j].update_v(w, fp, fg, rp, rg, g);
		swarm[j].move();
		if (val_fun_g > swarm[j].get_val_p())
		{
			g = swarm[j].getp();
			val_fun_g = f(g);
		}
	}
}