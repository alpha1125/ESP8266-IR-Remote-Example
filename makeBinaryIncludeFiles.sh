#!/bin/env bash
cd data
rm -rf * .*
rsync -a ../workdata/* .
gzip -r *

cd ..

find data -type f -print0 | while IFS= read -r -d '' filename; do
  echo this is hello world - $filename
done