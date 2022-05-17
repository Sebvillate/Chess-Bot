#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Window w;
  int s;
  GC gc;
  unsigned long colours[11];

 public:
  Display *d;
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  // Available colours.
  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown, Gray};

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);

  void clear(int x, int y, int width, int height);

  void drawPawn(int x, int y, int colour, int zoomFactor);
  
  void drawRook(int x, int y, int colour, int zoomFactor);

  void drawKnight(int x, int y, int colour, int zoomFactor);

  void drawBishop(int x, int y, int colour, int zoomFactor);

  void drawKing(int x, int y, int colour, int zoomFactor);

  void drawQueen(int x, int y, int colour, int zoomFactor);

};

#endif
