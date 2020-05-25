#!/usr/bin/env bash

rm -rf data
mkdir data
rsync -a workdata/* data/
gzip -r data/*

rm src/html.h
touch src/html.h

mkdir -p src/data

find data -type f -print0 | while IFS= read -r -d '' filename; do
   xxd -i $filename > src/data/${filename##*/}.h
   sed -i .bak 's/unsigned char/const char/g;
   s/\] \=/] PROGMEM =/g' src/data/${filename##*/}.h
   rm src/data/${filename##*/}.h.bak
   echo "#include \"data/${filename##*/}.h\"" >> src/html.h
done


