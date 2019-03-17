#!/bin/bash

#Snir Shaharabani 204040687
#Amir Hoshen 305151292

dirPath=$1
program=$2
cd $dirPath
make &> /dev/null



if [ $? -gt 0 ]; then
   ansCompilation=1
else
    ansCompilation=0

    valgrind --tool=memcheck ${@:3} --leak-check=full --error-exitcode=1 -q ./$program &> /dev/null
    if [ $? -gt 0 ]; then
       ansMemoryLeak=1
    else
       ansMemoryLeak=0
    fi
 
    valgrind --tool=helgrind --error-exitcode=1 -q ./$program &> /dev/null
    if [ $? -gt 0 ]; then 
       ansTreadRace=1
    else
       ansTreadRace=0
    fi
fi

answer=$ansCompilation$ansMemoryLeak$ansTreadRace

if [ $answer == '000' ]; then
    echo "Compilation-PASS   Memory leaks-PASS   Thread race-PASS"
    exit 0
elif [ $answer == '001' ]; then
      echo "Compilation-PASS   Memory leaks-PASS   Thread race-FAIL   ERROR number-001"
      exit 1
elif [ $answer == '010' ]; then
      echo "Compilation-PASS   Memory leaks-FAIL   Thread race-PASS   ERROR number-010"
      exit 2
elif [ $answer == '011' ];then 
      echo "Compilation-PASS   Memory leaks-FAIL   Thread race-FAIL  ERROR number-011"
      exit 3
else 
      echo "Compilation-FAIL   Memory leaks-FAIL   Thread race-FAIL  ERROR number-111"
      exit 7
fi







