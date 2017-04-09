#include "DQM/HcalTasks/interface/RawRunSummary.h"

namespace hcaldqm
{
	RawRunSummary::RawRunSummary(std::string const& name, 
		std::string const& taskname, edm::ParameterSet const& ps) :
		DQClient(name, taskname, ps), _booked(false)
	{}

	/* virtual */ void RawRunSummary::beginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		DQClient::beginRun(r,es);
		
		if (_ptype!=fOffline)
			return;

		//	INITIALIZE WHAT NEEDS TO BE INITIALIZE ONLY ONCE!
		_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);

		//	INTIALIZE CONTAINERS ACTING AS HOLDERS OF RUN INFORAMTION
		_cEvnMsm_Electronics.initialize(_name, "EvnMsm",
			hashfunctions::fElectronics,
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ElectronicsQuantity(quantity::fCrate),
			new quantity::ValueQuantity(quantity::fN),0);
		_cBcnMsm_Electronics.initialize(_name, "BcnMsm",
			hashfunctions::fElectronics,
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ElectronicsQuantity(quantity::fCrate),
			new quantity::ValueQuantity(quantity::fN),0);
		_cBadQuality_depth.initialize(_name, "BadQuality",
			 hashfunctions::fdepth,
			 new quantity::DetectorQuantity(quantity::fieta),
			 new quantity::DetectorQuantity(quantity::fiphi),
			 new quantity::ValueQuantity(quantity::fN),0);

		_xEvn.initialize(hashfunctions::fSubdet);
		_xBcn.initialize(hashfunctions::fSubdet);
		_xBadQ.initialize(hashfunctions::fSubdet);
		//	BOOK CONTAINERSXXX
		_xEvn.book(_emap); _xBcn.book(_emap); _xBadQ.book(_emap);
	}

	/*
	 *	END OF LUMINOSITY HARVESTING 
	 *	RAW FORMAT HAS ONLY LUMI BASED FLAGS!
	 *	THEREFORE STEPS ARE:
	 *	1) LOAD CONTAINERS YOU NEED (MUST BE LUMI BASED)
	 *	2) ANALYZE 
	 *	3) GENERATE SUMMARY FLAGS AND PUSH THEM
	 */
	/* virtual */ void RawRunSummary::endLuminosityBlock(DQMStore::IBooker& ib,
		DQMStore::IGetter& ig, edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		DQClient::endLuminosityBlock(ib, ig, lb, es);
		
		if (_ptype!=fOffline)
			return;

		//	INITIALIZE WHAT YOU NEED
		LSSummary lssum; // summary for this LS
		lssum._LS = _currentLS; // set the LS

		//	RESET CONTAINERS USED FOR ANALYSIS OF THIS LS
		_xEvn.reset(); _xBcn.reset(); _xBadQ.reset();
		
		//	INITIALIZE LUMI BASED HISTOGRAMS
		Container2D cEvnMsm_Electronics;
		Container2D cBcnMsm_Electronics;
		Container2D cBadQuality_depth;
		cEvnMsm_Electronics.initialize(_taskname, "EvnMsm",
			hashfunctions::fElectronics,
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ElectronicsQuantity(quantity::fCrate),
			new quantity::ValueQuantity(quantity::fN),0);
		cBcnMsm_Electronics.initialize(_taskname, "BcnMsm",
			hashfunctions::fElectronics,
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ElectronicsQuantity(quantity::fCrate),
			new quantity::ValueQuantity(quantity::fN),0);
		cBadQuality_depth.initialize(_taskname, "BadQuality",
			 hashfunctions::fdepth,
			 new quantity::DetectorQuantity(quantity::fieta),
			 new quantity::DetectorQuantity(quantity::fiphi),
			 new quantity::ValueQuantity(quantity::fN),0);

		//	LOAD LUMI BASED HISTOGRAMS
		cEvnMsm_Electronics.load(ig, _emap, _subsystem);
		cBcnMsm_Electronics.load(ig, _emap, _subsystem);
		cBadQuality_depth.load(ig, _emap, _subsystem);
		MonitorElement *meNumEvents = ig.get(_subsystem+
			"/RunInfo/NumberOfEvents");
		int numEvents = meNumEvents->getBinContent(1);

		//	BOOK for the very first time
		if (!_booked)
		{
			_cEvnMsm_Electronics.book(ib, _emap, _subsystem);
			_cBcnMsm_Electronics.book(ib, _emap, _subsystem);
			_cBadQuality_depth.book(ib, _emap, _subsystem);
			_booked=true;
		}

		// ANALYZE THIS LS
		// iterate over all channels	
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;
			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

			_xBadQ.get(eid)+=cBadQuality_depth.getBinContent(did);
			_cBadQuality_depth.fill(did, cBadQuality_depth.getBinContent(did));
			_xEvn.get(eid)+=cEvnMsm_Electronics.getBinContent(eid);
			_xBcn.get(eid)+=cBcnMsm_Electronics.getBinContent(eid);

			_cEvnMsm_Electronics.fill(eid, 
				cEvnMsm_Electronics.getBinContent(eid));
			_cBcnMsm_Electronics.fill(eid, 
				cBcnMsm_Electronics.getBinContent(eid));
		}
		

		//	GENERATE THE SUMMARY FOR THIS LS AND STORE IT
		std::vector<flag::Flag> vtmpflags; // tmp summary flags vector
		vtmpflags.resize(nRawFlag);
		vtmpflags[fEvnMsm]=flag::Flag("EvnMsm");
		vtmpflags[fBcnMsm]=flag::Flag("BcnMsm");
		vtmpflags[fBadQ]=flag::Flag("BadQ");
		for (auto& it_subdet : _vhashSubdets) {
			HcalElectronicsId eid(it_subdet);
			
			//	reset all the tmp flags to fNA
			//	MUST DO IT NOW! AS NCDAQ MIGHT OVERWRITE IT!
			for (std::vector<flag::Flag>::iterator ft=vtmpflags.begin();
				ft!=vtmpflags.end(); ++ft)
				ft->reset();
			
			if (_xEvn.get(eid)>0)
				vtmpflags[fEvnMsm]._state = flag::fBAD;
			else
				vtmpflags[fEvnMsm]._state = flag::fGOOD;
			if (_xBcn.get(eid)>0)
				vtmpflags[fBcnMsm]._state = flag::fBAD;
			else
				vtmpflags[fBcnMsm]._state = flag::fGOOD;

			if (double(_xBadQ.get(eid))>double(12*numEvents))
				vtmpflags[fBadQ]._state = flag::fBAD;
			else if (_xBadQ.get(eid)>0)
				vtmpflags[fBadQ]._state = flag::fPROBLEMATIC;
			else
				vtmpflags[fBadQ]._state = flag::fGOOD;

			// push all the flags for this subdet
			lssum._vflags.push_back(vtmpflags);
		}

		//	push all flags for all subdets for this LS
		_vflagsLS.push_back(lssum);
	}

	/*
	 * END JOB
	 * BOOK THE SUMMARY CONTAINERS, SET THE FLAGS
	 * RETURN THE LIST OF FLAGS FOR THIS DATATIER
	 */
	/* virtual */ std::vector<flag::Flag> RawRunSummary::endJob(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig)
	{

		if (_ptype!=fOffline)
			return std::vector<flag::Flag>();


		//	PREPARE LS BASED FLAGS to use it for booking
		std::vector<flag::Flag> vflagsLS;
		vflagsLS.resize(nRawFlag);
		vflagsLS[fEvnMsm]=flag::Flag("EvnMsm");
		vflagsLS[fBcnMsm]=flag::Flag("BcnMsm");
		vflagsLS[fBadQ]=flag::Flag("BadQ");


		//	INITIALIZE AND BOOK SUMMARY CONTAINERS
		ContainerSingle2D cSummaryvsLS; // summary per subdet: flag vs LS
		Container2D cSummaryvsLS_Subdet; // LS based flags vs LS for each subdet
		cSummaryvsLS.initialize(_name, "SummaryvsLS",
			new quantity::LumiSection(_maxProcessedLS),
			new quantity::DetectorQuantity(quantity::fSubdet),
			new quantity::ValueQuantity(quantity::fState),0);
		cSummaryvsLS_Subdet.initialize(_name, "SummaryvsLS",
			hashfunctions::fSubdet,
			new quantity::LumiSection(_maxProcessedLS),
			new quantity::FlagQuantity(vflagsLS),
			new quantity::ValueQuantity(quantity::fState),0);
		cSummaryvsLS_Subdet.book(ib, _emap, _subsystem);
		cSummaryvsLS.book(ib, _subsystem);

		/*
		 *	Iterate over each subdet
		 *		Iterate over each LS SUmmary
		 *			Iterate over all flags
		 *				set...
		 */

		std::vector<flag::Flag> sumflags; // flag per subdet
		int isubdet=0;
		for (auto& it_subdet : _vhashSubdets) {
			flag::Flag fSumRun("RAW"); // summary flag for this subdet
			HcalElectronicsId eid(it_subdet);

			//	ITERATE OVER EACH LS
			for (std::vector<LSSummary>::const_iterator itls=_vflagsLS.begin();
				itls!=_vflagsLS.end(); ++itls)
			{
				//	fill histograms per LS
				int iflag=0;
				flag::Flag fSumLS("RAW");
				for (std::vector<flag::Flag>::const_iterator ft=
					itls->_vflags[isubdet].begin(); ft!=itls->_vflags[isubdet].end();
					++ft)
				{
					//	Flag vs LS per subdet
					cSummaryvsLS_Subdet.setBinContent(eid, itls->_LS, int(iflag),
						ft->_state);
					fSumLS+=(*ft);
					iflag++;
				}
				//	Subdet vs LS
				cSummaryvsLS.setBinContent(eid, itls->_LS, fSumLS._state);
				fSumRun+=fSumLS;
			}
			
			//	push the summary flag for this Subdet for the whole RUN
			sumflags.push_back(fSumRun);
			
			//	increment the subdet counter
			isubdet++;
		}
	
		return sumflags;
	}
}
