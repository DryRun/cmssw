#include "CommonTools/PileupAlgos/interface/BDTDepthCalc.hh"
#include "TMVA/Reader.h"
#include <iostream>
#include <cmath>

using namespace baconhep; 

BDTDepthCalc::BDTDepthCalc():
  fIsInitialized(false),
  fReader(0),
  fMethodTag("BDT")
{}
  
BDTDepthCalc::~BDTDepthCalc(){
  delete fReader;
  fIsInitialized = false;
}

void BDTDepthCalc::initialize(const std::string iMethodTag, const std::string iPtWeightFile) {
    fMethodTag = iMethodTag;
    if(iPtWeightFile.length()>0) {
      if(fReader != 0) delete fReader;
      fReader = new TMVA::Reader();
      //fReader->AddVariable("pt", &_pt);
      fReader->AddVariable("eta", &_eta);
      fReader->AddVariable("phi", &_phi);
      fReader->AddVariable("depthFrac0", &_depth0);
      fReader->AddVariable("depthFrac1", &_depth1);
      fReader->AddVariable("depthFrac2", &_depth2);
      fReader->AddVariable("depthFrac3", &_depth3);
      fReader->AddVariable("depthFrac4", &_depth4);
      fReader->AddVariable("depthFrac5", &_depth5);
      fReader->AddVariable("depthFrac6", &_depth6);
      fReader->BookMVA(fMethodTag,  iPtWeightFile);
    }
    fIsInitialized = true;
}

double BDTDepthCalc::mvaValue(const float eta, const float phi, const float depth0, const float depth1, const float depth2, const float depth3, const float depth4, const float depth5, const float depth6){

    //_pt  = pt; 
    _eta = eta;
    _phi = phi;
    _depth0 = depth0;
    _depth1 = depth1;
    _depth2 = depth2;
    _depth3 = depth3;
    _depth4 = depth4;
    _depth5 = depth5;
    _depth6 = depth6;

    double val = -2;
    val = (fReader ? fReader->EvaluateMVA(fMethodTag) : -2);

    //std::cout << "BDT output: " << val << std::endl;
    return val;
}
