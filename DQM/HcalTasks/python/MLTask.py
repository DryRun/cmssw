import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
mlTask = DQMEDAnalyzer(
	"MLTask",
	
	#	standard parameters
	name = cms.untracked.string("MLTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(0),
	mtype = cms.untracked.bool(True),
	subsystem = cms.untracked.string("Hcal"),

	#	tags
	tagHBHE = cms.untracked.InputTag("hcalDigis"),
	tagHO = cms.untracked.InputTag("hcalDigis"),
	tagHF = cms.untracked.InputTag("hcalDigis"),

	#	Cuts
	cutSumQ_HBHE = cms.untracked.double(20),
	cutSumQ_HO = cms.untracked.double(20),
	cutSumQ_HF = cms.untracked.double(20),

	# Number of LSes to integrate
	nLS = cms.untracked.uint32(10),

	# Reference digi sizes
	refDigiSize = cms.untracked.vuint32(10, 10, 10, 4), # HB, HE, HO, HF
)

from Configuration.Eras.Modifier_run2_HF_2017_cff import run2_HF_2017
#run2_HF_2017.toModify(mlTask, qie10InConditions=cms.untracked.bool(True))
run2_HF_2017.toModify(mlTask, refDigiSize=cms.untracked.vuint32(10, 10, 10, 3))

from Configuration.Eras.Modifier_run2_HCAL_2018_cff import run2_HCAL_2018
#run2_HCAL_2018.toModify(mlTask, qie10InConditions=cms.untracked.bool(True))
run2_HCAL_2018.toModify(mlTask, refDigiSize=cms.untracked.vuint32(8, 8, 10, 3))
