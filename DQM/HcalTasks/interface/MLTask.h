#ifndef MLTask_h
#define MLTask_h

/**
 *	file:			MLTask.h
 *	Author:			VK
 *	Description:
 *		HCAL DIGI Data Tier Processing.
 *
 *	Online:
 *		
 *	Offline:
 *		- HF Q2/(Q1+Q2) is not included.
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/HashFilter.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf2D.h"
#include "DQM/HcalCommon/interface/ContainerXXX.h"

class MLTask : public hcaldqm::DQTask
{
	public:
		MLTask(edm::ParameterSet const&);
		~MLTask() override {
			_cOccupancy_depth_LS.clear();
		}

		void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&) override;
		void beginLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&) override;
		void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&) override;

	protected:
		void _process(edm::Event const&, edm::EventSetup const&) override;
		void _resetMonitors(hcaldqm::UpdateFreq) override;

		edm::InputTag		_tagHBHE;
		edm::InputTag		_tagHE;
		edm::InputTag		_tagHO;
		edm::InputTag		_tagHF;
		edm::EDGetTokenT<HBHEDigiCollection> _tokHBHE;
		edm::EDGetTokenT<QIE11DigiCollection> _tokHE;
		edm::EDGetTokenT<HODigiCollection>	 _tokHO;
		edm::EDGetTokenT<QIE10DigiCollection>	_tokHF;

		double _cutSumQ_HBHE, _cutSumQ_HE, _cutSumQ_HO, _cutSumQ_HF;
		unsigned int _nLS;

		//	flag vector
		std::vector<hcaldqm::flag::Flag> _vflags;
		enum MLFlag
		{
			fUnknownIds = 0,
			fExampleMLAlgorithm = 1,
			nMLFlag = 2
		};

		//	hashes/FED vectors
		std::vector<uint32_t> _vhashFEDs;

		std::map<HcalSubdetector, int> _refDigiSize;

		//	emap
		hcaldqm::electronicsmap::ElectronicsMap _ehashmap; // online only
		hcaldqm::electronicsmap::ElectronicsMap _dhashmap;

		//	Filters
		//hcaldqm::filter::HashFilter _filter_VME;

		/* hcaldqm::Containers */
		hcaldqm::Container2D _cOccupancy_depth;
		std::vector<hcaldqm::Container2D*> _cOccupancy_depth_LS;

		//	#events counters
		MonitorElement *meNumEvents1LS; // to transfer the #events to harvesting
		MonitorElement *meUnknownIds1LS;
		bool _unknownIdsPresent;

		hcaldqm::Container2D _cSummaryvsLS_FED; // online only
		hcaldqm::ContainerSingle2D _cSummaryvsLS; // online only

};

#endif
