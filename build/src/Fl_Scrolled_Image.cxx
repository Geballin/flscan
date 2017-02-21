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

#include <FL/Fl_Scrolled_Image.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Fl_Scrolled_Image::Fl_Scrolled_Image(int x,int y,int w,int h,char *l)
:Fl_Scroll(x,y,w,h,l)
{
}
int Fl_Scrolled_Image::handle(int e)
{
static int old_x, old_y;
int cur_x = Fl::event_x()-x();
int cur_y = Fl::event_y()-y();
int new_x, new_y, max_w, max_h;

    switch (e) {
        case FL_PUSH:
                if (
                    ( !scrollbar.visible() ||
                      Fl::event_x() < ( x() + w() - scrollbar.w() )
                    ) &&
                    ( !hscrollbar.visible() ||
                      Fl::event_y() < ( y() + h() - hscrollbar.h() )
                    )
                ) {
                    old_x = cur_x;
                    old_y = cur_y;
                    return 1;
                }
            break;
        case FL_DRAG:
                if (scrollbar.visible() || hscrollbar.visible()) {
                    // accumulate bounding box of children:
                    int l, r, t, b;
                    l = r = x()+Fl::box_dx(box());
                    t = b = y()+Fl::box_dy(box());
                    Fl_Widget*const* a = array();
                    for (int i=children()-2; i--;) {
                        Fl_Object* o = *a++;
                        if (o->x() < l) l = o->x();
                        if (o->y() < t) t = o->y();
                        if (o->x()+o->w() > r) r = o->x()+o->w();
                        if (o->y()+o->h() > b) b = o->y()+o->h();
                    }
                    max_w = r - l;
                    max_w -= w();
                    max_w += (hscrollbar.visible()) ? hscrollbar.h() : 0;

                    new_x = xposition()+old_x-cur_x;
                    new_x = ( new_x > max_w ) ? max_w : new_x;
                    new_x = ( new_x < 0 ) ? 0 : new_x;

                    max_h = b - t;
                    max_h -= h();
                    max_h += (scrollbar.visible()) ? scrollbar.w() : 0;

                    new_y = yposition()+old_y-cur_y;
                    new_y = ( new_y > max_h ) ? max_h : new_y;
                    new_y = ( new_y < 0 ) ? 0 : new_y;

                    position(new_x,new_y);

                    old_x = cur_x;
                    old_y = cur_y;
                    return 1;
                }
            break;
        default:
            break;
    }
    return Fl_Scroll::handle(e);
}
