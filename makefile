all:
	gcc -c 161044101_HW1_CSE344.c --std=gnu11
	gcc -o buNeDu 161044101_HW1_CSE344.o

debug:
	gcc -c 161044101_HW1_CSE344 -DDEBUG
	gcc -o buNeDu 161044101_HW1_CSE344.o

valgrind:
	valgrind ./buNeDu

clean:
	rm 161044101_HW1_CSE344.o buNeDu