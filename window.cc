#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[11][10]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown", "gray"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for (int i=0; i < 11; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}

void Xwindow::clear(int x, int y, int width, int height) {
  XClearArea(d, w, x, y, width, height, false);
}

void Xwindow::drawPawn(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 10));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));

  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 2));

  // left side
  XDrawLine(d, w, gc, x, (y - zoomFactor * 2), (x + zoomFactor * 2), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 4), (x + zoomFactor * 1), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 5), (x + zoomFactor * 1), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 6), (x + zoomFactor * 2), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 7), (x + zoomFactor * 1), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 8), (x + zoomFactor * 1), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 9), (x + zoomFactor * 1), (y - zoomFactor * 10));

  // top
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 10), (x + zoomFactor * 2), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 11), (x + zoomFactor * 5), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 11), (x + zoomFactor * 6), (y - zoomFactor * 10));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 2), (x + zoomFactor * 5), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 4), (x + zoomFactor * 6), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 5), (x + zoomFactor * 6), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 6), (x + zoomFactor * 5), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 7), (x + zoomFactor * 6), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 8), (x + zoomFactor * 6), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 9), (x + zoomFactor * 6), (y - zoomFactor * 10));

  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawRook(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 10));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));

  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 2));

  // left side
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 1), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 7), x, (y - zoomFactor * 11));

  // top
  XDrawLine(d, w, gc, x, (y - zoomFactor * 11), (x + zoomFactor * 1), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 11), (x + zoomFactor * 2), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 10), (x + zoomFactor * 3), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 10), (x + zoomFactor * 4), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 11), (x + zoomFactor * 5), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 11), (x + zoomFactor * 6), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 10), (x + zoomFactor * 7), (y - zoomFactor * 11));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 11), (x + zoomFactor * 7), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 7));

  // horizontals
  XDrawLine(d, w, gc, x, (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 7), (x + zoomFactor * 7), (y - zoomFactor * 7));
  
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawKnight(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 1));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 7), (x + zoomFactor * 1), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 8));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));
  
  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 1));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 1));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 1));


  // left side
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 1), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 4), (x + zoomFactor * 2), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 5), (x + zoomFactor * 2), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 6), x, (y - zoomFactor * 7));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 7), (x + zoomFactor * 2), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 9), (x + zoomFactor * 1), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 10), (x + zoomFactor * 1), (y - zoomFactor * 11));

  // top
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 11), (x + zoomFactor * 4), (y - zoomFactor * 11));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 11), (x + zoomFactor * 7), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 8), (x + zoomFactor * 7), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 5), (x + zoomFactor * 6), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 4), (x + zoomFactor * 6), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 1));

  // horizontals
  XDrawLine(d, w, gc, x, (y - zoomFactor * 6), (x + zoomFactor * 2), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 2));
  
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawBishop(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 6));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));
  
  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 1));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 1));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 1));

  // left side
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 1), (y - zoomFactor * 3));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 3), x, (y - zoomFactor * 4));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 4), x, (y - zoomFactor * 6));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 6), (x + zoomFactor * 1), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 7), (x + zoomFactor * 2), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 8), (x + zoomFactor * 1), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 9), (x + zoomFactor * 1), (y - zoomFactor * 10));

  // top
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 10), (x + zoomFactor * 2), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 11), (x + zoomFactor * 5), (y - zoomFactor * 11));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 11), (x + zoomFactor * 6), (y - zoomFactor * 10));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 3));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 3), (x + zoomFactor * 7), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 4), (x + zoomFactor * 7), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 6), (x + zoomFactor * 6), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 7), (x + zoomFactor * 5), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 8), (x + zoomFactor * 6), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 9), (x + zoomFactor * 6), (y - zoomFactor * 10));

  // horizontals
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 2));

  // bishop diagonal line
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 5), (x + zoomFactor * 6), (y - zoomFactor * 7));
  
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawKing(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 7));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 8));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));
  
  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 1));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 1));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 1));

  // left side
  XDrawLine(d, w, gc, x, (y - zoomFactor * 9), x, (y - zoomFactor * 5));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 5), (x + zoomFactor * 1), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 4), (x + zoomFactor * 1), (y - zoomFactor * 2));

  // top
  XDrawLine(d, w, gc, x, (y - zoomFactor * 9), (x + zoomFactor * 1), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 7), (x + zoomFactor * 1), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 7), (x + zoomFactor * 6), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 9), (x + zoomFactor * 7), (y - zoomFactor * 9));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 9), (x + zoomFactor * 7), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 5), (x + zoomFactor * 6), (y - zoomFactor * 4));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 4), (x + zoomFactor * 6), (y - zoomFactor * 2));

  // horizontals
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 2));
  
  XSetForeground(d, gc, colours[Black]);
  
  // cross
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 11), (x + zoomFactor * 5), (y - zoomFactor * 11));
  XFillRectangle(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 12), zoomFactor * 2, zoomFactor * 4);
  
}

void Xwindow::drawQueen(int x, int y, int colour, int zoomFactor) {
  x += zoomFactor * 3;
  y -= zoomFactor * 2;
  
  // colour fill
  XSetForeground(d, gc, colours[colour]);

  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 1), (x + zoomFactor * 2), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 3), (y - zoomFactor * 1), (x + zoomFactor * 3), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 1), (x + zoomFactor * 4), (y - zoomFactor * 10));
  XDrawLine(d, w, gc, (x + zoomFactor * 5), (y - zoomFactor * 1), (x + zoomFactor * 5), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 1), (x + zoomFactor * 6), (y - zoomFactor * 8));

  XSetForeground(d, gc, (colour == Black ? colours[Gray] : colours[Black]));
  
  // base
  XDrawLine(d, w, gc, x, y, (x + zoomFactor * 7), y);
  XDrawLine(d, w, gc, x, y, x, (y - zoomFactor * 1));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 1), (x + zoomFactor * 1), (y - zoomFactor * 2));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), y, (x + zoomFactor * 7), (y - zoomFactor * 1));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 2), (x + zoomFactor * 7), (y - zoomFactor * 1));

  // left side
  XDrawLine(d, w, gc, x, (y - zoomFactor * 9), x, (y - zoomFactor * 6));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 6), (x + zoomFactor * 1), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 5), (x + zoomFactor * 1), (y - zoomFactor * 2));

  // top
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 9), (x + zoomFactor * 4), (y - zoomFactor * 12));
  XDrawLine(d, w, gc, x, (y - zoomFactor * 9), (x + zoomFactor * 2), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 2), (y - zoomFactor * 8), (x + zoomFactor * 3), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 9), (x + zoomFactor * 3), (y - zoomFactor * 9));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 9), (x + zoomFactor * 5), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 4), (y - zoomFactor * 9), (x + zoomFactor * 5), (y - zoomFactor * 8));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 9), (x + zoomFactor * 3), (y - zoomFactor * 12));

  // right side
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 9), (x + zoomFactor * 7), (y - zoomFactor * 6));
  XDrawLine(d, w, gc, (x + zoomFactor * 7), (y - zoomFactor * 6), (x + zoomFactor * 6), (y - zoomFactor * 5));
  XDrawLine(d, w, gc, (x + zoomFactor * 6), (y - zoomFactor * 5), (x + zoomFactor * 6), (y - zoomFactor * 2));

  // horizontals
  XDrawLine(d, w, gc, (x + zoomFactor * 1), (y - zoomFactor * 2), (x + zoomFactor * 6), (y - zoomFactor * 2));
  
  XSetForeground(d, gc, colours[Black]);
}
