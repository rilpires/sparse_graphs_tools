#!/bin/bash

n=$1

for topology in cup four33 tetrahedron; do
    ./$topology $n | while read p; do
        echo "$topology($n) $p"
    done
done;

./thetagraph $n 4 | while read p; do
    echo "thetagraph($n) $p"
done