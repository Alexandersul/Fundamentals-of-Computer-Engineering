#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 4
#endif
#define N 10

double test_func(double x)
{
	return sin(x); //sa facem doar functia, indiferent de rezultat
	//return cos(x);								//il intereseaza doar implimentarea in ASM, nu rezultatul obligatoriu
}

double map(double x, double min_x, double max_x, double min_to, double max_to)
{
	return (x - min_x)/(max_x - min_x)*(max_to - min_to) + min_to;
}

void cheb_coef(double (*func)(double), int n, double min, double max, double *coef)
{
	memset(coef, 0, sizeof(double) * n);
	for (int i = 0; i < n; i++) {
		double f = func(map(cos(M_PI*(i + .5f)/n), -1, 1, min, max))*2/n;
		///prima jumatate f=0
		///idee posibila sa fie si 0.1 la un moment dat ///poate pt n=9 sau primele 3 0 mijl 4 0.1 ultimele 3 0.2
        ///in rest 0.2
		for (int j = 0; j < n; j++)
			coef[j] += f*cos(M_PI*j*(i + .5f)/n);///0 sau 0.2* (-1,0,1) tot timpul cifra de la i vine dupa ,
			///coef[j] = 0|0.1|0.2 * cos(0|0.4|0.8|1.2|1.6|2.0|2.4|2.8|3.2|3.6);
	}
}


double cheb_approx(double x, int n, double min, double max, double *coef)
{
	double a = 1, b = map(x, min, max, -1, 1), c;
	double res = coef[0]/2 + coef[1]*b;

	x = 2*b;
	for (int i = 2; i < n; a = b, b = c, i++)
		res += coef[i]*(c = x*b - a);

	return res;
}

int main(void)
{

	double c[N], min = 0, max = 1;
	cheb_coef(test_func, N, min, max, c);

	printf("Coefficients:\n");
	for (int i = 0; i < N; i++)
		printf(" %lg\n", c[i]);

	puts("\n\nApproximation:\n   x           func(x)     approx      diff");
	for (int i = 0; i <= 10; i++) {
		double x = map(i, 0, 10, min, max);
		double f = test_func(x);
		double approx = cheb_approx(x, N, min, max, c);

		printf("% 10.8lf % 10.8lf % 10.8lf % 4.1le\n",
			x, f, approx, approx - f);
	}

	return 0;
}
