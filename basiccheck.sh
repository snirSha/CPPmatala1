#!/bin/bash

dirPath=$1
program=$2
cd $dirPath

clang++-5.0 $program ${@:3}

if [ $? -gt 0 ]; then
   ansCompilation=1
else
    ansCompilation=0

    valgrind --tool=memcheck --leak-check=full --error-exitcode=3 -q ./a.out
    if [ $? -gt 0 ]; then
       ansMemoryLeak=1
    else
       ansMemoryLeak=0
    fi
 
    valgrind --tool=helgrind -q ./a.out
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







