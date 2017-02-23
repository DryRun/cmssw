import FWCore.ParameterSet.Config as cms

sourcingTask = cms.EDAnalyzer(
	"SourcingTask",
	
	#	standard parameters
	name = cms.untracked.string("SourcingTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(0),
	mtype = cms.untracked.bool(True),
	subsystem = cms.untracked.string('Hcal'),

	#	tags
    hcalTBTriggerDataTag = cms.InputTag("tbunpack"),
	tagUHTR = cms.untracked.InputTag("histoUnpack"),

)


