
# Name of the program $(PROGRAM).c will be added as a source file
PROGRAM = core_main
PROGRAM_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
# Any extra source files to include in the build. Use the upper case .S
# extension for assembly files
EXTRA_SRCS := core_list_join.c core_matrix.c core_state.c  core_util.c core_portme.c

PROGRAM_CFLAGS := -fno-builtin-printf -fno-builtin-malloc -DITERATIONS=1000 -DPERFORMANCE_RUN=1

include ${PROGRAM_DIR}/../common/common.mk
