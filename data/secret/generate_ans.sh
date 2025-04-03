#!/bin/bash

for i in {1..20}; do
    if [ "$i" -eq 1 ]; then
        echo "firstCase" > "$i.ans"
    elif [ "$i" -eq 20 ]; then
        echo "lastCase" > "$i.ans"
    else
        echo "otherCase" > "$i.ans"
    fi
done
