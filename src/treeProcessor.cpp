#include "treeProcessor.h"
/*
 * Class that reads an eventbuilt tree in the context of FSU SPS+SABRE, scours it for
 * SABRE ring/wedge correlations given a ring/wedge index for a particular SABRE detector among the 5.
 * Has the additional capability to save the scatter histogram with a robust ROOT linear fit,
 * and one additional fit with a given slope.
 * 
 * Written by Sudarsan B, sbalak2@lsu.edu 
 * */


/* Initialize channelmap, set root directory to where the analyzed ROOT files live, 
 and select the run number to use
 */
void treeProcessor::initialize(std::string& rootpath, std::string &chMapFile, int runNumber)
{
	channelMap.FillMap(chMapFile);
	pevent = new ProcessedEvent;
	inFile = new TFile(Form("%s/run_%d.root",rootpath.c_str(),runNumber));
	if(!inFile) { std::cout << "No ROOT file found!"; return; }
	inTree = static_cast<TTree*>(inFile->Get("SPSTree"));
	if(!inTree) { std::cout << "No TTree found in file!"; return; }
	inTree->SetBranchAddress("event",&pevent);

	scatter = new TGraph(); // TGraph that stores the scatter
	fit = new TGraph(); // TGraph that will store the externally provided slope's fit
	
	selectSABREDetector(0);
	nentries = inTree->GetEntries();
	iterator = 0;
	//inTree->SetMaxVirtualSize(4000000000LL);
	//inTree->LoadBaskets(4000000000LL);

}

void treeProcessor::selectSABREDetector(int ID)
{
	detectorID=ID;
}

treeProcessor::~treeProcessor()
{
	inFile->Close();
}

/*Self explanatory, scours the channelmap to find the channelnumber*/
void treeProcessor::selectRingWedgePair(int ringchan, int wedgechan)
{
	int i=0;
	int found2=0;
	for(auto iter = channelMap.Begin(); iter != channelMap.End(); iter++, i++)
	{
		auto chaninfo = channelMap.FindChannel(i);
		if(chaninfo->second.detectorType==SABREWEDGE && chaninfo->second.detectorPart==wedgechan && chaninfo->second.detectorID==detectorID){ 
			wedgeGchan=i;
			found2++;
		}
		if(chaninfo->second.detectorType==SABRERING && chaninfo->second.detectorPart==ringchan&& chaninfo->second.detectorID==detectorID){
			 ringGchan=i;
			 found2++;
		}
		if(found2==2) break; //save some time
	}
}

/*Find and add to the scatter TGraph, points with the right ring/wedge energies.*/
int treeProcessor::findNextEvent()
{
	//while(iterator < nentries)
	{
		inTree->GetEntry(iterator);
		if(pevent->sabreRingChannel[detectorID]==ringGchan 
		 && pevent->sabreWedgeChannel[detectorID]==wedgeGchan
		 &&	pevent->sabreArray[detectorID].rings.size()==1  //Multiplicity must be 1 to be safe
		 && pevent->sabreArray[detectorID].wedges.size()==1
//		 &&	pevent->sabreRingMultiplicity[detectorID]==1  //Multiplicity must be 1 to be safe
//		 && pevent->sabreWedgeMultiplicity[detectorID]==1
		 && pevent->sabreRingE[detectorID]>0  //Avoid pileup/saturation/error events
		 && pevent->sabreWedgeE[detectorID]>0)
			{
				//std::cout << "\noutputs: " << (pevent->sabreArray[detectorID].rings.size()==1 && pevent->sabreArray[detectorID].wedges.size()) << " " << iterator << " " << inTree->GetEntries() << std::endl;
				//std::cout << "mult:" << iterator << " " << pevent->sabreArray[detectorID].rings.size() << " " << pevent->sabreRingE[detectorID] << std::endl;
				scatter->SetPoint(scatter->GetN()+1,pevent->sabreRingE[detectorID],pevent->sabreWedgeE[detectorID]);
				iterator ++;
				if(iterator >= nentries) return -1; //end of file
				return 0;
			}
		else iterator++;
	}
	if(iterator >= nentries) return -1; //end of file
	iterator++;
	return 1;
}


/* Miscellaneous  function that accepts a slope and plots the following:
 * 1. scatter of ring vs wedge
 * 2. ROB=0.9 fit 
 * 3. line with the given slope. This can be made to be the max likelihood slope.
 */
int treeProcessor::savePlot(double_t slope)
{
	double_t x, max, min;
	max = TMath::MaxElement(scatter->GetN(),scatter->GetX()); //get minimum in the X array
	min = TMath::MinElement(scatter->GetN(),scatter->GetX()); //get maximum in the X array
	std::cout << max << " " << min << std::endl;
	for(int i=0; i<1000; i++){
		x = (max-min)*(static_cast<double_t>(i+1)/1000.0);
		fit->SetPoint(i,x,slope*x); //fill with y=slope*x
	}
	//TF1 pol1("root fit, migrad","pol1",1,1);
	TF1* pol1 = new TF1("root fit, ROB=0.9","[0]*x",0,2000);
	pol1->SetParameter(0,1.0); //Set initial value to be 1.0

	TMultiGraph* mg = new TMultiGraph();
	scatter->SetLineColor(1);
	scatter->SetMarkerSize(1);
	scatter->SetMarkerStyle(8);
	mg->Add(scatter,"p");
	//scatter->Fit(&pol1,"R");
	//pol1->SetTitle("Robust fit with ROB=0.94");
	scatter->Fit(pol1,"ROB=0.94");
	
	scatter->SetTitle("Raw data");	
	mg->Add(scatter,"sames");
	fit->SetLineColor(3);
	//fit->SetTitle("Cauchy-Lorentz max. lkhd");
	mg->Add(fit,"l");


	auto rinfo = channelMap.FindChannel(ringGchan);
	auto winfo = channelMap.FindChannel(wedgeGchan);
	
	TLegend *legend = new TLegend(0.2, 0.65, 0.6,0.85);
	legend->SetTextSize(0.04);
	legend->SetHeader(Form("bd%d: (r%d,w%d)",detectorID, rinfo->second.detectorPart,winfo->second.detectorPart));
	legend->AddEntry(scatter,"Raw data","p");
	legend->AddEntry(pol1,"ROB=0.94 fit","l");
	legend->AddEntry(fit,"Cauchy-Lorentz max. lkhd","l");

	TCanvas* c = new TCanvas();
	c->cd();
	mg->Draw("a");
	legend->Draw();
	c->Modified();
	c->Update();
	c->SetGrid();
	
	int temp = gErrorIgnoreLevel;	
	gErrorIgnoreLevel = kWarning;//Suppress the annoying info message
	c->SaveAs(Form("test_detector%d_rng%d_wdg%d.png",detectorID,rinfo->second.detectorPart,winfo->second.detectorPart));
	gErrorIgnoreLevel = temp; //no delete necessary to pol1, c, and mg because ROOT automatically deletes them
	// if put in, leads to a persistent warning.
	return 0;
	 
}
