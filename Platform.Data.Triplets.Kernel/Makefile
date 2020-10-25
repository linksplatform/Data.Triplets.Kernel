CC=gcc
OPTS=-O3 --std=c99 -fPIC -W -Wall -Wextra -pedantic
DEFS=-DLINKS_DLL_EXPORT -D_XOPEN_SOURCE=700
Platform.Data.Triplets.Kernel	: Link.h Link.c
	${CC} ${OPTS} ${DEFS} -c -o Timestamp.o Timestamp.c
	${CC} ${OPTS} ${DEFS} -c -o Link.o Link.c
	${CC} ${OPTS} ${DEFS} -c -o PersistentMemoryManager.o PersistentMemoryManager.c
	${CC} -shared -o Platform.Data.Triplets.Kernel Link.o PersistentMemoryManager.o  Timestamp.o
	${CC} ${OPTS} -o test test.c Platform.Data.Triplets.Kernel