#pragma once
#include <iostream> 
#include "GL\freeglut.h"
//#include "Quad.h"
#include "Tree.h"

#define show_matrix(matrix) 	for (int j = 0; j < 4; j++) { for (int i = 0; i < 4; i++) cout << matrix[i + j * 4] << " "; 		cout << endl; } cout << "_______________" << endl;
using std::cout;
using std::endl;


float tx, ty, tz, px, py, pz;
float nx, ny, nz;
float transX, transY, transZ;
float angleX, angleY, angleZ;
float fMin, fMax, alpha;
double zoom_2d = 1.0;
double scale_2d = 1.0;
double Lx_, Ly_;
extern double Lx, Ly, Lz;
extern double Lx0, Ly0, Lz0;
extern int width, height;
extern double x0_, y0_;
//GLdouble wx, wy, wz;
GLdouble mvmatrix[16], projmatrix[16];
extern Tree *Tree1;

extern double X, Y;
extern double *k_;
double mouseX, mouseY;
double shift, dx, dy;

void define_default_camera() {
	tx = 8; 	ty = 3.5; 	tz = 6.5;
	px = -4.5;	py = -0.5;	pz = -2;
	fMin = 0.5;	fMax = 100.0; 	alpha = 75.0;
	transX = 0, transY = 0, transZ = -1;
	angleX = 0, angleY = 0, angleZ = 0;
}

void coords(int x, int y) 
{
	mouseX = (-1.0 + 2.0 * x / width);
	mouseY = (1.0 - 2.0 * y / height);
	X = -dx*scale_2d + Lx0  + (mouseX + 1)*0.5*Lx_;
	Y = -dy*scale_2d + Ly0 + (mouseY + 1)*0.5*Ly_;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	//gluPerspective(alpha, 1.0, fMin, fMax);

	//glMatrixMode(GL_MODELVIEW);

}
void keyboard_spec(int key, int x, int y) {
	int stat = glutGetModifiers();
	if (stat == 0) {
		switch (key)
		{
		case GLUT_KEY_UP:
			transY += -0.1f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			transY += 0.1f;
			break;
		case GLUT_KEY_LEFT:
			transX += 0.1f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			transX += -0.1f;
			glutPostRedisplay();
			break;
		}
	}
	if (stat == GLUT_ACTIVE_SHIFT) {
		switch (key)
		{
		case GLUT_KEY_UP:
			angleX += -5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			angleX += 5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			angleY += -5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			angleY += 5;
			glutPostRedisplay();
			break;
		}
	}
	if (stat == GLUT_ACTIVE_ALT) {
		switch (key)
		{
		case GLUT_KEY_UP:
			scale_2d -= 0.001;
			break;
		case GLUT_KEY_DOWN:
			scale_2d += 0.001;
			break;
		case GLUT_KEY_LEFT:
			angleZ += -5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			angleZ += 5;
			glutPostRedisplay();
			break;
		}
	}
}
void mouseMovePassive(int x, int y) {
	//cout << x << " " << y << endl;


	//cout << mouseX << " "  << mouseY << " / " << X << " " << Y << endl;
}
void mouseMove(int key, int state, int x, int y) {

	int stat = glutGetModifiers();
	coords(x, y);
	//cout << stat << " " << key << " " << state << endl;

	switch (stat)
	{
	case 0:
		if (key == 3) //scroll up
		{
			if (state == GLUT_DOWN)
			{


				scale_2d -= 0.01;
			}
		}

		if (key == 4) { //scroll down
			if (state == GLUT_DOWN) {


				scale_2d += 0.01;
				//cout << "s(2d) = " << scale_2d << endl;

			}
		}

		if (key == 0) //left click
		{
			if (state == GLUT_DOWN) {


				//cout << X << " " << Y << endl;
			}
		}
		if (key == 2) //right click
		{
			if (state == GLUT_DOWN) {

				cd d;	system2(d, X, Y);
				cout << endl << "coords: " << X << " " << Y << endl;
				cout << "Det: " << real(d) << " " << imag(d) << endl;

			}
		}

		if (key == 1) { //middle click
			if (state == GLUT_DOWN) {
				cout << "coords: " << X << " " << Y << " normalized: " << mouseX << " " << mouseY << endl;
			}
		}


		break;
	case GLUT_ACTIVE_SHIFT:
		if (key == 3)
		{
			if (state == GLUT_DOWN)
			{

				scale_2d -= 0.1;


			}
		}

		if (key == 4) {
			if (state == GLUT_DOWN) {

				scale_2d += 0.1;


			}
		}
		break;


	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
float a = 0.5;

switch (key)
{

case '1':
	//Tree1->root[0].find_solution();
	Tree1->find_solution();
	std::cout << "level: " << Tree1->check_max_level() << " quads: " << Tree1->count() << std::endl;
break;

case '2':
	Tree1->find_solution();
	std::cout << "level: " << Tree1->check_max_level() << " quads: " << Tree1->count() << std::endl;
break;
case '3':
Tree1->immerse();
std::cout << "level: " << Tree1->check_max_level() << " quads: " << Tree1->count() << std::endl;
break;

case '4':
Tree1->immerse_all();
std::cout << "level: " << Tree1->check_max_level() << " quads: " << Tree1->count() << std::endl;
break;
case '5':
	Tree1->remove();
break;

case 'l':
case 'L':
	std::cout << "level: " << Tree1->check_max_level() << " Nun: " << Tree1->count() <<   std::endl;
break;

case 'd':
case 'D':
	/*
	int d;
	double x, y;
	x = y = 0;
	d = 1;
	Tree1->root[0].find_deepest(d, x, y);
	std::cout << "level: " << d << " root:  " << x << " " << y << std::endl;
	*/
	Tree1->find_deepest();
	break;
case 'k':

	break;
case 'z':
case 'Z':
	std::cout << "check: " << Tree1->root[0].check_something() << std::endl;
break;

case 'n':
case 'N':
	std::cout << "quads: " << Tree1->count() << std::endl;
break;


case '+':
	*k_ += 0.1;
break;
case '-':
	*k_ -= 0.1;
break;





default:
break;
}


}

int iter_draw;
void idle()
{
	//if (iter_draw % 100 == 0) 
	//show_matrix(projmatrix);

	glutReshapeFunc(reshape);
	glutPostRedisplay();
	iter_draw++;
}

void draw_test_functions() {
	//glLoadIdentity();

	glBegin(GL_LINE_LOOP);
	for (double x = 0; x < Lx; x = x + 0.00001)
	{
		double y = 1 - 2 * x;
		//glVertex3d(x, sqrt(-x*x+R*R), 0);
		if (y > 0) glVertex3d(x, y, 0);
	}
	glEnd();


	//glLoadIdentity();
	glBegin(GL_LINE_LOOP);
	for (double x = 0; x < Lx; x = x + 0.001)
	{
		double y = 0.0001*x;
		glVertex3d(x, y, 0);
	}
	glEnd();

}

void display()
{
	//iter++; if (iter % 1000 == 0) cout << "iter: " << iter << endl;
	glClearColor(1.0, 1.0, 1.0, 1.0);


	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();


	//____________________________________//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	projmatrix[12] = transX;
	projmatrix[13] = transY;
	glLoadMatrixd(projmatrix);



	//here:

	if (Lx_ <= 0 || Ly_ <= 0) {
		//scale_2d += 0.01; goto here;
	}
	
	shift = 0.1;
	dx = (Lx - Lx0)*shift;
	dy = (Ly - Ly0)*shift;

	Lx_ = Lx + dx*scale_2d - (Lx0 - dx*scale_2d);
	Ly_ = Ly + dy*scale_2d - (Ly0 - dy*scale_2d);

	
	gluOrtho2D( Lx0 - dx*scale_2d, Lx + dx*scale_2d, Ly0 - dy*scale_2d, Ly + dy*scale_2d);
	//gluLookAt(0.0, 0.0, zoom_2d, 0.0, 0.0, -10.0, 0, 1, 0);


	nx = (tx - px);
	ny = (ty - py);
	nz = (tz - pz);

	//____________________________________//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//glTranslatef(0.5, 0.5, 0);
	//glutSolidSphere(0.05, 15, 15);
	//glLoadIdentity();

	Tree1->draw_root();
	//Tree1->draw_rude_intersections();

	glColor3d(1.0, 0.0, 0.0);
	//draw_test_functions();


	//axes
	//glBegin(GL_LINES);
	//glColor3d(0.0, 0.0, 1.0);
	//glVertex3d(Lx0, (Ly + Ly0)*0.5, 0); 	 glVertex3d(Lx, (Ly + Ly0)*0.5, 0);
	//glVertex3d((Lx+Lx0)*0.5, Ly0, 0);  	glVertex3d((Lx + Lx0)*0.5, Ly, 0);
	//glEnd();
	glutSwapBuffers();
}



void GL_loop(int argc, char **argv)
{
	//window resolution
	height = 600;
	width = 600;
	//double aspect = Lx / Ly;
	//width = (int)(height*aspect);
	


	define_default_camera();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Window_freeglut");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutPassiveMotionFunc(mouseMovePassive);
	glutMouseFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_spec);
	
	/*
	while (1)
	{
		glutMainLoopEvent();
		glutPostRedisplay();
	}
	*/
	glutMainLoop();
}
