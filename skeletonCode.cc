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

double getSnr(double* ydata, int npoints);
double getPeak(TGraph *gr);
int main(int argc, char **argv) {    // this is for manual power threshold value


  TChain chain("AraTree");
  TChain chain2("AraTree2");
    string readfile;
  if (argc<2) { //no output, use default
      readfile = "outputs/AraOut.root";
  }

  else { // no mode for argc > 2!
     for(int i=1; i<argc; i++){
        chain.Add( argv[i] );
        chain2.Add( argv[i] );
     }

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


   chain.SetBranchAddress("settings",&settings);
   chain.SetBranchAddress("detector",&detector);
   chain.SetBranchAddress("trigger" ,&trigger);
   chain.SetBranchAddress("spectra",&spectra);
   chain.SetBranchAddress("icemodel",&icemodel);
   chain2.SetBranchAddress("report" ,&report);
   chain2.SetBranchAddress("event"  ,&event);

   chain.GetEntry(0);
   cout<<"EXPONENT: "<<settings->EXPONENT<<endl;
   cout<<"NNU: "<<settings->NNU<<endl;
   //printf("Station center X: %f Y: %f Z: %f\n",detector->stations[0].GetX(),detector->stations[0].GetY(),detector->stations[0].GetZ());

   int runEventCount = chain2.GetEntries();
   cout<<"Num Events "<<runEventCount<<endl;


  //TFile *AraFile=new TFile(( readfile ).c_str());
  //TFile *AraFile=new TFile((outputdir+"/AraOut.root").c_str());
  //cout<<"AraFile"<<endl;
  //TTree *AraTree=(TTree*)AraFile->Get("AraTree");
  //TTree *AraTree2=(TTree*)AraFile->Get("AraTree2");
  //cout<<"AraTree"<<endl;
  // AraTree->SetBranchAddress("detector",&detector);
  // AraTree->SetBranchAddress("settings",&settings);
  // AraTree->SetBranchAddress("spectra",&spectra);
  // AraTree->SetBranchAddress("icemodel",&icemodel);
  // AraTree->SetBranchAddress("trigger",&trigger);
  // AraTree2->SetBranchAddress("event",&event);
  // AraTree2->SetBranchAddress("report",&report);
  //cout<<"branch detector"<<endl;
  int bin = settings->NFOUR/2;
  bin = 680;
  chain.GetEvent(0);
  cout<<"getevent"<<endl;
  cout << "I'm here.\n";
  //cout << "GetGain(700,5,0,0) is " << detector->GetGain(700., 5., 0., 0) << "\n";
  //cout << "GetGain(10,5,0,0) is " << detector->GetGain(10., 5., 0., 0) << "\n";

  // cout<<"station x is "<<detector->stations[0].GetX()<<endl;
  // cout<<"string x is "<<detector->stations[0].strings[0].GetX()<<endl;
  // cout<<"antenna x is "<<detector->stations[0].strings[0].antennas[0].GetX()<<endl;
  //cout<<"antenna Gain(700,5,0) is "<<detector->stations[0].strings[0].antennas[0].GetG(detector,700.,5.,0.)<<endl;

  //Get the station coordinates and string coordinates printed out.

/* This should be a part of the skeleton code, in case you want to look back

  cout<<"station x is "<<detector->stations[0].GetX()<<endl;
  cout<<"station y is "<<detector->stations[0].GetY()<<endl;
  cout<<"station z is "<<detector->stations[0].GetZ()<<endl;
  double depth = detector->stations[0].GetZ();
  for (int i = 0; i < detector->params.number_of_strings;i++){
    cout<<" String "<<i<<" X = "<<(double)detector->stations[0].strings[i].GetX()<<endl;
    cout<<" String "<<i<<" Y = "<<(double)detector->stations[0].strings[i].GetY()<<endl;
    for (int j = 0; j < detector->params.number_of_antennas_string;j++){
      cout<<" String "<<i<<" Antenna "<<j<<" Z = "<<(double)detector->stations[0].strings[i].antennas[j].GetZ() - depth<<endl;
    }
  }
*/

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

  chain.GetEvent(0);
  chain2.GetEvent(0);
  cout<<"nnu x : "<<event->Nu_Interaction[0].nnu.GetX()<<endl;
  chain.GetEvent(1);
  cout<<"nnu x : "<<event->Nu_Interaction[0].nnu.GetX()<<endl;

  cout<<"Vmmhz size "<<report->stations[0].strings[0].antennas[0].vmmhz.size()<<endl;
  //cout<<"Vmmhz value "<<report->stations[0].strings[0].antennas[0].vmmhz[1].size()<<endl;



  int nnu_pass = 0; // number of nu events which passed PickUnbiased.
  double posnuX;
  double posnuY;
  double posnuZ[settings->NNU];
  double posnuR[settings->NNU];
  double snr[settings->NNU];
  double posnuTheta;
  double posnuPhi;

  double radius[settings->NNU];

  double dirnuX;
  double dirnuY;
  double dirnuZ;
  double dirnuR;
  double dirnuTheta;
  double dirnuPhi;

  double hpol[7][settings->NNU];
  double vpol[7][settings->NNU];

  int numEvents = settings->NNU;
  string current;

  vector <double> vmmhz1m;
  vector <int> signalBin;

  double emfrac;
  double hadfrac;





  double posnuX_max=-icemodel->R_EARTH;
  double posnuY_max=-icemodel->R_EARTH;

  double posnuX_min=icemodel->R_EARTH;
  double posnuY_min=icemodel->R_EARTH;


  ofstream outputFile;
  char nameofOutputFile[200];
  sprintf(nameofOutputFile,"eventInfo.txt");
  outputFile.open(nameofOutputFile);

  ///////////////////////////////////////////
  //  test Detector class
  ///////////////////////////////////////////


  double wf_time_offset = -450.;// in ns
  double offSet = (double)detector->stations[0].GetZ();
  double offSetX = (double)detector->stations[0].GetX();
  double offSetY = (double)detector->stations[0].GetY();
  //double offSetR = (double)detector->stations[0].strings[0].antennas[3].R();
  cout<<"OffSet = "<<(long)offSet<<endl;
  //double hpolTemp1; double hpolTemp2; double vpolTemp;
  double radDistance;
  if ( settings->DETECTOR != 0 ) {


      for (int inu=0;inu<runEventCount;inu++) { // loop over neutrinos


          chain2.GetEvent(inu); // this is for report and event class
          chain.GetEvent(inu); // this is for trigger, detector and other classes
          //cout<<"In the event loop"<<endl;
          // save X, Y of posnus which passed PickUnbiased
          if ( report->stations[0].Global_Pass > 0) {
//            if ( event->Nu_Interaction[0].pickposnu ) {
              //The position of all the events
              cout<<"In the global pass"<<endl;
              posnuX = event->Nu_Interaction[0].posnu.GetX() - offSetX;
              posnuY = event->Nu_Interaction[0].posnu.GetY() - offSetY;
              posnuZ[nnu_pass] = event->Nu_Interaction[0].posnu.GetZ() - offSet;
              radDistance = sqrt(posnuX*posnuX + posnuY*posnuY );
              //cout<<"Distance calculated from position "<<radDistance<<endl;
              cout<<"Pos nu Z "<<posnuZ[nnu_pass]<<endl;
              posnuR[nnu_pass] = radDistance;
              cout<<"Pos nu R " << posnuR[nnu_pass]<<endl;
              //posnuTheta = event->Nu_Interaction[0].posnu.Theta();
              //posnuPhi = event->Nu_Interaction[0].posnu.Phi();

              // if (posnuX[nnu_pass] > posnuX_max) posnuX_max = posnuX[nnu_pass];
              // if (posnuX[nnu_pass] < posnuX_min) posnuX_min = posnuX[nnu_pass];
              //
              // if (posnuY[nnu_pass] > posnuY_max) posnuY_max = posnuY[nnu_pass];
              // if (posnuY[nnu_pass] < posnuY_min) posnuY_min = posnuY[nnu_pass];
              //cout<<"Herea as well"<<endl;
              dirnuX = event->Nu_Interaction[0].nnu.GetX();
              dirnuY = event->Nu_Interaction[0].nnu.GetY();
              dirnuZ = event->Nu_Interaction[0].nnu.GetZ();
              dirnuR = event->Nu_Interaction[0].nnu.R();
              dirnuTheta = event->Nu_Interaction[0].nnu.Theta();
              dirnuPhi = event->Nu_Interaction[0].nnu.Phi();
              cout<<"Direction of neutrino "<<dirnuX<<" "<<dirnuY<<" "<<dirnuZ<<endl;
              radius[nnu_pass] = report->stations[0].strings[0].antennas[3].Dist[0];
              cout<<"Distance  = "<<radius[nnu_pass]<<endl;
              double getx[bin];
              double gety[bin];
              cout<<"Bin "<<bin<<endl;
              int numAntennas = 7;
              double tempSNR = 0;
              for (int ant = 0; ant < numAntennas; ant++ ){
              for (int l=0; l<bin; l++) {
                  getx[l] = report->stations[0].strings[0].antennas[ant].time_mimic[l];// in ns
                  gety[l] = report->stations[0].strings[0].antennas[ant].waveformVoltage[l];
              }
              //cout<<"A random Bin value "<<gety[150]<<endl;
              tempSNR = tempSNR + getSnr(gety,bin);
            }
              snr[nnu_pass] = tempSNR/numAntennas;
              cout<<"Average SNR "<<snr[nnu_pass]<<endl;
              cout<<"Stored SNR = "<<report->stations[0].strings[0].averageSNR<<endl;
              //emfrac = event->Nu_Interaction[0].emfrac;
              //hadfrac = event->Nu_Interaction[0].hadfrac;
              //radius[nnu_pass] = report->stations[0].strings[0].antennas[0].Dist[0];
              //signalBin = report->signal_bin;
              //current = event->Nu_Interaction[0].current;
              // for(int i = 0; i < 7; i++){
              //   vpolTemp = report->stations[0].strings[0].antennas[i].Pol_vector[3].GetZ();
              //   hpolTemp1 = report->stations[0].strings[0].antennas[i].Pol_vector[3].GetY();
              //   hpolTemp2 = report->stations[0].strings[0].antennas[i].Pol_vector[3].GetX();
              //   hpol[i][nnu_pass] = sqrt((hpolTemp1*hpolTemp1) + (hpolTemp2*hpolTemp2));
              //   vpol[i][nnu_pass] = vpolTemp;
              //   cout<<"Vpol : "<<vpolTemp<<" Hpol : "<<hpol[i][nnu_pass]<<"Total Magnitude : "<<sqrt((vpolTemp*vpolTemp) + (hpol[i][nnu_pass]*hpol[i][nnu_pass]))<<endl;
              // }
              // outputFile<<"Event Number "<<nnu_pass<<endl;
              //
              // outputFile<<"Position (Cartesian) (X,Y,Z) :"<<posnuX<<", "<<posnuY<<", "<<posnuZ<<endl;
              // outputFile<<"Position (Spherical) (R, Theta, Phi) :"<<posnuR<<" "<<posnuTheta<<" "<<posnuPhi<<endl;
              // outputFile<<"Direction (Cartesian)  :"<<dirnuX<<", "<<dirnuY<<", "<<dirnuZ<<endl;
              // outputFile<<"Position (Spherical)  :"<<dirnuR<<" "<<dirnuTheta<<" "<<dirnuPhi<<endl;
              // outputFile<<"Em frac : "<<emfrac<<" Hadronic frac : "<<hadfrac<<endl;
              // outputFile<<"Type of Interaction "<<current;
              // outputFile<<endl;
              // outputFile<<" Waveforms - without Diode Convolution"<<endl;
              // for (int i = 0; i < 4; i++){
              //   for (int j = 0; j < 4; j++){
              //     for (int l=0; l<bin; l++) {
              //       outputFile<<report->stations[0].strings[i].antennas[j].V_mimic[l]<<" ";
              //     }
              //   }
              // }
              // outputFile<<endl;



              //outputFile<<endl;
              //outputFile<<endl;
              cout<<"NNU Pass "<<nnu_pass<<endl;
              nnu_pass++;

          }
      }


  }
  //cout<<"Got here for now"<<endl;
  TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,1000,800);
  TGraph *gr_radiusDepth;
  //cout<<"Got here as well"<<endl;
  gr_radiusDepth = new TGraph(numEvents,radius,posnuZ);
  gr_radiusDepth->SetTitle("Radius vs Depth for triggered Events");
  //gr_string->GetHistogram()->SetMaximum(5300);
  //gr_string->GetHistogram()->SetMinimum(5100);
  //gr_string->GetXaxis()->SetLimits(-3100,-2900);
  //gr_radiusDepth->GetHistogram()->SetMaximum( (int)detector->stations[station_choice].strings[string_choice].GetZ() );
  //gr_radiusDepth->GetHistogram()->SetMaximum(8000);
  //gr_radiusDepth->GetHistogram()->SetMinimum(0);
  //gr_radiusDepth->GetXaxis()->SetLimits( (int)detector->stations[station_choice].GetX() - 100, (int)detector->stations[station_choice].GetX() + 100 );
  gr_radiusDepth->GetYaxis()->SetTitle("Z (depth, m)");
  gr_radiusDepth->SetMarkerColor(4);
  gr_radiusDepth->SetMarkerSize(0.5);
  gr_radiusDepth->SetMarkerStyle(20);
  gr_radiusDepth->GetHistogram()->SetXTitle("Radius (m)");
  gr_radiusDepth->GetYaxis()->SetTitleOffset(1.2);
  //cout<<"Got here"<<endl;
  gr_radiusDepth->Draw("ap");

  c1->Print("/home/rkhandelwal/public_html/phasedArrayPlots/PAradiusDepthE18.pdf");
}

double getPeak(TGraph *gr)
{
  double x,y;
  gr->GetPoint(0,x,y);
  double peakVal=y*y;
  int peakBin=0;
  for(int i=1;i<gr->GetN();i++) {
    gr->GetPoint(i,x,y);
    if( peakVal<(y*y) ) {
      peakVal=(y*y);
      peakBin=i;
    }
  }
  //return peakBin;
  return sqrt(peakVal);
}

double getSnr(double *ydata, int npoints)
{
  double snr = 0.0;
  double noiseRMS = 0.04;
  double peak = 0.0;
  for (int bin=0; bin<npoints; bin++) {   //BINSIZE should be NFOUR/2
      if (TMath::Abs(ydata[bin]) > peak) {
        peak = ydata[bin];
      }
  }
  peak = peak;
  cout<<"Peak Value : "<<peak<<endl;
  snr = 1.0*peak/(noiseRMS);
  return snr;
}
