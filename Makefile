CPP=g++ --std=c++11 -pthread

CFLAGS= -Wall

all : ray

ray : main.o
	$(CPP) $(CFLAGS) -o $@ $^

main.o : main.cpp Couleur.hpp forme/Sphere.hpp Outils.hpp Liste_touche.hpp forme/Rectangle.hpp Box.hpp Rotation.hpp Deplacement.hpp Pdf.hpp
	$(CPP) $(CFLAGS) -c $<


clean :
	rm *.o

test:
	./ray