#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include "DrawHelper.h"

class MyWindow : public Gtk::Window {

public:
    MyWindow(): mDrawRectangleButton{"Квадрат"}, 
                mDrawCircleButton{"Круг"}, 
                mDrawTriangleButton("Треугольник"), 
                mDrawRingButton("Кольцо"),
                mClearButton("Очистить") {

        set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);

        mHeaderBar.set_show_close_button(true);
        mHeaderBar.set_title("ShapesGTK");
        mHeaderBar.set_subtitle("Release 1.1.1");

        mBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        mBox.pack_start(mToolBar,false,false);
        mBox.pack_start(mDrawArea,true,true);

        mToolBar.append(mDrawRectangleButton);
        mToolBar.append(mDrawCircleButton);
        mToolBar.append(mDrawTriangleButton);
        mToolBar.append(mDrawRingButton);
        mToolBar.append(mClearButton);
        
        set_titlebar(mHeaderBar);
        set_hide_titlebar_when_maximized(true);
        add(mBox);

        mDrawRectangleButton.signal_clicked().connect([this](){RectangleButtonClicked();});
        mDrawCircleButton.signal_clicked().connect([this](){CircleButtonClicked();});
        mDrawTriangleButton.signal_clicked().connect([this](){TriangleButtonClicked();});
        mDrawRingButton.signal_clicked().connect([this](){RingButtonClicked();});
        mClearButton.signal_clicked().connect([this](){ClearButtonClicked();});
        show_all();
    }
    
private:

    Gtk::HeaderBar mHeaderBar;
    Gtk::Box mBox;
    Gtk::Toolbar mToolBar;
    Gtk::ToolButton mDrawRectangleButton;
    Gtk::ToolButton mDrawCircleButton;
    Gtk::ToolButton mDrawTriangleButton;
    Gtk::ToolButton mDrawRingButton;
    Gtk::ToolButton mClearButton;
    DrawHelper mDrawArea;
    
    void RectangleButtonClicked(){
        mDrawArea.SetCurrentShape(DrawHelper::Figure::Rectangle);
    }

    void CircleButtonClicked(){
        mDrawArea.SetCurrentShape(DrawHelper::Figure::Circle);
    }

    void TriangleButtonClicked(){
        mDrawArea.SetCurrentShape(DrawHelper::Figure::Triangle);
    }

    void RingButtonClicked(){
        mDrawArea.SetCurrentShape(DrawHelper::Figure::Ring);
    }

    void ClearButtonClicked(){
        mDrawArea.Clear();
    }
};

#endif