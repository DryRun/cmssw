#include "CommonTools/PileupAlgos/interface/MLPDepthCalc.hh"
#include "TMVA/Reader.h"
#include <iostream>
#include <cmath>

using namespace baconhep; 

MLPDepthCalc::MLPDepthCalc():
  fIsInitialized(false),
  fReader(0),
  MethodTag("")
{}
  
MLPDepthCalc::~MLPDepthCalc(){
  delete fReader;
  fIsInitialized = false;
}

void MLPDepthCalc::initialize(const std::string iMethodTag, const std::string iPtWeightFile) {
    MethodTag = iMethodTag;
    if(iPtWeightFile.length()>0) {
      delete fReader;
      std::cout << "apply weights" << std::endl;
      fReader = new TMVA::Reader();
      fReader->AddVariable("eta", &_eta);
      fReader->AddVariable("phi", &_phi);
      fReader->AddVariable("depthFrac0", &_depth0);
      fReader->AddVariable("depthFrac1", &_depth1);
      fReader->AddVariable("depthFrac2", &_depth2);
      fReader->AddVariable("depthFrac3", &_depth3);
      fReader->AddVariable("depthFrac4", &_depth4);
      fReader->AddVariable("depthFrac5", &_depth5);
      fReader->AddVariable("depthFrac6", &_depth6);
      fReader->BookMVA(MethodTag,  iPtWeightFile);
    }
    fIsInitialized = true;
}

double MLPDepthCalc::mvaValue(const float eta, const float phi, const float depth0, const float depth1, const float depth2, const float depth3, const float depth4, const float depth5, const float depth6){
 
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
    val = (fReader ? fReader->EvaluateMVA(MethodTag) : -2);

    return val;
}
