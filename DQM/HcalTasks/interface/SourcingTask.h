#ifndef SourcingTask_h
#define SourcingTask_h

/*
 *	file:			SourcingTask.h
 *	Author:			David Yu
 *	Description:
 *		Task for sourcing
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf1D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"
#include "DQM/HcalCommon/interface/HashFilter.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"
#include "DataFormats/HcalDigi/interface/HcalUHTRhistogramDigiCollection.h"

class SourcingTask : public hcaldqm::DQTask
{
	public:
		SourcingTask(edm::ParameterSet const&);
		virtual ~SourcingTask(){}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(hcaldqm::UpdateFreq);

		//	tags and tokens
		edm::InputTag	_tag_uhtr;
		edm::EDGetTokenT<HcalUHTRhistogramDigiCollection> _tok_uhtr;

		//	Electronics Maps/Hashes
		HcalElectronicsMap const* _emap;
		hcaldqm::electronicsmap::ElectronicsMap _ehashmap;
		
		hcaldqm::Container2D _cOccupancy_Crate[4];
		hcaldqm::Container2D _cOccupancy_CrateSlot[4];
		hcaldqm::Container2D _cOccupancy_depth[4];

		hcaldqm::ContainerProf2D _cEntries_Crate[4];
		hcaldqm::ContainerProf2D _cEntries_CrateSlot[4];
		hcaldqm::ContainerProf2D _cEntries_depth[4];

		hcaldqm::ContainerProf2D _cMean_Crate[4];
		hcaldqm::ContainerProf2D _cMean_CrateSlot[4];
		hcaldqm::ContainerProf2D _cMean_depth[4];

		hcaldqm::ContainerProf2D _cRMS_Crate[4];
		hcaldqm::ContainerProf2D _cRMS_CrateSlot[4];
		hcaldqm::ContainerProf2D _cRMS_depth[4];


};

#endif



