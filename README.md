# SimLUCID-lite

*A pre-framework GEANT4 simulation of the LUCID experiment.*

## Introduction

Welcome! This is the GitHub repo for the GEANT4 simulation of the
**L**angton **U**ltimate **C**osmic ray **I**ntensity **D**etector (LUCID)
experiment.
This code was used to generate the results featured in the CERN@school
paper:

* [T. Whyntie and M. A. Harrison, J. Phys.: Conf. Ser. 513 022038](http://dx.doi.org/10.1088/1742-6596/513/2/022038)

i.e. the proceedings from the corresponding conference presentation
at [CHEP 2013](https://indico.cern.ch/event/214784/contributions/1512648/).

### Disclaimers

* _This code dates from 2013. While every attempt has been
made to ensure that it is usable, some work may be required to get it
running on your own particular system.  Unfortunately CERN@school cannot
guarantee support for this.  Please proceed at your own risk_.
* _This repository is now deprecated, and remains here for legacy purposes.
For future work regarding LUCID and CERN@school, please refer to the
[Institute for Research in Schools](http://researchinschools.org) (IRIS)
[GitHub repository](https://github.com/InstituteForResearchInSchools).
Please also feel free to fork and modify this code as required for
your own research._

### Acknowledgements

CERN@school was supported by
the UK [Science and Technology Facilities Council](http://www.stfc.ac.uk) (STFC)
via grant numbers ST/J000256/1 and ST/N00101X/1,
as well as a Special Award from the Royal Commission for the Exhibition of 1851.
The CERN@school Collaboration would also like to acknowledge the support
provided by the [GridPP Collaboration](http://www.gridpp.ac.uk)
in terms of both computing resources and technical guidance from
collaboration members.

## Installation instructions

### Operating system and environment

We run **SimLUCID** (lite) on Xubuntu 13.04. If it's easier for you,
you can run a Xubuntu 13.04 *virtual machine* on Windows using
[VirtualBox](https://www.virtualbox.org/).

### Prerequisite software

You will need to install the following software on your system:

* `Python`
* `ROOT`
* `GEANT4`

Make a note of where your installations are located. For example, I
keep my `ROOT` installation here:

```bash
cd $ROOTSYS # change directory to my ROOT installation directory.
pwd # print the working directory, which is:
> /home/alovelace/software/root-v5-34
```

and my `GEANT4` installation here:

```bash
cd $GEANT4INSTALLDIR
pwd
> /home/alovelace/software/geant4.9.6.p02-install/
```

You will need these for configuring `SimLUCID` later.


### Getting the code from GitHub

To clone the SimLUCID-lite repository to your system open a terminal
shell and type the following commands.

```bash
cd $WORKINGDIR # environment variable for your working directory,
               # i.e. change directory to your working directory.
git clone https://github.com/CERNatschool/SimLUCID-lite
cd SimLUCID-lite
```

Advanced users may wish to *fork* the `SimLUCID-lite` repo so that
you can play with the code and commit your own changes to your GitHub
account. Go for it!

### Building SimLUCID

Now that you have the `SimLUCID-lite` code on your
`ROOT`- and `GEANT4`-ready system, you're ready to "build" the
`SimLUCID-lite` executable (i.e. the file that will actually perform the
simulation).

There are two shell scripts that must be generated in order to setup the
working environment and build the `SimLUCID` program. These are:

* `setup.sh`: sets up the environment variables;
* `buildandmake.sh`: builds the `SimLUCID` program.

These are not included with the code from GitHub; rather, the `Python`
script `configure.py` creates these for you with arguments you supply:

```bash
python configure.py $ROOTSYS $GEANT4INSTALLDIR
> # output from the script
cd simlucid-build
```

Then run the setup script to set the required environment variables:

```bash
source setup.sh # Run the setup script
```

Then build and make `SimLUCID`:

```bash
source buildandmake.sh #
```

(You can take a look at these scripts to see what they're doing, if you like.)

### Testing

If you're running on Xubuntu and have `OpenInventor` installed, you should
be able to run the visualisation example provided:

```bash
./SimLucid visualtest.in
```

Alternatively, you can run the simulation in *batch mode* using the
following example:

```bash
./SimLucid batchtest.in
```

## Physics analysis using SimLUCID
To start to investigate the physics,
you can look at the ROOT files produced by the simulation:

* `timepix.root`
* `pseudodet.root`

Further information can be found in the CHEP 2013 paper referenced
below.

## Useful links

* The [Institute for Research in Schools](http://researchinschools.org) (IRIS) homepage;
* The [IRIS CERN@school website](http://researchinschools.org/CERN);
* The [IRIS GitHub repository](https://github.com/InstituteForResearchInSchools);
* The [CHEP 2013 presentation on SimLUCID](https://indico.cern.ch/contributionDisplay.py?contribId=415&sessionId=3&confId=214784);
* The [CHEP 2013 conference proceedings](http://dx.doi.org/10.1088/1742-6596/513/2/022038) (_JINST_, Open Access);
* The [CERN@school Document Index](http://doi.org/10.5281/zenodo.227090).
