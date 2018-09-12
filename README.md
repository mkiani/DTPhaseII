# Demonstrator


Code for DT demonstrator for electronics upgrade. This package contains the code needed for CMS DT Prompt Offline Analysis and for DT root-Ntuple production.

To install it and run DTNtuple production:

```
cmsrel CMSSW_10_1_1
cd CMSSW_10_1_1/src/
cmsenv

git clone https://github.com/cmsdtoffline/DTDPGAnalysis UserCode/DTDPGAnalysis
git clone https://github.com/mkiani/Demonstrator.git EventFilter/DTuROSRawToDigi 

cp -r EventFilter/DTuROSRawToDigi/test . 
mv BunchIDfromTDC.h UserCode/DTDPGAnalysis/interface/

scramv1 b -j 5

cd test
cmsRun RunTree_cosmics_cfg_testuros_Demonstrator.py

enter a run number:

```
The instruction on how to take and copy runs to the eos is provided in the wiki: https://github.com/mkiani/Demonstrator/wiki/DT-Demonstrator 
