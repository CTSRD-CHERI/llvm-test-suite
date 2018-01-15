// PR704

#include <stdio.h>

void hello(void) {
  printf("Hello, world!\n");
}

void goodbye(void) {
  printf("Goodbye, world!\n");
}

int main(int argc, char **argv) {
  void (*f)(void) = (argc > 1) ? &hello : &goodbye;
  f();
  return 0;
}

