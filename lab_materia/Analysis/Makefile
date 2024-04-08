LIBS:= `root-config --libs`
INCS:= `root-config --cflags`

all: main

main: main.o Measures.o
	g++ -o main main.o Measures.o ${LIBS}

main.o: main.cpp Measures.h File_Management.h 
	g++ -c main.cpp ${INCS}

Measures.o: Measures.cpp Measures.h
	g++ -c Measures.cpp 


#Differences: Differences.o
#	g++ -o Differences Differences.o ${LIBS}

#Differences.o: Differences.cpp Measures.h File_Management.h
#	g++ -c Differences.cpp ${INCS}



clear:
	rm *.o
	rm main
	rm Differences