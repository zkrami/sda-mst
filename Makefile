CC= g++ 
CXXFLAGS= -Wall -ansi --pedantic -g

CPP_O_FILE = citiesReader.o mst.o main.o 

all: $(CPP_O_FILE)
	$(CC) $(CXXFLAGS) -o graph $(CPP_O_FILE) 

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf graph


%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -g $< -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) -fpermissive -c -g $< -o $@