EXE=nerdengine
SOURCE=$(shell du -a src | cut -f2 | grep '\.c')
OBJECTS=$(SOURCE:.c=.o)

WFLAGS=-Wall -Wextra -Wshadow -Werror
STANDARD_FLAGS=-O3 -DNDEBUG ${WFLAGS}
DEBUG_FLAGS=-O0 -DDEBUG ${WFLAGS}
CFLAGS=$(STANDARD_FLAGS)

ifeq ($(MAKECMDGOALS),all)
	CFLAGS=$(STANDARD_FLAGS)
endif
ifeq ($(MAKECMDGOALS),debug)
	CFLAGS=$(DEBUG_FLAGS)
endif

MAKE_VERSION=$(shell cat .make_version)

all: check ${EXE}

debug: check ${EXE}

check:
ifneq ($(MAKE_VERSION),$(MAKECMDGOALS))
ifeq ($(strip $(MAKECMDGOALS)),)
ifneq ($(MAKE_VERSION),all)
	@make clean
	@echo all > .make_version
endif
else
	@make clean
	@echo $(MAKECMDGOALS) > .make_version
endif
endif

${EXE}: ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXE}

clean:
	rm ${OBJECTS} ${EXE}

.PHONY: 
	all debug clean
