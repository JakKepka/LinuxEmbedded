#!/bin/bash
BR_NAME=buildroot-2023.11.1
BR_FILE=${BR_NAME}.tar.xz
BR_DL=../${BR_FILE}
set -e
if [ ! -f ${BR_DL} ] || ! ( xz -q -t ${BR_DL}); then
  (  
     cd ..
     rm -f ${BR_FILE}
     wget https://buildroot.org/downloads/${BR_FILE}
  )
fi
tar -xJf ${BR_DL}
cp BR_config ${BR_NAME}/.config
cd ${BR_NAME}
make


