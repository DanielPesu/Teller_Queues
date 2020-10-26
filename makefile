Simulation:	main.o simulation.o teller.o
	g++ -o Simulation main.o simulation.o teller.o

main.o:	main.cpp simulation.h
	g++ -c main.cpp

simulation.o:	simulation.cpp simulation.h ./datastructures/heap/heap.h ./datastructures/queue/queue.h ./datatypes/teller/teller.h ./datatypes/customer/customer.h
	g++ -c simulation.cpp

teller.o:	./datatypes/teller/teller.cpp ./datatypes/teller/teller.h
	g++ -c ./datatypes/teller/teller.cpp

clean:
	rm -f Simulation
	rm -f *.o
