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

#ifndef FL_IMAGE_PREVIEW_H
#define FL_IMAGE_PREVIEW_H

#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <Image_Stream.H>

class Fl_Image_Preview: public Fl_Widget, public Image_Stream
{
private:
    int newcols_;
    int newrows_;
    unsigned char *preview_data_;
    int off_screen_id_;

    bool set_horizont_;
    Fl_Button *set_horizont_tb_;

    int xib_;
    int yib_;
    int wib_;
    int hib_;

    int xsb_;
    int ysb_;
    int wsb_;
    int hsb_;

    int xh_;
    int yh_;

    int imgv_w_;
    int imgv_h_;
    bool imgv_color_;

    bool refresh_;

    void draw_image();
    void transform(double rot, bool v_flip, bool h_flip);

protected:
    FL_EXPORT void draw();

public:
    unsigned char *row_buffer_;

    FL_EXPORT Fl_Image_Preview(int x,int y,int w,int h,char *l=0);
    ~Fl_Image_Preview();
    void undo_rotate();
    void calc_horizont(int x1,int y1,int x2,int y2);
    void set_horizont(bool set,Fl_Button *act);
    int handle(int event);
    bool begin(int image_width,int bit_depth,bool iscolor);
    void end();
    void reset();
    void selection(double& sx,double& sy,double& sw,double& sh);
    void rotate(double angle);
    void rotate_cw();
    void rotate_ccw();
    void flip_vertical();
    void flip_horizontal();
    void begin_image(int width,int height,bool iscolor);
    void add_row(unsigned char *buf);
};

#endif /* FL_IMAGE_PREVIEW_H */
