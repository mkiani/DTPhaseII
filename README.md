# Demonstrator


Code for DT demonstrator for electronics upgrade. This package contains the code needed for CMS DT Prompt Offline Analysis and for DT root-Ntuple production.

To install it and run DTNtuple production:

```
cmsrel CMSSW_10_1_1

cd CMSSW_10_1_1/src/

cmsenv

git clone https://github.com/cmsdtoffline/DTDPGAnalysis UserCode/DTDPGAnalysis

git clone https://github.com/mkiani/Demonstrator.git EventFilter/DTuROSRawToDigi 

scramv1 b -j 5

```
