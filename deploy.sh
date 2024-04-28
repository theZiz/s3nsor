#!/bin/sh

BUILD=build-ev3
if [ $# -ne 0 ]; then
	BUILD=$1
fi

scp $BUILD/s3nsor  robot@ev3dev:/home/robot
