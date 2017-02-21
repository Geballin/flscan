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

#ifndef FL_SCROLLED_IMAGE_H
#define FL_SCROLLED_IMAGE_H

#include <FL/Fl_Scroll.H>

class FL_EXPORT Fl_Scrolled_Image: public Fl_Scroll
{
public:
    Fl_Scrolled_Image(int x,int y,int w,int h,char *l=0);
    int handle(int);
};

#endif /* FL_SCROLLED_IMAGE_H */
