#include "DQM/HcalTasks/interface/HcalOfflineHarvesting.h"

using namespace hcaldqm;
using namespace hcaldqm::constants;
using namespace hcaldqm::filter;

HcalOfflineHarvesting::HcalOfflineHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps), _reportSummaryMap(NULL)
{
	_vsumgen.resize(nSummary);
	_vmarks.resize(nSummary);
	_vnames.resize(nSummary);
	#ifndef HIDE_RAW
	_vnames[fRaw]="RawTask";
	#endif
	_vnames[fDigi]="DigiTask";
	_vnames[fReco]="RecHitTask";
	_vnames[fTP]="TPTask";
	for (uint32_t i=0; i<_vmarks.size(); i++)
		_vmarks[i]=false;

	#ifndef HIDE_RAW
	_vsumgen[fRaw]=new hcaldqm::RawRunSummary("RawRunHarvesting", _vnames[fRaw],ps);
	#endif
	_vsumgen[fDigi]=new hcaldqm::DigiRunSummary("DigiRunHarvesting", _vnames[fDigi],ps);
	_vsumgen[fReco]=new hcaldqm::RecoRunSummary("RecoRunHarvesting", _vnames[fReco],ps);
	_vsumgen[fTP]=new hcaldqm::TPRunSummary("TPRunHarvesting", _vnames[fTP],ps);
}

/* virtual */ void HcalOfflineHarvesting::beginRun(
	edm::Run const& r, edm::EventSetup const& es)
{
	DQHarvester::beginRun(r,es);

	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
		(*it)->beginRun(r,es);
}

//
//	For OFFLINE there is no per LS evaluation
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndLuminosityBlock(
	DQMStore::IBooker& ib,
	DQMStore::IGetter& ig, edm::LuminosityBlock const& lb, 
	edm::EventSetup const& es)
{	
	#ifndef HIDE_RAW
	if (ig.get(_subsystem+"/"+_vnames[fRaw]+"/EventsTotal")!=NULL)
		_vmarks[fRaw]=true;
	#endif
	if (ig.get(_subsystem+"/"+_vnames[fDigi]+"/EventsTotal")!=NULL)
		_vmarks[fDigi]=true;
	if (ig.get(_subsystem+"/"+_vnames[fTP]+"/EventsTotal")!=NULL)
		_vmarks[fTP]=true;
	if (ig.get(_subsystem+"/"+_vnames[fReco]+"/EventsTotal")!=NULL)
		_vmarks[fReco]=true;

	//	CALL ALL THE HARVESTERS
	int ii=0;
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
	{	
		//	run only if have to
		if (_vmarks[ii])
			(*it)->endLuminosityBlock(ib,ig,lb,es);
		ii++;
	}
}

//
//	Evaluate and Generate Run Summary
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndJob(DQMStore::IBooker& ib,
	DQMStore::IGetter& ig)
{
	//	OBTAIN/SET WHICH MODULES ARE PRESENT
	int num=0; std::map<std::string, int> datatiers;
	#ifndef HIDE_RAW
	if (_vmarks[fRaw])
	{
		datatiers.insert(std::pair<std::string, int>("RAW",num));
		num++;
	}
	#endif
	if (_vmarks[fDigi])
	{
		datatiers.insert(std::pair<std::string, int>("DIGI",num));
		num++;
	}
	if (_vmarks[fTP])
	{
		datatiers.insert(std::pair<std::string, int>("TP",num));
		num++;
	}
	if (_vmarks[fReco])
	{
		datatiers.insert(std::pair<std::string, int>("RECO",num));
		num++;
	}
	
	//	CREATE THE REPORT SUMMARY MAP
	//	num is #modules
	//	datatiers - std map [DATATIER_NAME] -> [positional value [0,num-1]]
	//	-> bin wise +1 should be
	if (!_reportSummaryMap)
	{
		ib.setCurrentFolder(_subsystem+"/EventInfo");
		_reportSummaryMap = ib.book2D("reportSummaryMap", "reportSummaryMap",
			_vSubdets.size(), 0, _vSubdets.size(), num,0,num);
		//	x axis labels
		
		for (uint32_t i=0; i<_vSubdets.size(); i++)
		{
			_reportSummaryMap->setBinLabel(i+1, hcaldqm::constants::SUBDETENUM_NAME.at(_vSubdets[i]), 1);
		}
		//	y axis lables
		for (std::map<std::string, int>::const_iterator
			it=datatiers.begin(); it!=datatiers.end(); ++it)
		{
			std::string name = it->first;
			int value = it->second;
			_reportSummaryMap->setBinLabel(value+1, name, 2);
		}
	}

	//	iterate over all summary generators and get the flags
	int ii=0;
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
	{
		//	IF MODULE IS NOT PRESENT IN DATA SKIP
		if (!_vmarks[ii])
		{ii++;continue;}

		//	OBTAIN ALL THE FLAGS FOR THIS MODULE
		//	AND SET THE REPORT STATUS MAP
		//	NOTE AGAIN: datatiers map [DATATIER]->[value not bin!]+1 therefore
		if (_debug>0)
			std::cout << _vnames[ii] << std::endl;
		std::vector<hcaldqm::flag::Flag> flags = (*it)->endJob(ib,ig);
		if (_debug>0)
		{
			std::cout << "********************" << std::endl;
			std::cout << "SUMMARY" << std::endl;
		}
		//for (uint32_t ifed=0; ifed<_vFEDs.size(); ifed++)
		for (uint32_t isubdet=0; isubdet<_vSubdets.size(); isubdet++)
		{
			_reportSummaryMap->setBinContent(isubdet+1, 
				datatiers[flags[isubdet]._name]+1, (int)flags[isubdet]._state);
			if (_debug>0)
			{
				std::cout << "Subdet =" << hcaldqm::constants::SUBDETENUM_NAME.at(_vSubdets[isubdet]) << std::endl;
				std::cout << flags[isubdet]._name << "  " << flags[isubdet]._state
				<<std::endl;
			}
		}
		ii++;
	}
}

DEFINE_FWK_MODULE(HcalOfflineHarvesting);
