LIBS:= `root-config --libs`
INCS:= `root-config --cflags`


main: main.o 
	g++ -o main main.o ${LIBS}

main.o: main.cpp Measures.h File_Management.h 
	g++ -c main.cpp ${INCS}


clear:
	rm *.o
	rm main