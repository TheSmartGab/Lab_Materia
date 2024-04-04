LIBS:= `root-config --libs`
INCS:= `root-config --cflags`

all: main Differences

main: main.o 
	g++ -o main main.o ${LIBS}

main.o: main.cpp Measures.h File_Management.h 
	g++ -c main.cpp ${INCS}

Differences: Differences.o
	g++ -o Differences Differences.o ${LIBS}

Differences.o: Differences.cpp Measures.h File_Management.h
	g++ -c Differences.cpp ${INCS}



clear:
	rm *.o
	rm main
	rm Differences