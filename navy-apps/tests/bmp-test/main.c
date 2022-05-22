#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>

struct BitmapHeader {
  uint16_t type;
  uint32_t filesize;
  uint32_t resv_1;
  uint32_t offset;
  uint32_t ih_size;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bitcount; // 1, 4, 8, or 24
  uint32_t compression;
  uint32_t sizeimg;
  uint32_t xres, yres;
  uint32_t clrused, clrimportant;
} __attribute__((packed));

void* BMP_Load(const char *filename, int *width, int *height) {
  // printf("filename: %s\n", filename);
  FILE *fp = fopen(filename, "r");
  if (!fp) return NULL;

  struct BitmapHeader hdr;
  assert(sizeof(hdr) == 54);
  assert(1 == fread(&hdr, sizeof(struct BitmapHeader), 1, fp));

  if (hdr.bitcount != 24) return NULL;
  if (hdr.compression != 0) return NULL;
  int w = hdr.width;
  int h = hdr.height;
  uint32_t *pixels = malloc(w * h * sizeof(uint32_t));
  printf("w: %d, h: %d\n", w, h);
  printf("pxiels's addr: %p\n", pixels);
  // printf("hdr.offset: %d\n", hdr.offset);
  int line_off = (w * 3 + 3) & ~0x3;
  for (int i = 0; i < h; i ++) {
    fseek(fp, hdr.offset + (h - 1 - i) * line_off, SEEK_SET);
    printf("i: %d\n", i);
    int nread = fread(&pixels[w * i], 3, w, fp);
    for (int j = w - 1; j >= 0; j --) {
      uint8_t b = *(((uint8_t*)&pixels[w * i]) + 3 * j);
      uint8_t g = *(((uint8_t*)&pixels[w * i]) + 3 * j + 1);
      uint8_t r = *(((uint8_t*)&pixels[w * i]) + 3 * j + 2);
      pixels[w * i + j] = (r << 16) | (g << 8) | b;
    }
  }

  fclose(fp);
  if (width) *width = w;
  if (height) *height = h;
  return pixels;
}

int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  // FILE *fp = fopen("/share/pictures/projectn.bmp", "r");
  // assert(fp);
  // fclose(fp);
  // assert(bmp);
  // NDL_OpenCanvas(&w, &h);
  // NDL_DrawRect(bmp, 0, 0, w, h);
  free(bmp);
  NDL_Quit();
  // printf("w: %d, h: %d\n", w, h);
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
