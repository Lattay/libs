/*
 * Written in 2021 by Théo Cavignac
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * This is a very simple set of function to read and write integer matrix from
 * text file.
 * This is a header-only lib.
 * To use it include it everywhere you need.
 * In *one* of the inclusion site, definer TEXT_MAT_IMPL before inclusion.
 */

#ifndef TEXT_MAT_H
#define TEXT_MAT_H
#include <stdlib.h>

/* Open filename and write the matrix from data in it.
 * Return 0 if everything is OK, 1 if the file cannot be opened,
 * 2 if fprintf return an error, 3 if fclose return an error.
 */
int txtmat_dump(const char* filename, const int* data, size_t w, size_t h);

/* Open filename and read the matrix into a newly allocated array.
 * Set the data pointer pointer to point to the array, and w and h to have the
 * right width and heigh values.
 * Return 0 if everything is OK, 1 if the file cannot be opened,
 * 2 if there is a format error, 3 if fclose return an error.
 */
int txtmat_load(const char* filename, int** data, size_t* w, size_t* h);

#endif
#ifdef TEXT_MAT_IMPL
#include <stdio.h>

int txtmat_dump(const char* filename, const int* data, size_t w, size_t h){
  FILE* f = fopen(filename, "w");
  if(!f){
    return 1;
  }
  int res = fprintf(f, "DATA %lu %lu\n", w, h);
  if(res < 0){
    return 2;
  }
  for(size_t y = 0; y < h; ++y){
    for(size_t x = 0; x < w; ++x){
      res = fprintf(f, "%d ", data[x + w * y]);
      if(res <= 0){
        return 2;
      }
    }
    res = fprintf(f, "\n");
    if(res < 0){
      return 2;
    }
  }
  res = fclose(f);
  return res == 0 ? 0 : 3;
}

int txtmat_load(const char* filename, int** target, size_t* w, size_t* h){
  int res;
  FILE* f = fopen(filename, "r");
  if(!f){
    return 1;
  }
  res = fscanf(f, "DATA %lu %lu\n", w, h);
  if(res <= 0){
    fclose(f);
    return 2;
  }

  int* data = malloc(sizeof(int) * (*w) * (*h));

  for(size_t y = 0; y < (*h); ++y){
    for(size_t x = 0; x < (*w); ++x){
      res = fscanf(f, "%d ", &data[x + (*w) * y]);
      if(res <= 0){
        fclose(f);
        free(data);
        return 2;
      }
    }
    res = fscanf(f, "\n");
    if(res <= 0){
      fclose(f);
      free(data);
      return 2;
    }
  }

  res = fclose(f);
  *target = data;
  return res == 0 ? 0 : 3;
}
#endif
