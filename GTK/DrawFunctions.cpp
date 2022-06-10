#include <gtkmm.h>
#include <cairo/cairo.h>
#include "DrawFunctions.h"

constexpr double LINE_WIDTH = 5.0;

void DrawRectangle(const Cairo::RefPtr<Cairo::Context>& pContext, double pStartX, double pStartY, double pWidth, double pHeight) {
        pContext->save();

        pContext->set_line_width(LINE_WIDTH);
        pContext->set_source_rgba(0, 0, 1, 1);
        pContext->rectangle(pStartX, pStartY, pWidth, pHeight);
        pContext->stroke();
        pContext->restore();
}

void DrawCircle(const Cairo::RefPtr<Cairo::Context>& pContext, double pStartX, double pStartY, double pWidth) {
        pContext->save();

        pContext->set_line_width(LINE_WIDTH);
        pContext->set_source_rgba(0, 0, 1, 1);
        pContext->arc(pStartX, pStartY, abs(pWidth), 0, 2 * M_PI);

        pContext->stroke();
        pContext->restore();
}

void DrawRing(const Cairo::RefPtr<Cairo::Context>& pContext, double pStartX, double pStartY, double pWidth, double pWidth2) {
        pContext->save();

        pContext->set_line_width(LINE_WIDTH);
        pContext->set_source_rgba(0, 0, 1, 1);
        pContext->arc(pStartX, pStartY, abs(pWidth), 0, 2 * M_PI);
        pContext->stroke();
        pContext->arc(pStartX, pStartY, abs(pWidth2), 0, 2 * M_PI);

        pContext->stroke();
        pContext->restore();
}

void DrawTriangle(const Cairo::RefPtr<Cairo::Context>& pContext, double pmX1, double pmY1, double pmX2, double pmY2) {
        pContext->save();

        pContext->set_line_width(LINE_WIDTH);
        pContext->set_source_rgba(0, 0, 1, 1);
        
        pContext->move_to(pmX1, pmY1);
        pContext->line_to(pmX1, pmY2);
        pContext->line_to(pmX2, pmY2);
        pContext->close_path();
        pContext->set_line_join(Cairo::LINE_JOIN_BEVEL);

        pContext->stroke();
        pContext->restore();
}