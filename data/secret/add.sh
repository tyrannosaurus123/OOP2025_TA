#!/bin/bash

cd /home/adduser/OOP_TA/testcases_v2 || { echo "無法進入資料夾"; exit 1; }

for i in {2..19}; do
    filename="${i}.ans"
    if [ -f "$filename" ]; then
        echo "修改 $filename"
        sed -i '1i otherCase' "$filename"
    else
        echo "找不到 $filename，略過"
    fi
done

echo "已在 2.ans ~ 19.ans 開頭加入 'otherCase'"
