#include "CommonTools/PileupAlgos/interface/DepthNNId.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include <iostream>
#include <cmath>


//--------------------------------------------------------------------------------------------------
DepthNNId::DepthNNId()
{
  NNvectorVar_.clear();
}

//--------------------------------------------------------------------------------------------------
DepthNNId::~DepthNNId() {
  tensorflow::closeSession(session);
  delete graphDef;
}

void DepthNNId::initialize(const std::string iWeightFile){
  std::string cmssw_base_src = getenv("CMSSW_BASE");
  graphDef= tensorflow::loadGraphDef((cmssw_base_src+"/src/"+iWeightFile).c_str());
  session = tensorflow::createSession(graphDef);
}

void DepthNNId::SetNNVectorVar(){
    NNvectorVar_.clear();
    //NNvectorVar_.push_back(fEcal) ;
    NNvectorVar_.push_back(fEta) ;
    NNvectorVar_.push_back(fPhi) ;
    NNvectorVar_.push_back(fDepth1) ;
    NNvectorVar_.push_back(fDepth2) ;
    NNvectorVar_.push_back(fDepth3) ;
    NNvectorVar_.push_back(fDepth4) ;
    NNvectorVar_.push_back(fDepth5) ;
    NNvectorVar_.push_back(fDepth6) ;
    NNvectorVar_.push_back(fDepth7) ;
}

float DepthNNId::EvaluateNN(){
    tensorflow::Tensor input(tensorflow::DT_FLOAT, {1,(unsigned int)NNvectorVar_.size()});//was {1,35} but get size mismatch, CHECK
    for (unsigned int i = 0; i < NNvectorVar_.size(); i++){
      //std::cout<<"i:"<<i<<" x:"<<NNvectorVar_[i]<<std::endl;
        input.matrix<float>()(0,i) =  float(NNvectorVar_[i]);
    }
    std::vector<tensorflow::Tensor> outputs;
    tensorflow::run(session, { { "dense_1_input:0",input } }, { "dense_6/Sigmoid:0" }, &outputs);
    //std::cout << "===> result " << outputs[0].matrix<float>()(0, 0) << std::endl;
    float disc = outputs[0].matrix<float>()(0, 0);
    return disc;

}//end EvaluateNN
