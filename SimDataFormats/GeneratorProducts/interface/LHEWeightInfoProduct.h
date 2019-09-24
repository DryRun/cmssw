#ifndef SimDataFormats_GeneratorProducts_LHEWeightInfoProduct_h
#define SimDataFormats_GeneratorProducts_LHEWeightInfoProduct_h

#include <iterator>
#include <memory>
#include <vector>
#include <string>

//#include <hepml.hpp>

#include "DataFormats/Common/interface/OwnVector.h"
#include "SimDataFormats/GeneratorProducts/interface/LesHouches.h"
#include "SimDataFormats/GeneratorProducts/interface/WeightGroupInfo.h"

class LHEWeightInfoProduct {
    public:
        LHEWeightInfoProduct() {}
        LHEWeightInfoProduct(edm::OwnVector<gen::WeightGroupInfo>& weightGroups);
	    LHEWeightInfoProduct(const LHEWeightInfoProduct& other); 
	    LHEWeightInfoProduct(LHEWeightInfoProduct&& other);
        ~LHEWeightInfoProduct() {}
        LHEWeightInfoProduct& operator=(const LHEWeightInfoProduct &other); 
        LHEWeightInfoProduct& operator=(LHEWeightInfoProduct &&other); 

        const edm::OwnVector<gen::WeightGroupInfo>& allWeightGroupsInfo() const;
        const gen::WeightGroupInfo* containingWeightGroupInfo(int index) const;
        const gen::WeightGroupInfo* orderedWeightGroupInfo(int index) const;
        std::vector<gen::WeightGroupInfo*> weightGroupsByType(gen::WeightType type) const;
        std::vector<int> weightGroupIndicesByType(gen::WeightType type) const;
        void addWeightGroupInfo(gen::WeightGroupInfo* info);
        const int numberOfGroups() const { return weightGroupsInfo_.size(); }

    private:
        edm::OwnVector<gen::WeightGroupInfo> weightGroupsInfo_;


};

#endif // GeneratorWeightInfo_LHEInterface_LHEWeightInfoProduct_h

