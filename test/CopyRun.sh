ssh -Y dtdqm@lxplus.cern.ch << EOF

	echo "Change the alias in the script accordingly, e.g. run000, split, c_split"
        echo "run number:" "run000$1"
        scp -r dtdqm@dtsx5-02.cern.ch:/home/runs/run000"$1" .
        eos mkdir /eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000"$1"
	xrdcp -r run000"$1" root://eoscms//eos/cms/store/group/dpg_dt/comm_dt/uROS/DemonstratorTests/RawData/run000"$1" 
	rm -rf run000"$1"
        bash -l

EOF
