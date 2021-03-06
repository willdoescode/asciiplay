#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int win_width;
int win_height;
char* map;

struct Coord {
  int row;
  int col;
};

enum MOVES {
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

enum MOVES currentMove = UP;

struct Coord playerPosition;

const char* move_to = "\033[%d;%dH";
const char* hide_cursor = "\e[?25l";
const char* show_cursor = "\e[?25h";

void print_map();
void move_cursor(int, int);
void init_map(int, int);
void update_map_at_coord(int, int, char);
char get_map_at_coord(int, int);
void move_player(enum MOVES);
void main_loop();
int mod(int, int);

int main() {
  printf("%s", hide_cursor);
  printf("Welcome to asciiplay! Press <esc> to leave at any time. Use wasd or vim keybinds to move around.");
  fflush(stdout);
  sleep(3);

  struct winsize w;

  ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
  win_width = w.ws_col;
  win_height = w.ws_row;

  playerPosition.row = win_height / 2;
  playerPosition.col = win_width / 2;

  init_map(win_width, win_height);
  print_map();

  // getchar without flushing terminal buffer
  system("/bin/stty raw");
  main_loop();

  system("/bin/stty cooked");
  free(map);

  system("clear");
  printf("%s", show_cursor);
  return 0;
}

int mod(int k, int n) {
  return ((k %= n) < 0) ? k + n : k;
}

void move_player(enum MOVES move) {
  update_map_at_coord(playerPosition.row, playerPosition.col, '.');

  switch (move) {
    case UP:
      playerPosition.row = mod((playerPosition.row - 1), win_height);
      break;
    case DOWN:
      playerPosition.row = (playerPosition.row + 1) % win_height;
      break;
    case RIGHT:
      playerPosition.col = (playerPosition.col + 1) % win_width;
      break;
    case LEFT:
      playerPosition.col = mod((playerPosition.col - 1), win_width);
      break;
  }
  update_map_at_coord(playerPosition.row, playerPosition.col, '#');
}

void main_loop() {
  int c = 'x';

  while (c != '\e') {
    c = getchar_unlocked();

    switch (c) {
      case 'k':
      case 'w':
        currentMove = UP;
        break;
      case 'j':
      case 's':
        currentMove = DOWN;
        break;
      case 'l':
      case 'd':
        currentMove = RIGHT;
        break;
      case 'h':
      case 'a':
        currentMove = LEFT;
        break;
    }
    move_player(currentMove);

    move_cursor(0, 0);
    print_map();
  }
}

void update_map_at_coord(int row, int col, char val) {
  map[col + win_width * row] = val;
}

char get_map_at_coord(int row, int col) {
  return map[col + win_width * row];
}

void init_map(int width, int height) {
  map = (char *) malloc(width * height * sizeof(char));
  move_cursor(0, 0);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      update_map_at_coord(row, col, '.');
    }
  }

  update_map_at_coord(playerPosition.row, playerPosition.col, '#');
}

void move_cursor(int x, int y) {
  printf(move_to, y, x);
}

void print_map() {
  for (int row = 0; row < win_height; row++) {
    for (int col = 0; col < win_width; col++) {
      putchar_unlocked(get_map_at_coord(row, col));
    }
  }
}
