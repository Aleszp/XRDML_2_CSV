#!/bin/bash
for i in *.xrdml; do
	./xrdml2csv -n "$i" "${i%.*}.csv"
done
