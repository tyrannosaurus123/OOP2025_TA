#!/bin/bash

cd /home/adduser/OOP_TA/testcases_v2 || { echo "無法進入資料夾"; exit 1; }

EXEC=../hw1

if [ ! -f "$EXEC" ]; then
    echo "找不到執行檔 $EXEC, 請先編譯 hw1.cpp"
    exit 1
fi

for infile in *.in; do
    base="${infile%.in}"
    outfile="${base}.ans"
    echo "$infile → $outfile"
    "$EXEC" < "$infile" > "$outfile"
done

echo "全部輸出完成"
