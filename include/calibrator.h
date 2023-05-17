#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <THashTable.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "GainMap.h"
#include "ChannelMap.h"
#include "DataStructs.h"

class calibrator{
	private:
		//GainMap gMap;
		//ChannelMap cMap;
		std::vector<double> xVec;
	public:
	calibrator();
	~calibrator();
	void calibrate_qqq(std::string);
	std::vector<double> GetPeaks(TH1D *hist);
	void FitPeaks();
	void MyFill(THashTable *table, std::string name, int binsx, double minx, double maxx, double valuex, int binsy, double miny, double maxy, double valuey);
	void MyFill(THashTable *table, std::string name, int bins, double min, double max, double value);
	
};

