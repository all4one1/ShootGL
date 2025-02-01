#pragma once
#include "Quad.h"
#include <iomanip>
#include <fstream>
struct Tree
{
	double Lx, Ly;
	double Hx, Hy; //half-size of a square 
	int Mx, My, MN; // numbers of squares
	double test_var_tree;
	int *quad_id;
	int *max_level;
	int current_max = 1;

	Quad *root;

	//Lx, Ly, Mx, My
	Tree(double Lx_, double Ly_, int Mx_, int My_, int max = 1, double Lx0 = 0, double Ly0 = 0)
	{
		test_var_tree = 1.234;
		quad_id = new int;
		max_level = new int;
		quad_id[0] = -1;
		max_level[0] = max;
		Lx = Lx_ - Lx0;
		Ly = Ly_ - Ly0;
		Mx = Mx_;
		My = My_;
		MN = Mx*My;
		Hx = Lx / Mx * 0.5;
		Hy = Ly / My * 0.5;


		root = new Quad[MN];

		for (int j = 0; j < My; j++) {
			for (int i = 0; i < Mx; i++) {

				double x = (2 * i + 1)*Hx + Lx0;
				double y = (2 * j + 1)*Hy + Ly0;

				root[i + Mx*j].init_quad(x, y, 1, Hx, Hy, quad_id, max_level);
			}
		}
	}

#define TreeLoop(tree) for (int tree = 0; tree < MN; tree++)

	void insert_one() {
		TreeLoop(i)
			root[i].insert_one();
	}
	void remove() {
		TreeLoop(i)
			root[i].remove();
	}
	void remove_all() {
		//????
	}


	void draw_root() {
		TreeLoop(i)
			root[i].draw_quad();
	}

	void immerse() {
		TreeLoop(i) {
			//int qy = i / Mx;
			//int qx = i - qy*Mx;
			root[i].immerse();
		}
	}

	void immerse_all() {
		TreeLoop(i) {
			//int qy = i / Mx;
			//int qx = i - qy*Mx;
			root[i].immerse_all();
		}
	}

	int check_max_level() {
		int Lmax = 1;
		TreeLoop(i)
		{
			int L_ = root[i].check_max_level();
			if (L_ > Lmax)
				Lmax = L_;
		}
		return Lmax;
	}

	int count()
	{
		int N_ = 0;
		TreeLoop(i)
			root[i].count(N_);
		return N_;
	}

	void find_solution() 
	{
		TreeLoop(i)
		{
			if (i % 10 == 0 || i == MN - 1) 
			{
				std::cout << "\033[1A\033[2K";
				cout << fixed << setprecision(1) << "Top-level quads covered: " << 100.0 * (i + 1) / MN << " %" << endl;
			}
			root[i].find_solution();
		}
	}

	void find_deepest() {
		TreeLoop(i) {
			int deepest_level = 1;
			double x, y;
			root[i].find_deepest(deepest_level, x, y);
			cout << setprecision(10);
			if (deepest_level > 5)
			{
				std::cout << "Root[" << i << "],level: " << deepest_level << " H: " << sqrt(Hx / pow(2, deepest_level) * Hy / pow(2, deepest_level)) << ", X,Y:  " << x << " " << y << std::endl;
			}
		}
	}

	void find_deepest_v2(std::vector<double> extra = {0}) {
		TreeLoop(i) {
			int deepest_level = 1;
			double x, y;
			root[i].find_deepest(deepest_level, x, y);
			cout << setprecision(10);
			if (deepest_level > 8)
			{
				std::cout << "v2, Root[" << i << "],level: " << deepest_level << " H: " << sqrt(Hx / pow(2, deepest_level) * Hy / pow(2, deepest_level)) << ", X,Y:  " << x << " " << y << std::endl;
				std::ofstream wm("deepest_mon.dat", std::ios::app);
				std::ofstream wo("deepest_osc.dat", std::ios::app);

				std::ofstream& file = (std::abs(y) < 1e-5) ? wm : wo;

				file << x << " " << y << " ";

				for (auto it : extra)
					file << it << " ";

				file << deepest_level << endl;
			}
		}
	}

	void draw_rude_intersections()
	{
		TreeLoop(i) {
			root[i].draw_rude_intersections();
		}
	}

};


