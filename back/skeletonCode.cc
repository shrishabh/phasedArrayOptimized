#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "TTreeIndex.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TRandom.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLine.h"
#include "TROOT.h"
#include "TPostScript.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TText.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include <unistd.h>
#include "TVector3.h"
#include "TRotation.h"
#include "TSpline.h"
#include "Math/Integrator.h"

//#include <fftw3.h>

using namespace std;

#include "Tools.h"
#include "Constants.h"
#include "Vector.h"
#include "Position.h"
#include "EarthModel.h"
#include "IceModel.h"
#include "Efficiencies.h"
#include "Spectra.h"
#include "Event.h"
#include "Trigger.h"
#include "Detector.h"
#include "Settings.h"
#include "counting.hh"
#include "Primaries.h"
#include "Report.h"

#include "Ray.h"

class EarthModel; //class


int main(int argc, char **argv) {    // this is for manual power threshold value



    string readfile;
  if (argc<2) { // no setup file input, use default
      readfile = "outputs/AraOut.root";
  }
  else if (argc == 2) { // read file!!
      readfile = string( argv[1] );
  }
  else { // no mode for argc > 2!
      cout<<"too many info! just use default AraOut.root file!"<<endl;
      readfile = "outputs/AraOut.root";
  }

  //TODO : Modify this code to take in multiple input files using TChain

  //  Settings *settings = new Settings();

  //  Detector *detector=new Detector(settings->DETECTOR); // builds antenna array, 0 for testbed
  //  Detector *detector=0; // builds antenna array, 0 for testbed
  Detector *detector = 0;
  Settings *settings = 0;
  Spectra *spectra = 0;
  IceModel *icemodel = 0;
  Event *event = 0;
  Report *report = 0;
  Trigger *trigger = 0;
  cout<<"construct detector"<<endl;


  TFile *AraFile=new TFile(( readfile ).c_str());
  //TFile *AraFile=new TFile((outputdir+"/AraOut.root").c_str());
  cout<<"AraFile"<<endl;
  TTree *AraTree=(TTree*)AraFile->Get("AraTree");
  TTree *AraTree2=(TTree*)AraFile->Get("AraTree2");
  cout<<"AraTree"<<endl;
  AraTree->SetBranchAddress("detector",&detector);
  AraTree->SetBranchAddress("settings",&settings);
  AraTree->SetBranchAddress("spectra",&spectra);
  AraTree->SetBranchAddress("icemodel",&icemodel);
  AraTree->SetBranchAddress("trigger",&trigger);
  AraTree2->SetBranchAddress("event",&event);
  AraTree2->SetBranchAddress("report",&report);
  cout<<"branch detector"<<endl;

  AraTree->GetEvent(0);
  cout<<"getevent"<<endl;
  cout << "I'm here.\n";
  //cout << "GetGain(700,5,0,0) is " << detector->GetGain(700., 5., 0., 0) << "\n";
  //cout << "GetGain(10,5,0,0) is " << detector->GetGain(10., 5., 0., 0) << "\n";

  cout<<"station x is "<<detector->stations[0].GetX()<<endl;
  cout<<"string x is "<<detector->stations[0].strings[0].GetX()<<endl;
  cout<<"antenna x is "<<detector->stations[0].strings[0].antennas[0].GetX()<<endl;
  //cout<<"antenna Gain(700,5,0) is "<<detector->stations[0].strings[0].antennas[0].GetG(detector,700.,5.,0.)<<endl;

  cout<<"params.number_of_stations : "<<detector->params.number_of_stations<<endl;
  cout<<"params.station_spacing : "<<detector->params.station_spacing<<endl;

  cout<<"\n"<<endl;
  cout<<"Settings->NNU : "<<settings->NNU<<endl;
  cout<<"Settings->DETECTOR : "<<settings->DETECTOR<<endl;
  cout<<"Settings->EXPONENT : "<<settings->EXPONENT<<endl;


  cout<<"random energy from Spectra : "<<spectra->GetNuEnergy()<<endl;

  cout<<"Detector static const double freq_init : "<<detector->Getfreq_init()<<endl;


  cout<<"Detector -> freq_forfft[0] : "<<detector->freq_forfft[0]<<endl;
  cout<<"Detector -> freq_forfft[9] : "<<detector->freq_forfft[9]<<endl;
  cout<<"Detector -> freq_forfft[100] : "<<detector->freq_forfft[100]<<endl;

  cout<<"icemodel surface : "<<icemodel->Surface(0.,0.)<<endl;

  AraTree2->GetEvent(0);
  cout<<"nnu x : "<<event->Nu_Interaction[0].nnu.GetX()<<endl;
  AraTree2->GetEvent(1);
  cout<<"nnu x : "<<event->Nu_Interaction[0].nnu.GetX()<<endl;

  int nnu_pass = 0; // number of nu events which passed PickUnbiased.
  double posnuX[settings->NNU];
  double posnuY[settings->NNU];
  double posnuR[settings->NNU];
  double posnuX_max=-icemodel->R_EARTH;
  double posnuY_max=-icemodel->R_EARTH;

  double posnuX_min=icemodel->R_EARTH;
  double posnuY_min=icemodel->R_EARTH;




  ///////////////////////////////////////////
  //  test Detector class
  ///////////////////////////////////////////




  if ( settings->DETECTOR != 0 ) {


      for (int inu=0;inu<settings->NNU;inu++) { // loop over neutrinos


          AraTree2->GetEvent(inu);

          // save X, Y of posnus which passed PickUnbiased
          if ( event->Nu_Interaction[0].pickposnu ) {
              posnuX[nnu_pass] = event->Nu_Interaction[0].posnu.GetX();
              posnuY[nnu_pass] = event->Nu_Interaction[0].posnu.GetY();
              posnuR[nnu_pass] = event->Nu_Interaction[0].posnu.R();

              if (posnuX[nnu_pass] > posnuX_max) posnuX_max = posnuX[nnu_pass];
              if (posnuX[nnu_pass] < posnuX_min) posnuX_min = posnuX[nnu_pass];

              if (posnuY[nnu_pass] > posnuY_max) posnuY_max = posnuY[nnu_pass];
              if (posnuY[nnu_pass] < posnuY_min) posnuY_min = posnuY[nnu_pass];

              nnu_pass++;

          }
      }


  }


}
