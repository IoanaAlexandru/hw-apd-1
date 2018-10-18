#include "homework.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int num_threads;
int resize_factor;

void alloc_image(image *img) {
  img->image = (unsigned char **) malloc(
      (size_t) img->height * sizeof(unsigned char **));
  int real_width = img->type == 5 ? img->width : img->width * 3;

  for (int i = 0; i < img->height; i++)
    img->image[i] =
        (unsigned char *) malloc((size_t) real_width * sizeof(unsigned char *));
}

unsigned char get_pixel(const image *in,
                        int i1,
                        int j1,
                        int gaussian_kernel[3][3],
                        int count,
                        int num_colors) {
  int sum = 0;
  for (int i2 = i1 * resize_factor; i2 < (i1 + 1) * resize_factor; i2++) {
    for (int j2 = j1 * resize_factor; j2 < (j1 + num_colors) * resize_factor;
         j2 += num_colors) {
      if (resize_factor == 3)
        sum += in->image[i2][j2] * gaussian_kernel[i2 % 3][j2 / num_colors % 3];
      else
        sum += in->image[i2][j2];
    }
  }
  return (unsigned char) (sum / count);
}

void resize_image(image *in, image *out) {
  int i1, j1;
  int gaussian_kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
  int count = resize_factor == 3 ? 16 : resize_factor * resize_factor;
  int num_colors = in->type == 5 ? 1 : 3;

  for (i1 = 0; i1 < out->height; i1++) {
    for (j1 = 0; j1 < out->width * num_colors; j1++) {
      out->image[i1][j1] =
          get_pixel(in, i1, j1, gaussian_kernel, count, num_colors);
    }
  }
}

void readInput(const char *fileName, image *img) {
  FILE *in = fopen(fileName, "rb");
  if (in == NULL) {
    fprintf(stderr, "Could not open %s!\n", fileName);
    return;
  }

  char buf[10];
  fscanf(in, "%2s", buf);

  if (strcmp(buf, "P5") == 0) {
    img->type = 5;
  } else if (strcmp(buf, "P6") == 0) {
    img->type = 6;
  } else {
    fprintf(stderr, "Invalid file type!\n");
    return;
  }

  fscanf(in, "%10s", buf);
  img->width = (int) strtol(buf, NULL, 10);
  fscanf(in, "%10s", buf);
  img->height = (int) strtol(buf, NULL, 10);
  fscanf(in, "%10s", buf);
  img->maxval = (unsigned char) strtol(buf, NULL, 10);

  fseek(in, 1, SEEK_CUR);  // skip whitespace

  alloc_image(img);
  int real_width = img->type == 5 ? img->width : img->width * 3;

  for (int i = 0; i < img->height; i++)
    fread(img->image[i], sizeof(char), (size_t) real_width, in);

  fclose(in);
}

void writeData(const char *fileName, image *img) {
  FILE *out = fopen(fileName, "wb");
  if (out == NULL) {
    fprintf(stderr, "Could not open %s!\n", fileName);
    return;
  }

  if (img->type == 5) {
    fprintf(out, "P5\n");
  } else if (img->type == 6) {
    fprintf(out, "P6\n");
  } else {
    fprintf(stderr, "Invalid image type %d!\n", img->type);
    return;
  }

  fprintf(out, "%d ", img->width);
  fprintf(out, "%d\n", img->height);
  fprintf(out, "%d\n", img->maxval);

  int real_width = img->type == 5 ? img->width : img->width * 3;

  for (int i = 0; i < img->height; i++) {
    fwrite(img->image[i], sizeof(char), (size_t) real_width, out);
  }

  fclose(out);
}

void resize(image *in, image *out) {
  out->type = in->type;
  out->height = in->height / resize_factor;
  out->width = in->width / resize_factor;
  out->maxval = in->maxval;
  alloc_image(out);

  resize_image(in, out);
}