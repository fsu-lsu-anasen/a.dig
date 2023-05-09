
#include "calibrator.h"
//R__LOAD_LIBRARY(lib/libSPSdict.so)

calibrator::calibrator()//:  gMap("etc/gainmatch_June2021_internal.dat"),cMap("./etc/ChannelMap_Feb2021_SABRE.dat")
{}

calibrator::~calibrator(){}

void calibrator::MyFill(THashTable *table, std::string name, int bins, double min, double max, double value) {
  TH1F *histo = (TH1F*) table->FindObject(name.c_str());
  if(histo == NULL) {
    TH1F* h = new TH1F(name.c_str(), name.c_str(), bins, min, max);
    h->Fill(value);
    table->Add(h);
  } else {
    histo->Fill(value);
  }
}

void calibrator::MyFill(THashTable *table, std::string name, int binsx, double minx, double maxx, double valuex, int binsy, double miny, double maxy, double 
valuey) {
  TH2F *histo = (TH2F*) table->FindObject(name.c_str());
  if(histo == NULL) {
    TH2F* h = new TH2F(name.c_str(), name.c_str(), binsx, minx, maxx, binsy, miny, maxy);
    h->Fill(valuex, valuey);
    table->Add(h);
  } else {
    histo->Fill(valuex, valuey);
  }
}

void calibrator::calibrate_qqq(std::string filename) {


  TFile *input = new TFile(filename.c_str(), "READ");
  if(!input->IsOpen()) {
	 std::cout << "Error! Couldn't find file at " << filename << " ... check it exists." << std::endl;
	 return;
  }
  TTree *intree = (TTree*) input->Get("SortTree");
  if(!intree){
	 std::cout << "Error! Couldn't find tree 'SortTree' in specified root file" << std::endl;
	 return;
  }
  ULong64_t ts;
  UShort_t e, c, b, es;
  UInt_t f;
  intree->SetBranchAddress("Timestamp", &ts); 
  intree->SetBranchAddress("Channel", &c); 
  intree->SetBranchAddress("Board", &b); 
  intree->SetBranchAddress("Energy", &e); 
  intree->SetBranchAddress("EnergyShort", &es); 
  intree->SetBranchAddress("Flags", &f); 
//  intree->SetMaxVirtualSize(4000000000);

  TFile *output = TFile::Open("./singles_run_1.root","RECREATE");
  TH1D *h1 = new TH1D("h1","h1",1024,0,8192);
  TH1D *h2 = new TH1D("h2","h2",1024,0,8192);
  TH1D *h3 = new TH1D("h3","h3",1024,0,8192);
  TH1D *h4 = new TH1D("h4","h4",1024,0,8192);
  
  //THashTable *hash = new THashTable();

  int gchan;
  std::cout<<"Total Number of Entries: "<<intree->GetEntries()<<std::endl;
  for(ULong64_t i=0; i<intree->GetEntries(); i++) {
    intree->GetEntry(i);
    if(i%1000 == 0) std::cout<<"\rNumber of entries processed: "<<i<< " " << std::flush;
	gchan = c + b*16;
    //auto chanGain = gMap.FindParameters(gchan);
    
    //selects the 4 rings (one per detector) with the least amount of straggling for a centered source
    if(e>100){
    if(b==3 && c==15) h1->Fill(e);
    if(b==4 && c==0) h2->Fill(e);
    if(b==5 && c==0) h3->Fill(e);
    if(b==6 && c==0) h4->Fill(e);
	}
    //auto chanConfig = ;
    //if(cMap.FindChannel(gchan)->second.detectorType==SABRERING && cMap.FindChannel(gchan)->second.detectorID==0) {
	   //auto chanGain = gMap.FindParameters(gchan);
      // MyFill(hash, Form("Board_%d_Channel0",b), 4096, 0, 4095, e);//*chanGain->second
   // }
  }
  std::cout << std::endl;

  input->Close();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();

  //scale those 4 rings to an alpha peak so 1 keV = 1 channel
  //TODO: look into ROOT fitting functions (gaussian, exp. modified gaus, or crystal ball?)
  std::cout << " Global scale factors needed to align 5.486MeV peak:" << std::endl; //TODO: probably wrong peak
  std::cout << "----------------------------------------" << std::endl;
  std::cout << " 3." << 5486./h1->GetXaxis()->GetBinCenter(h1->GetMaximumBin()) << std::endl;
  std::cout << " 2." << 5486./h2->GetXaxis()->GetBinCenter(h2->GetMaximumBin()) << std::endl;
  std::cout << " 1." << 5486./h3->GetXaxis()->GetBinCenter(h3->GetMaximumBin()) << std::endl;
  std::cout << " 0." << 5486./h4->GetXaxis()->GetBinCenter(h4->GetMaximumBin()) << std::endl;

  std::ofstream ofile("./etc/global_gain_scalefactors.dat");
  ofile << " 3\t" << 5486./h1->GetXaxis()->GetBinCenter(h1->GetMaximumBin()) << std::endl;
  ofile << " 2\t" << 5486./h2->GetXaxis()->GetBinCenter(h2->GetMaximumBin()) << std::endl;
  ofile << " 1\t" << 5486./h3->GetXaxis()->GetBinCenter(h3->GetMaximumBin()) << std::endl;
  ofile << " 0\t" << 5486./h4->GetXaxis()->GetBinCenter(h4->GetMaximumBin()) << std::endl;
  ofile.close();
  output->Close();
}



int main(int argc, char** argv)
{
if(argc < 2){
std::cout << " Usage: ./calibrator <raw compass rootfile with Am241 spectrum>" << std::endl; //TODO: probably wrong spectrum
return -1;
}

std::string filename(argv[1]);
calibrator c;
c.calibrate_qqq(filename);
return 0 ;
}
