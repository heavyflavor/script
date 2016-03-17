#!/bin/bash
. ~/bin/FunPool.sh

date

LOG_INFO "======#1 Sync Paer======"
LOG_INFO "Sync local Paper to Rcf..."
rsync -avrz ~/Paper zhoulong@rftpexp.rhic.bnl.gov:/star/u/zhoulong/
rsync -avrz zhoulong@rftpexp.rhic.bnl.gov:/star/u/zhoulong/Paper ~/

LOG_INFO "======#2 Sync scripts======"
rsync -avrz ~/script zhoulong@rftpexp.rhic.bnl.gov:/star/u/zhoulong/
rsync -avrz zhoulong@rftpexp.rhic.bnl.gov:/star/u/zhoulong/script ~/

LOG_INFO "Sync Done !"
