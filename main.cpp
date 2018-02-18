#define _USE_MATH_DEFINES
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <fstream>

#include "Swarm_gbest.h"
#include "Swarm_quantum.h"
#include "Swarm_hpso.h"
using namespace std;

inline double f(const vector<double> &x)
{
	double s = 0.;
	//f6
	//int A = 10;
	//s = A*x.size();
	//for (int i = 0; i < x.size(); ++i)
	//	s += pow(x[i], 2) - A*cos(2 * M_PI*x[i]);
	
	//f1
	for (int i = 0; i < x.size(); ++i)
		s += x[i] * x[i];

	//f2
	//double p = 1.;
	//for (int i = 0; i < x.size(); ++i)
	//{
	//	s += fabs(x[i]);
	//	p *= fabs(x[i]);
	//}
	//s += p;

	//f3
	//for (int i = 0; i < x.size(); ++i)
	//{
	//	double quad = 0.0;
	//	for (int j = 0; j < i; ++j)
	//		quad += x[j];
	//	s += pow(quad, 2);
	//}

	//f4
	//for (int i = 0; i < x.size() - 1; ++i)
	//	s += pow(1 - x[i], 2) + 100 * pow(x[i + 1] - pow(x[i], 2), 2); //Розенброк

	//f5
	//for (int i = 0; i < x.size(); ++i)
	//	s += pow((int)(x[i] + 0.5), 2);

	return s;
}



int main(int argc, char *argv[])
{
	double w = 0.72984,
		fp = 1.496172,
		fg = fp,
		x_min = -100.0,
		x_max = 100.0;
	const int n = 30;
	int num_iter = 1000;
	double suc = 0.1;
	
	w = 0.72984, fp = 2.04355, fg = 0.94879;
	
	vector<double> gbest, hpso, qpso;

	vector<double> step_gbest(1000), step_hpso(1000), step_qpso(1000);

	for (int k = 0; k < 1; ++k)
	{
		Swarm_gbest sw_gbest(w, fp, fg, n, 40, x_min, x_max);
		Swarm_quantum sw_qpso(w, fp, fg, 0.75, n, 40, x_min, x_max, num_iter);
		Swarm_HPSO sw_hpso(w, fp, fg, n, 40, x_min, x_max);
		for (int i = 0; i < num_iter; ++i)
		{
			step_gbest[i] += sw_gbest.get_val_best();
			step_qpso[i] += sw_qpso.get_val_best();
			step_hpso[i] += sw_hpso.get_val_best();
			sw_gbest.next_step();
			sw_qpso.next_step();
			sw_hpso.next_step();
		}
		auto best = sw_gbest.getBest();
		//cout << k << endl <<	"Gbest: \t" << f(best) << endl;
		gbest.push_back(f(best));
		best = sw_qpso.getBest();
		//cout << "QPSO: \t" << f(best) << endl;
		qpso.push_back(f(best));
		best = sw_hpso.getBest();
		//cout << "HPSO: \t" << f(best) << endl << endl;
		hpso.push_back(f(best));
		if (k % 10)
			cout << k / 100. << endl;
	}

	for (int i = 0; i < 100; ++i)
	{
		step_gbest[i] /= 100.0;
		step_qpso[i] /= 100.0;
		step_hpso[i] /= 100.0;
	}
	
	ofstream fout("result.txt");

	for (int i = 0; i < num_iter; ++i)
		fout << i << '\t';
	fout << endl;
	for (int i = 0; i < num_iter; ++i)
		fout << step_gbest[i] << '\t';

	cout << endl << endl;

	double a_gbest, a_qpso, a_hpso, min_gbest, min_qpso, min_hpso,
		max_gbest, max_qpso, max_hpso, suc_gbest, suc_qpso, suc_hpso;

	a_gbest = a_qpso = a_hpso = 0.0;

	for (int i = 0; i < 100; ++i)
	{
		a_gbest += gbest[i];
		a_qpso += qpso[i];
		a_hpso += hpso[i];
	}

	a_gbest /= 100.;
	a_qpso /= 100.;
	a_hpso /= 100.;

	min_gbest = *std::min_element(gbest.begin(), gbest.end());
	min_qpso = *std::min_element(qpso.begin(), qpso.end());
	min_hpso = *std::min_element(hpso.begin(), hpso.end());

	max_gbest = *std::max_element(gbest.begin(), gbest.end());
	max_qpso = *std::max_element(qpso.begin(), qpso.end());
	max_hpso = *std::max_element(hpso.begin(), hpso.end());

	int count = std::count_if(gbest.begin(), gbest.end(), [suc](int x) {return x < suc; });
	suc_gbest = (double)count / 100.0;
	count = std::count_if(qpso.begin(), qpso.end(), [suc](int x) {return x < suc; });
	suc_qpso = (double)count / 100.0;
	count = std::count_if(hpso.begin(), hpso.end(), [suc](int x) {return x < suc; });
	suc_hpso = (double)count / 100.0;


	cout << "Average:" << endl
		<< "PSO-gbest: " << a_gbest << endl
		<< "QPSO: " << a_qpso << endl
		<< "H-PSO: " << a_hpso << endl << endl
		<< "Min:" << endl
		<< "PSO-gbest: " << min_gbest << endl
		<< "QPSO: " << min_qpso << endl
		<< "H-PSO: " << min_hpso << endl << endl
		<< "Max:" << endl
		<< "PSO-gbest: " << max_gbest << endl
		<< "QPSO: " << max_qpso << endl
		<< "H-PSO: " << max_hpso << endl << endl
		<< "Suc:" << endl
		<< "PSO-gbest: " << suc_gbest << endl
		<< "QPSO: " << suc_qpso << endl
		<< "H-PSO: " << suc_hpso << endl << endl;
	cout << endl;
	system("PAUSE");
	return 0;
}
