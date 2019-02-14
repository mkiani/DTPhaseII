import FWCore.ParameterSet.Config as cms

dtab7unpacker = cms.EDProducer("DTAB7RawToDigi",
                                  DTAB7_FED_Source = cms.InputTag("source"),
                                  feds     = cms.untracked.vint32( 1368,),
                                  debug = cms.untracked.bool(True),
                               )
