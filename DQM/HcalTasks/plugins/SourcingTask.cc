
#include "DQM/HcalTasks/interface/SourcingTask.h"

using namespace hcaldqm;
using namespace hcaldqm::constants;
using namespace hcaldqm::filter;
SourcingTask::SourcingTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags
	_tag_uhtr = ps.getUntrackedParameter<edm::InputTag>("tagUHTR", edm::InputTag("HcalUHTRhistogramDigiCollectionTag"));
 	_tok_uhtr = consumes<HcalUHTRhistogramDigiCollection>(_tag_uhtr);
  	//tok_uhtr_ = consumes<HcalUHTRhistogramDigiCollection>(iConfig.getParameter<edm::InputTag>("HcalUHTRhistogramDigiCollectionTag"));
}
	
/* virtual */ void SourcingTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	if (_ptype==fLocal)
		if (r.runAuxiliary().run()==1)
			return;

	DQTask::bookHistograms(ib, r, es);
	
	edm::ESHandle<HcalDbService> dbService;
	es.get<HcalDbRecord>().get(dbService);
	_emap = dbService->getHcalMapping();

	//	INITIALIZE
	for (int i_capid = 0; i_capid < 4; ++i_capid) {
		// Occupancy
		_cOccupancy_Crate[i_capid].initialize(_name,
										 "Occupancy", hashfunctions::fCrate,
										 new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
										 new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
										 new quantity::ValueQuantity(quantity::fN));
		_cOccupancy_CrateSlot[i_capid].initialize(_name,
											 "Occupancy", hashfunctions::fCrateSlot,
											 new quantity::ElectronicsQuantity(quantity::fFiberuTCA),
											 new quantity::ElectronicsQuantity(quantity::fFiberCh),
											 new quantity::ValueQuantity(quantity::fN));
		_cOccupancy_depth[i_capid].initialize(_name, "Occupancy",
			hcaldqm::hashfunctions::fdepth,
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta),
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi),
			new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN));

		// Entries
		_cEntries_Crate[i_capid].initialize(_name,
										 "Entries", hashfunctions::fCrate,
										 new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
										 new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
										 new quantity::ValueQuantity(quantity::fN));
		_cEntries_CrateSlot[i_capid].initialize(_name,
											 "Entries", hashfunctions::fCrateSlot,
											 new quantity::ElectronicsQuantity(quantity::fFiberuTCA),
											 new quantity::ElectronicsQuantity(quantity::fFiberCh),
											 new quantity::ValueQuantity(quantity::fN));
		_cEntries_depth[i_capid].initialize(_name, "Entries",
			hcaldqm::hashfunctions::fdepth,
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta),
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi),
			new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN));

		// Mean
		_cMean_Crate[i_capid].initialize(_name,
										 "Mean", hashfunctions::fCrate,
										 new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
										 new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
										 new quantity::ValueQuantity(quantity::fN));
		_cMean_CrateSlot[i_capid].initialize(_name,
											 "Mean", hashfunctions::fCrateSlot,
											 new quantity::ElectronicsQuantity(quantity::fFiberuTCA),
											 new quantity::ElectronicsQuantity(quantity::fFiberCh),
											 new quantity::ValueQuantity(quantity::fN));
		_cMean_depth[i_capid].initialize(_name, "Mean",
			hcaldqm::hashfunctions::fdepth,
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta),
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi),
			new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN));

		// RMS
		_cRMS_Crate[i_capid].initialize(_name,
										 "RMS", hashfunctions::fCrate,
										 new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
										 new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
										 new quantity::ValueQuantity(quantity::fN));
		_cRMS_CrateSlot[i_capid].initialize(_name,
											 "RMS", hashfunctions::fCrateSlot,
											 new quantity::ElectronicsQuantity(quantity::fFiberuTCA),
											 new quantity::ElectronicsQuantity(quantity::fFiberCh),
											 new quantity::ValueQuantity(quantity::fN));
		_cRMS_depth[i_capid].initialize(_name, "RMS",
			hcaldqm::hashfunctions::fdepth,
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta),
			new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi),
			new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN));


		//	BOOK
		char aux[100];
		sprintf(aux, "/CapId%d", i_capid);

		_cOccupancy_Crate[i_capid].book(ib, _emap, _subsystem, aux);
		_cOccupancy_CrateSlot[i_capid].book(ib, _emap, _subsystem, aux);
		_cOccupancy_depth[i_capid].book(ib, _emap, _subsystem, aux);
		_cEntries_Crate[i_capid].book(ib, _emap, _subsystem, aux);
		_cEntries_CrateSlot[i_capid].book(ib, _emap, _subsystem, aux);
		_cEntries_depth[i_capid].book(ib, _emap, _subsystem, aux);
		_cMean_Crate[i_capid].book(ib, _emap, _subsystem, aux);
		_cMean_CrateSlot[i_capid].book(ib, _emap, _subsystem, aux);
		_cMean_depth[i_capid].book(ib, _emap, _subsystem, aux);
		_cRMS_Crate[i_capid].book(ib, _emap, _subsystem, aux);
		_cRMS_CrateSlot[i_capid].book(ib, _emap, _subsystem, aux);
		_cRMS_depth[i_capid].book(ib, _emap, _subsystem, aux);
	}
	std::cout << "[SourcingTask::bookHistograms] DEBUG : My emap is " << &_emap << std::endl;
	_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);
}

/* virtual */ void SourcingTask::_resetMonitors(hcaldqm::UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void SourcingTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HcalUHTRhistogramDigiCollection> col_uhtr;  
	e.getByToken(_tok_uhtr, col_uhtr);

	for (unsigned int i_digi = 0; i_digi < col_uhtr->size(); ++i_digi) {
		const HcalUHTRhistogramDigi digi = col_uhtr->at(i_digi);
		const HcalDetId did = digi.id();
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));
		for (unsigned int i_capid = 0; i_capid < 4; ++i_capid) {
			TH1F* h_temp = new TH1F("h_temp","",32,0,32);
			// loop over histogram bins
			for(int i_bin = 0; i_bin < 32; i_bin++) {
				h_temp->Fill(i_bin, digi.get(i_capid, i_bin));
			}
			_cOccupancy_Crate[i_capid].fill(eid);
			_cOccupancy_CrateSlot[i_capid].fill(eid);
			_cOccupancy_depth[i_capid].fill(did);
			_cEntries_Crate[i_capid].fill(eid, h_temp->Integral());
			_cEntries_CrateSlot[i_capid].fill(eid, h_temp->Integral());
			_cEntries_depth[i_capid].fill(did, h_temp->Integral());
			_cMean_Crate[i_capid].fill(eid, h_temp->GetMean());
			_cMean_CrateSlot[i_capid].fill(eid, h_temp->GetMean());
			_cMean_depth[i_capid].fill(did, h_temp->GetMean());
			_cRMS_Crate[i_capid].fill(eid, h_temp->GetRMS());
			_cRMS_CrateSlot[i_capid].fill(eid, h_temp->GetRMS());
			_cRMS_depth[i_capid].fill(did, h_temp->GetRMS());
		}
	}

}

DEFINE_FWK_MODULE(SourcingTask);


