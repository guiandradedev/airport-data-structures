#ifndef colors_H_INCLUDED
#define colors_H_INCLUDED
void vermelho () {
  printf("\033[1;31m");
}

void amarelho () {
  printf("\033[1;33m");
}

void verde () {
  printf("\033[1;32m");
}

void resetcor () {
  printf("\033[0m");
}

#endif