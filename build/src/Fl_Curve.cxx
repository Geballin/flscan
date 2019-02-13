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

#define RADIUS 2
#define MIN_DISTANCE 8

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Curve.H>

int Fl_Curve::project(double value,double min,double max,int norm)
{
    return (int)((norm - 1) * ((value - min) / (max - min)) + 0.5);
}
double Fl_Curve::unproject(int value,double min,double max,int norm)
{
    return value / (double) (norm - 1) * (max - min) + min;
}
/* Solve the tridiagonal equation system that determines the second
   derivatives for the interpolation points.  (Based on Numerical
   Recipies 2nd Edition.) */
void Fl_Curve::spline_solve(int n,double x[],double y[],double y2[])
{
double p, sig, *u;
int i, k;

    u = new double[n-1];

    y2[0] = u[0] = 0.0;   /* set lower boundary condition to "natural" */

    for (i=1;i<n-1;++i) {
        sig = (x[i]-x[i-1])/(x[i+1]-x[i-1]);
        p = sig*y2[i-1]+2.0;
        y2[i] = (sig-1.0)/p;
        u[i] = ((y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/(x[i]-x[i-1]));
        u[i] = (6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }
    y2[n-1] = 0.0;
    for (k=n-2;k>=0;--k) {
        y2[k]=y2[k]*y2[k+1]+u[k];
    }
    delete[] u;
}
double Fl_Curve::spline_eval (
    int n,
    double x[],
    double y[],
    double y2[],
    double val
) {
int k_lo, k_hi, k;
double h, b, a;

    /* do a binary search for the right interval: */
    k_lo = 0;
    k_hi = n-1;
    while (k_hi-k_lo > 1) {
        k = (k_hi+k_lo)/2;
        if (x[k]>val) {
            k_hi = k;
        } else {
            k_lo = k;
        }
    }
    h = x[k_hi] - x[k_lo];

    assert (h > 0.0);

    a = (x[k_hi]-val)/h;
    b = (val-x[k_lo])/h;
    return a*y[k_lo] +
           b*y[k_hi] +
           ((a*a*a-a)*y2[k_lo]+(b*b*b-b)*y2[k_hi])*(h*h)/6.0;
}
void Fl_Curve::interpolate(int width,int height)
{
double *vector;
int i;

    vector = new double[width];

    get_vector(width,vector);

    height_ = height;
    if (num_points_ != width) {
        num_points_ = width;
        if (point_) {
            delete point_;
        }
        point_ = new Fl_Coord2d[num_points_];
    }
    for (i=0;i<width;++i) {
        point_[i].x = RADIUS + i;
        point_[i].y = RADIUS +
                      height - project (
                          vector[i],
                          min_y_,
                          max_y_,
                          height
                      );
    }
    delete[] vector;
}
void Fl_Curve::cursor(Fl_Cursor c)
{
    if (last_cursor_ != c) {
        last_cursor_=c;
        window()->cursor(c);
    }
}
int Fl_Curve::handle(int event)
{
#define FL_CLAMP(x,l,h)  (((x)>(h)) ? (h) : (((x)<(l)) ? (l) : (x)))

int i, src, dst, leftbound, rightbound;
int tx, ty;
int cx, px, py, width, height;
int closest_point = 0;
double rx, ry, min_x;
unsigned int distance;
int x1, x2, y1, y2;
Fl_Cursor save_cursor;

    width = w() - RADIUS * 2;
    height = h() - RADIUS * 2;

    if ((width < 0) || (height < 0)) {
        return 0;
    }
    /*  get the pointer position  */
    tx = Fl::event_x() - x();
    ty = Fl::event_y() - y();
    px = FL_CLAMP ((tx - RADIUS),0,width-1);
    py = FL_CLAMP ((ty - RADIUS),0,height-1);

    min_x = min_x_;

    distance = ~0U;
    for (i=0; i<num_ctlpoints_;++i) {
        cx = project(ctlpoint_[i].x,min_x,max_x_,width);
        if ((unsigned int)abs(px-cx) < distance) {
            distance = abs(px-cx);
            closest_point = i;
        }
    }
    switch (event) {
        case FL_ENTER:
            cursor(last_cursor_);
            break;
        case FL_LEAVE:
        case FL_UNFOCUS:
            save_cursor=last_cursor_;
            cursor(FL_CURSOR_DEFAULT);
            last_cursor_=save_cursor;
            break;
        case FL_PUSH:
            /* grab(this); */
            cursor(FL_CURSOR_CROSS);
            switch (type()) {
                case Fl_Curve::LINEAR:
                case Fl_Curve::SPLINE:
                    if (distance > MIN_DISTANCE) {
                        /* insert a new control point */
                        if (num_ctlpoints_ > 0) {
                            cx = project (
                                ctlpoint_[closest_point].x,
                                min_x,
                                max_x_,
                                width
                            );
                            if (px > cx) {
                              ++closest_point;
                            }
                        }
                        ++num_ctlpoints_;
                        Fl_Point *old_ctlpoint=ctlpoint_;
                        ctlpoint_ = new Fl_Point[num_ctlpoints_];
                        for (i=0;i<num_ctlpoints_;i++) {
                            ctlpoint_[i].x = old_ctlpoint[i].x;
                            ctlpoint_[i].y = old_ctlpoint[i].y;
                        }
                        delete old_ctlpoint;
                    }
                    grab_point_ = closest_point;
                    ctlpoint_[grab_point_].x = unproject (
                        px,
                        min_x,
                        max_x_,
                        width
                    );
                    ctlpoint_[grab_point_].y = unproject (
                        height-py,
                        min_y_,
                        max_y_,
                        height
                    );
                    interpolate(width,height);
                    break;
                case Fl_Curve::FREE:
                    point_[px].x = RADIUS + px;
                    point_[px].y = RADIUS + py;
                    grab_point_ = px;
                    last_ = py;
                    break;
            }
            needs_redraw();
            break;
        case FL_RELEASE:
            /* ungrab(this); */
            /* delete inactive points: */
            if (type() != Fl_Curve::FREE) {
                for (src = dst = 0; src < num_ctlpoints_; ++src) {
                    if (ctlpoint_[src].x >= min_x) {
                        ctlpoint_[dst].x = ctlpoint_[src].x;
                        ctlpoint_[dst].y = ctlpoint_[src].y;
                        ++dst;
                    }
                }
                if (dst < src) {
                    num_ctlpoints_ -= (src - dst);
                    if (num_ctlpoints_ <= 0) {
                        num_ctlpoints_ = 1;
                        ctlpoint_[0].x = min_x;
                        ctlpoint_[0].y = min_y_;
                        interpolate(width,height);
                        needs_redraw();
                    }
                    Fl_Point *old_ctlpoint=ctlpoint_;
                    ctlpoint_ = new Fl_Point[num_ctlpoints_];
                    for (i=0;i<num_ctlpoints_;i++) {
                        ctlpoint_[i].x = old_ctlpoint[i].x;
                        ctlpoint_[i].y = old_ctlpoint[i].y;
                    }
                    delete old_ctlpoint;
                }
            }
            cursor(FL_CURSOR_DEFAULT);
            grab_point_ = -1;
            break;
        case FL_MOVE:
        case FL_DRAG:
            switch (type()) {
                case Fl_Curve::LINEAR:
                case Fl_Curve::SPLINE:
                    if (grab_point_ == -1) {
                        /* if no point is grabbed...  */
                        if (distance <= MIN_DISTANCE) {
                            cursor(FL_CURSOR_DEFAULT);
                        } else {
                            cursor(FL_CURSOR_CROSS);
                        }
                    } else {
                        /* drag the grabbed point  */
                        cursor(FL_CURSOR_CROSS);
                  
                        leftbound = -MIN_DISTANCE;
                        if (grab_point_ > 0) {
                            leftbound = project (
                                ctlpoint_[grab_point_ - 1].x,
                                min_x,
                                max_x_,
                                width
                            );
                        }
                        rightbound = width + RADIUS * 2 + MIN_DISTANCE;
                        if (grab_point_ + 1 < num_ctlpoints_) {
                            rightbound = project (
                                ctlpoint_[grab_point_ + 1].x,
                                min_x,
                                max_x_,
                                width
                            );
                        }
                        if (
                            tx <= leftbound ||
                            tx >= rightbound ||
                            ty > height + RADIUS * 2 + MIN_DISTANCE ||
                            ty < -MIN_DISTANCE
                        ) {
                            ctlpoint_[grab_point_].x = min_x - 1.0;
                        } else {
                            rx = unproject(px,min_x,max_x_,width);
                            ry = unproject(height - py,min_y_,max_y_,height);
                            ctlpoint_[grab_point_].x = rx;
                            ctlpoint_[grab_point_].y = ry;
                        }
                        interpolate(width,height);
                        needs_redraw();
                    }
                    break;
                case Fl_Curve::FREE:
                    if (grab_point_ != -1) {
                        if (grab_point_ > px) {
                            x1 = px;
                            x2 = grab_point_;
                            y1 = py;
                            y2 = last_;
                        } else {
                            x1 = grab_point_;
                            x2 = px;
                            y1 = last_;
                            y2 = py;
                        }
                
                        if (x2 != x1) {
                            for (i = x1; i <= x2; i++) {
                                point_[i].x = RADIUS + i;
                                point_[i].y = RADIUS +
                                (y1 + ((y2 - y1) * (i - x1)) / (x2 - x1));
                            }
                        } else {
                            point_[px].x = RADIUS + px;
                            point_[px].y = RADIUS + py;
                        }
                        grab_point_ = px;
                        last_ = py;
                        needs_redraw();
                    }
                    if (Fl::event_button()==FL_LEFT_MOUSE) {
                        cursor(FL_CURSOR_CROSS);
                    } else {
                        cursor(FL_CURSOR_HAND);
                    }
                    break;
            }
            break;
        default:
            break;
    }
    return 1;
}
void Fl_Curve::draw()
{
int i;

    interpolate(w()-(RADIUS*2),h()-(RADIUS*2));
    fl_clip(x(),y(),w(),h());
    // clear the area
    fl_color(color());
    fl_rectf(x(),y(),w(),h());
    // begin the draw
    fl_push_matrix();
    fl_translate(x(),y());
#if 1
    // draw the grid lines: (XXX make more meaningful) */
    fl_color(fl_contrast(FL_DARK3,color()));
    for (i = 0; i < 5; i++) {
        fl_begin_line();
        fl_vertex(RADIUS,i*((h()-RADIUS*2)/4.0)+RADIUS);
        fl_vertex(w()-RADIUS,i*((h()-RADIUS*2)/4.0)+RADIUS);
        fl_end_line();
        fl_begin_line();
        fl_vertex(i*((w()-RADIUS*2)/4.0)+RADIUS,RADIUS);
        fl_vertex(i*((w()-RADIUS*2)/4.0)+RADIUS,h()-RADIUS);
        fl_end_line();
    }
#endif
    fl_color(fl_contrast(FL_BLACK,color()));
    // draw the curve
    fl_begin_points();
    for (i=0;i<num_points_;i++) {
        fl_vertex(point_[i].x,point_[i].y);
    }
    fl_end_points();
    fl_begin_polygon();
    if (type() != Fl_Curve::FREE) {
        // draw the bullets
        for (i=0;i<num_ctlpoints_;++i) {
            if (ctlpoint_[i].x<min_x_) {
                continue;
            }
            int x = project(ctlpoint_[i].x,min_x_,max_x_,w());
            int y = h()-project(ctlpoint_[i].y,min_y_,max_y_,h());
            // draw a bullet:
            fl_circle(x,y,RADIUS);
        }
    }
    fl_pop_matrix();
    fl_pop_clip();
}
void Fl_Curve::bounds(double *min_x,double *min_y,double *max_x,double *max_y)
{
    if (min_x) *min_x=min_x_;
    if (min_y) *min_y=min_y_;
    if (max_x) *max_x=max_x_;
    if (max_y) *max_y=max_y_;
}
void Fl_Curve::bounds(double min_x, double min_y, double max_x, double max_y)
{
    min_x_ = min_x; min_y_ = min_y;
    max_x_ = max_x; max_y_ = max_y;
    reset_vector();
}
void Fl_Curve::gamma(double gamma)
{
double x, one_over_gamma, height, one_over_width;
uchar old_type;
int i;

    if (num_points_ < 2) {
        return;
    }
    old_type = type();
    Fl_Widget::type(Fl_Curve::FREE);
    if (gamma <= 0) {
        one_over_gamma = 1.0;
    } else {
        one_over_gamma = 1.0 / gamma;
    }
    one_over_width = 1.0 / (num_points_ - 1);
    height = height_;
    for (i=0; i<num_points_; ++i) {
        x = (double)i/(num_points_ - 1);
        point_[i].x = RADIUS + i;
        point_[i].y = (int)(RADIUS + 
                      (height * (1.0 - pow(x,one_over_gamma)) + 0.5));
    }
    needs_redraw();
}
void Fl_Curve::type(uchar t)
{
double rx,dx;
int x,i;
int width, height;

    if (t != type()) {
        width  = w() - RADIUS * 2;
        height = h() - RADIUS * 2;
        if (t==Fl_Curve::FREE) {
            interpolate(width, height);
            Fl_Widget::type(t);
        } else if (type()) {
            if (ctlpoint_) {
                delete ctlpoint_;
            }
            num_ctlpoints_=9;
            ctlpoint_ = new Fl_Point[num_ctlpoints_];
            rx = 0.0;
            dx = (width-1)/(double)(num_ctlpoints_-1);
            for (i=0; i<num_ctlpoints_; ++i, rx+=dx) {
                x = (int)(rx+0.5);
                ctlpoint_[i].x = unproject (
                    x,
                    min_x_,
                    max_x_,
                    width
                );
                ctlpoint_[i].y = unproject (
                    RADIUS + height - point_[x].y,
                    min_y_,
                    max_y_,
                    height
                );
            }
            Fl_Widget::type(t);
            interpolate(width, height);
        } else {
            Fl_Widget::type(t);
            interpolate(width, height);
        }
        needs_redraw();
    }
}
void Fl_Curve::reset_vector()
{
int width, height;

    if (ctlpoint_) {
        delete ctlpoint_;
    }
    num_ctlpoints_ = 2;
    ctlpoint_ = new Fl_Point[2];
    ctlpoint_[0].x = min_x_;
    ctlpoint_[0].y = min_y_;
    ctlpoint_[1].x = max_x_;
    ctlpoint_[1].y = max_y_;
    width  = w() - RADIUS * 2;
    height = h() - RADIUS * 2;
    if (type()==Fl_Curve::FREE) {
        Fl_Widget::type(Fl_Curve::LINEAR);
        interpolate(width,height);
        Fl_Widget::type(Fl_Curve::FREE);
    } else {
        interpolate(width,height);
    }
    needs_redraw();
}
void Fl_Curve::reset()
{
uchar old_type;

    old_type = type();
    Fl_Widget::type(Fl_Curve::SPLINE);
    reset_vector();
}
void Fl_Curve::set_vector(int veclen,double vector[])
{
uchar old_type;
double rx, dx, ry;
int i, height;

    old_type = type();
    Fl_Widget::type(Fl_Curve::FREE);
    if (point_) {
        height = h() - RADIUS * 2;
    } else {
        height = (int)(max_y_-min_y_);
        if (height>Fl::h()/4) {
            height = Fl::h()/4;
        }
        height_ = height;
        num_points_ = veclen;
        point_ = new Fl_Coord2d[num_points_];
    }
    rx = 0;
    dx = (veclen-1.0)/(num_points_-1.0);
    for (i=0;i<num_points_;++i,rx+=dx) {
        ry = vector[(int)(rx+0.5)];
        if (ry>max_y_) ry=max_y_;
        if (ry<min_y_) ry=min_y_;
        point_[i].x = RADIUS + i;
        point_[i].y = RADIUS + 
                      height - project (
                          ry,
                          min_y_,
                          max_y_,
                          height
                      );
    }
    needs_redraw();
}
void Fl_Curve::get_vector(int veclen,double vector[])
{
double rx, ry, dx, dy, min_x, delta_x, *mem, *xv, *yv, *y2v, prev;
int dst, i, x, next, num_active_ctlpoints = 0, first_active = -1;

    min_x = min_x_;
    if (type() != Fl_Curve::FREE) {
        /* count active points: */
        prev = min_x-1.0;
        for (i=num_active_ctlpoints=0;i<num_ctlpoints_;++i) {
            if (ctlpoint_[i].x>prev) {
                if (first_active<0) {
                    first_active = i;
                }
                prev = ctlpoint_[i].x;
                ++num_active_ctlpoints;
            }
        }
        /* handle degenerate case: */
        if (num_active_ctlpoints<2) {
            if (num_active_ctlpoints>0) {
                ry = ctlpoint_[first_active].y;
            } else {
                ry = min_y_;
            }
            if (ry<min_y_) ry=min_y_;
            if (ry>max_y_) ry=max_y_;
            for (x=0;x<veclen;++x) {
                vector[x]=ry;
            }
            return;
        }
    }
    switch (type()) {
        case Fl_Curve::SPLINE:
            mem = new double[3*num_active_ctlpoints];
            xv  = mem;
            yv  = mem + num_active_ctlpoints;
            y2v = mem + 2*num_active_ctlpoints;
      
            prev = min_x - 1.0;
            for (i = dst = 0; i < num_ctlpoints_; ++i) {
                if (ctlpoint_[i].x > prev) {
                    prev    = ctlpoint_[i].x;
                    xv[dst] = ctlpoint_[i].x;
                    yv[dst] = ctlpoint_[i].y;
                    ++dst;
                }
            }
            spline_solve(num_active_ctlpoints,xv,yv,y2v);
      
            rx = min_x;
            dx = (max_x_-min_x)/(veclen-1);
            for (x=0; x<veclen; ++x,rx+=dx) {
                ry = spline_eval(num_active_ctlpoints,xv,yv,y2v,rx);
                if (ry<min_y_) ry=min_y_;
                if (ry>max_y_) ry=max_y_;
                vector[x] = ry;
            }
            delete[] mem;
            break;
        case Fl_Curve::LINEAR:
            dx = (max_x_-min_x)/(veclen-1);
            rx = min_x;
            ry = min_y_;
            dy = 0.0;
            i  = first_active;
            for (x=0; x<veclen; ++x,rx+=dx) {
                if (rx>=ctlpoint_[i].x) {
                    if (rx>ctlpoint_[i].x) {
                        ry = min_y_;
                    }
                    dy = 0.0;
                    next = i + 1;
                    while (
                        next < num_ctlpoints_ &&
                        ctlpoint_[next].x <= ctlpoint_[i].x
                    ) {
                        ++next;
                    }
                    if (next<num_ctlpoints_) {
                        delta_x = ctlpoint_[next].x-ctlpoint_[i].x;
                        dy = ((ctlpoint_[next].y-ctlpoint_[i].y)/delta_x);
                        dy *= dx;
                        ry = ctlpoint_[i].y;
                        i = next;
                    }
                }
                vector[x] = ry;
                ry += dy;
            }
            break;
        case Fl_Curve::FREE:
            if (point_) {
                rx = 0.0;
                dx = num_points_/(double)veclen;
                for (x=0; x<veclen; ++x,rx+=dx) {
                    vector[x] = unproject (
                        RADIUS+height_ - point_[(int)rx].y,
                        min_y_,
                        max_y_,
                        height_
                    );
                }
            } else {
                memset(vector,0,veclen*sizeof(vector[0]));
            }
            break;
    }
}
