LIB=libmastik.a
LIBSRCS=vlist.c l3.c timestats.c l1.c l1i.c fr.c util.c
LIBOBJS=${LIBSRCS:.c=.o}

all: ${LIB} ${TARGETS}
include LocalMakefile

CFLAGS=-g -std=gnu99 -O2 -DNDEBUG ${EXTRA_CFLAGS} 





${LIB}:${LIBOBJS}
	ar crv ${LIB} ${LIBOBJS}
	ranlib ${LIB}

l3.o: l3.h vlist.h timestats.h low.h

vlist.o: vlist.h

timestats.o: timestats.h


#pp.o: vlist.h pp.h low.h

l1.o: l1.h low.h 

l1i.o: l1i.h low.h

btb.o: btb.h low.h

fr.o: fr.h low.h vlist.h

LocalMakefile:
	touch LocalMakefile

clean:
	rm -f ${LIB} ${LIBOBJS}
