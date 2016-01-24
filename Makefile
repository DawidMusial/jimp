hermite: main.o splines.o points.o aproksymator_wielomianami_hermite.o gaus/libge.a
	$(CC) -o hermite  main.o splines.o points.o aproksymator_wielomianami_hermite.o -L gaus -l ge -lm -Wall -ansi -pedantic

aprox: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge

intrp: main.o splines.o points.o interpolator.o gaus/libge.a
	$(CC) -o intrp  main.o splines.o points.o interpolator.o -L gaus -l ge

prosta: main.o splines.o points.o prosta.o
	$(CC) -o prosta  main.o splines.o points.o prosta.o	

aproksymator_na_bazie.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_na_bazie.c

aproksymator_wielomianami_hermite.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_wielomianami_hermite.c -Wall -ansi -pedantic

interpolator.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c interpolator.c

.PHONY: clean test clean

test:
	./hermite -s spl -p test/dane.3 -g myplot -f -5 -t 10 -n 300	

clean:
	-rm *.o aprox intrp prosta aproksymator_wielomianami_hermite
