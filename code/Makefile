CC= gcc 
CXXFLAGS= -Wall -ansi --pedantic -std=c11 -g

CPP_O_FILE = citiesReader.o  main.o 

all: $(CPP_O_FILE)
	$(CC) $(CXXFLAGS) -o main $(CPP_O_FILE) 

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf main


%.o: %.c
	$(CC)  $(CXXFLAGS) -c $< -o $@ -lm