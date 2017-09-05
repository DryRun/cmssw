#ifndef HashFunctions_h
#define HashFunctions_h

/**
 *	file:			HashFunctions.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Hash accoring to various Detector Elements
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

namespace hcaldqm
{
	using namespace constants;
	namespace hashfunctions
	{
		/**
		 *	by HcalDetId
		 */
		uint32_t hash_Subdet(HcalDetId const&);
		uint32_t hash_Subdetiphi(HcalDetId const&);
		uint32_t hash_Subdetieta(HcalDetId const&);
		uint32_t hash_Subdetdepth(HcalDetId const&);
		uint32_t hash_SubdetPM(HcalDetId const&);
		uint32_t hash_SubdetPMiphi(HcalDetId const&);
		uint32_t hash_iphi(HcalDetId const&);
		uint32_t hash_ieta(HcalDetId const&);
		uint32_t hash_depth(HcalDetId const&);
		uint32_t hash_HFPMiphi(HcalDetId const&);
		uint32_t hash_HBHEPartition(HcalDetId const&);
		uint32_t hash_SubdetPlan1(HcalDetId const&);
		uint32_t hash_DChannel(HcalDetId const&);

		/**
		 *	by ElectronicsId
		 */
		uint32_t hash_FED(HcalElectronicsId const&);
		uint32_t hash_FEDSpigot(HcalElectronicsId const&);
		uint32_t hash_FEDSlot(HcalElectronicsId const&);
		uint32_t hash_Crate(HcalElectronicsId const&);
		uint32_t hash_CrateSpigot(HcalElectronicsId const&);
		uint32_t hash_CrateSlot(HcalElectronicsId const&);
		uint32_t hash_Fiber(HcalElectronicsId const&);
		uint32_t hash_FiberFiberCh(HcalElectronicsId const&);
		uint32_t hash_FiberCh(HcalElectronicsId const&);
		uint32_t hash_Electronics(HcalElectronicsId const&);
		uint32_t hash_EChannel(HcalElectronicsId const&);

		/**
		 *	by TrigTowerDetId
		 */
		uint32_t hash_TTSubdet(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetPM(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetPMiphi(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetieta(HcalTrigTowerDetId const&);
		uint32_t hash_TTdepth(HcalTrigTowerDetId const&);
		uint32_t hash_TChannel(HcalTrigTowerDetId const&);

		std::string name_Subdet(HcalDetId const&);
		std::string name_Subdetiphi(HcalDetId const&);
		std::string name_Subdetieta(HcalDetId const&);
		std::string name_Subdetdepth(HcalDetId const&);
		std::string name_SubdetPM(HcalDetId const&);
		std::string name_SubdetPMiphi(HcalDetId const&);
		std::string name_iphi(HcalDetId const&);
		std::string name_ieta(HcalDetId const&);
		std::string name_depth(HcalDetId const&);
		std::string name_HFPMiphi(HcalDetId const&);
		std::string name_HBHEPartition(HcalDetId const&);
		std::string name_SubdetPlan1(HcalDetId const&);
		std::string name_DChannel(HcalDetId const&);

		uint32_t hash_Subdet(std::string const&);
		uint32_t hash_Subdetiphi(std::string const&);
		uint32_t hash_Subdetieta(std::string const&);
		uint32_t hash_Subdetdepth(std::string const&);
		uint32_t hash_SubdetPM(std::string const&);
		uint32_t hash_SubdetPMiphi(std::string const&);
		uint32_t hash_iphi(std::string const&);
		uint32_t hash_ieta(std::string const&);
		uint32_t hash_depth(std::string const&);
		uint32_t hash_HFPMiphi(std::string const&);
		uint32_t hash_HBHEPartition(std::string const&);
		uint32_t hash_SubdetPlan1(std::string const&);
		uint32_t hash_HEP17(std::string const&);
		uint32_t hash_DChannel(std::string const&);

		std::string name_FED(HcalElectronicsId const&);
		std::string name_FEDSpigot(HcalElectronicsId const&);
		std::string name_FEDSlot(HcalElectronicsId const&);
		std::string name_Crate(HcalElectronicsId const&);
		std::string name_CrateSpigot(HcalElectronicsId const&);
		std::string name_CrateSlot(HcalElectronicsId const&);
		std::string name_Fiber(HcalElectronicsId const&);
		std::string name_FiberFiberCh(HcalElectronicsId const&);
		std::string name_FiberCh(HcalElectronicsId const&);
		std::string name_Electronics(HcalElectronicsId const&);
		std::string name_EChannel(HcalElectronicsId const&);

		uint32_t hash_FED(std::string const&);
		uint32_t hash_FEDSpigot(std::string const&);
		uint32_t hash_FEDSlot(std::string const&);
		uint32_t hash_Crate(std::string const&);
		uint32_t hash_CrateSpigot(std::string const&);
		uint32_t hash_CrateSlot(std::string  const&);
		uint32_t hash_Fiber(std::string const&);
		uint32_t hash_FiberFiberCh(std::string const&);
		uint32_t hash_FiberCh(std::string const&);
		uint32_t hash_Electronics(std::string const&);
		uint32_t hash_EChannel(std::string const&);

		std::string name_TTSubdet(HcalTrigTowerDetId const&);
		std::string name_TTSubdetPM(HcalTrigTowerDetId const&);
		std::string name_TTSubdetPMiphi(HcalTrigTowerDetId const&);
		std::string name_TTSubdetieta(HcalTrigTowerDetId const&);
		std::string name_TTdepth(HcalTrigTowerDetId const&);
		std::string name_TChannel(HcalTrigTowerDetId const&);

		uint32_t hash_TTSubdet(std::string const&);
		uint32_t hash_TTSubdetPM(std::string const&);
		uint32_t hash_TTSubdetPMiphi(std::string const&);
		uint32_t hash_TTSubdetieta(std::string const&);
		uint32_t hash_TTdepth(std::string const&);
		uint32_t hash_TChannel(std::string const&);

		enum HashType
		{
			fSubdet,
			fSubdetiphi,
			fSubdetieta,
			fSubdetdepth,
			fSubdetPM,
			fSubdetPMiphi,
			fiphi,
			fieta,
			fdepth,
			fHFPMiphi,
			fHBHEPartition,
			fDChannel,
			MAXDIDHASHTYPE,
			fFED,
			fFEDSpigot,
			fFEDSlot,
			fCrate,
			fCrateSpigot,
			fCrateSlot,
			fFiber,
			fFiberFiberCh,
			fFiberCh,
			fElectronics,
			fEChannel,
			MAXEIDHASHTYPE,
			fTTSubdet,
			fTTSubdetPM,
			fTTSubdetPMiphi,
			fTTSubdetieta,
			fTTdepth,
			fTChannel,
			MAXTIDHASHTYPE,
		};
		typedef uint32_t (*hash_function_did)(HcalDetId const&);
		typedef uint32_t (*hash_function_eid)(HcalElectronicsId const&);
		typedef uint32_t (*hash_function_tid)(HcalTrigTowerDetId const&);
		typedef std::string (*name_function_did)(HcalDetId const&);
		typedef std::string (*name_function_eid)(HcalElectronicsId const&);
		typedef std::string (*name_function_tid)(HcalTrigTowerDetId const&);
		std::map<HashType, hash_function_did> const hash_did = {
			{fSubdet, hash_Subdet},
			{fSubdetiphi, hash_Subdetiphi},
			{fSubdetieta, hash_Subdetieta},
			{fSubdetdepth, hash_Subdetdepth},
			{fSubdetPM, hash_SubdetPM},
			{fSubdetPMiphi, hash_SubdetPMiphi},
			{fiphi, hash_iphi},
			{fieta, hash_ieta},
			{fdepth, hash_depth},
			{fHFPMiphi, hash_HFPMiphi},
			{fHBHEPartition, hash_HBHEPartition},
			{fDChannel, hash_DChannel},
		};
		std::map<HashType, hash_function_eid> const hash_eid = {
			{fFED, hash_FED},
			{fFEDSpigot, hash_FEDSpigot},
			{fFEDSlot, hash_FEDSlot},
			{fCrate, hash_Crate},
			{fCrateSpigot, hash_CrateSpigot},
			{fCrateSlot, hash_CrateSlot},
			{fFiber, hash_Fiber},
			{fFiberFiberCh, hash_FiberFiberCh},
			{fFiberCh, hash_FiberCh},
			{fElectronics, hash_Electronics},
			{fEChannel, hash_EChannel},
		};
		std::map<HashType, hash_function_tid> const hash_tid = {
			{fTTSubdet, hash_TTSubdet},
			{fTTSubdetPM, hash_TTSubdetPM},
			{fTTSubdetPMiphi, hash_TTSubdetPMiphi},
			{fTTSubdetieta, hash_TTSubdetieta},
			{fTTdepth, hash_TTdepth},
			{fTChannel, hash_TChannel},
		};
		std::map<HashType, name_function_did> const name_did = {
			{fSubdet, name_Subdet},
			{fSubdetiphi, name_Subdetiphi},
			{fSubdetieta, name_Subdetieta},
			{fSubdetdepth, name_Subdetdepth},
			{fSubdetPM, name_SubdetPM},
			{fSubdetPMiphi, name_SubdetPMiphi},
			{fiphi, name_iphi},
			{fieta, name_ieta},
			{fdepth, name_depth},
			{fHFPMiphi, name_HFPMiphi},
			{fHBHEPartition, name_HBHEPartition},
			{fDChannel, name_DChannel},
		};
		std::map<HashType, name_function_eid> const name_eid = {
			{fFED, name_FED},
			{fFEDSpigot, name_FEDSpigot},
			{fFEDSlot, name_FEDSlot},
			{fCrate, name_Crate},
			{fCrateSpigot, name_CrateSpigot},
			{fCrateSlot, name_CrateSlot},
			{fFiber, name_Fiber},
			{fFiberFiberCh, name_FiberFiberCh},
			{fFiberCh, name_FiberCh},
			{fElectronics, name_Electronics},
			{fEChannel, name_EChannel},
		};
		std::map<HashType, name_function_tid> const name_tid = {
			{fTTSubdet, name_TTSubdet},
			{fTTSubdetPM, name_TTSubdetPM},
			{fTTSubdetPMiphi, name_TTSubdetPMiphi},
			{fTTSubdetieta, name_TTSubdetieta},
			{fTTdepth, name_TTdepth},
			{fTChannel, name_TChannel},
		};
		std::map<HashType, std::string> const hash_names = {
			{fSubdet, "Subdet"},
			{fSubdetiphi, "Subdetiphi"},
			{fSubdetieta, "Subdetieta"},
			{fSubdetdepth, "Subdetdepth"},
			{fSubdetPM, "SubdetPM"},
			{fSubdetPMiphi, "SubdetPMiphi"},
			{fiphi, "iphi"},
			{fieta, "ieta"},
			{fdepth, "depth"},
			{fHFPMiphi, "HFPMiphi"},
			{fHBHEPartition, "HBHEPartition"},
			{fDChannel, "DChannel"},
			{fFED, "FED"},
			{fFEDSpigot, "FEDSpigot"},
			{fFEDSlot, "FEDSlot"},
			{fCrate, "Crate"},
			{fCrateSpigot, "CrateSpigot"},
			{fCrateSlot, "CrateSlot"},
			{fFiber, "Fiber"},
			{fFiberFiberCh, "FiberFiberCh"},
			{fFiberCh, "FiberCh"},
			{fElectronics, "Electronics"},
			{fEChannel, "EChannel"},
			{fTTSubdet, "TTSubdet"},
			{fTTSubdetPM, "TTSubdetPM"},
			{fTTSubdetPMiphi, "TTSubdetPMiphi"},
			{fTTSubdetieta, "TTSubdetieta"},
			{fTTdepth, "TTdepth"},
			{fTChannel, "TChannel"},
		};
	}
}

#endif
