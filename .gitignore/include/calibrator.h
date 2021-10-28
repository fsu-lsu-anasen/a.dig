#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <THashTable.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "GainMap.h"
#include "ChannelMap.h"
#include "DataStructs.h"

class calibrator{
	private:
		//GainMap gMap;
		//ChannelMap cMap;
	public:
	calibrator();
	~calibrator();
	void calibrate_sabre(std::string);
	void MyFill(THashTable *table, std::string name, int binsx, double minx, double maxx, double valuex, int binsy, double miny, double maxy, double valuey);
	void MyFill(THashTable *table, std::string name, int bins, double min, double max, double value);
	
};

