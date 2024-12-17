all: predictors

predictors: Main.o Predictor.o
	g++ Main.o Predictor.o -o predictors

Main.o: Main.cpp Predictor.h
	g++ -c Main.cpp

Predictor.o: Predictor.cpp Predictor.h
	g++ -c Predictor.cpp

clean:
	rm -f *.o predictors
