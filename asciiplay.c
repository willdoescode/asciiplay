#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int win_width;
int win_height;
char* map;

const char* move_to = "\033[%d;%dH";

void print_map();
void move_cursor(int x, int y);
void init_map(int width, int height);
void update_map_at_coord(int x, int y, char val);
char get_map_at_coord(int x, int y);

int main() {
  system("clear");
  printf("Welcome to asciiplay, press <esc> to exit the game at any time\n");
  sleep(3);

  struct winsize w;

  ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
  win_width = w.ws_col;
  win_height = w.ws_row;

  init_map(win_width, win_height);
  print_map();
  move_cursor(0, 0);

  // getchar without flushing terminal buffer
  system("/bin/stty raw");

  int c = 0;
  while (c != '\e') {

    c = getchar();
    move_cursor(0, 0);
    print_map();

  }

  system("/bin/stty cooked");
  free(map);

  return 0;
}

void update_map_at_coord(int x, int y, char val) {
  map[x + y - win_height] = val;
}

char get_map_at_coord(int x, int y) {
  return map[x + y - win_height];
}

void init_map(int width, int height) {
  map = (char *) malloc(width * height * sizeof(char));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      update_map_at_coord(i, j, '*');
    }
  }
}

void move_cursor(int x, int y) {
  printf(move_to, y, x);
}

void print_map() {
  for (int width = 0; width < win_width; width++) {
    for (int height = 0; height < win_height; height++) {
      putchar(get_map_at_coord(width, height));
    }
  }
}
