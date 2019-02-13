//
// Copyright 2001-2002 by Francesco Bradascio.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fbradasc@katamail.com".
//
//         FlScan is based in part on the work of
//         the FLTK project (http://www.fltk.org).
//
//

#ifndef FL_CURVE_H
#define FL_CURVE_H

struct Fl_Coord2d {
    short x;
    short y;
};

struct Fl_Point {
    double x;
    double y;
};

class Fl_Curve: public Fl_Widget
{
private:
    Fl_Cursor last_cursor_;

    double min_x_;
    double max_x_;
    double min_y_;
    double max_y_;

    int height_;
    int grab_point_;
    int last_;

    int num_points_;
    Fl_Coord2d *point_;

    int num_ctlpoints_;
    Fl_Point *ctlpoint_;

protected:
    void cursor(Fl_Cursor c);
    static int project(double value,double min,double max,int norm);
    static double unproject(int value,double min,double max,int norm);
    /* Solve the tridiagonal equation system that determines the second
       derivatives for the interpolation points.  (Based on Numerical
       Recipies 2nd Edition.) */
    static void spline_solve(int n,double x[],double y[],double y2[]);
    static double spline_eval (
        int n,double x[],double y[],double y2[],double val
    ) ;
    void interpolate(int width,int height);
    void needs_redraw()
    {
        if (when() & FL_WHEN_CHANGED) {
            do_callback();
        } else {
            set_changed();
        }
        redraw();
    }

public:
    enum { /* values for type() */
        LINEAR=0,
        SPLINE,
        FREE
    };

    Fl_Curve(int x,int y, int w, int h,char *l=0): Fl_Widget(x,y,w,h,l)
    {
        last_cursor_=FL_CURSOR_DEFAULT;
        num_points_=0;
        point_=0;
        height_=0;
        num_ctlpoints_=0;
        ctlpoint_=0;
        min_x_ = 0.0;
        max_x_ = 1.0;
        min_y_ = 0.0;
        max_y_ = 1.0;
        grab_point_=-1;
        last_=-1;
        reset_vector();
    }
    ~Fl_Curve()
    {
        if (point_) {
            delete[] point_;
        }
        if (ctlpoint_) {
            delete[] ctlpoint_;
        }
    }
    int handle(int);
    void draw();
    void bounds(double *min_x,double *min_y,double *max_x,double *max_y);
    void minimum(double x,double y) { bounds(x,y,max_x_,max_y_); }
    void min_x(double x) { bounds(x,min_y_,max_x_,max_y_); }
    void min_y(double y) { bounds(min_x_,y,max_x_,max_y_); }
    void maximum(double x,double y) { bounds(min_x_,min_y_,x,y); }
    void max_x(double x) { bounds(min_x_,min_y_,x,max_y_); }
    void max_y(double y) { bounds(min_x_,min_y_,max_x_,y); }
    void bounds(double min_x, double min_y, double max_x, double max_y);
    void gamma(double gamma);
    uchar type() { return Fl_Widget::type(); }
    void type(uchar t);
    void reset_vector();
    void reset();
    void set_vector(int veclen,double vector[]);
    void get_vector(int veclen,double vector[]);
};

#endif // FL_CURVE_H
