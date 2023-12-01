#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "rk4.h"
#include "winbgi2.h"
#define PI 4*atan(1.0)
const double FPS = 60;
const double g = 9.81;
const double l1 = 80;
const double l2 = 80;
const double m1 = 1;
const double m2 = 1;
const double fi01 = 1.8;
const double fi02 = 3.5;
const double h = 0.01;


void prawastrona(double x, double* Y, double* Ynew)
{
	double prawa1[2], prawa2[2];
	prawa1[0] = (-g*(2 * m1 + m2)*sin(Y[1]) - m2*g*sin(Y[1] - 2 * Y[3]) - 2 * sin(Y[1] - Y[3])*m2*(Y[2] * Y[2] * l2 + Y[0] * Y[0] * l1*cos(Y[1] - Y[3]))) / (l1*(2 * m1 + m2 - m2*cos(2 * Y[1] - 2 * Y[3])));
	prawa1[1] = Y[0];
	prawa2[0] = (2 * sin(Y[1] - Y[3])*(Y[0] * Y[0] * l1*(m1 + m2) + g*(m1 + m2)*cos(Y[1]) + Y[2] * Y[2] * l2*m2*cos(Y[1] - Y[3]))) / (l2*(2 * m1 + m2 - m2*cos(2 * Y[1] - 2 * Y[3])));//ostatnie Y[0]x` chyba do zmiany na Y[1], Y[2] na Y[3]
	prawa2[1] = Y[2];
	Ynew[0] = prawa1[0];
	Ynew[1] = prawa1[1];
	Ynew[2] = prawa2[0];
	Ynew[3] = prawa2[1];
}



int main()
{
	graphics(400, 400);
	double y[4] = { 0, fi01, 0, fi02 };
	double t = 0, E, Ek, Ep, t_max = 10000;
	int past_pos = 100;
	double *prev_x, *prev_y;
	prev_x = (double*)malloc(past_pos*sizeof(double));
	prev_y = (double*)malloc(past_pos*sizeof(double));
	char dane[20],energie[25];
	

	for(int i = 0; i<past_pos; ++i)
	{
		prev_x[i] = 0;
		prev_y[i] = 0;
	}
	int n = t_max/h;

	for(int i=0;i<100000;++i)
	{
	
		clear();
		circle(200 + l1*sin(y[1]), 200 + l1*cos(y[1]), 5);
		line(200, 200, 200 + l1*sin(y[1]), 200 + l1*cos(y[1]));
		circle(200 + l1*sin(y[1]) + l2*sin(y[3]), 200 + l1*cos(y[1]) + l2*cos(y[3]), 5);
		line(200 + l1*sin(y[1]), 200 + l1*cos(y[1]), 200 + l1*sin(y[1]) + l2*sin(y[3]), 200 + l1*cos(y[1]) + l2*cos(y[3]));
		
		E = -g*m1*l1*cos(y[1]) -m2*g*(l1*cos(y[1])+l2*cos(y[3])) + m1/2.*y[0]*y[0]*l1*l1+m2/2.*(pow(y[0]*l1*cos(y[1])+y[2]*l2*cos(y[3]),2)+pow(y[0]*l1*sin(y[1])+y[2]*l2*sin(y[3]),2));
		Ek = m1 / 2.*y[0] * y[0] * l1*l1 + m2 / 2.*(pow(y[0] * l1*cos(y[1]) + y[2] * l2*cos(y[3]), 2) + pow(y[0] * l1*sin(y[1]) + y[2] * l2*sin(y[3]), 2));
		Ep = -g*m1*l1*cos(y[1]) - m2*g*(l1*cos(y[1]) + l2*cos(y[3]));
		
		sprintf(dane, "E=%lg i: %d", E,i);
		outtextxy(0, 0, dane);
		sprintf(energie, "Ek=%lg Ep= %lg", Ek, Ep);
		outtextxy(0, 18, energie);

		prev_x[past_pos-1] = 200 + l1*sin(y[1]) + l2*sin(y[3]);
		prev_y[past_pos-1] = 200 + l1*cos(y[1]) + l2*cos(y[3]);

		for(int i = 0; i<past_pos; i+=10)
		{
			circle(prev_x[i], prev_y[i], 2);
		}
	
		vrk4(t, y, h, 4, prawastrona, y);
		t += h;
		
		for(int i = 0; i<past_pos;++i)
		{
			prev_x[i] = prev_x[i+1];		
			prev_y[i] = prev_y[i+1];
		}
		
	}
			
	wait();
	free(prev_x);
	free(prev_y);
}