#!/bin/bash

date 

echo -e "\033[32mThis Macro used to fix firefox no responds \033[0m"

rm -f /star/u/zhoulong/.mozilla/firefox/u8uy8r3v.default/lock
rm -f /star/u/zhoulong/.mozilla/firefox/u8uy8r3v.default/.parentlock

echo -e "\033[32mFix Done !\033[0m"
