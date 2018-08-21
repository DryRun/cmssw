#include "DQM/HcalTasks/interface/MLTask.h"

using namespace hcaldqm;
using namespace hcaldqm::constants;
using namespace hcaldqm::filter;

MLTask::MLTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHE = ps.getUntrackedParameter<edm::InputTag>("tagHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));

	_tokHBHE = consumes<HBHEDigiCollection>(_tagHBHE);
	_tokHE = consumes<QIE11DigiCollection>(_tagHE);
	_tokHO = consumes<HODigiCollection>(_tagHO);
	_tokHF = consumes<QIE10DigiCollection>(_tagHF);

	_cutSumQ_HBHE = ps.getUntrackedParameter<double>("cutSumQ_HBHE", 200.);
	_cutSumQ_HE = ps.getUntrackedParameter<double>("cutSumQ_HE", 40000.);
	_cutSumQ_HO = ps.getUntrackedParameter<double>("cutSumQ_HO", 100.);
	_cutSumQ_HF = ps.getUntrackedParameter<double>("cutSumQ_HF", 1000.);

	_vflags.resize(nMLFlag);
	_vflags[fExampleMLAlgorithm]=hcaldqm::flag::Flag("ExampleMLAlgorithm"); // Bin label on summary plot
	_vflags[fUnknownIds]=hcaldqm::flag::Flag("UnknownIds");

	// Get reference digi sizes. Convert from unsigned to signed int, because <digi>::size()/samples() return ints for some reason.
	std::vector<uint32_t> vrefDigiSize = ps.getUntrackedParameter<std::vector<uint32_t>>("refDigiSize");
	_refDigiSize[HcalBarrel]  = (int)vrefDigiSize[0];
	_refDigiSize[HcalEndcap]  = (int)vrefDigiSize[1];
	_refDigiSize[HcalOuter]   = (int)vrefDigiSize[2];
	_refDigiSize[HcalForward] = (int)vrefDigiSize[3];

	_nLS = ps.getUntrackedParameter<unsigned int>("nLS", 10);
}

/* virtual */ void MLTask::bookHistograms(DQMStore::IBooker& ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib,r,es);

	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();

	_cOccupancy_depth_nLS.initialize(_name, "Occupancy_depth_LS", hcaldqm::hashfunctions::fdepth, 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta), 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi), 
		new hcaldqm::quantity::LumiSection(_nLS), 0);
	_cOccupancy_depth_nLS.book(ib, _emap, _subsystem);

	_cOccupancyCut_depth_nLS.initialize(_name, "OccupancyCut_depth_LS", hcaldqm::hashfunctions::fdepth, 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta), 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi), 
		new hcaldqm::quantity::LumiSection(_nLS), 0);
	_cOccupancyCut_depth_nLS.book(ib, _emap, _subsystem);

	_cOccupancy_depth.initialize(_name, "Occupancy_depth", hcaldqm::hashfunctions::fdepth, 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta), 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi), 
		new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN), 0);
	_cOccupancy_depth.book(ib, _emap, _subsystem);

	_cOccupancyCut_depth.initialize(_name, "OccupancyCut_depth", hcaldqm::hashfunctions::fdepth, 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fieta), 
		new hcaldqm::quantity::DetectorQuantity(hcaldqm::quantity::fiphi), 
		new hcaldqm::quantity::ValueQuantity(hcaldqm::quantity::fN), 0);
	_cOccupancyCut_depth.book(ib, _emap, _subsystem);


	//	book Number of Events vs LS histogram
	ib.setCurrentFolder(_subsystem+"/RunInfo");
	meNumEvents1LS = ib.book1D("NumberOfEvents", "NumberOfEvents",
		1, 0, 1);
	meNumEvents1LS->setLumiFlag();

	//	book the flag for unknown ids and the online guy as well
	ib.setCurrentFolder(_subsystem+"/"+_name);
	meUnknownIds1LS = ib.book1D("UnknownIds", "UnknownIds",
		1, 0, 1);
	_unknownIdsPresent = false;
	meUnknownIds1LS->setLumiFlag();

	_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);
	_dhashmap.initialize(_emap, electronicsmap::fE2DHashMap);


}

/* virtual */ void MLTask::_resetMonitors(hcaldqm::UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);

	switch(uf)
	{
		case hcaldqm::f1LS:
			_unknownIdsPresent = false;
			break;
		case hcaldqm::f50LS:
			//	^^^ONLINE ONLY!
			//if (_ptype==fOnline)
				//_cOccupancyvsiphi_SubdetPM.reset();
			//	^^^
			break;
		default:
			break;
	}
}

/* virtual */ void MLTask::_process(edm::Event const& e,
	edm::EventSetup const&)
{
	edm::Handle<HBHEDigiCollection>     chbhe;
	edm::Handle<QIE11DigiCollection>     che_qie11;
	edm::Handle<HODigiCollection>       cho;
	edm::Handle<QIE10DigiCollection>       chf;

	if (!e.getByToken(_tokHBHE, chbhe))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByToken(_tokHE, che_qie11))
		_logger.dqmthrow("Collection QIE11DigiCollection isn't available"
			+ _tagHE.label() + " " + _tagHE.instance());
	if (!e.getByToken(_tokHO, cho))
		_logger.dqmthrow("Collection HODigiCollection isn't available"
			+ _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByToken(_tokHF, chf))
		_logger.dqmthrow("Collection QIE10DigiCollection isn't available"
			+ _tagHF.label() + " " + _tagHF.instance());

	//	extract some info per event
	//int bx = e.bunchCrossing();
	meNumEvents1LS->Fill(0.5); // just increment

	//uint32_t rawidValid = 0;
	//uint32_t rawidHBValid = 0;
	//uint32_t rawidHEValid = 0;

	for (HBHEDigiCollection::const_iterator it=chbhe->begin(); it!=chbhe->end();
		++it)
	{
		//	Explicit check on the DetIds present in the Collection
		HcalDetId const& did = it->id();
		if (did.subdet() != HcalBarrel) {
			continue;
		}
		uint32_t rawid = _ehashmap.lookup(did);
		if (rawid == 0) {
			meUnknownIds1LS->Fill(1); 
			_unknownIdsPresent=true;
			continue;
		} else {
			/*
			if (did.subdet()==HcalBarrel) {
				rawidHBValid = did.rawId();
			} else if (did.subdet()==HcalEndcap) {
				rawidHEValid = did.rawId();
			}
			*/
		}
		//HcalElectronicsId const& eid(rawid);

		//	filter out channels that are masked out
		if (_xQuality.exists(did)) 
		{
			HcalChannelStatus cs(did.rawId(), _xQuality.get(did));
			if (
				cs.isBitSet(HcalChannelStatus::HcalCellMask) ||
				cs.isBitSet(HcalChannelStatus::HcalCellDead))
				continue;
		}

		CaloSamples digi_fC = hcaldqm::utilities::loadADC2fCDB<HBHEDataFrame>(_dbService, did, *it);
		double sumQ = hcaldqm::utilities::sumQDB<HBHEDataFrame>(_dbService, digi_fC, did, *it, 0, it->size()-1);

		_cOccupancy_depth.fill(did);
		_cOccupancy_depth_nLS.fill(did);
		if (sumQ > _cutSumQ_HBHE) {
			_cOccupancyCut_depth.fill(did);
			_cOccupancyCut_depth_nLS.fill(did);
		}

	}

	// HE QIE11 collection
	for (QIE11DigiCollection::const_iterator it=che_qie11->begin(); it!=che_qie11->end();
		++it)
	{
		const QIE11DataFrame digi = static_cast<const QIE11DataFrame>(*it);

		//	Explicit check on the DetIds present in the Collection
		HcalDetId const& did = digi.detid();
		if (did.subdet() != HcalEndcap) {
			continue;
		}
		uint32_t rawid = _ehashmap.lookup(did);
		if (rawid == 0) {
			meUnknownIds1LS->Fill(1);
			_unknownIdsPresent=true;
			continue;
		} else {
			/*
			if (did.subdet()==HcalBarrel) { // Note: since this is HE, we obviously expect did.subdet() always to be HcalEndcap, but QIE11DigiCollection will have HB for Run 3.
				rawidHBValid = did.rawId();
			} else if (did.subdet()==HcalEndcap) {
				rawidHEValid = did.rawId();
			}
			*/
		}
		// HcalElectronicsId const& eid(rawid);

		//	filter out channels that are masked out
		if (_xQuality.exists(did)) 
		{
			HcalChannelStatus cs(did.rawId(), _xQuality.get(did));
			if (cs.isBitSet(HcalChannelStatus::HcalCellMask) || cs.isBitSet(HcalChannelStatus::HcalCellDead)) {
				continue;
			}
		}

		CaloSamples digi_fC = hcaldqm::utilities::loadADC2fCDB<QIE11DataFrame>(_dbService, did, digi);
		double sumQ = hcaldqm::utilities::sumQDB<QIE11DataFrame>(_dbService, digi_fC, did, digi, 0, digi.samples()-1);

		_cOccupancy_depth.fill(did);
		_cOccupancy_depth_nLS.fill(did);
		if (sumQ > _cutSumQ_HE) {
			_cOccupancyCut_depth.fill(did);
			_cOccupancyCut_depth_nLS.fill(did);
		}

	}

	//	reset
	//rawidValid = 0;


	//	HO collection
	for (HODigiCollection::const_iterator it=cho->begin(); it!=cho->end();
		++it)
	{		
		//	Explicit check on the DetIds present in the Collection
		HcalDetId const& did = it->id();
		if (did.subdet() != HcalOuter) {
			continue;
		}
		uint32_t rawid = _ehashmap.lookup(did);
		if (rawid == 0) {
			meUnknownIds1LS->Fill(1);
			_unknownIdsPresent = true;
			continue;
		} else {
			//rawidValid = did.rawId();
		}
		// // HcalElectronicsId const& eid(rawid);

		//	filter out channels that are masked out
		if (_xQuality.exists(did)) 
		{
			HcalChannelStatus cs(did.rawId(), _xQuality.get(did));
			if (
				cs.isBitSet(HcalChannelStatus::HcalCellMask) ||
				cs.isBitSet(HcalChannelStatus::HcalCellDead))
				continue;
		}

		CaloSamples digi_fC = hcaldqm::utilities::loadADC2fCDB<HODataFrame>(_dbService, did, *it);
		double sumQ = hcaldqm::utilities::sumQDB<HODataFrame>(_dbService, digi_fC, did, *it, 0, it->size()-1);
		_cOccupancy_depth.fill(did);
		_cOccupancy_depth_nLS.fill(did);
		if (sumQ > _cutSumQ_HO) {
			_cOccupancyCut_depth.fill(did);
			_cOccupancyCut_depth_nLS.fill(did);
		}

	}

	//	reset
	//rawidValid = 0;

	//	HF collection
	for (QIE10DigiCollection::const_iterator it=chf->begin(); it!=chf->end(); ++it) {
		const QIE10DataFrame digi = static_cast<const QIE10DataFrame>(*it);

		//	Explicit check on the DetIds present in the Collection
		HcalDetId const& did = digi.detid();
		if (did.subdet() != HcalForward) {
			continue;
		}

		uint32_t rawid = _ehashmap.lookup(did);
		if (rawid == 0) {
			meUnknownIds1LS->Fill(1); 
			_unknownIdsPresent=true;
			continue;
		} else {
			//rawidValid = did.rawId();
		}
		// HcalElectronicsId const& eid(rawid);

		//	filter out channels that are masked out
		if (_xQuality.exists(did)) 
		{
			HcalChannelStatus cs(did.rawId(), _xQuality.get(did));
			if (
				cs.isBitSet(HcalChannelStatus::HcalCellMask) ||
				cs.isBitSet(HcalChannelStatus::HcalCellDead))
				continue;
		}

		CaloSamples digi_fC = hcaldqm::utilities::loadADC2fCDB<QIE10DataFrame>(_dbService, did, digi);
		double sumQ = hcaldqm::utilities::sumQDB<QIE10DataFrame>(_dbService, digi_fC, did, digi, 0, digi.samples()-1);
		
		_cOccupancy_depth.fill(did);
		_cOccupancy_depth_nLS.fill(did);
		if (sumQ > _cutSumQ_HF) {
			_cOccupancyCut_depth.fill(did);
			_cOccupancyCut_depth_nLS.fill(did);
		}
	}
}

/* virtual */ void MLTask::beginLuminosityBlock(
	edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	DQTask::beginLuminosityBlock(lb, es);
}

/* virtual */ void MLTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	if (lb.luminosityBlock() % _nLS == 0) {
		computeSummaryFlags();
	}

	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

void MLTask::computeSummaryFlags() {
	// Make maps of depth : TH2F*
	std::map<unsigned int, const TH2F*> hOccupancy_depth_nLS;
	for (auto& it_did_me : *(_cOccupancy_depth_nLS.getMEs())) {
		HcalDetId did(it_did_me.first);
		hOccupancy_depth_nLS[did.depth()] = it_did_me.second->getTH2F();
	}

	std::map<unsigned int, const TH2F*> hOccupancyCut_depth_nLS;
	for (auto& it_did_me : *(_cOccupancyCut_depth_nLS.getMEs())) {
		HcalDetId did(it_did_me.first);
		hOccupancyCut_depth_nLS[did.depth()] = it_did_me.second->getTH2F();
	}

	// INSTRUCTIONS
	// Pass hOccupancy_depth_nLS to ML algorithm
	// Or similarly _cOccupancy_depth->getTH2F(), for occupancy for whole run
	std::string teststring = "hOccupancy_depth_nLS[1] integral = " + std::to_string(hOccupancy_depth_nLS[1]->Integral());
	_logger.warn(teststring.c_str());

	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		hcaldqm::flag::Flag fSum("DIGI");
		HcalElectronicsId eid = HcalElectronicsId(*it);

		std::vector<uint32_t>::const_iterator cit=std::find(
			_vcdaqEids.begin(), _vcdaqEids.end(), *it);
		if (cit==_vcdaqEids.end())
		{
			//	not @cDAQ
			for (uint32_t iflag=0; iflag<_vflags.size(); iflag++)
				_cSummaryvsLS_FED.setBinContent(eid, _currentLS, int(iflag),
					int(hcaldqm::flag::fNCDAQ));
			_cSummaryvsLS.setBinContent(eid, _currentLS, int(hcaldqm::flag::fNCDAQ));
			continue;
		}

		//	FED is @cDAQ		
		if (_unknownIdsPresent) 
			_vflags[fUnknownIds]._state = hcaldqm::flag::fBAD;
		else
			_vflags[fUnknownIds]._state = hcaldqm::flag::fGOOD;


		int iflag=0;
		for (std::vector<hcaldqm::flag::Flag>::iterator ft=_vflags.begin();
			ft!=_vflags.end(); ++ft)
		{
			_cSummaryvsLS_FED.setBinContent(eid, _currentLS, iflag,
				int(ft->_state));
			fSum+=(*ft);
			iflag++;

			//	reset!
			ft->reset();
		}
		_cSummaryvsLS.setBinContent(eid, _currentLS, fSum._state);
	}

	_cOccupancy_depth_nLS.reset();
	_cOccupancyCut_depth_nLS.reset();

}

DEFINE_FWK_MODULE(MLTask);

