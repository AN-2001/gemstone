#!/bin/bash
PROJECTS=/home/amber/projects
GEMSTONE=$PROJECTS/gemstone
GEMSTONE_C=$GEMSTONE/gemstonec
GEMSTONE_CPP=$GEMSTONE/gemstonecpp
echo "please enter the project name:"
read project
echo "please enter the project description:"
read desc
echo "C or C++?"
read CorCpp

mkdir $PROJECTS/$project
cd $PROJECTS/$project

if [ "$CorCpp" == "c" ] || [ "$CorCpp" == "C" ] 
then
	cp $GEMSTONE_C/* -r .
	mv gemstone.c $project.c
elif [ "$CorCpp" == "C++" ] || [ "$CorCpp" == "c++" ]
then
	cp $GEMSTONE_CPP/* -r .
	mv gemstone.cpp $project.cpp
else
	echo "bad input"
	exit 1
fi

cp $GEMSTONE/palettes -r .
cp $GEMSTONE/config.h .
cp $GEMSTONE/default.csv .

make clean
sed -i "s/#define PROJ \"gemstone\"/#define PROJ \"$project\"/" ./config.h
sed -i "s/#define DESC \"the gem creator!\"/#define DESC \"$desc\"/" ./config.h
sed -i "s/proj := gemstone/proj := $project/" ./makefile
echo "done!"


