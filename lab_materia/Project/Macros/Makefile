LIBS:= `root-config --libs`
INCS:= `root-config --cflags`
LibsPath:= `../Libs/Libraries`

all: DrawTabs main AbsToTrans Run libs

libs: 

main: main.o Measures.o
	g++ -o main main.o Measures.o ${LIBS}

main.o: main.cpp Measures.h
	g++ -c main.cpp ${INCS}

DrawTabs: DrawTabs.o Measures.o
	g++ -o DrawTabs DrawTabs.o Measures.o ${LIBS}

DrawTabs.o: DrawTabs.cpp Measures.h 
	g++ -c DrawTabs.cpp ${INCS}

AbsToTrans: AbsToTrans.o Measures.o
	g++ -o AbsToTrans AbsToTrans.o  Measures.o ${LIBS}

AbsToTrans.o: AbsToTrans.cpp Measures.h
	g++ -c AbsToTrans.cpp ${INCS}

Run: Run.o Measures.o
	g++ -o Run Run.o Measures.o ${LIBS}

Run.o: Run.cpp Measures.h
	g++ -c Run.cpp ${INCS}


clear:
	rm *.o
	rm main
	rm AbsToTrans
	rm DrawTabs
	rm Run