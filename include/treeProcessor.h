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
 * Written by Sudarsan B, sbalak2@lsu.edu 
 * Updated for ANASEN by Keilah D, kdav246@lsu.edu
 * 
 * Class that reads an eventbuilt tree for ANASEN, scours it for QQQ ring/wedge correlations given
 * a ring/wedge index for a particular QQQ detector among the 4. 
 * Has the additional capability to save the scatter histogram with a robust ROOT linear fit, 
 * and one additional fit with a given slope.
 *
 * */


class treeProcessor
{
private:
TFile *inFile;
TTree *inTree;
int detectorID;
CoincEvent *pevent; //no ProcessedEvent struct for ANASEN (yet)

ChannelMap channelMap;
uint64_t nentries;
uint64_t iterator;
int ringGchan;
int wedgeGchan;
TGraph *scatter, *fit;

public:
double getQQQRingE() { return pevent->fqqq[detectorID].rings[0].energy; }; //grabbing ring with biggest energy
double getQQQWedgeE(){ return pevent->fqqq[detectorID].wedges[0].energy;}; //grabbing wedge with biggest energy
uint64_t getNEntries() {return nentries;}
uint64_t getIterator() {return iterator;}
void resetIterator() { iterator = 0; }
~treeProcessor();


void initialize(std::string& path, std::string &chMapFile, int runNumber);
void selectQQQDetector(int detectorID);
//Supply global channel pair
void selectGChannelPair(int rchan, int wchan) {ringGchan = rchan; wedgeGchan=wchan;};
//Supply RingWedge pair
void selectRingWedgePair(int ringchan, int wedgechan);
int findNextEvent();
int savePlot(double_t slope);
};
