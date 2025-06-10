navigatechecker: navigatechecker.c navigatefunctions.c navigatefunctions.h
	gcc -Wall -std=c99 -O3 navigatechecker.c navigatefunctions.c -o navigatechecker

clean:
	rm -f navigatechecker