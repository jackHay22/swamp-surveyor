#!/bin/bash

#copy resources to application support
BASE_DIR=/Library/'Application Support/io.jackhay/swamp_surveyor/'
APP_DIR=SwampSurveyor.app

#make target directories
mkdir -p "$BASE_DIR"
mkdir -p $APP_DIR

#copy resources
cp -a resources/. "$BASE_DIR"
