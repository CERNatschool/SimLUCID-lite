# SimLUCID-lite

*A pre-framework GEANT4 simulation of the LUCID experiment.*

## Introduction

Welcome! This is the GitHub repo for the GEANT4 simulation of the
**L**angton **U**ltimate **C**osmic ray **I**ntensity **D**etector (LUCID)
experiment.

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

"""bash
cd $ROOTSYS # change directory to my ROOT installation directory.
pwd # print the working directory, which is:
> /home/twhyntie/software/root-v5-34
"""

and my `GEANT4` installation here:

"""bash
cd $GEANT4INSTALLDIR
pwd
> /home/twhyntie/software/geant4.9.6.p02-install/
"""

You will need these for configuring `SimLUCID` later.


### Getting the code from GitHub

To clone the SimLUCID-lite repository to your system open a terminal
shell and type the following commands.

"""bash
cd $WORKINGDIR # environment variable for your working directory,
               # i.e. change directory to your working directory.
git clone https://github.com/CERNatschool/SimLUCID-lite
cd SimLUCID-lite
"""

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

"""bash
python configure.py $ROOTSYS $GEANT4INSTALLDIR
> # output from the script
cd simlucid-build
"""

Then run the setup script to set the required environment variables:

"""bash
source setup.sh # Run the setup script
"""

Then build and make `SimLUCID`:

"""bash
source buildandmake.sh #
"""

(You can take a look at these scripts to see what they're doing, if you like.)

### Testing

## Physics analysis using SimLUCID

Instructions to follow!

For now, you can look at the ROOT files produced by the simulation:

* `timepix.root`
* `pseudodet.root`


## Further reading

* [An introduction to LUCID](http://cernatschool.web.cern.ch/content/lucid);
* [CHEP2013 presentation on SimLUCID](https://indico.cern.ch/contributionDisplay.py?contribId=415&sessionId=3&confId=214784);
* [The Simon Langton Grammar School for Boys](http://www.thelangton.org.uk/);
* [The Langton Star Centre](http://thelangtonstarcentre.org);
* [The CERN@school website](http://cernatschool.web.cern.ch/).
