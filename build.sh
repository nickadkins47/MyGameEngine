#!/bin/bash

odin run Src \
    -out:MyGameEngine.exe \
    -keep-executable \
    -vet-cast \
    -vet-semicolon \
    -vet-unused \
    -vet-unused-imports \
    -vet-unused-variables \
    -vet-using-param \
    -vet-using-stmt