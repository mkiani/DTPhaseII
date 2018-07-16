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

scramv1 b -j 5

cd test
cmsRun RunTree_cosmics_cfg_testuros_Demonstrator.py

enter a run number:

```


The runs are located in this location:  dtdqm@dtsx5-02.cern.ch:/home/runs/run000XXX


Copy the runs using the CopyRuns.sh to the eos space: /eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/


Rememeber: Change the alias inside the script of run e.g. run000, split, whatever is chosen:

```
./CopyRuns.sh RunNumber 

e.g. 

./CopyRuns.sh 240

will copy run: dtdqm@dtsx5-02.cern.ch:/home/runs/run000240 
to  /eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000240
```

You have to put the password twice for dtdqm@lxplus and dtdqm@sx5-02 (Same \*\*Top\*\*18)

