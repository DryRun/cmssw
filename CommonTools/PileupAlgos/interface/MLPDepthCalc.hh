#ifndef COMMONTOOLS_PILEUPALGOS_MLPDEPTHCALC_HH
#define COMMONTOOLS_PILEUPALGOS_MLPDEPTHCALC_HH
#include <string>

namespace TMVA {
  class Reader;
}

namespace baconhep {
  class MLPDepthCalc{
    public:
      MLPDepthCalc();
      ~MLPDepthCalc();

      void initialize(const std::string iMethodTag, const std::string iPtWeightFile);
      double mvaValue(const float eta, const float phi, const float depth0, const float depth1, const float depth2, const float depth3, const float depth4, const float depth5, const float depth6);
    private:
      float _eta, _phi, _depth0, _depth1, _depth2, _depth3, _depth4, _depth5, _depth6;
      void initReader(TMVA::Reader *reader, const std::string filename);
      bool fIsInitialized;
      TMVA::Reader *fReader;
      std::string MethodTag;
 
   };
}
#endif
