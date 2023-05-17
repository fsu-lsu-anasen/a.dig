
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

//sorts peak positions from lowest to highest
bool SortPosition(const double i, const double j)
{
  return i<j;
}

std::vector<double> calibrator::GetPeaks(TH1D *hist)
{
  double xmax = hist->GetMaximum();
  int binmax = hist->GetNbinsX();
  int bintemp;
  xVec.clear(); //so we can reuse
  //threshold set high enough so only center bins of 2 highest peaks get found
  while(hist->FindLastBinAbove(xmax*0.9,1,1,binmax)>-1) //FindLastBinAbove(threshold,1,firstBin,lastBin) returns -1 if none found
  {
    //grab that bin
    bintemp = hist->FindLastBinAbove(xmax*0.9,1,1,binmax);
    //push the bin center to vector of x values
    xVec.push_back(hist->GetXaxis()->GetBinCenter(bintemp));
    //reset binmax
    binmax = bintemp-1;
  }
  std::sort(xVec.begin(), xVec.end(), SortPosition); //sort lowest to highest
  return xVec;
}

//TODO: look into ROOT fitting functions (gaussian, exp. modified gaus, or crystal ball?)
//TODO: spot check --> pick a few channels to fit (i.e. gaussian) and compare to what I get with above method
//TODO: could also do the calibration and look for ones to fix individually by hand
void calibrator::FitPeaks(){
  //do stuff
}

void calibrator::calibrate_qqq(std::string filename) {


  TFile *input = new TFile(filename.c_str(), "READ");
  if(!input->IsOpen()) {
	 std::cout << "Error! Couldn't find file at " << filename << " ... check it exists." << std::endl;
	 return;
  }
  TTree *intree = (TTree*) input->Get("Data");
  if(!intree){
	 std::cout << "Error! Couldn't find tree 'Data' in specified root file" << std::endl;
	 return;
  }
  ULong_t ts;
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
  TH1D *h1 = new TH1D("h1","h1",1024,0,4096);
  TH1D *h2 = new TH1D("h2","h2",1024,0,4096);
  TH1D *h3 = new TH1D("h3","h3",1024,0,4096);
  TH1D *h4 = new TH1D("h4","h4",1024,0,4096);
  
  //THashTable *hash = new THashTable();

  int gchan;
  std::cout<<"Total Number of Entries: "<<intree->GetEntries()<<std::endl;
  for(ULong64_t i=0; i<intree->GetEntries(); i++) {
    intree->GetEntry(i);
    if(i%1000 == 0) std::cout<<"\rNumber of entries processed: "<<i<< " " << std::flush;
	gchan = c + b*16;
    //auto chanGain = gMap.FindParameters(gchan);
    
    //UPDATE WITH EACH EXPERIMENT: select the 4 rings (one per detector) with the least amount of straggling for a centered source
    if(e>100){ //threshold cuts out noise
    if(b==0 && c==10) h1->Fill(e);
    if(b==0 && c==26) h2->Fill(e);
    if(b==5 && c==10) h3->Fill(e);
    if(b==5 && c==26) h4->Fill(e);
	}
    //auto chanConfig = ;
    //if(cMap.FindChannel(gchan)->second.detectorType==SABRERING && cMap.FindChannel(gchan)->second.detectorID==0) {
	   //auto chanGain = gMap.FindParameters(gchan);
      // MyFill(hash, Form("Board_%d_Channel0",b), 4096, 0, 4095, e);//*chanGain->second
   // }
  }
  std::cout << std::endl;

  //get alpha peak positions from histograms
  std::vector<double> x1 = GetPeaks(h1);
  std::vector<double> x2 = GetPeaks(h2);
  std::vector<double> x3 = GetPeaks(h3);
  std::vector<double> x4 = GetPeaks(h4);

  input->Close();
  output->cd();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  output->Close();

  //scale those 4 rings to an alpha peak so 1 keV = 1 channel
  //with triple alpha source, the 2nd peak (i.e. x1[1]) is Am-241
  //TODO: start with only fitting one peak from triple alpha spectrum and see how it goes
  std::cout << " Global scale factors needed to align Am-241 5.486MeV peak:" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << " 3." << " 5486./" << x1[1] << std::endl;
  std::cout << " 2." << " 5486./" << x2[1] << std::endl;
  std::cout << " 1." << " 5486./" << x3[1] << std::endl;
  std::cout << " 0." << " 5486./" << x4[1] << std::endl;

  std::ofstream ofile("./etc/global_gain_scalefactors.dat");
  ofile << " 3\t" << 5486./x1[1] << std::endl;
  ofile << " 2\t" << 5486./x2[1] << std::endl;
  ofile << " 1\t" << 5486./x3[1] << std::endl;
  ofile << " 0\t" << 5486./x4[1] << std::endl;
  ofile.close();
}



int main(int argc, char** argv)
{
if(argc < 2){
std::cout << " Usage: ./bin/calibrator <raw compass rootfile with alpha spectrum>" << std::endl;
return -1;
}

std::string filename(argv[1]);
calibrator c;
c.calibrate_qqq(filename);
return 0 ;
}
