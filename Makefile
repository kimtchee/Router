router.out: router.o RouterRunner.o
	g++ -Wall -ansi -g -o router.out router.o RouterRunner.o 


router.o: router.cpp router.h 
	g++ -Wall -ansi -g -c router.cpp

RouterRunner.o: RouterRunner.cpp RouterRunner.h CPUTimer.h router.h 
	g++ -Wall -ansi -g -c RouterRunner.cpp

clean:
	rm -f router.out router.o RouterRunner.o 
