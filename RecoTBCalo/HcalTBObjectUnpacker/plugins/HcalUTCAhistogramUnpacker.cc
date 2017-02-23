using namespace std;

#include "DataFormats/HcalDigi/interface/HcalUHTRhistogramDigiCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "RecoTBCalo/HcalTBObjectUnpacker/plugins/HcalUTCAhistogramUnpacker.h"
#include <iostream>


  HcalUTCAhistogramUnpacker::HcalUTCAhistogramUnpacker(edm::ParameterSet const& conf)
  {
    rawDump = conf.getParameter<bool>("rawDump"); //prints out lots of debugging info including the raw data
    fedNumber = conf.getParameter<int>("fedNumber");
    tok_raw_ = consumes<FEDRawDataCollection>(conf.getParameter<edm::InputTag>("fedRawDataCollectionTag"));
    produces<HcalUHTRhistogramDigiCollection>();
  }

  // Virtual destructor needed.
  HcalUTCAhistogramUnpacker::~HcalUTCAhistogramUnpacker() { }  

  // Functions that gets called by framework every event
  void HcalUTCAhistogramUnpacker::produce(edm::Event& e, const edm::EventSetup& es)
  {
    edm::Handle<FEDRawDataCollection> rawraw;  
    edm::ESHandle<HcalElectronicsMap>   item;
    edm::ESHandle<HcalDbService> pSetup;

    e.getByToken(tok_raw_, rawraw);          
    es.get<HcalDbRecord>().get(pSetup);
    es.get<HcalElectronicsMapRcd>().get(item);

    //const HcalElectronicsMap* readoutMap = item.product();
    const HcalElectronicsMap* readoutMap = pSetup->getHcalMapping();

    std::auto_ptr<HcalUHTRhistogramDigiCollection> hd(new HcalUHTRhistogramDigiCollection);

    const FEDRawData& fed = rawraw->FEDData(fedNumber);

    histoUnpacker_.unpack(fed, *readoutMap, hd, rawDump);
    e.put(hd);
  }

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(HcalUTCAhistogramUnpacker);
