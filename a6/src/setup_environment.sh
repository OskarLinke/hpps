#!/bin/bash
echo "Setting up environment at ${1}"
apptainer exec ~/modi_images/hpc-notebook-latest.sif ${1}/work.sh ${2} ${3} ${4} ${5}
