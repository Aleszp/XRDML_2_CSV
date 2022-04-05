#!/bin/bash
for i in *.xrdml; do
	./xrdml2csv "$i" "${i%.*}.csv"
done
