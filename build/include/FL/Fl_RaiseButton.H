//
// "$Id: Fl_RaiseButton.H,v 1.5.2.3.2.3 2002/01/01 15:11:27 easysw Exp $"
//
// Button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_RaiseButton_H
#define Fl_RaiseButton_H

#include <FL/Fl_Button.H>

class Fl_RaiseButton : public Fl_Button {

private:
  uchar hilighted_;
  uchar hilighted_box_;
  uchar hilighted_color_;
  uchar unhilighted_box_;
  uchar unhilighted_color_;

  Fl_Boxtype unhilighted_box() const {return (Fl_Boxtype)unhilighted_box_;}
  void unhilighted_box(Fl_Boxtype b) {unhilighted_box_ = b;}
  Fl_Color unhilighted_color() const {return (Fl_Color)unhilighted_color_;}
  void unhilighted_color(uchar c) {unhilighted_color_ = c;}

public:
  void hilighted(uchar onoff);
  Fl_Boxtype hilighted_box() const {return (Fl_Boxtype)hilighted_box_;}
  void hilighted_box(Fl_Boxtype b) {hilighted_box_ = b;}
  Fl_Color hilighted_color() const {return (Fl_Color)hilighted_color_;}
  void hilighted_color(uchar c) {hilighted_color_ = c;}

protected:

  virtual FL_EXPORT void draw();

public:

  virtual FL_EXPORT int handle(int);
  FL_EXPORT Fl_RaiseButton(int,int,int,int,const char * = 0);
};

#endif

//
// End of "$Id: Fl_RaiseButton.H,v 1.5.2.3.2.3 2002/01/01 15:11:27 easysw Exp $".
//
