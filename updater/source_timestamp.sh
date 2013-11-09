#!/bin/bash


# This script only updates the timestamp used to decide if a client should
# update.
# This script should thus be called only when client resources are updated.
# Currently, that responsibility lies with the makefile on the main folder.


# Just a regular timestamp generator
timestamp()
{
  date +"%c"
}


timestamp > TS
