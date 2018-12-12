import FWCore.ParameterSet.Config as cms

process = cms.Process("DTNT")

process.load("EventFilter.DTAB7RawToDigi.dtab7unpacker_cfi")
#process.load("EventFilter.DTRawToDigi.uROSStage2Digis_cfi")

##process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/Services_cff')

###process.load('Configuration/StandardSequences/Geometry_cff')  ##  Deprecated in new versions > 53X
##process.load('Configuration.Geometry.GeometryIdeal_cff')  ## In versions 7X problems with few STA events crashing the runing # but it works in the first tests of 750pre5 
##process.load('Configuration/StandardSequences/GeometryDB_cff')  
process.load('Configuration/StandardSequences/GeometryRecoDB_cff')  ##  solve STA problem
process.load('Configuration/EventContent/EventContent_cff')
process.load("Geometry.DTGeometryBuilder.dtGeometryDB_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

###process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cff")
##process.load("Geometry.DTGeometry.dtGeometry_cfi")
##process.load("Geometry.CSCGeometry.cscGeometry_cfi")
##process.load("Geometry.DTGeometryBuilder.idealForDigiDtGeometry_cff")
##process.load("Geometry.DTGeometryBuilder.idealForDigiDtGeometry_cff")

########################################################################### 
##### EEEEEEEEEEEEE Line added for uros test, could be commented 
###process.Timing = cms.Service("Timing")
########################################################################### 

######   COMENTED FOR uROS !!!!!!  #######################################  
##process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
##process.load("EventFilter.DTAB7RawToDigi.dtunpackerDDUGlobal_cfi")
##process.dtunpacker.readOutParameters.debug = False
##process.dtunpacker.readOutParameters.rosParameters.debug = False
########################################################################### 
########   UNPACKER FOR uROS !!!!!!  #######################################  
### For.dat or .root files but NOT for .raw data!!!!!!!!!!!!!!!
process.dtab7unpacker.DTuROS_FED_Source = 'rawDataCollector'
########################################################################### 


# for TWINMUX (Start to use in 2016)
process.load("EventFilter.L1TXRawToDigi.twinMuxStage2Digis_cfi")

#for RAW
###process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
##process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")
##process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
##process.load('Configuration/StandardSequences/GeometryExtended_cff')  ## For Run1
###process.load('Configuration.Geometry.GeometryExtended2016_cff')  ## 
process.load("RecoMuon.TrackingTools.MuonServiceProxy_cff")

##process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
##process.GlobalTag.globaltag = "GR_E_V48::All"
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")  #DB v2, at least since GR_E_V42
##process.GlobalTag.globaltag = '80X_dataRun2_Express_v0' ##for CMSSW_8_0_X X>0
##process.GlobalTag.globaltag = '80X_dataRun2_Express_v1' ##for CMSSW_8_0_X X>0
##process.GlobalTag.globaltag = '80X_dataRun2_Express_v4' ##for CMSSW_8_0_X X>0
##process.GlobalTag.globaltag = '90X_dataRun2_Express_v1' ##for CMSSW_9_0_X 
process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' ## For Express after 920 May2017 



# for the emulator
process.load("L1TriggerConfig.DTTPGConfigProducers.L1DTTPGConfigFromDB_cff")
process.load("L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi")
process.dtTriggerPrimitiveDigis.debug = False
process.L1DTConfigFromDB.debug = False

process.load('EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi')
process.load('RecoLuminosity.LumiProducer.lumiProducer_cfi')

# process.load('EventFilter.L1TRawToDigi.l1tRawtoDigiBMTF_cfi')
process.load('EventFilter.L1TRawToDigi.bmtfDigis_cfi')


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
labelRawDataLikeMC = cms.untracked.bool(False)
############################# EVENTS FILES ROOT  ############################################## 
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring
   (
###     ##'/store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-PromptReco-v4/000/258/159/00000/0EFE474F-D26B-E511-9618-02163E011F4B.root'
###     ##'/store/express/Commissioning2016/ExpressPhysics/FEVT/Express-v1/000/270/389/00000/A0D5994E-9506-E611-A4A7-02163E0141CE.root'
###     ##'/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/290/348/00000/7215813E-1614-E711-90DC-02163E011E08.root'
###     ##'/store/express/Run2017A/ExpressCosmics/FEVT/Express-v1/000/294/696/00000/9AFC1E6C-713D-E711-8F3B-02163E011CE3.root'
#       'file:/afs/cern.ch/user/m/mkiani/work/Demonstrator/test/run000506.root',
### 
	'file:A01CC3C8-B413-D449-BB55-AE4C78CD93F7.root'
   ),
   secondaryFileNames = cms.untracked.vstring(
  )
)
############################################################################################### 
####################### EVENTS uROS Slice test (.dat files) ################################### 
###process.source = cms.Source("NewEventStreamFileReader",
###           fileNames = cms.untracked.vstring(
###          ## '/store/caf/user/dtdqm/InputMiniDAQRuns/dat/Minidaq.00213193.0001.A.storageManager.00.0000.dat'
###          ##'file:/afs/cern.ch/user/d/dtdqm/public/run224439/run224439_ls0064_streamA_StorageManager.dat'
###          'file:../../CodefromJorge_23May17/SlideTestDataAndPython/run291038_ls0079_streamA_StorageManager.dat'
###         )
###)
### ###readFiles = cms.untracked.vstring()
### ### secFiles = cms.untracked.vstring()
### ###process.source = cms.Source ("NewEventStreamFileReader", fileNames = readFiles)
### ###readFiles.extend( [
### ###         ####'file:run291038_ls0079_streamA_StorageManager.dat'
### ###         'file:../../CodefromJorge_23May17/SlideTestDataAndPython/run291038_ls0079_streamA_StorageManager.dat'
### ###          ] );
### ### 
### ###secFiles.extend( ( ) )
############################################################################################### 


############################################################################################### 
########### EVENTS uROS demonstrator test (.raw files) ######################################## 
############################################################################################### 

# creating a list for all the filesi
#import os
#file_list = []
#for file in os.listdir('/eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000'+runnumberString+'/'): 
#    if file.endswith('.raw'):
#        file_list.append('file:'+'/eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000'+runnumberString+'/'+str(file))
#
#
#process.source =   cms.Source("FRDStreamSource",
#                              fileNames = cms.untracked.vstring(
#        #                  'file:/eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000'+runnumberString+'/run000'+runnumberString+'_ls0001_index000000.raw',
#        file_list
#        )
#                              )
#
#
#
#from CondCore.DBCommon.CondDBSetup_cfi import * ## Deprecated v800
process.load("CondCore.CondDB.CondDB_cfi") ## Substitute the deprecated version
process.CondDBSetup = process.CondDB.clone() ## New lines needed when using CondCore.CondDB.CondDB_cfi
process.CondDBSetup.__delattr__('connect') ## New lines needed when using CondCore.CondDB.CondDB_cfi

##############  tTrig DB ######################################################################### 
#####   Uncoment and change the appropriate lines in case you need a 
#####   different tTrig to the one use in the Global tag
################################################################################################## 
###
process.ttrigsource = cms.ESSource("PoolDBESSource",
                                   ##CondDBSetup,
                                   process.CondDBSetup,
#                                  timetype = cms.string('runnumber'),
                                   toGet = cms.VPSet(cms.PSet(record = cms.string('DTTtrigRcd'),
                                                              label = cms.untracked.string('cosmics'),  ## ONLY if using cosmic reconstruction  
                                                              ###tag = cms.string('DT_tTrig_CRAFT31X_V01_offline')
                                                              tag = cms.string('ttrig')
                                                              ###tag = cms.string('DT_tTrig_cosmic2009_V02_hlt')
                                                              ###tag = cms.string('DT_tTrig_cosmic2009_V02_prompt')
                                                              ###tag = cms.string('DT_tTrig_cosmics_2009_v3_prompt')
                                                              )
                                                     ),
                                   ####### For a different DB in ORCON/ORCOFF use the next sentence ########################## 
                                   ###connect = cms.string('frontier://Frontier/CMS_CONDITIONS'),  ## DB V2
                                   #####connect = cms.string('frontier://Frontier/CMS_COND_31X_DT'), ## Old DBv1
                                   ####### For private db in a private directory change accordly the next sentence ########### 
                                   ##connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DT/OfflineCode/LocalDataBases/ttrig_residuals_227331-new2.db'),
                                                                      connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DT/OfflineCode/uROS/Demonstrator/tTrigsDB/ttrig_Demonstrator_Run331.db'),
                                   #connect = cms.string('sqlite_file:./ttrig_new.db'),
                                   
                                   
                                   authenticationMethod = cms.untracked.uint32(0)
                                   )

process.es_prefer_ttrigsource = cms.ESPrefer('PoolDBESSource','ttrigsource')
##########   End tTrig DB ######################################################################## 


    
    
############################################################################################### 
##     LOCAL RECO FOR COSMIC DATA!!!! 
############################################################################################### 
####from RecoTracker.Configuration.RecoTracker_cff import *  ## Needed at least in  710pre8 to avoid an error in RecoMuonCosmics file (GroupedCkfTrajectoryBuilder)
from RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff import *
#process.dt1DRecHits.dtDigiLabel = 'dtunpacker'
process.dt1DRecHits.dtDigiLabel = 'dtab7unpacker:DTuROSDigis'
#process.dt1DRecHits.dtDigiLabel = 'uROSStage2Digis:DTDigis'


#this is to select collisions
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           #cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
                                           cut = cms.string("!isFake && ndof > 4"), # && abs(z) <= 15 && position.Rho <= 2" # tracksSize() > 3 for the older cut
                                           filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
                                           )

process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )

process.DTMuonSelection = cms.EDFilter("DTMuonSelection",
                                       src = cms.InputTag('muons'),
                                       Muons = cms.InputTag('muons'),
                                       SAMuons = cms.InputTag('standAloneMuons'),
                                       dtSegmentLabel = cms.InputTag('dt4DSegments'),
                                       etaMin = cms.double(-1.25),
                                       etaMax = cms.double(1.25),
                                       ptMin = cms.double(3.),
                                       tightness = cms.int32(0) # 0 = loose (e.g. unstable collisions, minimum bias, requires a DT segment)
                                       # 1 = medium (e.g. cosmics, requires a stand alone muon)
                                       # 2 = tight (good collisions, requires a global muon)
                                       )


process.load("UserCode/DTDPGAnalysis/DTTTreGenerator_cfi")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.myDTNtuple.localDTmuons = cms.untracked.bool(True)
# OTPUT
process.myDTNtuple.outputFile = "DTNtuple_uros_ab7.root"
process.myDTNtuple.dtTrigSimDCCLabel = cms.InputTag("dtTriggerPrimitiveDigis")
##process.myDTNtuple.dtDigiLabel = cms.InputTag("dtunpacker")
process.myDTNtuple.dtDigiLabel = cms.InputTag("dtab7unpacker:DTuROSDigis")
#process.myDTNtuple.dtDigiLabel = cms.InputTag("uROSStage2Digis:DTDigis")

##process.myDTNtuple.staMuLabel = cms.InputTag("standAloneMuons")

process.myDTNtuple.bmtfOutputDigis = cms.InputTag("bmtfDigis:BMTF")

## RPC unpacking
process.load("EventFilter.RPCRawToDigi.rpcUnpackingModule_cfi")

## RPC recHit
process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
process.load("Geometry.RPCGeometry.rpcGeometry_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.load("RecoLocalMuon.RPCRecHit.rpcRecHits_cfi")
process.rpcRecHits.rpcDigiLabel = cms.InputTag('rpcUnpackingModule')

## pp collisions before 2016 (DTTF - DCC)
###process.p = cms.Path(process.DTMuonSelection *  process.dtunpacker * process.dttfunpacker * process.scalersRawToDigi * process.muonDTDigis * process.dtTriggerPrimitiveDigis + process.myDTNtuple)

## pp collisions from 2016 (TM)
##process.p = cms.Path( process.DTMuonSelection * process.dtunpacker * process.twinMuxStage2Digis  * process.scalersRawToDigi * process.lumiProducer * process.muonDTDigis * process.dtTriggerPrimitiveDigis + process.myDTNtuple)

### Cosmics before 2016 (DTTF - DCC) 
###process.p = cms.Path(process.dtunpacker * process.dttfunpacker + process.myDTNtuple)

### Cosmics since 2016 (TM) 
### For cosmics MWGR2016 first is needed to put false the part of Global muons because the global cosmics muon tracks are not available
############ process.myDTNtuple.AnaTrackGlobalMu = cms.untracked.bool(False)    ## Comment this when the global cosmics muon tracks are available again
##process.p = cms.Path(process.dtunpacker * process.twinMuxStage2Digis * process.lumiProducer + process.myDTNtuple)


### Cosmics after RPC variables included on CMSSW_9_0_0 (2017) 
####process.p = cms.Path(process.dtunpacker * process.twinMuxStage2Digis * process.lumiProducer + process.BMTFStage2Digis + process.rpcUnpackingModule + process.rpcRecHits + process.myDTNtuple)
###process.p = cms.Path(process.dtunpacker * process.twinMuxStage2Digis * process.lumiProducer  + process.bmtfDigis + process.rpcUnpackingModule + process.rpcRecHits + process.myDTNtuple)


###### Cosmics uros Slice TEST on CMSSW_9_2_0_patch1 (2017) 
###### Only DTDigiProduction & storage 
##process.myDTNtuple.Include4DSegments =  cms.untracked.bool(False)
##process.p = cms.Path(process.dtab7unpacker + process.myDTNtuple) ## Uncomment also previous line to avoid crashes with the non-existen 4DSegs
###### DTDigiProduction & 4DSegment production
process.p = cms.Path(process.dtab7unpacker * process.dt1DRecHits * process.dt2DSegments * process.dt4DSegments + process.myDTNtuple)
#process.p = cms.Path(process.uROSStage2Digis * process.dt1DRecHits * process.dt2DSegments * process.dt4DSegments + process.myDTNtuple)


### Collisions after RPC variables included on CMSSW_9_0_0 (2017) 
####process.p = cms.Path( process.DTMuonSelection * process.dtunpacker * process.twinMuxStage2Digis  * process.scalersRawToDigi * process.lumiProducer * process.muonDTDigis * process.dtTriggerPrimitiveDigis + process.BMTFStage2Digis + process.rpcUnpackingModule + process.rpcRecHits + process.myDTNtuple)
##process.p = cms.Path( process.DTMuonSelection * process.dtunpacker * process.twinMuxStage2Digis  * process.scalersRawToDigi * process.lumiProducer * process.muonDTDigis * process.dtTriggerPrimitiveDigis + process.bmtfDigis + process.rpcUnpackingModule + process.rpcRecHits + process.myDTNtuple)


## RE-RECO with CMSSW712: RECO only in the dataset!
#process.myDTNtuple.runOnRaw = cms.bool(False)
#process.p = cms.Path(process.myDTNtuple)

# Output
process.out = cms.OutputModule("PoolOutputModule"
                               , outputCommands = cms.untracked.vstring(
        "keep *",
        "keep *_*_*_testRPCTwinMuxRawToDigi"
        , "keep *_*_*_DTNTandRPC"
        )
                               #                                , fileName = cms.untracked.string("file:cia.root")
                               , SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
                               )


    
