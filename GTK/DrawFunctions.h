#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

#include <gtkmm.h>
#include <cairo/cairo.h>

void DrawRectangle(const Cairo::RefPtr<Cairo::Context>& ,double, double, double, double);
void DrawCircle(const Cairo::RefPtr<Cairo::Context>&, double, double, double);
void DrawTriangle(const Cairo::RefPtr<Cairo::Context>&, double, double, double, double);
void DrawRing(const Cairo::RefPtr<Cairo::Context>&, double, double, double, double);

#endif