#!/bin/bash
for var in {0..500}
do
    if ((var%7==0))
    then
        echo $var
    else
        continue
    fi
done