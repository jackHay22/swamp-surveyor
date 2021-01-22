#!/bin/bash

#copy resources to application support
BASE_DIR=/Library/'Application Support/io.jackhay/swamp_surveyor/'
#make target directory
mkdir -p "$BASE_DIR"
#copy resources
cp -a resources/. "$BASE_DIR"
