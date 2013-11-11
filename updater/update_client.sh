#!/bin/bash


# This script updates the client side of CISS465NetworkGame to the latest
# available version


# Update the clientside game then their local timestamp to last update
wget -rd --no-parent "https://raw.github.com/sgluebbert/CISS465NetworkGame/master/app/*" -p "."

rm -rf Art/Source Makefile Main.cpp

mv sTS lTS
