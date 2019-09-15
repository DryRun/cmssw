#ifndef COMMONTOOLS_PILEUPALGOS_DEPTHNNID_HH
#define COMMONTOOLS_PILEUPALGOS_DEPTHNNID_HH

#include <string>
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

class DepthNNId  {
    public:
      DepthNNId();
      ~DepthNNId();
      
      void initialize(const std::string iWeightFile);
      void SetNNVectorVar();
      float EvaluateNN();
    
      float fEta;
      float fPhi;
      float fEcal;
      float fDepth1;
      float fDepth2;
      float fDepth3;
      float fDepth4;
      float fDepth5;
      float fDepth6;
      float fDepth7;

    private:
      tensorflow::Session* session;
      tensorflow::GraphDef* graphDef;
      std::vector<float> NNvectorVar_; 
};
#endif
