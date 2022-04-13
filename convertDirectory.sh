#!/bin/bash

#
# Use with XRDML_2_CSV program. 
# Run this script in directory with xrdml2csv binary and .xrdml files you would like to convert, for all those files copies converted to .csv will be created.
# Author: mgr inż. Aleksander Szpakiewicz-Szatan
# (c) 2021-2022
# Licensed under GPL-3.0 License
# 

for i in *.xrdml; do
	./xrdml2csv -n "$i" "${i%.*}.csv"
done
