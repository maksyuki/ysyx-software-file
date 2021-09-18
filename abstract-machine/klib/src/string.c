#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *p = s;
  while (*p) { p++; }

  return p - s;
}

char *strcpy(char *dst, const char *src) {
  char* d = dst;
  while ((*d++ = *src++)) {}

  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char* d = dst;
  while ((n--) > 0 && (*d++ = *src++)) {}
  while((n--) > 0) { *d++ = 0; }

  return dst;
}

char *strcat(char *dst, const char *src) {
  char* d = dst;

  while (*d++) {}
  d--;
  while ((*d++ = *src++)) {}

  return dst;
}

int strcmp(const char *s1, const char *s2) {
  // unsigned char type is very important
  unsigned char c1, c2;

  do {
    c1 = *s1++;
    c2 = *s2++;
  } while (c1 != 0 && c1 == c2);

  return c1 - c2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  unsigned char c1, c2;

  do {
    c1 = *s1++;
    c2 = *s2++;
  } while ((n--) > 0 && c1 != 0 && c1 == c2);

  if(!n) return 0;
  return c1 - c2;
}

void *memset(void *dst, int c, size_t n) {
  char *cdst = (char *) dst;

  for(int i = 0; i < n; i++){
    cdst[i] = c;
  }

  return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
  const char *s = src;
  char *d = dst;

  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0) {*--d = *--s;}
  } else {
    while(n-- > 0) {*d++ = *s++;}
  }

  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  return memmove(out, in, n);
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *v1 = s1;
  const unsigned char *v2 = s2;

  while((n--) > 0) {
    if(*v1 != *v2) { return *v1 - *v2; }
    v1++, v2++;
  }

  return 0;
}

#endif
