#!/bin/sh

if (( $# != 2 )) ;then
    echo "Usage:" $0 "FileName Type"
    exit
fi

CPP_INCLUDE_OPEN="#include \"../"
CPP_INCLUDE_END="\""

echo $CPP_INCLUDE_OPEN$1$CPP_INCLUDE_END > Include/$2 
git add Include/$2
