#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


/*
 * Wielomiany bazowe: i - stopien funkcji x - wspolrzedna dla ktorej obliczana jest wartosc funkcji
 */

double
fi( int i, double x)
{	
		
	if ( i == 0 ) {
		return 	1;
	} else if ( i == 1 ) {
		return 	2 * x;
	} else {
                return  2 * x * fi( i-1, x ) - 2 * ( i - 1 ) * fi( i-2, x );
	} 
}



/* Pierwsza pochodna wielomianu */
double
dfi( int i, double x)
{
	
	
        if ( i == 0 ) {
		return 	0;
	} else if ( i == 1 ) {
		return 	2;
	} else {
                return  2 * fi( i-1, x ) + 2 * x * dfi(i-1, x ) - 2 * ( i - 1 ) * dfi( i-2, x );
        }



}

/* Druga pochodna wielomianu */
double
d2fi( int i, double x)
{	
	if ( i == 0 ) {
                return 	0;
        } else if ( i == 1 ) {
                return 	0;
        } else {
                return  4 * dfi( i-1, x ) + 2 * x * d2fi( i-1, x ) - 2 * ( i - 1 ) * d2fi( i-2, x );      
        }


	

}

/* Ta pochodna wielomianu */
double
d3fi( int i, double x)
{	
        if ( i == 0 ) {
                return 	0;
        } else if ( i == 1 ) {
                return 	0;
        } else {
                return  6 * d2fi( i-1, x ) + 2 * x * d3fi( i-1, x ) - 2 * ( i - 1 ) * d3fi( i-2, x );
        }


}

void
make_spl(points_t * pts, spline_t * spl)
{

	matrix_t       *eqs= NULL;
	double         *x = pts->x;
	double         *y = pts->y;
	double 		a = x[0];
	double		b = x[pts->n-1];
	int		i, j, k;
	int		nb = pts->n - 3 > 10 ? 10 : pts->n - 3;
  	char *nbEnv= getenv( "APROX_BASE_SIZE" );

	if( nbEnv != NULL && atoi( nbEnv ) >= 0 )
		nb = atoi( nbEnv ) + 1 ;

	eqs = make_matrix(nb, nb + 1);

	for (j = 0; j < nb; j++) {
		for (i = 0; i < nb; i++)
			for (k = 0; k < pts->n; k++)
				add_to_entry_matrix(eqs, j, i, fi( i, x[k]) * fi( j, x[k]) );

		for (k = 0; k < pts->n; k++)
			add_to_entry_matrix(eqs, j, nb,  y[k] * fi( j, x[k]));
	}
        write_matrix(eqs, stdout);


	if (piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}
        write_matrix(eqs, stdout);
	
        if (alloc_spl(spl, nb) == 0) {
                for (i = 0; i < spl->n; i++) {
                        double xx = spl->x[i] = a + i*(b-a)/(spl->n-1);
		
			
                        xx+= 10.0*DBL_EPSILON;  /* zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale */
                        spl->f[i] = 0;
                        spl->f1[i] = 0;
                        spl->f2[i] = 0;
                        spl->f3[i] = 0;
                        for (k = 0; k < nb; k++) {
                                double          ck = get_entry_matrix(eqs, k, nb);
                                spl->f[i]  += ck * fi  (k, xx);
                                spl->f1[i] += ck * dfi (k, xx);
                                spl->f2[i] += ck * d2fi(k, xx);
                                spl->f3[i] += ck * d3fi(k, xx);
                        }
                }
        }



}
