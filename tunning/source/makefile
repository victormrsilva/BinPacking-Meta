CC= g++
OPTIONS= -c -O2 -m64 -march=native -std=c++11 -w

all: Main
Main: Main.o 
	$(CC) -o Main Main.o Construcao.o Arquivos.o Util.o -lm

Main.o: Main.cpp Util.o Arquivos.o Construcao.o
	$(CC) $(OPTIONS) Main.cpp

Construcao.o: Construcao.cpp Construcao.h Util.o
	$(CC) $(OPTIONS) Construcao.cpp

Arquivos.o: Arquivos.cpp Arquivos.h Util.o 
	$(CC) $(OPTIONS) Arquivos.cpp

Util.o: Util.cpp Util.h
	$(CC) $(OPTIONS) Util.cpp

clean:
	rm *.o Main -f

#./Main ../instances/Particao-200-4-150.txt --MAX_GRASP 300 --ALPHA_GRASP 0.3

#executar irace na pasta tuning: $IRACE_HOME/bin/irace

