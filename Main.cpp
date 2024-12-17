#include "Predictor.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
  std::ofstream outputFile("output.txt");  
  Predictor predictor;
  predictor.read(argv[1]);
  predictor.taken();
  predictor.notTaken();
  int bimodalExps[] = {2, 3, 5, 6, 8, 10, 12};
  for(int i = 0; i < 7; i++) {
    int param = pow(2, bimodalExps[i]);
    predictor.bimodal1(param);
  }
  for(int i = 0; i < 7; i++) {
    int param = pow(2, bimodalExps[i]);
    predictor.bimodal2(param);
  }
  for(int i = 0; i < 7; i++) {
    int param = pow(2, bimodalExps[i]);
    predictor.bimodal3(param);
  }
  for(int i = 2; i < 13; i++) {
    predictor.gshare(i);
  }
  predictor.tournament();
  int loopIntervals[] = {0, 1, 8, 15, 22, 33, 34};
  int j = 0;
  std::vector<outcome> outputs = predictor.getOutputs();
  for(int i = 0; i < 35; i++) {
    outputFile << outputs[i].correctPredictions << "," << outputs[i].branchesTotal << "; ";
    if(i == loopIntervals[j]) {
      outputFile << endl;
      j++;
    }
  }
}
  
