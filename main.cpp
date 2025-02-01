#include <stdio.h>
#include <omp.h>
#include <fstream>
#include <ctime>
#include <iostream> 
#include <vector>
#include "windows.h"
#include <iomanip>
#include <random>
#include "GL\freeglut.h"
//#include "Quad.h"


using namespace std;
#define pause cout << "Pause at line " << __LINE__ << endl; system("pause"); 
#define timer timer2 = clock()/CLOCKS_PER_SEC; 	cout << "time_computed (seconds)= " << (timer2 - timer1) << endl;
#include "Tree.h"
#include "Shooting.h"
bool restart = false;


int height, width;
double timeq = 0;
int iter = 0;
unsigned int timer1, timer2;
double Lx, Ly, Lz;
double Lx0 = 0, Ly0 = 0, Lz0 = 0;
double X, Y;


//Tree parameters
int mx, my, MN;
int max_level; 
Tree *Tree1; 
Shooting_Method *SM;

void GL_loop(int argc, char **argv);

double *k_; // wave number

void system2(cd &d, double Ra, double Im)
{
	SM->ssystem(d, Ra, Im);
}


int main(int argc, char **argv)
{
	//set 1 for the console only mode
	omp_set_num_threads(2);

	//determinant
	cd d; 

	//Space division
	mx = my = 32*1;

	// level of the space division
	max_level = 20;
	
	//Lx = Ra 
	Lx0 =  5000;  
	Lx =  15000;

	//Ly = Omega (frequency)
	Ly0 = -0.0001;
	Ly = 6.5;

	
	Tree1 = new Tree(Lx, Ly, mx, my, max_level, Lx0, Ly0);
	SM = new Shooting_Method;
	k_ = &(SM->k);

	SM->Pr = 7;
	SM->Le = 130;

	//1 - inclination of the layer, 2 - direction of the oscillation, 
	//3 - Rav (vibrational Rayleigh number, 4 - buoyancy ratio
	SM->recalculate_vibro_coefs(90, 90, 0, -0.75); 
	SM->k = 3.14159;
	

#pragma omp parallel
	{
		
		
		if (omp_get_thread_num() == 0) 
		{
			double t_sm = 0.0, t_, t_start = clock();
			int NT = 1;
			t_ = clock();
			for (*k_ = 2; *k_ < 4; *k_+= 0.1)
			//for (*k_ = 4; *k_ > 2; *k_ -= 0.1)
			{
				Tree1->remove();
				Tree1->find_solution();
				Tree1->find_deepest_v2({ *k_, SM->K, SM->Le });
				std::cout << "k = " << *k_ << " " << std::endl << std::endl;
			}
			t_sm += clock() - t_;
			std::cout << "level: " << Tree1->check_max_level() << " quads: " << Tree1->count() << std::endl;
			
			
			cout << setprecision(4) << endl << "Finish,  " << t_sm / CLOCKS_PER_SEC / NT << endl;


		}

		if (omp_get_thread_num() == 1)
		{
			GL_loop(argc, argv);
		}
	}
	
	return 0;
}
