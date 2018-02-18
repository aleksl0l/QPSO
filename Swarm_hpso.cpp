#include "Swarm_hpso.h"


Swarm_HPSO::Swarm_HPSO(double _w,
	double _fp,
	double _fg,
	int _dim,
	int _n_part,
	double _x_min,
	double _x_max) : w(_w), fp(_fp), fg(_fg), dim(_dim), num_swarm(_n_part), x_min(_x_min), x_max(_x_max)
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

		swarm.push_back(Particle(tmp));

		vector<double> v(dim);
		for (int j = 0; j < dim; ++j)
			v[j] = 0.;
		swarm[i].setv(v);
	}

	sort(swarm.begin(), swarm.end(), [](Particle & a, Particle & b)->bool
	{
		return f(a.getx()) < f(b.getx());
	});
	lbest = vector< vector<double> >(d + 1);
	num_branch = vector<vector<int>>(d + 1);
	for (int i = 0; i < d + 1; ++i)
		num_branch[0].push_back(i);
	lbest[0] = swarm[0].getx();

	int n_branch = (num_swarm - 1 - d) / d;
	int n_begin = 1 + d;

	for (int i = 0; i < d; ++i)
		num_branch[i + 1].push_back(i + 1);
	for (int i = 0; i < d; ++i)
	{
		num_branch.push_back(vector<int>());
		for (int j = n_begin; j < n_begin + n_branch; ++j)
			num_branch[i + 1].push_back(j);
		lbest[i + 1] = swarm[n_begin].getx();
		n_begin += n_branch;
	}
	for (int i = n_begin; i < num_swarm; ++i)
		num_branch[1 + i%d].push_back(i);

}


void Swarm_HPSO::next_step()
{
	random_device rd;
	mt19937 gen(rd());
	for (int i = 0; i < d + 1; ++i)
	{
		for (int j = 0; j < num_branch[i].size(); ++j)
		{
			vector<double> rp(dim), rg(dim); //случайные вектора
			for (int l = 0; l < dim; ++l)
			{
				rp[l] = generate_canonical<double, 10>(gen);
				rg[l] = generate_canonical<double, 10>(gen);
			}
			swarm[num_branch[i][j]].update_v(w, fp, fg, rp, rg, lbest[i]);
			swarm[num_branch[i][j]].move();
			if (f(lbest[i]) > swarm[num_branch[i][j]].get_val_p()) //если лучше значение частицы лучше значения подветки
			{
				lbest[i] = swarm[num_branch[i][j]].getp();		//заменяем лучшее
				iter_swap(num_branch[i].begin(), num_branch[i].begin() + j); //меняем индекс лучшего и текущей частицы
				if (i == 0) //если это первый первый уровень
					num_branch[i + 1][0] = num_branch[i][j]; //заменить на уровне выше лучшую частицы на лучшую частицу первого уровня этого подведви
				else
				{
					if (f(lbest[i]) > f(lbest[0]))
					{
						iter_swap(lbest.begin(), lbest.begin() + i);
						int tmp = num_branch[0][0];
						num_branch[0][0] = num_branch[i][0];
						num_branch[0][i] = num_branch[i][0] = tmp;
					}
				}
			}
		}
	}
}