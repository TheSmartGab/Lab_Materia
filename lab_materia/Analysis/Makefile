LIBS:= `root-config --libs`
INCS:= `root-config --cflags`

all: DrawTabs main AbsToTrans

main: main.o Measures.o
	g++ -o main main.o Measures.o ${LIBS}

main.o: main.cpp Measures.h
	g++ -c main.cpp ${INCS}

DrawTabs: DrawTabs.o Measures.o
	g++ -o DrawTabs DrawTabs.o Measures.o ${LIBS}

DrawTabs.o: DrawTabs.cpp Measures.h 
	g++ -c DrawTabs.cpp ${INCS}

Measures.o: Measures.cpp Measures.h
	g++ -c Measures.cpp  ${INCS} 

AbsToTrans: AbsToTrans.o Measures.o
	g++ -o AbsToTrans AbsToTrans.o  Measures.o ${LIBS}

AbsToTrans.o: AbsToTrans.cpp Measures.h
	g++ -c AbsToTrans.cpp ${INCS}




clear:
	rm *.o
	rm main
	rm AbsToTrans
	rm DrawTabs