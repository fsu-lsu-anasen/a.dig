/*
 likelihoodNet.cpp
 Class that implements a net of continuously evaluated likelihoods on a 1d grid
*/

#include "likelihoodNet.h"

void likelihoodNet::initialize(int N, double_t min, double_t max, double_t w)
{
	numTrials=0;
	size = N;
	cauchyLorentzWidth = w; //Assume the chosen width
	lkGrid.resize(N); //N elements in grid
	std::fill(lkGrid.begin(),lkGrid.end(),1.0/N); //At the beginning, we have uniform distbn in range chosen
	minS = min;
	maxS = max;
	//ofile.open("out.out");
}

double_t likelihoodNet::getLikelihoodSum()
{
	double_t sum=0;
	for(auto &n :lkGrid) 
		sum+=n;
	return sum;
}

double_t likelihoodNet::getLikelihoodAt(int i)
{
	return lkGrid[i];
}

double_t likelihoodNet::getGridXAt(int i)
{
	//ith position between minS and maxS divided equally in N steps
	return minS+(maxS-minS)*(static_cast<double_t>(i)/static_cast<double_t>(size));
}

void likelihoodNet::stepForward(double_t frontE, double_t backE)
{
	numTrials += 1;
	double_t n, S;
	double_t w =  cauchyLorentzWidth;
	for(int i=0; i<lkGrid.size(); i++)
	{
		n = getLikelihoodAt(i);
		S = getGridXAt(i);
		lkGrid[i] *= (1.0/(w*w+log(backE/(frontE*S))*log(backE/(frontE*S))));
	}
	double_t sum = getLikelihoodSum();
	for(auto &n : lkGrid)
		n = n/sum;
}

double_t likelihoodNet::getMaxLikelihood()
{
	double_t maxy = 0.0;
	for(int i=0; i<lkGrid.size(); i++){
		if(lkGrid[i]>maxy) 
			maxy = lkGrid[i];
	}
	return (maxy);
}

double_t likelihoodNet::getMaxLikelihoodS()
{
	double_t maxy = 0.0, maxx;
	for(int i=0; i<lkGrid.size(); i++){
		if(lkGrid[i]>maxy) 
		{
			maxx = getGridXAt(i);
			maxy = lkGrid[i];
		}
	}
	return (maxx);
}

std::vector<double_t> likelihoodNet::getS16_50_84()
{
	double_t sum = 0.0, S16,S50,S84;
	bool found16=false; 
	bool found50=false;
	bool found84=false;
	for(int i=0; i<lkGrid.size(); i++){
		if(sum>0.16 && !found16) 
		{
			S16 = getGridXAt(i-1);
			found16=true;
		}
		if(sum>0.50 && !found50) 
		{
			S50 = getGridXAt(i-1);
			found50=true;
		}
		if(sum>0.84 && !found84) 
		{
			S84 = getGridXAt(i-1);
			found84=true;
		}
		sum = sum + lkGrid[i];
		//std::cout << getGridXAt(i) << " " << lkGrid[i] << " " <<  sum << std::endl;
	}
	//std::cout << sum << " " << S50 << " " << S16 << " " << S84;
	std::vector<double_t> results;
	results.push_back(S16);
	results.push_back(S50);
	results.push_back(S84);
	return results;
}

likelihoodNet::~likelihoodNet()
{
	//ofile.close();
}
