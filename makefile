GCC=gcc -g -Wall -Wextra -std=gnu11 

ppm_example: ppm_example.o ppm.o
	$(GCC) $^ -o $@ -lpthread

ppm_example.o: ppm_example.c
	$(GCC) $< -c  

ppm.o: ppm.c ppm.h
	$(GCC) $< -c 

clean:
	rm -f *.o ppm_example
