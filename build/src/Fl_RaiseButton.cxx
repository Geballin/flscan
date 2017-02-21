//
// "$Id: Fl_RaiseButton.cxx,v 1.4.2.6.2.10 2002/01/01 15:11:30 easysw Exp $"
//
// Button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <FL/Fl.H>
#include <FL/Fl_RaiseButton.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>

void Fl_RaiseButton::hilighted(uchar onoff) {
  if (onoff) {
    if (!hilighted_) {
      unhilighted_box(box());
      unhilighted_color(color());
      box(hilighted_box());
      color(hilighted_color());
      hilighted_=true;
    }
  } else {
    if (hilighted_) {
      box(unhilighted_box());
      color(unhilighted_color());
      hilighted_=false;
    }
  }
}
void Fl_RaiseButton::draw() {
  if (type() == FL_HIDDEN_BUTTON) return;
  Fl_Color col = value() ? selection_color() : color();
//if (col == FL_GRAY && Fl::belowmouse()==this) col = FL_LIGHT1;
  draw_box(value() ? (down_box()?down_box():fl_down(box())) : box(), col);
  draw_label();
  if (Fl::focus() == this) draw_focus();
}
int Fl_RaiseButton::handle(int event) {
  int newval;
  switch (event) {
  case FL_ENTER:
  case FL_LEAVE:
    if (hilighted_box()) {
        hilighted((event==FL_ENTER)?true:false);
        redraw();
    }
    return 1;
  default:
    return this->Fl_Button::handle(event);
  }
}

Fl_RaiseButton::Fl_RaiseButton(int x,int y,int w,int h, const char *l)
: Fl_Button(x,y,w,h,l) {
  hilighted_ = false;
  unhilighted_box_ = 0;
  hilighted_box_ = 0;
  unhilighted_color_ = color();
  hilighted_color_ = color();
}

//
// End of "$Id: Fl_RaiseButton.cxx,v 1.4.2.6.2.10 2002/01/01 15:11:30 easysw Exp $".
//
