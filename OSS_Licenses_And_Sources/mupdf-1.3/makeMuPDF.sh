#!/bin/bash
# Script zum Bauen von MuPDF für SH4
make nuke
make
make clean
make OS=SH4 build=release

