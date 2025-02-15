// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
  state *s = malloc(sizeof(state));
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = LINE;
  s->start = 0;
  s->data = 0;
  s->end = false;
  return s;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  return (b&0xC0) >> 6;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  signed char q;
  
  if ((b & 0x20) != 0) {
    q = (b & 0x1F) - 32;
  } else {
    q = (signed char)(b & 0x3F);
  }
  
  return q;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  int opcode = getOpcode(op);
  int operand = getOperand(op);

  switch (opcode) {
    case TOOL:
      s->tool = operand;
      break;
    case DATA:
      s->data = (s->data << 6) | (op&0x3F);
      break;
    case DX: 
      s->tx += operand;
      break;
    case DY:
      s->ty += operand;
      
      if (s->tool == LINE) {
        line(d, s->x, s->y, s->tx, s->ty);
      }

      s->x = s->tx;
      s->y = s->ty;
      break;
    default:
      break;
  }
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, const char pressedKey, void *data) {
  if (data == NULL) return (pressedKey == 27);

  state *s = (state*)data;
  char *name = getName(d);
  FILE *f = fopen(name, "rb");

  if (f == NULL) {
    printf("Error: could not open file %s\n", name);
    freeDisplay(d);
    exit(1);
  }

  byte op;
  while (fread(&op, sizeof(byte), 1, f) == 1) obey(d, s, op);

  fclose(f);
  show(d);
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = LINE;
  s->start = 0;
  s->data = 0;
  s->end = false;
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
