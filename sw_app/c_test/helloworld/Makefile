
# Name of the program $(PROGRAM).c will be added as a source file
PROGRAM = helloworld
PROGRAM_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
# Any extra source files to include in the build. Use the upper case .S
# extension for assembly files
EXTRA_SRCS :=

include ${PROGRAM_DIR}/../common/common.mk
