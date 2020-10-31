EXE=nerdengine
SOURCE=src/main.o src/board/board.o

WFLAGS=-Wall -Wextra -Wshadow -Werror
STANDARD_FLAGS=-O3 -DNDEBUG ${WFLAGS}
DEBUG_FLAGS=-O0 -DDEBUG ${WFLAGS}
CFLAGS=${STANDARD_FLAGS}

${EXE}: ${SOURCE}
	${CC} ${CFLAGS} ${SOURCE} -o ${EXE}

all: ${EXE}

clean:
	rm ${SOURCE} ${EXE}

.PHONY: 
	all clean
