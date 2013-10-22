#!/usr/bin/env python

#========================================
# SimLUCID (lite) - configuration script
#========================================

import os, inspect
import sys
import argparse

if __name__=="__main__":

    # Get the path of the current directory
    path = os.path.dirname(
        os.path.abspath(inspect.getfile(inspect.currentframe())))

    # Get the arguments from the command line

    parser = argparse.ArgumentParser()
    parser.add_argument("rootsys",   help="Path to your ROOT installation"  )
    parser.add_argument("geant4sys", help="Path to your GEANT4 installation")
    args = parser.parse_args()

    rootsysdir       = args.rootsys
    geant4installdir = args.geant4sys

    print
    print(" * Current working directory is %s" % (path))
    print(" *")
    print(" * ROOT   is installed in '%s'"     % (rootsysdir))
    print(" * GEANT4 is installed in '%s'"     % (geant4installdir))
    print

    # Create the setup script (for setting the environment variables)
    sf = open("simlucid-build/setup.sh", "w")
    sf.write("#!bin/bash\n")
    sf.write("#----------------------------------------------------\n")
    sf.write("# SimLUCID setup script - autogenerated, DO NOT EDIT \n")
    sf.write("#----------------------------------------------------\n")
    sf.write("#\n")
    sf.write("source %s/bin/thisroot.sh\n" % (rootsysdir))
    sf.write("source %s/share/Geant4-9.6.2/geant4make/geant4make.sh\n" % \
        (geant4installdir))
    sf.close()

    # Create the build and make script
    bf = open("simlucid-build/buildandmake.sh", "w")
    bf.write("#!bin/bash\n")
    bf.write("#----------------------------------------------------\n")
    bf.write("# SimLUCID build script - autogenerated, DO NOT EDIT \n")
    bf.write("#----------------------------------------------------\n")
    bf.write("#\n")
    bf.write("cmake ")
    bf.write("-DGeant4_DIR=%s/lib/Geant4-9.6.2/ " % (geant4installdir))
    bf.write("-DCMAKE_MODULE_PATH=%s/lib/Geant4-9.6.2/Modules/ " \
        % (geant4installdir))
    bf.write("../simlucid\n")
    bf.write("make\n")
    bf.close()
