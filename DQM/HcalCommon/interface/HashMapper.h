#ifndef HashMapper_h
#define HashMapper_h

/**
 *	file:			HashMapper.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *
 */

#include "DQM/HcalCommon/interface/Mapper.h"
#include "DQM/HcalCommon/interface/HashFunctions.h"

namespace hcaldqm
{
	using namespace hashfunctions;
	namespace mapper
	{
		class HashMapper : public Mapper
		{
			public:
				//	constructors/destructors
				HashMapper() {}
				HashMapper(HashType htype) : Mapper(), _htype(htype) 
				{}
				virtual ~HashMapper() {}

				//	initialize
				virtual void initialize(HashType htype) {_htype = htype;}

				//	get hash
				using Mapper::getHash;
				virtual uint32_t getHash(HcalDetId const& did) const
				{return hash_did.at(_htype)(did);}
				virtual uint32_t getHash(HcalElectronicsId const& eid) const
				{return hash_eid.at(_htype)(eid);}
				virtual uint32_t getHash(HcalTrigTowerDetId const& tid) const
				{return hash_tid.at(_htype)(tid);}

				//	get name of the hashed element
				using Mapper::getName;
				virtual std::string getName(HcalDetId const &did) const
				{return hashfunctions::name_did.at(_htype)(did);}
				virtual std::string getName(HcalElectronicsId const& eid) const
				{return hashfunctions::name_eid.at(_htype)(eid);}
				virtual std::string getName(HcalTrigTowerDetId const& tid) const
				{return hashfunctions::name_tid.at(_htype)(tid);}

				//	get the Hash Type Name
				virtual std::string getHashTypeName() const
				{return hash_names.at(_htype);}
				virtual HashType getHashType() const
				{return _htype;}

				//	determine the type of the hash
				virtual bool isDHash()  const
				{return _htype<MAXDIDHASHTYPE ? true : false;}
				virtual bool isEHash() const
				{
					return (_htype>MAXDIDHASHTYPE && _htype<MAXEIDHASHTYPE) ? 
						true : false;
				}
				virtual bool isTHash() const
				{
					return (_htype>MAXEIDHASHTYPE && _htype<MAXTIDHASHTYPE) ? 
						true : false;
				}

			protected:
				HashType _htype;
		};
	}
}

#endif
