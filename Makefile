EXE=nerdengine
SOURCE=src/main.o

STANDARD_FLAGS=-O3 -Wall -Wextra -Wshadow -Werror
CFLAGS=${STANDARD_FLAGS}

${EXE}: ${SOURCE}
	${CC} ${CFLAGS} $? -o ${EXE}

all: ${EXE}

clean:
	rm ${SOURCE} ${EXE}

.PHONY: 
	all clean
