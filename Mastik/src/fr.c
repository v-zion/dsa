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
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "low.h"
#include "vlist.h"
#include "fr.h"

struct fr { 
  vlist_t vl;
};



fr_t fr_prepare() {
  fr_t rv = malloc(sizeof(struct fr));
  rv->vl = vl_new();
  return rv;
}

int fr_monitor(fr_t fr, void *adrs) {
  assert(fr != NULL);
  assert(adrs != NULL);
  if (vl_find(fr->vl, adrs) >= 0)
    return 0;
  vl_push(fr->vl, adrs);
  return 1;
}


int fr_unmonitor(fr_t fr, void *adrs) {
  assert(fr != NULL);
  assert(adrs != NULL);
  int i = vl_find(fr->vl, adrs);
  if (i < 0)
    return 0;
  vl_del(fr->vl, i);
  return 1;
}


int fr_getmonitoredset(fr_t fr, void **adrss, int nlines) {
  assert(fr != NULL);

  if (adrss != NULL) {
    int l = vl_len(fr->vl);
    if (l > nlines)
      l = nlines;
    for (int i = 0; i < l; i++)
      adrss[i] = vl_get(fr->vl, i);
  }
  return vl_len(fr->vl);
}

void fr_randomise(fr_t fr) {
  assert(fr != NULL);
  assert(0);
}

void fr_probe(fr_t fr, uint16_t *results) {
  assert(fr != NULL);
  assert(results != NULL);
  int l = vl_len(fr->vl);
  for (int i = 0; i < l; i++)  {
    void *adrs = vl_get(fr->vl, i);
    int res = memaccesstime(adrs);
    results[i] = res > UINT16_MAX ? UINT16_MAX : res;
    clflush(adrs);
  }
}

inline int is_active(uint16_t *results, int len, int threshold) {
  for (int i = 0; i < len; i++)
    if (results[i] < threshold)
      return 1;
  return 0;
}

int fr_repeatedprobe(fr_t fr, int max_records, uint16_t *results, int slot, int threshold, int max_idle) {
  assert(fr != NULL);
  assert(results != NULL);

  if (max_records == 0)
    return 0;
  if (max_idle == 0)
    max_idle = max_records;

  int len = vl_len(fr->vl);

  // Wait to hit threshold
  uint64_t prev_time = rdtscp64();
  fr_probe(fr, results);
  do {
    if (slot > 0) {
      do {
	prev_time += slot;
      } while (slotwait(prev_time));
    }
    fr_probe(fr, results);
  } while (!is_active(results, len, threshold));

  int count = 1;
  int idle_count = 0;
  int missed = 0;

  while (idle_count < max_idle && count < max_records) {
    idle_count++;
    results += len;
    count++;
    if (missed) {
      for (int i = 0; i < len; i++)
	results[i] = 0;
    } else {
      fr_probe(fr, results);
      if (is_active(results, len, threshold))
	idle_count = 0;
    }
    prev_time += slot;
    missed = slotwait(prev_time);
  }
  return count;
}

  

void fr_flush(fr_t fr) {
  void *p1,*p2, *p3;
  vlist_t vl = fr->vl;
  int len = vl_len(vl);

  switch (len) {
    case 0: return;
    case 1:
	    p1 = vl_get(fr->vl, 0);
	    for (;;) {
	      clflush(p1);
	    }
    case 2:
	    p1 = vl_get(fr->vl, 0);
	    p2 = vl_get(fr->vl, 1);
	    for (;;) {
	      clflush(p1);
	      clflush(p2);
	    }
    case 3:
	    p1 = vl_get(fr->vl, 0);
	    p2 = vl_get(fr->vl, 1);
	    p3 = vl_get(fr->vl, 1);
	    for (;;) {
	      clflush(p1);
	      clflush(p2);
	      clflush(p3);
	    }
    default:
	    vl = fr->vl;
	    for (;;) {
	      for (int i = 0; i < len; i++)
		clflush(vl_get(vl, i));
	    }
    }
}

