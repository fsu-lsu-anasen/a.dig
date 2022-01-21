## (Another) DSSSD Internal Gainmatcher

Code for internal gain-matching multi segmented Si detectors. Presently tailor made for Micron MMM's having 16 rings on one side and 8 wedges on the other side, particularly for the Enge spectrograph+SABRE measurements at FSU.

Algorithm based on Reese et al (2015) at https://www.sciencedirect.com/science/article/pii/S0168900215000637

Requires output trees eventbuilt according to the FSU GWM sort code, courtesy Gordon W McCann 
(Ref: https://github.com/gwm17/GWM_EventBuilder)


#### How to use:


Three broad parts to it, encapsulated into 2 shell scripts and an executable. The shell scripts themselves call executables which need to be compiled using 'make' from the root (top-most) directory.


1) ./correlateAllMMMs.sh <run#> which calls ./correlateOneMMM.sh
This needs an eventbuilt ROOT tree. This step essentially generates all correlations inside of a detector, and saves the results in a folder named 'run#'.

2) ./bin/calibrator <fileX.root> which reads a ROOT file saved in Compass(v1.3.0 or above) that has recorded Am-241 data on SABRE, and calculates the global scale factors in gains to the five different MMM detectors. These numbers are then saved to the file ./etc/global_gain_scalefactors.dat - this file can be adjusted by hand if necessary.

3) ./IntGainMatcher.sh <run#>
Finally, once we generate all correlations, we use the internal gainmatcher part that reads the text outputs in steps 1 and 2 to generate a final output that can be directly used as a gainmatch file in the eventbuilder. 


#### Catches:


* Requires 5 'config' files inputN.dat(N=1 to 5) that set up the directory to look for the ROOT files. Edit the directory in all 5 of these so the program looks in the right place for 'analyzed' files. 
	
* These config files also have particulars pertaining to the bayesian propagator, and usually don't need to be varied across different detectors. But in the future, maybe this is necessary.
	
* At the moment, there's no documentation made available for the bayesian max-likelihood propagator but the procedure is almost identical to the recommendation in the original publication. 
	
* Lastly, the 'fitter' treats for dead channels inside the code at src/fitter/fitter.cpp. I haven't included the dead-channel information as a separate text-file, because this stage is best done with some caution. Remember to edit this as per your needs! 

Acknowledgements are due to Gordon McCann for design of the GainMap, ChannelMap classes, data structures, and ROOT dictionary definitions borrowed for use here from the original eventbuilder code.

#### Dependencies:

*	Tested on ROOT v6.22.06 in Debian 10, Linux 4.19.0-14-amd64
*	gcc/g++ v8.3
*	GNU Make v4.2.1

---

		Sudarsan B 
		LSU Baton Rouge 
		sbalak2@lsu.edu 
		bsudarsan92@gmail.com 

