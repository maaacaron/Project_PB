
createEvent: createEvent.o
	gcc -o createEvent createEvent.o
createEvent.o: createEvent.c event.h
	gcc -c createEvent.c
clear:
	rm -rf createEvent createEvent.o
