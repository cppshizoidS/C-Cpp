#ifndef DRAWHELPER_H
#define DRAWHELPER_H

#include <gtkmm.h>
#include <iostream>

#include "DrawFunctions.h"
#include "IFigures.h"
#include "Figures.h"
#include "WindowNotify.h"

using namespace std;
constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 800;

class DrawHelper : public Gtk::DrawingArea {
    public:
        enum class Figure {
            None,
            Rectangle,
            Circle,
            Triangle,
            Ring,
        };
        DrawHelper() {
            add_events(Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
            signal_button_press_event().connect([this](GdkEventButton* pEvent){return OnButtonPressed(pEvent);});
            signal_motion_notify_event().connect([this](GdkEventMotion* pEvent){return OnMouseMotion(pEvent);});
            signal_button_release_event().connect([this](GdkEventButton* pEvent){return OnButtonReleased(pEvent);});
        }

        void SetCurrentShape(Figure pFigure) {
            mCurrentFigure = pFigure;
        }
        const Cairo::RefPtr<Cairo::Context> pContext;
        void Clear() {
            mAlreadyDrawn.clear();
            queue_draw();
        }

    private: 
        double mEndX;
        double mEndY;

        double mWidth;
        double mHeight;

        Coords coords;
        std::vector<std::unique_ptr<IFigures>> mAlreadyDrawn;

        bool on_draw(const Cairo::RefPtr<Cairo::Context>& pContext) override {
            
            if(!mBuffer){
                mBuffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,1,8,DEFAULT_WIDTH,DEFAULT_HEIGHT);
            }

            Gdk::Cairo::set_source_pixbuf(pContext, mBuffer, 0, 0);
            pContext->paint();  

            for(const auto& figure : mAlreadyDrawn) {
                figure->Draw(pContext);
            }

            if(mCurrentFigure == Figure::Rectangle) {
                DrawRectangle(pContext, coords.x, coords.y, mWidth, mHeight);
            }

            if(mCurrentFigure == Figure::Circle) {
                DrawCircle(pContext, coords.x, coords.y, mWidth);
            }

            if(mCurrentFigure == Figure::Triangle) {
                DrawTriangle(pContext, coords.x, coords.y, mEndX, mEndY);
            }

            if(mCurrentFigure == Figure::Ring) {
                DrawRing(pContext, coords.x, coords.y, mWidth, mWidth*2);
            }
            return false;
        }

        bool OnButtonPressed(GdkEventButton* pEvent) {
            coords.x = pEvent->x;
            coords.y = pEvent->y;

            return true;
        }

        bool OnMouseMotion(GdkEventMotion* pEvent) {
            mEndX = pEvent->x;
            mEndY = pEvent->y;

            mWidth  = mEndX - coords.x;
            mHeight = mEndY - coords.y;

            queue_draw();

            return true;
        }

        bool OnButtonReleased(GdkEventButton* pEvent) {
            if(mCurrentFigure == Figure::Rectangle) {
                Figures::Rectangle rectangle(mWidth, mHeight);
                IRectangle iRectangle(rectangle);
                iRectangle.setCoords(coords.x,coords.y);
                mAlreadyDrawn.push_back(std::make_unique<IRectangle>(iRectangle));

                showInfoDialog("Площадь: " + std::to_string(abs(rectangle.CalcArea())) + 
                               "\nПериметр: " + std::to_string(abs(rectangle.CalcPerimeter())) +
                               "\nШирина: " + std::to_string(abs(mWidth)) + "; Высота: " + std::to_string(abs(mHeight)) +
                               "\nX1: " + std::to_string(coords.x) + "; Y1: " + std::to_string(coords.y));
            }

            if(mCurrentFigure == Figure::Circle) {
                Figures::Circle circle(mWidth);
                ICircle iCircle(circle);
                iCircle.setCoords(coords.x,coords.y);
                mAlreadyDrawn.push_back(std::make_unique<ICircle>(iCircle));

                showInfoDialog("Площадь: " + std::to_string(abs(circle.CalcArea())) + 
                               "\nПериметр: " + std::to_string(abs(circle.CalcPerimeter())) +
                               "\nРадиус: " + std::to_string(abs(mWidth)) +
                               "\nX1: " + std::to_string(coords.x) + "; Y1: " + std::to_string(coords.y));
            }

            if(mCurrentFigure == Figure::Ring) {
                Figures::Ring ring(mWidth, mWidth*2);
                IRing iRing(ring);
                iRing.setCoords(coords.x, coords.y);
                mAlreadyDrawn.push_back(std::make_unique<IRing>(iRing));

                showInfoDialog("Площадь: " + std::to_string(abs(ring.CalcArea())) + 
                               "\nПериметр: " + std::to_string(abs(ring.CalcPerimeter())) +
                               "\nr: " + std::to_string(abs(mWidth)) + "; R: " + std::to_string(abs(mWidth*2)) + 
                               "\nX1: " + std::to_string(coords.x) + "; Y1: " + std::to_string(coords.y));
            }

            if(mCurrentFigure == Figure::Triangle) {
                // mAlreadyDrawn.push_back(std::make_unique<ITriangle>(coords.x, coords.y, mEndX, mEndY));
            }

            return true;
        }
        
        Figure mCurrentFigure = Figure::None;
        Glib::RefPtr<Gdk::Pixbuf> mBuffer;
};

#endif