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

#include <FL/Fl_Image_View.H>
#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>
#include <FL/x.H>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void Fl_Image_View::draw()
{
int ww,hh,bpp;
unsigned char *image;
int X,Y,W,H,cx,cy,XP=0,YP=0,WP=0,HP=0;

    draw_box();
    if ((image=data())) {
        measure(ww,hh);
        if (ww>0 && hh>0) {
            XP=x(); YP=y(); WP=ww; HP=hh;
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
            if (!idoffs_) {
                idoffs_ = fl_create_offscreen(ww,hh);
                fl_begin_offscreen(idoffs_);
                bpp=depth();
                if (iscolor()) {
                    fl_draw_image(image,0,0,ww,hh,bpp,ww*bpp);
                } else {
                    fl_draw_image_mono(image,0,0,ww,hh,bpp,ww*bpp);
                }
                fl_end_offscreen();
            }
            fl_copy_offscreen(X,Y,W,H,idoffs_,cx,cy);
        }
    }
}
Fl_Image_View::Fl_Image_View(int x,int y,int w,int h,char *l)
:Fl_Widget(x,y,w,h,0),Image_Stream()
{
    idoffs_ = 0;
    box(FL_FLAT_BOX);
}
Fl_Image_View::~Fl_Image_View()
{
    fl_delete_offscreen(idoffs_);
}
void Fl_Image_View::end()
{
int ww,hh;
unsigned char *image;

    if (idoffs_) {
        fl_delete_offscreen(idoffs_);
    }
    idoffs_=0;
    if ((image=data())) {
        measure(ww,hh);
        size(ww,hh);
    }
    damage(FL_DAMAGE_ALL);
    redraw();
}
void Fl_Image_View::reset()
{
    if (idoffs_) {
        fl_delete_offscreen(idoffs_);
    }
    idoffs_=0;
    this->Image_Stream::reset();
    damage(FL_DAMAGE_ALL);
    draw();
}
