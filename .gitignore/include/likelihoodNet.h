#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

class likelihoodNet
{
private:
std::vector<double_t> lkGrid;
double_t minS;
double_t maxS;
int size;
int numTrials;
double_t cauchyLorentzWidth;
std::ofstream ofile;

public:
~likelihoodNet();
void initialize(int N,double_t minS, double_t maxS, double_t w);
double_t getLikelihoodSum();
std::vector<double_t> getS16_50_84();
double_t getGridXAt(int i);
double_t getLikelihoodAt(int i);
double_t getMaxLikelihoodS();
double_t getMaxLikelihood();
int getNumTrials();
void stepForward(double_t frontE, double_t backE);
};


