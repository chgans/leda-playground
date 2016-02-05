#!/bin/bash

set -e

qucs=/home/krys/Projects/qucs/qucs/qucs/qucs/qucs

out=$PWD/qucs-symbols
rm -Rf $out/*
mkdir -p $out
cd $out
$qucs -doc
for f in */*_data.csv;
do
    echo Converting \"$f\"
    python3 ../csv2xml.py "$f" "${f//_data.csv/_draw.csv}" \
	    | xmllint --format - > "${f//_data.csv/_sym.xml}" ;
done
for f in */*_prop.csv;
do
    echo Converting \"$f\"
    python3 ../csv2xml_prop.py "$f" "${f//_data.csv/_prop.csv}" \
	    | xmllint --format - > "${f//_data.csv/_prop.xml}" ;
done

