#include "Predictor.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

void Predictor::read(string fileName) {
  unsigned long long addr;
  string action;
  ifstream infile(fileName);
  while(infile >> hex >> addr >> action) {
    Data newData;
    newData.address = addr;
    newData.action = (action == "T") ? 1 : 0;
    dataVector.push_back(newData);
  }
}

void Predictor::taken() {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    if(dataVector[i].action == 1) {
      correctPredictions++;
    }
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

void Predictor::notTaken() {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    if(dataVector[i].action == 0) {
      correctPredictions++;
    }
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

void Predictor::bimodal1(int tableSize) {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  vector<int> predictionTable(tableSize, 0);
  int indexMask = tableSize - 1;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    int index = dataVector[i].address & indexMask;
    int prediction = predictionTable[index];
    if(dataVector[i].action == prediction) {
      correctPredictions++;
    } else {
      if(dataVector[i].action == 0) predictionTable[index]--;
      if(dataVector[i].action == 1) predictionTable[index]++;
    }
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

void Predictor::bimodal2(int tableSize) {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  vector<int> predictionTable(tableSize, 0);
  int indexMask = tableSize - 1;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    int index = dataVector[i].address & indexMask;
    if(dataVector[i].action == 1) {
      if(predictionTable[index] >= 2) {
	correctPredictions++;
	// if prediction is taken and outcome is taken, the prediction at that index will become 3 no matter what
        predictionTable[index] = 3;
      } else {
	predictionTable[index]++;
      }
    } else {
      if(predictionTable[index] < 2) {
	correctPredictions++;
	// if prediction is not taken and the outcome is not taken, the prediction will become 0 no matter what
	predictionTable[index] = 0;
      } else {
	predictionTable[index]--;
      }
    }
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

void Predictor::bimodal3(int tableSize) {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  vector<int> predictionTable(tableSize, 1);
  int indexMask = tableSize - 1;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    int index = dataVector[i].address & indexMask;
    if(dataVector[i].action == 1) {
      if(predictionTable[index] <= 3) {
	correctPredictions++;
	if(predictionTable[index] != 1) predictionTable[index]--;
      } else {
	predictionTable[index]--;
      }
    } else {
      if(predictionTable[index] > 3) {
	correctPredictions++;
	if(predictionTable[index] != 6) predictionTable[index]++;
      } else {
	if(predictionTable[index] == 2) predictionTable[index]++;
	predictionTable[index]++;
      }
    }
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

std::vector<outcome> Predictor::getOutputs() {
  return output;
}

void Predictor::gshare(int historyBits) {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  vector<int> predictionTable(4096, 0);
  int globalHistory = 1;
  unsigned int historyMask = (1 << historyBits) - 1;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    int PC = dataVector[i].address % 4096;
    int index = PC ^ (globalHistory & historyMask);
    if(dataVector[i].action == 1) {
      if(predictionTable[index] >= 2) {
	correctPredictions++;
	predictionTable[index] = 3;
      } else {
	predictionTable[index]++;
      }
    } else {
      if(predictionTable[index] < 2) {
	correctPredictions++;
	predictionTable[index] = 0;
      } else {
	predictionTable[index]--;
	}
      }
    branches++;
    globalHistory = ((globalHistory << 1) | dataVector[i].action) & historyMask;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}

void Predictor::tournament() {
  unsigned long long correctPredictions = 0;
  unsigned long long branches = 0;
  vector<int> gTable(4096, 0);
  vector<int> bTable(4096, 6);
  vector<int> sTable(4096, 3);
  unsigned int globalHistory = 0;
  unsigned int historyMask = (1 << 12) - 1;
  for(unsigned long long i = 0; i < dataVector.size(); i++) {
    int gIndex = (dataVector[i].address % 4096) ^ (globalHistory & historyMask);
    int bIndex = dataVector[i].address % 4096;
    int gPrediction = (gTable[gIndex] >= 2) ? 1 : 0;
    int bPrediction = (bTable[bIndex] <= 3) ? 1 : 0;
    int actualOutcome = dataVector[i].action;
    
    int finalPrediction = (sTable[bIndex] >= 2) ? bPrediction : gPrediction;
    if(finalPrediction == actualOutcome) correctPredictions++;

    if(actualOutcome == 1) {
      if(gTable[gIndex] != 3) gTable[gIndex]++;
      if(bTable[bIndex] != 1) bTable[bIndex]--;
    } else {
      if(gTable[gIndex] != 0) gTable[gIndex]--;
      if(bTable[bIndex] == 2) bTable[bIndex]++;
      if(bTable[bIndex] != 6) bTable[bIndex]++;
    }

    if(gPrediction != bPrediction) {
      if(gPrediction == actualOutcome && sTable[bIndex] != 0) sTable[bIndex]--;
      else if(bPrediction == actualOutcome && sTable[bIndex] != 3) sTable[bIndex]++;
    }
    globalHistory = ((globalHistory << 1) | actualOutcome) & historyMask;
    branches++;
  }
  outcome current;
  current.correctPredictions = correctPredictions;
  current.branchesTotal = branches;
  output.push_back(current);
}
