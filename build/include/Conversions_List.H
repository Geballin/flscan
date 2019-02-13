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

#ifndef CONVERSIONS_LIST_H
#define CONVERSIONS_LIST_H

#include <FL/Flv_Table.H>

class Image_Conversion
{
private:
    char *filename_;
    int w_;
    int h_;
    int progress_;
    int visual_;
    char *message_;
    Image_Conversion* next_;
    Image_Conversion* prec_;
public:
    enum {
        PROGRESS=0,
        MESSAGE
    };
    Image_Conversion(char *fn,int wdt,int hgt);
    ~Image_Conversion();
    void progress(int i);
    void message(const char *msg);
    void visualize(int v) { visual_=(v>=PROGRESS && v<=MESSAGE)?v:PROGRESS; }
    const char *filename() { return (const char *)filename_; }
    int w() { return w_; }
    int h() { return h_; }
    int progress() { return progress_; }
    const char *message() { return message_; }
    char *column(int C);
    void next(Image_Conversion*n);
    void prec(Image_Conversion*p);
    Image_Conversion* next(void) { return next_; }
    Image_Conversion* prec(void) { return prec_; }
    void operator =(Image_Conversion&);
};
class Conversions_List: public Flv_Table
{
private:
    Image_Conversion *first;
    Image_Conversion *last;
    int items_;
public:
    Conversions_List(int X,int Y,int W,int H,const char *l=0);
    virtual void draw_cell(int Offset,int &X,int &Y,int &W,int &H,int R,int C);
    void get_style(Flv_Style &s,int R,int C);
    virtual int col_width(int C);
    int add(Image_Conversion *conv);
    Image_Conversion *get(int i);
    void replace(int i,Image_Conversion *);
    void redraw();
    const char *get_value(int R,int C);
};

#endif /* CONVERSIONS_LIST_H */
