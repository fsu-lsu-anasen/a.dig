#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <string>
#include "DataStructs.h"
#include "ChannelMap.h"
#include <TLegend.h>

/*
 * Class that reads an eventbuilt tree in the context of FSU SPS+SABRE, scours it for
 * SABRE ring/wedge correlations given a ring/wedge index for a particular SABRE detector among the 5.
 * Has the additional capability to save the scatter histogram with a robust ROOT linear fit,
 * and one additional fit with a given slope.
 * 
 * Written by Sudarsan B, sbalak2@lsu.edu 
 * */


class treeProcessor
{
private:
TFile *inFile;
TTree *inTree;
int detectorID;
ProcessedEvent *pevent;

ChannelMap channelMap;
uint64_t nentries;
uint64_t iterator;
int ringGchan;
int wedgeGchan;
TGraph *scatter, *fit;

public:
double getSabreRingE() { return pevent->sabreRingE[detectorID]; };
double getSabreWedgeE(){ return pevent->sabreWedgeE[detectorID];};
uint64_t getNEntries() {return nentries;}
uint64_t getIterator() {return iterator;}
void resetIterator() { iterator = 0; }
~treeProcessor();


void initialize(std::string& path, std::string &chMapFile, int runNumber);
void selectSABREDetector(int detectorID);
//Supply global channel pair
void selectGChannelPair(int rchan, int wchan) {ringGchan = rchan; wedgeGchan=wchan;};
//Supply RingWedge pair
void selectRingWedgePair(int ringchan, int wedgechan);
int findNextEvent();
int savePlot(double_t slope);
};
