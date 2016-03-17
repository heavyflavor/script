#!/bin/bash 

mystr=16081031abc

echo "----Get str length---"
echo ${#mystr}
expr length $mystr

echo "----find the position.---"
expr index $mystr "a"

echo "----find the position.---"
echo ${mystr:2:3}