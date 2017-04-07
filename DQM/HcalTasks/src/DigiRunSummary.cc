#include "DQM/HcalTasks/interface/DigiRunSummary.h"

namespace hcaldqm
{
	DigiRunSummary::DigiRunSummary(std::string const& name, 
		std::string const& taskname, edm::ParameterSet const& ps) :
		DQClient(name, taskname, ps), _booked(false)
	{
		_thresh_unihf = ps.getUntrackedParameter<double>("thresh_unihf",
			0.2);
	}

	/* virtual */ void DigiRunSummary::beginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		DQClient::beginRun(r,es);

		if (_ptype!=fOffline)
			return;

		//  INITIALIZE WHAT NEEDS TO BE INITIALIZE ONLY ONCE!
		_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);
		// Subdet filters
		_vhashHF.push_back(HcalDetId(HcalForward, 1, 1, 1).rawId());
		_filter_HF.initialize(filter::fPreserver, hashfunctions::fSubdet, _vhashHF);

		_xDead.initialize(hashfunctions::fSubdet);
		_xDigiSize.initialize(hashfunctions::fSubdet);
		_xUni.initialize(hashfunctions::fSubdet);
		_xUniHF.initialize(hashfunctions::fSubdet);
		_xNChs.initialize(hashfunctions::fSubdet);
		_xNChsNominal.initialize(hashfunctions::fSubdet);

		_xDead.book(_emap);
		_xDigiSize.book(_emap); 
		_xUni.book(_emap, _filter_HF);
		_xUniHF.book(_emap);
		_xNChs.book(_emap); 
		_xNChsNominal.book(_emap);
		
		_cOccupancy_depth.initialize(_name, "Occupancy",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN),0);

		//	GET THE NOMINAL NUMBER OF CHANNELS PER FED
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;
			HcalDetId did(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));
			_xNChsNominal.get(eid)++;
		}
	}

	/*
	 *	END LUMI. EVALUATE LUMI BASED FLAGS
	 */
	/* virtual */ void DigiRunSummary::endLuminosityBlock(DQMStore::IBooker& ib,
		DQMStore::IGetter& ig, edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		DQClient::endLuminosityBlock(ib, ig, lb, es);

		if (_ptype!=fOffline)
			return;

		LSSummary lssum;
		lssum._LS=_currentLS;

		_xDigiSize.reset(); _xNChs.reset();
		
		//	INITIALIZE LUMI BASED HISTOGRAMS
		Container2D cDigiSize_Subdet, cOccupancy_depth;
		cDigiSize_Subdet.initialize(_taskname, "DigiSize",
			hashfunctions::fSubdet,
			new quantity::ValueQuantity(quantity::fDigiSize),
			new quantity::ValueQuantity(quantity::fN),0);
		cOccupancy_depth.initialize(_taskname, "Occupancy",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN),0);

		//	LOAD LUMI BASED HISTOGRAMS
		cOccupancy_depth.load(ig, _emap, _subsystem);
		cDigiSize_Subdet.load(ig, _emap, _subsystem);
		MonitorElement *meNumEvents = ig.get(_subsystem+
			"/RunInfo/NumberOfEvents");
		int numEvents = meNumEvents->getBinContent(1);
		bool unknownIdsPresent = ig.get(_subsystem+"/"
			+_taskname+"/UnknownIds")->getBinContent(1)>0;

		//	book the Numer of Events - set axis extendable
		if (!_booked)
		{
			ib.setCurrentFolder(_subsystem+"/"+_taskname);
			_meNumEvents = ib.book1D("NumberOfEvents", "NumberOfEvents",
				1000, 1, 1001); // 1000 to start with
			_meNumEvents->getTH1()->SetCanExtend(TH1::kXaxis);

			_cOccupancy_depth.book(ib, _emap, _subsystem);
			_booked=true;
		}
		_meNumEvents->setBinContent(_currentLS, numEvents);

		//	ANALYZE THIS LS for LS BASED FLAGS
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

			cOccupancy_depth.getBinContent(did)>0?_xNChs.get(eid)++:
				_xNChs.get(eid)+=0;
			_cOccupancy_depth.fill(did, cOccupancy_depth.getBinContent(did));
			//	digi size
			cDigiSize_Subdet.getMean(did)!=
				constants::DIGISIZE[did.subdet()-1]?
				_xDigiSize.get(eid)++:_xDigiSize.get(eid)+=0;
			cDigiSize_Subdet.getRMS(did)!=0?
				_xDigiSize.get(eid)++:_xDigiSize.get(eid)+=0;
		}

		//	GENERATE SUMMARY AND STORE IT
		std::vector<flag::Flag> vtmpflags; 
		vtmpflags.resize(nLSFlags);
		vtmpflags[fDigiSize]=flag::Flag("DigiSize");
		vtmpflags[fNChsHF]=flag::Flag("NChsHF");
		vtmpflags[fUnknownIds]=flag::Flag("UnknownIds");
		for (auto& it_subdet : _vhashSubdets) {
			HcalElectronicsId eid(it_subdet);

			//	reset all the tmp flags to fNA
			//	MUST DO IT NOW! AS NCDAQ MIGHT OVERWRITE IT!
			for (std::vector<flag::Flag>::iterator ft=vtmpflags.begin();
				ft!=vtmpflags.end(); ++ft)
				ft->reset();

			if (_xDigiSize.get(eid)>0)
				vtmpflags[fDigiSize]._state = flag::fBAD;
			else
				vtmpflags[fDigiSize]._state = flag::fGOOD;
			if (utilities::isFEDHF(eid))
			{
				if (_xNChs.get(eid)!=_xNChsNominal.get(eid))
					vtmpflags[fNChsHF]._state = flag::fBAD;
				else
					vtmpflags[fNChsHF]._state = flag::fGOOD;
			}
			if (unknownIdsPresent)
				vtmpflags[fUnknownIds]._state = flag::fBAD;
			else
				vtmpflags[fUnknownIds]._state = flag::fGOOD;

			// push all the flags for this FED
			lssum._vflags.push_back(vtmpflags);
		}

		//	push all the flags for all FEDs for this LS
		_vflagsLS.push_back(lssum);
	}

	/*
	 *	End Job
	 */
	/* virtual */ std::vector<flag::Flag> DigiRunSummary::endJob(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig)
	{
		if (_ptype!=fOffline)
			return std::vector<flag::Flag>();

		_xDead.reset(); _xUniHF.reset(); _xUni.reset();

		//	PREPARE LS AND RUN BASED FLAGS TO USE IT FOR BOOKING
		std::vector<flag::Flag> vflagsPerLS;
		std::vector<flag::Flag> vflagsPerRun;
		vflagsPerLS.resize(nLSFlags);
		vflagsPerRun.resize(nDigiFlag-nLSFlags+1);
		vflagsPerLS[fDigiSize]=flag::Flag("DigiSize");
		vflagsPerLS[fNChsHF]=flag::Flag("NChsHF");
		vflagsPerRun[fDigiSize]=flag::Flag("DigiSize");
		vflagsPerRun[fNChsHF]=flag::Flag("NChsHF");
		vflagsPerRun[fUniHF-nLSFlags+1]=flag::Flag("UniSlotHF");
		vflagsPerRun[fDead-nLSFlags+1]=flag::Flag("Dead");

		//	INITIALIZE SUMMARY CONTAINERS
		ContainerSingle2D cSummaryvsLS;
		Container2D cSummaryvsLS_Subdet;
		cSummaryvsLS.initialize(_name, "SummaryvsLS",
			new quantity::LumiSection(_maxProcessedLS),
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ValueQuantity(quantity::fState),0);
		cSummaryvsLS_Subdet.initialize(_name, "SummaryvsLS",
			hashfunctions::fSubdet,
			new quantity::LumiSection(_maxProcessedLS),
			new quantity::FlagQuantity(vflagsPerLS),
			new quantity::ValueQuantity(quantity::fState),0);
		cSummaryvsLS_Subdet.book(ib, _emap, _subsystem);
		cSummaryvsLS.book(ib, _subsystem);

		// INITIALIZE CONTAINERS WE NEED TO LOAD or BOOK
		Container2D cOccupancyCut_depth;
		Container2D cDead_depth, cDead_Subdet;
		cOccupancyCut_depth.initialize(_taskname, "OccupancyCut",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN),0);
		cDead_depth.initialize(_name, "Dead",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN),0);
		cDead_Subdet.initialize(_name, "Dead",
			hashfunctions::fSubdet,
			new quantity::ElectronicsQuantity(quantity::fSpigot),
			new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
			new quantity::ValueQuantity(quantity::fN),0);

		//	LOAD
		cOccupancyCut_depth.load(ig, _emap, _subsystem);
		cDead_depth.book(ib, _emap, _subsystem);
		cDead_Subdet.book(ib, _emap, _subsystem);

		//	ANALYZE RUN BASED QUANTITIES
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

			if (_cOccupancy_depth.getBinContent(did)<1)
			{
				_xDead.get(eid)++;
				cDead_depth.fill(did);
				cDead_Subdet.fill(eid);
			}
			if (did.subdet()==HcalForward)
				_xUniHF.get(eid)+=cOccupancyCut_depth.getBinContent(did);
		}
		//	ANALYZE FOR HF SLOT UNIFORMITY
		for (uintCompactMap::const_iterator it=_xUniHF.begin();
			it!=_xUniHF.end(); ++it)
		{
			uint32_t hash1 = it->first;
			HcalElectronicsId eid1(hash1);
			double x1 = it->second;

			for (uintCompactMap::const_iterator jt=_xUniHF.begin();
				jt!=_xUniHF.end(); ++jt)
			{
				if (jt==it)
					continue;

				double x2 = jt->second;
				if (x2==0)
					continue;
				if (x1/x2<_thresh_unihf)
					_xUni.get(eid1)++;
			}
		}

		/*
		 *	Iterate over each FED
		 *		Iterate over each LS Summary
		 *			Iterate over all flags
		 *				set...
		 */
		//	iterate over all Subdets
		std::vector<flag::Flag> sumflags;
		int isubdet=0;
		for (auto& it_subdet : _vhashSubdets) {

			flag::Flag fSumRun("DIGI"); // summary flag for this FED
			flag::Flag ffDead("Dead");
			flag::Flag ffUniSlotHF("UniSlotHF");
			HcalElectronicsId eid(it_subdet);

			//	ITERATE OVER EACH LS
			for (std::vector<LSSummary>::const_iterator itls=_vflagsLS.begin();
				itls!=_vflagsLS.end(); ++itls)
			{
				int iflag=0;
				flag::Flag fSumLS("DIGI");
				for (std::vector<flag::Flag>::const_iterator ft=
					itls->_vflags[isubdet].begin(); ft!=itls->_vflags[isubdet].end();
					++ft)
				{
					cSummaryvsLS_Subdet.setBinContent(eid, itls->_LS, int(iflag),
						ft->_state);
					fSumLS+=(*ft);
					iflag++;
				}
				cSummaryvsLS.setBinContent(eid, itls->_LS, fSumLS._state);
				fSumRun+=fSumLS;
			}

			//	EVALUATE RUN BASED FLAGS
			if (_xDead.get(eid)>0)
				ffDead._state = flag::fBAD;
			else
				ffDead._state = flag::fGOOD;
			if (utilities::isFEDHF(eid))
			{
				if (_xUni.get(eid)>0)
					ffUniSlotHF._state = flag::fBAD;
				else
					ffUniSlotHF._state = flag::fGOOD;
			}
			fSumRun+=ffDead+ffUniSlotHF;

			// push the summary flag for this FED for the Whole Run
			sumflags.push_back(fSumRun);

			//	 increment fed
			isubdet++;
		}

		return sumflags;
	}
}
