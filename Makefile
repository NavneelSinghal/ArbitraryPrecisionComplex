all:
	gcc -c -o obj/bignum.o src/bignum.c
	ar cr obj/libs/libarbprecision.a obj/bignum.o
	gcc -c -o obj/cmplx.o src/cmplx.c -L ./obj/libs -larbprecision
	gcc -c -o obj/mainfile.o src/mainfile.c
	gcc -o mainfile obj/mainfile.o obj/bignum.o obj/cmplx.o -lm
	./mainfile input.txt output.txt plot.png
clean:
	rm obj/*.o
	rm obj/libs/*
	rm mainfile
