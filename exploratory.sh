#!/bin/bash

n=$1

./tetrahedron $n | while read p; do
    echo "tetrahedron($n) $p"
done

./four33 $n | while read p; do
    echo "four33($n) $p"
done

./cup $n | while read p; do
    echo "cup($n) $p"
done

./thetagraph $n 4 | while read p; do
    echo "thetagraph($n) $p"
done