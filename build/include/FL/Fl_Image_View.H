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

#ifndef FL_IMAGE_VIEW_H
#define FL_IMAGE_VIEW_H

#include <FL/x.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <Image_Stream.H>

class FL_EXPORT Fl_Image_View: public Fl_Widget, public Image_Stream
{
private:
    Fl_Offscreen idoffs_;

protected:
    void draw();

public:
    Fl_Image_View(int x,int y,int w,int h,char *l=0);
    ~Fl_Image_View();
    void end();
    void reset();
};

#endif /* FL_IMAGE_VIEW_H */
