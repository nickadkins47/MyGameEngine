
#default action
default: build_run

#build target directory & file name
target_dir  = Build
target_base = MyGameEngine.exe

#silence makefile commands with "@", else leave blank
sil = @

########################################

.PHONY: default run build_run build test clean

SHELL := /bin/bash

target = $(target_dir)/$(target_base)

source_files = $(shell find Src -name '*.odin')

run_cmd = cd $(target_dir) && ./$(target_base)

flags = \
	-debug \
	-out:$@ \
	-keep-executable
#-vet-cast \
	-vet-semicolon \
	-vet-unused \
	-vet-unused-imports \
	-vet-unused-variables \
	-vet-using-param \
	-vet-using-stmt

run:
	$(sil) $(run_cmd)
	
build_run: $(target)
	$(sil) $(run_cmd)

build: $(target)
$(target): $(source_files) | $(target_dir)
	$(sil) odin build Src $(flags)
		
test:
	$(sil) odin run .

clean:
	$(sil) rm -f $(target)

$(target_dir):
	$(sil) mkdir -p $@