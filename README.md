# Demonstrator


Code for DT demonstrator for electronics upgrade. This package contains the code needed for CMS DT Prompt Offline Analysis and for DT root-Ntuple production.

To install it and run DTNtuple production:

```
cmsrel CMSSW_10_1_1

cd CMSSW_10_1_1/src/

cmsenv

git clone https://github.com/cmsdtoffline/DTDPGAnalysis UserCode/DTDPGAnalysis

git clone https://github.com/mkiani/Demonstrator.git EventFilter/DTuROSRawToDigi 

mv EventFilter/DTuROSRawToDigi/test . 

scramv1 b -j 5

cd test

cmsRun RunTree_cosmics_cfg_testuros_Demonstrator.py

enter a run number:

```


The runs are located in this location: /eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/
