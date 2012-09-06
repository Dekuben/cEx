#!/bin/bash
#script to compile database program and test
set -e
#-f is an argument of test function, here represented as square brackets. it returns true of succesful
if [ -f ex17.c ]&&[! -f ex17];
then
	echo ex17.c found
	echo ex17 not found
else
	echo ex17.c not found
	echo or ex17 found
fi
