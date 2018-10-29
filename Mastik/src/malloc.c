/*
 * Copyright 2016 CSIRO
 *
 * This file is part of Mastik.
 *
 * Mastik is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mastik is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mastik.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
/* Shamelessly copied from https://www.math.uni-bielefeld.de/~rehmann/Ckurs-f/b04/alloc.h */


/* titel: malloc()/free()-Paar nach K&R 2, p.185ff */

char malloc_buffer[1024*1024*20];

typedef long Align;

union header {			/* Kopf eines Allokationsblocks */
    struct {
	union header	*ptr;  	/* Zeiger auf zirkulaeren Nachfolger */
	size_t 	size;	/* Groesse des Blocks	*/
    } s;
    Align x;			/* Erzwingt Block-Alignierung	*/
};

typedef union header Header;



static Header base;		/* Anfangs-Header	*/
static Header *freep = NULL;	/* Aktueller Einstiegspunkt in Free-Liste */

static Header *malloc_init();	/* Aufruf ans Betriebssystem */

void prmalloc(char c, int num, void *address) {
  static char hexdigit[]="0123456789ABCDEF";
  char buf[100];
  char *p = buf;
  uintptr_t ptr = (uintptr_t) address;
  *p++ = c;
  *p++ = ' ';
  *p++ = '0';
  *p++ = 'x';
  *p++ = hexdigit[(num >> 28) & 0x0f];
  *p++ = hexdigit[(num >> 24) & 0x0f];
  *p++ = hexdigit[(num >> 20) & 0x0f];
  *p++ = hexdigit[(num >> 16) & 0x0f];
  *p++ = hexdigit[(num >> 12) & 0x0f];
  *p++ = hexdigit[(num >>  8) & 0x0f];
  *p++ = hexdigit[(num >>  4) & 0x0f];
  *p++ = hexdigit[(num >>  0) & 0x0f];
  *p++ = ' ';
  *p++ = '0';
  *p++ = 'x';
  *p++ = hexdigit[(ptr >> 28) & 0x0f];
  *p++ = hexdigit[(ptr >> 24) & 0x0f];
  *p++ = hexdigit[(ptr >> 20) & 0x0f];
  *p++ = hexdigit[(ptr >> 16) & 0x0f];
  *p++ = hexdigit[(ptr >> 12) & 0x0f];
  *p++ = hexdigit[(ptr >>  8) & 0x0f];
  *p++ = hexdigit[(ptr >>  4) & 0x0f];
  *p++ = hexdigit[(ptr >>  0) & 0x0f];
  *p++ = '\n';
  write(2, buf, p-buf);
}

void* malloc(size_t nbytes) {
    Header *p, *prevp;		
    unsigned nunits;
    
     /* Kleinstes Vielfaches von sizeof(Header), das die
	geforderte Byte-Zahl enthalten kann, plus 1 fuer den Header selbst: */

    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {	/* Erster Aufruf, Initialisierung */
	base.s.ptr = freep = prevp = &base;
	base.s.size = 0;		/* base wird Block der Laenge 0 */
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
	
	/* p durchlaeuft die Free-Liste, gefolgt von prevp, keine
		Abbruchbedingung!!	*/

	if (p->s.size >= nunits) {	/* Ist p gross genug? 		*/
	    if (p->s.size == nunits) 	/* Falls exakt, wird er... 	*/
		prevp->s.ptr = p->s.ptr;/* ... der Liste entnommen 	*/
	    else {			/* andernfalls...	   	*/
		p->s.size -= nunits;	/* wird p verkleinert		*/
		p += p->s.size;		/* und der letzte Teil ... 	*/
		p->s.size = nunits;	/* ... des Blocks...		*/
	    }
	    freep = prevp;
	    //prmalloc('m', nbytes, p+1);
	    return (void*) (p+1);	/* ... zurueckgegeben, allerdings 
					   unter der Adresse von p+1,
					   da p auf den Header zeigt.  	*/
	}
	if ( p == freep){		/* Falls die Liste keinen Block
				           ausreichender Groesse enthaelt,
					   wird morecore() aufgrufen	*/
	    malloc_init();
	}
    }
}

#define NALLOC 	1024	/* Mindestgroesse fuer morecore()-Aufruf	*/

/* Eine static-Funktion ist ausserhalb ihres Files nicht sichtbar	*/

static Header *malloc_init() {
    static init = 0;
    if (init) {
	    write(2, "out of memory\n", 14);
	    exit(143);
    }
    init = 1;
    void free(void*);
    Header *up = (Header*) malloc_buffer;
    up->s.size = sizeof(malloc_buffer)/sizeof(Header);			/* Groesse wird eingetragen	*/
    free((void*)(up+1));		/* Einbau in Free-Liste		*/
    return freep;
}

void free(void *ap) {			/* Rueckgabe an Free-Liste	*/
    //prmalloc('f', 0, ap);
    if (ap == NULL)
      return;
    Header *bp, *p;
    
    bp = (Header*) ap - 1;		/* Hier ist der Header des Blocks */

	/* Die Liste wird durchmustert, der Block soll der
	   Adressgroesse nach richtig eingefuegt werden,
	   um Defragmentierung zu ermoeglichen.				*/

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
	if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
	    break;	/* bp liegt vor Block mit kleinster oder hinter 
			   Block mit groesster Adresse */

    if (bp + bp->s.size == p->s.ptr) {	
				/* Vereinigung mit oberem Nachbarn 	*/
	bp->s.size += p->s.ptr->s.size;
	bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
	bp->s.ptr = p->s.ptr;
    if ( p + p->s.size == bp ) {
				/* Vereinigung mit unterem Nachbarn 	*/
	p->s.size += bp->s.size;
	p->s.ptr = bp->s.ptr;
    } else
	p->s.ptr = bp;
    freep = p;
}

void *calloc(size_t nmemb, size_t size) {
  void *rv = malloc(nmemb * size);
  memset(rv, 0, nmemb * size);
  return rv;
}

void *realloc(void *ptr, size_t size) {
  Header *up = (Header *)ptr -1;
  size_t copysize = sizeof(Header) * up->s.size;
  if (size < copysize)
    copysize = size;
  void *new = malloc(size);
  if (new == NULL)
    return NULL;
  memcpy(new, ptr, copysize);
  free(ptr);
  return new;
}


