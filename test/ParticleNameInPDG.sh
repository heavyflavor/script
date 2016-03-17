#!/bin/bash 
. ~/bin/ColorText.sh

LOG_INFO "Create Particle name and PDG ID in STARSIM"

if [ -e ParticleNameAndPdgCode.txt ]
then 
    rm ParticleNameAndPdgCode.txt
fi 

root -l -b -q ParticleNameInPDG.C > ParticleNameAndPdgCode.txt 

LOG_INFO "Done! "
