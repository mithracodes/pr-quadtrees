dict3: dict3.o dictionary.o read.o quadtree.o
	gcc -Wall -o dict3 dict3.o dictionary.o read.o quadtree.o -g

dict3.o: dict3.c dictionary.h read.h quadtree.h
	gcc -Wall -o dict3.o dict3.c -g -c
  
dictionary.o: dictionary.c dictionary.h record_struct.c record_struct.h
	gcc -Wall -o dictionary.o dictionary.c -g -c

read.o: read.c read.h record_struct.c record_struct.h
	gcc -Wall -o read.o read.c -g -c

quadtree.o: quadtree.c quadtree.h
	gcc -Wall -o quadtree.o quadtree.c -g -c

dict4: dict4.o dictionary.o read.o quadtree.o
	gcc -Wall -o dict4 dict4.o dictionary.o read.o quadtree.o -g

dict4.o: dict3.c dictionary.h read.h quadtree.h
	gcc -Wall -o dict4.o dict3.c -g -c
