# !/bin/sh

# Simple script to automate creating directory structure for C projects.

mkdir $1
mkdir $1/src

# Set name of the executable to name of the project
sed -e "s/#EXECUTABLE#/$1/g" makefile > $1/makefile
