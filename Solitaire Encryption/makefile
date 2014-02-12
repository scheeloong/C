FLAGS = -g -Wall 

sol : sol.o card.o reading.o
	gcc ${FLAGS} -o $@ $^

%.o: card.h reading.h
	gcc ${FLAGS} -c $*.c

clean:
	rm *.o sol
