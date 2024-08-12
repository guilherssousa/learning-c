#include <stdio.h>

struct fish {
  char name[1024];
  float length;
  int mass;
};

void debug_fish(struct fish *fish, char *return_buf) {
  sprintf(return_buf, "Hello, my name is %s, I'm %f and I weight %dkg!",
          fish->name, fish->length, fish->mass);
}

int main(void) {
  FILE *fp;
  int c;
  char s[1024];
  int linecount = 0;

  char *source_file = "./demo/hello.txt";
  char *multiline_source_file = "./demo/quote.txt";
  char *scan_source_file = "./demo/whales.txt";
  char *output_file = "./demo/new_file.txt";

  fp = fopen(source_file, "r"); // <- Abrir o arquivo para leitura

  if (fp == NULL) {
    printf("%s does not exist", source_file);
  }

  while ((c = fgetc(fp)) != EOF) {
    printf("%c", c);
  }

  fclose(fp);

  fp = fopen(multiline_source_file, "r");

  while (fgets(s, sizeof s, fp) != NULL) {
    printf("%d: %s", ++linecount, s);
  }

  fclose(fp);

  fp = fopen(scan_source_file, "r");

  struct fish fish = {};
  char return_buf[1024];

  while (fscanf(fp, "%s %f %d", fish.name, &fish.length, &fish.mass) != EOF) {
    debug_fish(&fish, return_buf);
    printf("%s\n", return_buf);
  }

  fclose(fp);
  int x = 32;

  // writing to a file
  fp = fopen(output_file, "w");
  fputc('B', fp);
  fputc('\n', fp); // newline
  fprintf(fp, "x = %d\n", x);
  fputs("Hello, world!\n", fp);

  fclose(fp);

  unsigned char bytes[6] = {5, 37, 0, 88, 255, 12};

  fp = fopen("output.bin", "wb"); // wb is write binary

  fwrite(bytes, sizeof(char), 6, fp);

  fclose(fp);
}
