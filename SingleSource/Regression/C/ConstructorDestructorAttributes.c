#include <stdio.h>

void ctor(void) __attribute__((constructor));

void ctor(void) {
   printf("Create!\n");
}
void dtor(void) __attribute__((destructor));

void dtor(void) {
   printf("Create!\n");
}

int main() { return 0; }
