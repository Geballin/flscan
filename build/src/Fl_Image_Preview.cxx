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

#include <FL/Fl_Image_Preview.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static int pxmin=0,pxmax=0,pymin=0,pymax=0,px=0,py=0,cross_drawn=0;
static bool first_draw=true;
static int x1s=-1,y1s=-1,x2s=-1,y2s=-1;

static Fl_Image_Preview *currentImagePreview=0;

static void Image_Preview_setCurrent(Fl_Image_Preview *ip)
{
    currentImagePreview=ip;
}
static void Image_Preview_begin(int w,int h,bool iscolor)
{
    if (currentImagePreview) {
        currentImagePreview->begin_image(w,h,iscolor);
    }
}
static void Image_Preview_addRow(unsigned char *buf)
{
    if (currentImagePreview) {
        currentImagePreview->add_row(buf);
    }
}

static void draw_current_cross()
{
#ifdef WIN32
    int old = SetROP2(fl_gc, R2_NOT);
    fl_xyline(pxmin, py, pxmax);
    fl_yxline(px, pymin, pymax);
    SetROP2(fl_gc, old);
#else
    XSetFunction(fl_display, fl_gc, GXxor);
    XSetForeground(fl_display, fl_gc, 0xffffffff);
    fl_xyline(pxmin, py, pxmax);
    fl_yxline(px, pymin, pymax);
    XSetFunction(fl_display, fl_gc, GXcopy);
#endif
    cross_drawn=1;
}
void erase_cross()
{
    if (cross_drawn) { /* erase previous cross */
        draw_current_cross();
        cross_drawn=0;
    }
}
void draw_cross(int xmin,int ymin,int xx,int yy,int xmax,int ymax)
{
    erase_cross();
    if (!cross_drawn) {
        xx = (xx<xmin)?xmin:(xx>xmax)?xmax:xx;
        yy = (yy<ymin)?ymin:(yy>ymax)?ymax:yy;
        if (
            xmin==pxmin && ymin==pymin && 
            xmax==pxmax && ymax==pymax && 
            xx==px       && yy==py
        ) {
            return;
        }
        // erase_cross();
        pxmin=xmin;
        pymin=ymin; 
        pxmax=xmax;
        pymax=ymax; 
        px=xx;
        py=yy;
        draw_current_cross(); /* draw new cross */
    }
}
void erase_box()
{
    fl_line_style(FL_DASH);
    fl_overlay_clear();
    fl_line_style(FL_SOLID);
}
void draw_box(int x,int y,int w,int h)
{
    erase_box();
    fl_line_style(FL_DASH);
    fl_overlay_rect(x,y,w,h);
    fl_line_style(FL_SOLID);
}
void erase_rubber_line(bool erase)
{
    if (x1s>=0 && y1s>=0 && x2s>=0 && y2s>=0 && x1s!=x2s && y1s!=y2s) {
        fl_line_style(FL_DASH);
#ifdef WIN32
        int old = SetROP2(fl_gc, R2_NOT);
        fl_line(x1s,y1s,x2s,y2s);
        SetROP2(fl_gc, old);
#else
        XSetFunction(fl_display, fl_gc, GXxor);
        XSetForeground(fl_display, fl_gc, 0xffffffff);
        fl_line(x1s,y1s,x2s,y2s);
        XSetFunction(fl_display, fl_gc, GXcopy);
#endif
        fl_line_style(FL_SOLID);
    }
    if (erase) {
        x1s=-1; y1s=-1; x2s=-1; y2s=-1;
    }
}
void draw_rubber_line(int x1,int y1,int x2,int y2)
{
    erase_rubber_line(1);
    x1s=x1; y1s=y1; x2s=x2; y2s=y2;
    erase_rubber_line(0);
}
void Fl_Image_Preview::draw_image()
{
int X,Y,W,H,cx,cy,XP=0,YP=0,WP=0,HP=0;

    draw_box();
    xib_=XP=x()+(w()-newcols_)/2;
    yib_=YP=y()+(h()-newrows_)/2;
    wib_=WP=newcols_;
    hib_=HP=newrows_;
    fl_clip_box(XP,YP,WP,HP,X,Y,W,H);
    cx = X-XP;
    cy = Y-YP;
    if (cx < 0) {
        W += cx;
        X -= cx;
        cx = 0;
    }
    if (cx+W > w()) {
        W = w()-cx;
    }
    if (W <= 0) {
        return;
    }
    if (cy < 0) {
        H += cy;
        Y -= cy;
        cy = 0;
    }
    if (cy+H > h()) {
        H = h()-cy;
    }
    if (H <= 0) {
        return;
    }
    Fl_Offscreen id = fl_create_offscreen(newcols_,newrows_);
    fl_begin_offscreen(id);
    fl_draw_image(preview_data_,0,0,newcols_,newrows_,3,0);
    fl_end_offscreen();
    fl_copy_offscreen(X,Y,W,H,id,cx,cy);
    fl_delete_offscreen(id);
}
void Fl_Image_Preview::add_row(unsigned char *buf)
{
int row_size=0,old_size=0;

    if (buf && imgv_w_>0) {
        newcols_=imgv_w_;
        row_size = newcols_*3;
        old_size = newrows_*row_size;
        preview_data_=(unsigned char*)realloc (
            (void*)preview_data_,
            old_size + row_size
        );
        if (imgv_color_) {
            memcpy((void*)(preview_data_+old_size),(void*)buf,row_size);
        } else {
            for (int j=0,i=0;i<newcols_;i++,j+=3) {
                (preview_data_+old_size)[j+0]=buf[i];
                (preview_data_+old_size)[j+1]=buf[i];
                (preview_data_+old_size)[j+2]=buf[i];
            }
        }
        newrows_++;
    }
}
void Fl_Image_Preview::rotate(double angle)
{
    this->Image_Stream::rotate(angle);
    draw();
}
void Fl_Image_Preview::rotate_cw()
{
    this->Image_Stream::rotate_cw();
    draw();
}
void Fl_Image_Preview::rotate_ccw()
{
    this->Image_Stream::rotate_ccw();
    draw();
}
void Fl_Image_Preview::undo_rotate()
{
    this->Image_Stream::undo_rotate();
    draw();
}
void Fl_Image_Preview::flip_vertical()
{
    this->Image_Stream::flip_vertical();
    draw();
}
void Fl_Image_Preview::flip_horizontal()
{
    this->Image_Stream::flip_horizontal();
    draw();
}
void Fl_Image_Preview::draw()
{
// unsigned char *buf=0;

    if (visible() && active()) {
        first_draw=false;
        erase_cross();
        erase_box();
        if (refresh_) {
/*
            newrows_=0; newcols_=0;
            if (preview_data_) {
                delete preview_data_;
            }
            preview_data_ = 0;
*/
            Image_Preview_setCurrent(this);
            if (
                this->Image_Stream::serialize (
                    &row_buffer_,
                    w(),h(),
                    Image_Preview_begin,
                    Image_Preview_addRow
                )
            ) {
                damage(FL_DAMAGE_ALL);
                draw_image();
            }
/*
            if (buf) {
                delete buf;
            }
*/
        }
    }
}
Fl_Image_Preview::Fl_Image_Preview(int x,int y,int w,int h,char *l)
:Fl_Widget(x,y,w,h,0),Image_Stream()
{
    set_horizont_=false;
    set_horizont_tb_=0;
    row_buffer_=0;
    preview_data_=0;
    newcols_=0;
    newrows_=0;
    off_screen_id_=0;
    xib_=yib_=wib_=hib_=0;
    xsb_=ysb_=wsb_=hsb_=0;
    refresh_=false;
}
Fl_Image_Preview::~Fl_Image_Preview() {
    if (row_buffer_) {
        delete row_buffer_;
    }
    row_buffer_=0;
    if (preview_data_) {
        delete preview_data_;
    }
    preview_data_=0;
}
void Fl_Image_Preview::begin_image(int w,int h,bool iscolor)
{
    newrows_=0; newcols_=0;
    if (preview_data_) {
        delete preview_data_;
    }
    preview_data_ = 0;
    imgv_w_=w;
    imgv_h_=h;
    imgv_color_=(iscolor)?1:0;
}
bool Fl_Image_Preview::begin(int image_width,int bit_depth,bool iscolor)
{
    refresh_=false;
    return this->Image_Stream::begin(image_width,bit_depth,iscolor);
}
void Fl_Image_Preview::end()
{
    refresh_=true;
    damage(FL_DAMAGE_ALL);
    redraw();
}
void Fl_Image_Preview::reset()
{
    this->Image_Stream::reset();
    damage(FL_DAMAGE_ALL);
    draw();
    xh_=-1; yh_=-1;
    xsb_=ysb_=wsb_=hsb_=0;
    xib_=yib_=wib_=hib_=0;
}
void Fl_Image_Preview::calc_horizont(int x1,int y1,int x2,int y2)
{
double ang,pi;

    if ((y2-y1)!=0 && (x2-x1)!=0) {
        pi=acos(-1);
        ang=(180.0*atan(fabs((double)(y2-y1))/fabs((double)(x2-x1))))/pi;
        if (ang >45.0) {
            ang = ang - 90.0;
        }
        if (x2>x1) {     // quadranti 1 e 4
            if (y2>y1) { // quadrante 1
                ang *= -1.0;
            } else {     // quadrante 4
                ang *= +1.0;
            }
        } else {         // quadranti 2 e 3
            if (y2>y1) { // quadrante 2
                ang *= +1.0;
            } else {     // quadrante 3
                ang *= -1.0;
            }
        }
        if (set_horizont_tb_) {
            set_horizont_tb_->value(0);
        }
        set_horizont_=0;
        xh_=-1;
        yh_=-1;
        this->Image_Stream::rotate(ang);
    }
}
void Fl_Image_Preview::set_horizont(bool set,Fl_Button *act)
{
    set_horizont_=set;
    set_horizont_tb_=act;
    if (!set) {
        xh_=-1;
        yh_=-1;
    }
}
void Fl_Image_Preview::selection(double& sx,double& sy,double& sw,double& sh)
{
    if (wsb_*hsb_) {
        sx=(double)(xsb_-(x()+(w()-newcols_)/2))/newcols_;
        sy=(double)(ysb_-(y()+(h()-newrows_)/2))/newrows_;
        sw=(double)wsb_/newcols_;
        sh=(double)hsb_/newrows_;
    } else {
        sx=sy=0.0;
        sw=sh=1.0;
    }
}
int Fl_Image_Preview::handle(int event)
{
static int ix, iy;
static int dragged;
static int button;
int x2,y2;

    if (visible() && active() && !first_draw) {
        switch (event) {
            case FL_MOVE:
                window()->make_current();
                // erase_cross();
                ix = Fl::event_x();
                if (ix<x()) {
                    ix=x();
                }
                if (ix>=x()+w()) {
                    ix=x()+w()-1;
                }
                iy = Fl::event_y();
                if (iy<y()) {
                    iy=y();
                }
                if (iy>=y()+h()) {
                    iy=y()+h()-1;
                }
                draw_cross(x(),y(),ix,iy,x()+w()-1,y()+h()-1);
                break;
            case FL_ENTER:
                window()->cursor(FL_CURSOR_CROSS);
                window()->make_current();
                if (!set_horizont_ && wsb_*hsb_) {
                    ::draw_box(xsb_,ysb_,wsb_,hsb_);
                }
                return 1;
            case FL_LEAVE:
                window()->make_current();
                window()->cursor(FL_CURSOR_DEFAULT);
                if (set_horizont_) {
                    erase_rubber_line(1);
                } else {
                    erase_box();
                }
                erase_cross();
                return 1;
            case FL_RELEASE:
                if (set_horizont_) {
                    erase_rubber_line(1);
                    calc_horizont(xh_,yh_,Fl::event_x(),Fl::event_y());
                    xh_=-1;
                    yh_=-1;
                    damage(FL_DAMAGE_ALL);
                    redraw();
                } else {
                    set_changed();
                    if (callback() && when()==FL_WHEN_CHANGED) {
                        do_callback();
                    }
                }
                return 1;
            case FL_PUSH:
                if (set_horizont_) {
                    window()->make_current();
                    erase_cross();
                    erase_box();
                    xsb_=ysb_=wsb_=hsb_=0;
                    xh_ = Fl::event_x();
                    yh_ = Fl::event_y();
                    button = Fl::event_button();
                    if (button==2) {
                        damage(FL_DAMAGE_ALL);
                        redraw();
                    }
                    return 1;
                } else if (xib_*yib_*wib_*hib_) {
                    xsb_=ysb_=wsb_=hsb_=0;
                    window()->make_current();
                    erase_cross();
                    erase_box();
                    ix = Fl::event_x();
                    if (ix<xib_) {
                        ix=xib_;
                    }
                    if (ix>=xib_+wib_) {
                        ix=xib_+wib_-1;
                    }
                    iy = Fl::event_y();
                    if (iy<yib_) {
                        iy=yib_;
                    }
                    if (iy>=yib_+hib_) {
                        iy=yib_+hib_-1;
                    }
                    dragged = 0;
                    button = Fl::event_button();
                    if (button==2) {
                        damage(FL_DAMAGE_ALL);
                        redraw();
                    }
                    return 1;
                }
                break;
            case FL_DRAG:
                if (set_horizont_) {
                    window()->make_current();
                    x2 = Fl::event_x();
                    if (x2<xib_) {
                        x2=xib_;
                    }
                    if (x2>=xib_+wib_) {
                        x2=xib_+wib_-1;
                    }
                    y2 = Fl::event_y();
                    if (y2<yib_) {
                        y2=yib_;
                    }
                    if (y2>=yib_+hib_) {
                        y2=yib_+hib_-1;
                    }
                    if (button != 1) {
                        erase_rubber_line(1);
                        return 1;
                    }
                    draw_rubber_line(xh_,yh_,x2,y2);
                    return 1;
                } else if (xib_*yib_*wib_*hib_) {
                    xsb_=ysb_=wsb_=hsb_=0;
                    window()->make_current();
                    // erase_box();
                    x2 = Fl::event_x();
                    if (x2<xib_) {
                        x2=xib_;
                    }
                    if (x2>=xib_+wib_) {
                        x2=xib_+wib_-1;
                    }
                    y2 = Fl::event_y();
                    if (y2<yib_) {
                        y2=yib_;
                    }
                    if (y2>=yib_+hib_) {
                        y2=yib_+hib_-1;
                    }
                    if (button != 1) {
                        ix = x2;
                        iy = y2;
                        erase_box();
                        return 1;
                    }
                    if (ix < x2) {
                        xsb_ = ix;
                        wsb_ = x2-ix;
                    } else {
                        xsb_ = x2;
                        wsb_ = ix-x2;
                    }
                    if (iy < y2) {
                        ysb_ = iy;
                        hsb_ = y2-iy;
                    } else {
                        ysb_ = y2;
                        hsb_ = iy-y2;
                    }
                    ::draw_box(xsb_,ysb_,wsb_,hsb_);
                    return 1;
                }
                break;
        }
    }
    return 0;
}
