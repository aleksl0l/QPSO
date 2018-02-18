#include "Swarm_quantum.h"

Swarm_quantum::Swarm_quantum(double _w, 
	double _fp, 
	double _fg, 
	double _b, 
	int _dim, 
	int _n_part, 
	double _x_min, 
	double _x_max,
	int n_iter) : w(_w), fp(_fp), fg(_fg), b(_b), dim(_dim), num_swarm(_n_part), x_min(_x_min), x_max(_x_max), num_iter(n_iter)
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
			v[j] = 0.;
		swarm[i].setv(v);
	}
	iter = 0;
}

void Swarm_quantum::next_step()
{
	random_device rd;
	mt19937 gen(rd());

	

	b = 0.6 + (double)(num_iter - iter)*(1.1 - 0.6) / (double)num_iter;
	//iter++;
	//vector<double> c(dim, 0);
	//for (int i = 0; i < num_swarm; ++i)
	//{
	//	for (int j = 0; j < dim; ++j)
	//	{
	//		c[j] += swarm[i].getp()[j];
	//	}
	//}
	//for (int i = 0; i < dim; ++i)
	//	c[i] /= num_swarm;

	iter++;
	for (int j = 0; j < num_swarm; ++j) //для всех частиц
	{
		//vector<double> r(dim);
		//for (int l = 0; l < dim; ++l)
		//	r[l] = generate_canonical<double, 10>(gen);

		
		double r, fi;
		vector<double> t(dim), Ql(dim), x_new(dim);
		for (int i = 0; i < dim; ++i)
		{
			r = generate_canonical<double, 10>(gen);
			
			int sig = ((rand() % 2) == 0) ? 1 : (-1);
			fi = generate_canonical<double, 10>(gen);
			t[i] = fi*swarm[j].getp()[i] + (1 - fi)*getBest()[i];
			//t[i] = (fp*swarm[j].getp()[i] + fg*getBest()[i]) / (fp + fg);			
			Ql[i] = sig*b*fabs(swarm[j].getx()[i] - t[i]);
			x_new[i] = t[i] + Ql[i] * log(1 / r);

			
			//x_new[i] = t[i] + (double)sig*fabs(c[i] - swarm[j].getx()[i])*log(1 / r);
		}
		swarm[j].setx(x_new);

		if (val_fun_g > swarm[j].get_val_p())
		{
			g = swarm[j].getp();
			val_fun_g = f(g);
		}
	}
}