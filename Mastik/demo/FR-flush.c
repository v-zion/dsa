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
#include <fr.h>
#include <util.h>

int main(int ac, char **av) {
  fr_t fr = fr_prepare();

  void *ptr = map_offset("gpg-1.4.13", 0x91060);   //mpihelp_sub_n
  void *ptr2 = map_offset("gpg-1.4.13", 0x91080);   //mpihelp_sub_n
  fr_monitor(fr, ptr);
  fr_monitor(fr, ptr2);

  fr_flush(fr);

}

