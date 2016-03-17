#!/bin/bash

day=$1
RUN=$2

  echo $day $RUN
  rm -f daq/${RUN}.lis
  ls /star/institutions/lbl_prod/hft/Run14/daq/${day}/${RUN}/st*.daq --color=none > daq/${RUN}.lis

