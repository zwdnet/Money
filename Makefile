Money:main.cpp database.o datadeal.o
	g++ main.cpp database.o datadeal.o -o money -lsqlite3
	rm *.o

Debug:main.cpp database.o datadeal.o
	g++ main.cpp database.o datadeal.o -o money -lsqlite3 -g
	rm *.o

database:database.cpp
	g++ database.cpp -c database.o -g

datadeal:datadeal.cpp
	g++ datadeal.cpp -c datadeal.o -g
