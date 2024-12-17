#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

struct Data {
  long long address;
  int action;
  Data(long long addr = -1, int act = -1): address(addr), action(act) {}
};

struct outcome {
  long correctPredictions;
  long branchesTotal;
};

class Predictor {
 public:
  void read(std::string fileName);
  void taken();
  void notTaken();
  void bimodal1(int tableSize);
  void bimodal2(int tableSize);
  void bimodal3(int tableSize);
  void gshare(int historyBits);
  void tournament();
  std::vector<outcome> getOutputs();
  void print();
 private:
  std::vector<Data> dataVector;
  std::vector<outcome> output;
};

#endif
