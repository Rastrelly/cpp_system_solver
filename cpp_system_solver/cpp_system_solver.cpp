#include <iostream>
#include <GL/freeglut.h>

using namespace std;

//solves the equation
class solver
{
private:
	float a1, a2, a3, a4, a5, a6;
	float rx, ry;
public:
	float getx() { return rx; }
	float gety() { return ry; }

	float geta1() { return a1; }
	float geta2() { return a2; }
	float geta3() { return a3; }
	float geta4() { return a4; }
	float geta5() { return a5; }
	float geta6() { return a6; }

	solver(float na1, float na2, float na3, float na4, float na5, float na6)
	{
		a1 = na1; a2 = na2; a3 = na3; a4 = na4; a5 = na5; a6 = na6;
	}
	void solve(float &x, float &y)
	{
		if (a1 == 0.0f)
		{
			if (a2 == 0.0f)
			{
				rx = NULL; 
				ry = NULL;
			}
			else
			{
				rx = (a6 - (a5 * (a3 / a2))) / a4;
				ry = a3 / a2;
			}
		}
		else
		{
			if ((a4 == 0) && (a5 == 0))
			{
				rx = NULL;
				ry = NULL;
			}
			else
			{
				rx = (a3 / a1) - ((a2 / a1)*((a6 - a4*a3/a1)/(a5-a4*a2/a1)));
				ry = (a6 - a4 * a3 / a1) / (a5 - a4 * a2 / a1);
			}
		}
		x = rx;
		y = ry;
	}
};

solver * solvptr;

void drawLine(float x1,float y1,float x2, float y2, 
			  float r, float g, float b)
{
	glBegin(GL_LINES);

	glColor3f(r, g, b);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);

	glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
				  float r, float g, float b)
{
	glBegin(GL_LINE_LOOP);

	glColor3f(r, g, b);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);

	glEnd();
}

void cb_draw()
{
	//cleanup
	glClear(GL_COLOR_BUFFER_BIT);

	//set up the drawspace
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate rendering coords
	float rtx = solvptr->getx();
	float rty = solvptr->gety();

	float minx = rtx - 10;
	if (minx > 0) minx = -10;
	float maxx = rtx + 10;
	if (maxx < 0) maxx = 10;

	float miny = rty - 10;
	if (miny > 0) miny = -10;
	float maxy = rty + 10;
	if (maxy < 0) maxy = 10;

	float cx1 = minx;
	float cx2 = maxx;

	float cy1_1 = (solvptr->geta3() - (solvptr->geta1())*cx1) / solvptr->geta2();
	float cy1_2 = (solvptr->geta3() - (solvptr->geta1())*cx2) / solvptr->geta2();

	float cy2_1 = (solvptr->geta6() - (solvptr->geta4())*cx1) / solvptr->geta5();
	float cy2_2 = (solvptr->geta6() - (solvptr->geta4())*cx2) / solvptr->geta5();

	gluOrtho2D(minx, maxx, miny, maxy);

	//draw the stuff
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//1 - draw axis
	//X
	drawLine(minx, 0, maxx, 0, 1.0f, 0.0f, 0.0f);
	//Y
	drawLine(0, miny, 0, maxy, 0.0f, 1.0f, 0.0f);

	//2 - draw charts
	//1
	drawLine(cx1, cy1_1, cx2, cy1_2, 1.0f, 1.0f, 0.0f);
	//2
	drawLine(cx1, cy2_1, cx2, cy2_2, 0.0f, 1.0f, 1.0f);

	//draw root
	drawTriangle(rtx, rty, rtx - 0.5f, rty + 1, rtx + 0.5f, rty + 1,
				 1.0f, 0.0f, 1.0f);

	//render stuff
	glutSwapBuffers();

}

void initOpenGL(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Moving Reactangle");

	glutDisplayFunc(cb_draw);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
};

int main(int argc, char *argv[])
{
	//calculation part	
	float a1, a2, a3, a4, a5, a6;
	float x = 0, y = 0;
	cout << "Input a1, a2, a3, a4, a5, a6:\n";
	cin >> a1 >> a2 >> a3 >> a4 >> a5 >> a6;
	solver slv = solver(a1, a2, a3, a4, a5, a6);
	solvptr = &slv; //allow others to have fun with data
	slv.solve(x, y);
	cout << "x="<<x<<"; y="<<y<<".\n";

	//visualization part

	initOpenGL(argc, argv);

	glutMainLoop();

	system("pause");
	return 0;
}