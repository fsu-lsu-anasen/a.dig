#include "treeProcessor.h"
#include "likelihoodNet.h"
#include "progressbar2.h"
#include <fstream>
#include <iostream>
R__LOAD_LIBRARY(lib/libSPSDict.so)

int main(int argc, char** argv)
{
	if(argc < 4){
		std::cout << "Usage: ./findCorrelations <runNumber> <input_file> <ringID> <wedgeID>" << std::endl;
		return -1;
	}
	std::ifstream in;
	std::string temp,sourceDir, channelMap;
	int runID, gridpoints, detectorID, r, w;
	double_t minS, maxS;
	
	runID = atoi(argv[1]);
	r = atoi(argv[3]); //atoi converts string to integer
	w = atoi(argv[4]);
	
	std::string file(argv[2]);
	in.open(file.c_str());
	if(in.eof()) {
		std::cout << "No file found at " << file << " .. check input file.." << std::endl ;
		return -1;
	}
		
	while(!in.eof())
	{
		in>>temp>>temp>>sourceDir;
		in>>temp>>channelMap;
		in>>temp>>detectorID;
		in>>temp>>gridpoints;
		in>>temp>>temp>>minS;
		in>>temp>>temp>>maxS;
	}
	
	likelihoodNet net1;
	net1.initialize(gridpoints, minS, maxS, 0.2);

	treeProcessor proc1;
	proc1.initialize(sourceDir,channelMap,runID);
	proc1.selectSABREDetector(detectorID);
		
	proc1.selectRingWedgePair(r,w);
	//proc1.selectGChannelPair(112,42);
	int nentries = proc1.getNEntries();
	int next = 0;
	while(next!=-1){
		next = proc1.findNextEvent();
		if(next == 0) {
			net1.stepForward(static_cast<double_t>(proc1.getSabreRingE()),
						     static_cast<double_t>(proc1.getSabreWedgeE())
						     );
		}
		//uncomment if you need a progressbar
		if(next!=-1) progressbar2(proc1.getIterator(),nentries);		
	}
	double_t maxLkhd_slope = net1.getMaxLikelihoodS();
	std::vector<double_t> results = net1.getS16_50_84();
	
	std::cout  << '\n'  << r << "\t" << w << "\t" << maxLkhd_slope << "\t" ;
	std::cout << results[1] << "+/-(" << results[2]-results[1] << "," << results[1]-results[0] << ")" << std::endl;
	proc1.savePlot(maxLkhd_slope);
	proc1.resetIterator();
	return 0;
}

