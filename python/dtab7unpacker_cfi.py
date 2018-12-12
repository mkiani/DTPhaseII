import FWCore.ParameterSet.Config as cms

dturosunpacker = cms.EDProducer("DTAB7RawToDigi",
                                  DTuROS_FED_Source = cms.InputTag("source"),
                                  feds     = cms.untracked.vint32( 1368,),
                                  debug = cms.untracked.bool(True),
                               )
