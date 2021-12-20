#!/bin/bash

day=`date +%Y%m%d`

#<<HH
curl https://tj.ke.com/ershoufang/pg1co21l1c1211045557472/?sug=%E6%9D%A5%E5%AE%89%E9%87%8C > ${day}_1.txt
curl https://tj.ke.com/ershoufang/pg2co21l1c1211045557472/?sug=%E6%9D%A5%E5%AE%89%E9%87%8C > ${day}_2.txt
./parse ${day}_1.txt ${day}_la.csv
./parse ${day}_2.txt ${day}_la.csv 1

curl https://tj.ke.com/ershoufang/co21l1c1211045565921/?sug=%E9%87%91%E6%B9%BE%E8%8A%B1%E5%9B%AD > ${day}_1.txt
./parse ${day}_1.txt ${day}_jw.csv

curl https://tj.ke.com/ershoufang/co21c1211045678395/?sug=%E9%87%91%E5%A0%82%E5%8D%97%E9%87%8C > ${day}_1.txt
./parse ${day}_1.txt ${day}_jt.csv

rm -rf ${day}_1.txt ${day}_2.txt
