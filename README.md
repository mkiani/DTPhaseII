# DTPhaseII


Code for DT demonstrator for electronics upgrade. This package contains the code needed for CMS DT Prompt Offline Analysis and for DT root-Ntuple production.

To install it and run DTNtuple production:

```
cmsrel CMSSW_10_3_1_patch3
cd CMSSW_10_3_1_patch3/src/
cmsenv

git clone https://github.com/mkiani/DTPhaseII.git .
git clone https://github.com/cmsdtoffline/DTDPGAnalysis UserCode/DTDPGAnalysis

scramv1 b -j 5

cd test
cmsRun RunTree_cosmics_cfg_testAB7_Demonstrator.py

A test data file from the miniDAQ is added in the test, one can change the path to new runs in the config file

```

To update the direcotry, 

git pull 
