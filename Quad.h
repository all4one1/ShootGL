#pragma once
#include "GL\freeglut.h"
#include <cmath>
#include "Shooting.h"
using std::cout;
using std::endl;

enum kid_name 
{
	botLeft,
	botRight,
	topLeft,
	topRight
};

struct Point {
	double x, y;
	Point(double xx, double yy) : x(xx), y(yy)
	{
	}

};



typedef complex<double> cd;
typedef complex<int> ci;
void system2(cd &d, double Ra, double Im);


#define EPS_QUAD 1e-6

struct Quad
{
	double x, y, hx, hy;
	double f;
	int level, id, hasKids;
	int *quad_num;
	int *max_level;
	int *current_max;

#define KidLoop(kid) for (int kid = 0; kid < 4; kid++)
	Quad *child[4];

	Quad() : quad_num() {};

	Quad(double x_, double y_, int level_, double hx_, double hy_, int *num, int *max)
	{
		init_quad(x_, y_, level_, hx_, hy_, num, max);
	};

	void init_quad(double x_, double y_, int level_, double hx_, double hy_, int *num, int *max)
	{
		KidLoop(i)
			child[i] = NULL;

		x = x_;
		y = y_;
		level = level_;
		hx = hx_;
		hy = hy_;

		hasKids = 0;

		quad_num = num;
		max_level = max;
		quad_num[0]++;
		id = quad_num[0];

		//if (id % 10000 == 0) cout << "Id: " << id << ", level: " << level << endl;
	}




	void draw_quad() {
		GLdouble R = 0.0;
		GLdouble G = 0.5;// +1.0*level / max_level;
		GLdouble B = 0.1;
		glColor3d(R, G, B);

		glBegin(GL_LINE_LOOP);

		glVertex3d(x - hx, y - hy, 0);
		glVertex3d(x + hx, y - hy, 0);
		glVertex3d(x + hx, y + hy, 0);
		glVertex3d(x - hx, y + hy, 0);
		glEnd();
		//glTranslatef(x, y, 0);
		//glutSolidSphere(h*0.05, 15, 15);
		//glLoadIdentity();

		if (hasKids)
		{
			KidLoop(i)
				child[i]->draw_quad();
		}
	}

	void insert() {

		if (level < max_level[0]) {

			child[botLeft] = new Quad(x - 0.5*hx, y - 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[botLeft]->insert();

			child[botRight] = new Quad(x + 0.5*hx, y - 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[botRight]->insert();

			child[topLeft] = new Quad(x - 0.5*hx, y + 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[topLeft]->insert();

			child[topRight] = new Quad(x + 0.5*hx, y + 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[topRight]->insert();
			hasKids = 1;
		}
	}


	void insert_one() {
		if (level >= max_level[0]) return;

		if (hasKids) {
			KidLoop(i)
				child[i]->insert_one();
		}

		if (!hasKids) {
			child[botLeft] = new Quad(x - 0.5*hx, y - 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[botRight] = new Quad(x + 0.5*hx, y - 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[topLeft] = new Quad(x - 0.5*hx, y + 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			child[topRight] = new Quad(x + 0.5*hx, y + 0.5*hy, level + 1, 0.5*hx, 0.5*hy, quad_num, max_level);
			hasKids = 1;
		}


	}

	void remove()
	{
		if (hasKids) {
			KidLoop(i)
			{
				if (!child[i]->hasKids) {
					hasKids = 0;
					delete child[i];
				}

			}

			KidLoop(i) 				child[i]->remove();

		}
	}


	void create_quads(double x, double y, int level_, double hx_, double hy_, int *num, int *max)
	{
		child[botLeft] = new Quad(x - 0.5*hx, y - 0.5*hy, level_ + 1, 0.5*hx_, 0.5*hy_, num, max);
		child[botLeft]->test_func();
		child[botRight] = new Quad(x + 0.5*hx, y - 0.5*hy, level_ + 1, 0.5*hx_, 0.5*hy_, num, max);
		child[botRight]->test_func();
		child[topLeft] = new Quad(x - 0.5*hx, y + 0.5*hy, level_ + 1, 0.5*hx_, 0.5*hy_, num, max);
		child[topLeft]->test_func();
		child[topRight] = new Quad(x + 0.5*hx, y + 0.5*hy, level_ + 1, 0.5*hx_, 0.5*hy_, num, max);
		child[topRight]->test_func();
		hasKids = 1;
	}



	double test_func(double x, double y, double delta = 0.1)
	{
		//return 0.5 - 0.5*tanh((y - 0.5 - 0.1*cos(4.0*Pi*x / Lx) - 0.0*(x-0.5)) / delta);
		return 0.5 + 0.5*tanh((sqrt(pow(x - 0.5, 2) + pow(y - 0.5, 2)) - 0.1) / delta);
	}

	void test_func()
	{
		f = test_func(x, y);
	}


	void immerse()
	{

		if (level >= max_level[0]) return;

		if (hasKids)
		{
			KidLoop(i)
				child[i]->immerse();
		}

		if (!hasKids)
		{
			if (abs(f - 0.5)<0.2)
			{
				create_quads(x, y, level, hx, hy, quad_num, max_level);
			}
		}
	}

	void immerse_one()
	{

		if (level >= max_level[0]) return;

		if (hasKids) immerse_one();

		if (!hasKids)
			create_quads(x, y, level, hx, hy, quad_num, max_level);
	}


	void immerse_all()
	{

		if (level >= max_level[0]) return;

		if (hasKids)
		{
			KidLoop(i)
				child[i]->immerse_all();
		}

		if (!hasKids)
		{
			//if (abs(f - 0.5)<0.05)
			{
				create_quads(x, y, level, hx, hy, quad_num, max_level);
			}
		}
	}

	int check_max_level()
	{
		if (!hasKids)
		{
			return level;
		}


		int L[4], Lmax = 1;
		if (hasKids)
		{
			KidLoop(i)
			{
				L[i] = child[i]->check_max_level();
				if (L[i] > Lmax)
					Lmax = L[i];
			}

		}

		return Lmax;
	}

	double check_something()
	{
		double L;
		if (hasKids)
		{
			KidLoop(i)
				L = child[i]->check_something();
		}


		if (!hasKids)
		{
			return 0.5*(hx+hy);
		}

		return L;
	}

	void count(int &N_)
	{
		if (hasKids) {
			KidLoop(i)
				child[i]->count(N_);
		}

		if (!hasKids)
			N_++;
	}

	//clicking
	void create_quad_by_click(double xx, double yy)
	{
		if (level >= max_level[0]) return;
		//std::cout << "level: " << level << std::endl;

		if (abs(x - xx) < hx && abs(y - yy) < hy) 
		{
			if (hasKids) {
				KidLoop(i)	child[i]->create_quad_by_click(xx, yy);
			}
			if (!hasKids) {

				create_quads(x, y, level, hx, hy, quad_num, max_level);
			}
			
		}
	}

	//clicking
	void find_by_click(double xx, double yy, Quad **target)
	{
		//if (level >= max_level[0]) return;
		//std::cout << "level: " << level << std::endl;

		if (abs(x - xx) < hx && abs(y - yy) < hy)
		{
			if (hasKids) {
				KidLoop(i)	child[i]->find_by_click(xx, yy, target);
			}
			if (!hasKids) {
				//cout << "Quad: " << id << " level: " << level  << " " << this << endl;
				*target = this;
			}

		}
	}

	void find_solution()
	{
		if (level >= max_level[0])
			return;


		if (!hasKids) {
			if (2 * hx < EPS_QUAD && 2 * hy < EPS_QUAD) return;

			int hasRoot = solve_eq();
			if (hasRoot) 
			{
				create_quads(x, y, level, hx, hy, quad_num, max_level);
				//cout << "level: " << level << " Root:" << x << " " << y << endl;
			}
		}

		if (hasKids) {
			KidLoop(i) child[i]->find_solution();
			//cout << this->id << endl;
		}
	}


	int solve_eq()
	{
		double F1, F2, F3, F4;
		double G1, G2, G3, G4;
		cd d1, d2, d3, d4;

		system2(d1, x - hx, y - hy);
		system2(d2, x + hx, y - hy);
		system2(d3, x - hx, y + hy);
		system2(d4, x + hx, y + hy);

		F1 = real(d1);	G1 = imag(d1);
		F2 = real(d2);	G2 = imag(d2);
		F3 = real(d3);	G3 = imag(d3);
		F4 = real(d4);	G4 = imag(d4);

		int hasRoot = 1;

		if ((F1 > 0 && F2 > 0 && F3 > 0 && F4 > 0) || (F1 < 0 && F2 < 0 && F3 < 0 && F4 < 0))
			hasRoot = 0;

		// f2
		if (hasRoot)
		{
			if ((G1 > 0 && G2 > 0 && G3 > 0 && G4 > 0) || (G1 < 0 && G2 < 0 && G3 < 0 && G4 < 0))
				hasRoot = 0;
		}
		return hasRoot;


		
		
	}




	void find_deepest(int &d, double &x_, double &y_)
	{
		if (hasKids) {
			KidLoop(i) child[i]->find_deepest(d, x_, y_);
		}

		if (!hasKids) {
			if (level > d) {
				d = level;
				x_ = x;
				y_ = y;
			}
		}

	}

	void draw_rude_intersections()
	{
		if (hasKids) {
			KidLoop(i) child[i]->draw_rude_intersections();
		}

		if (!hasKids) 
		{
			GLdouble R = 0.0;
			GLdouble G = 0.0;
			GLdouble B = 0.0;
			
			cd d;
			system2(d, x, y);
			double F1 = real(d);
			double G1 = imag(d);

			if (F1 > 0) R = 1;
			if (G1 > 0) B = 1;

			glColor3d(R, G, B);

			glBegin(GL_POLYGON);
			{
				glVertex3d(x - hx, y - hy, 0);
				glVertex3d(x + hx, y - hy, 0);
				glVertex3d(x + hx, y + hy, 0);
				glVertex3d(x - hx, y + hy, 0);

			}
			glEnd();
		}
	}



	int solve_eq_test()
	{
		Point p1(x - hx, y - hy);
		Point p2(x + hx, y - hy);
		Point p3(x - hx, y + hy);
		Point p4(x + hx, y + hy);

		int hasRoot = 1;

		// f1
		double F1 = f1(p1);
		double F2 = f1(p2);
		double F3 = f1(p3);
		double F4 = f1(p4);

		if ((F1 > 0 && F2 > 0 && F3 > 0 && F4 > 0) || (F1 < 0 && F2 < 0 && F3 < 0 && F4 < 0))
			hasRoot = 0;

		// f2
		if (hasRoot)
		{
			F1 = f2(p1);
			F2 = f2(p2);
			F3 = f2(p3);
			F4 = f2(p4);

			if ((F1 > 0 && F2 > 0 && F3 > 0 && F4 > 0) || (F1 < 0 && F2 < 0 && F3 < 0 && F4 < 0))
				hasRoot = 0;
		}
		return hasRoot;
	}
	double f1(double x, double y)
	{
		double R_f = sqrt(0.4);
		x = x;
		y = y;
		//return 1-2*x-y;
		return x*x + y*y - R_f*R_f;

	};
	double f2(double x, double y)
	{
		return 0.0001*x - y;
	};
	double f1(Point p) {
		return f1(p.x, p.y);
	}
	double f2(Point p) {
		return f2(p.x, p.y);
	}
};




