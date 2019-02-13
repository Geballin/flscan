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

#include <Conversions_List.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <string.h>

void Image_Conversion::operator =(Image_Conversion& i)
{
    if (filename_) {
      free(filename_);
    }
    filename_=nullptr;
    if (i.filename()) {
        filename_=strdup(i.filename());
    }
    w_ = i.w();
    h_ = i.h();
    progress_ = i.progress();
}

Image_Conversion::Image_Conversion(char *fn,int wdt,int hgt)
{
    filename_=0;
    if (fn) {
        filename_=strdup(fn);
    }
    w_=wdt;
    h_=hgt;
    message_=nullptr;
    progress_=0;
    next_=nullptr;
    prec_=nullptr;
}

Image_Conversion::~Image_Conversion()
{
    if (filename_) {
      free(filename_);
    }
    if (message_) {
      free(message_);
    }
}

void Image_Conversion::next(Image_Conversion*n)
{
    next_=n;
}

void Image_Conversion::prec(Image_Conversion*p)
{
    prec_=p;
}

void Image_Conversion::progress(int i)
{
    progress_=(i<0)?0:(i>100)?100:i;
}

void Image_Conversion::message(const char *msg)
{
    if (message_) {
      free(message_);
    }
    message_ = nullptr;
    if (msg) {
        message_ = strdup(msg);
    }
}

char *Image_Conversion::column(int C)
{
static char scolumn[256];

    switch (C) {
        case 0:
            strncpy(scolumn,filename_,255);
            break;
        case 1:
            sprintf(scolumn,"%dx%d",w_,h_);
            break;
        case 2:
            if (visual_==MESSAGE && message_) {
                snprintf(scolumn,255,"%s",message_);
            } else {
                sprintf(scolumn,"%d%%",progress_);
            }
            break;
        default:
            break;
    }
    scolumn[255]='\0';
    return scolumn;
}

int Conversions_List::add(Image_Conversion *conv)
{
    if (!first) {
        first=conv;
    } else {
        last->next(conv);
        conv->prec(last);
    }
    last=conv;
    rows(rows()+1);
    return rows()-1;
}

Image_Conversion *Conversions_List::get(int R)
{
int i;
Image_Conversion *cnv,*cnv1,*cnvf;

    i=0;
    cnvf=nullptr;
    cnv=first;
    while (cnv) {
        if (i==R) {
            cnvf=cnv;
            break;
        }
        cnv1=cnv->next();
        cnv=cnv1;
        i++;
    }
    return cnvf;
}

static const char *header[3] = {
    "File Name",
    "Image Size",
    "Progress"
};

const char *Conversions_List::get_value(int R,int C)
{
Image_Conversion *cnv;
const char *value=nullptr;

    if (R==-1) {
        value=header[C];
    } else if (R<rows() && (cnv=get(R))) {
        value=cnv->column(C);
    }
    return value;
}

Conversions_List::Conversions_List(int X,int Y,int W,int H,const char *l)
:Flv_Table(X,Y,W,H,l)
{
    first=nullptr;
    last=nullptr;
    //    row height (17) * rows (10) +
    //    row height headers (17+4) * number of headers (3) +
    //    Top/Bottom box margins (2)
    cols(3);
    has_scrollbar(FLVS_VERTICAL);
    feature(FLVF_ROW_HEADER|FLVF_ROW_SELECT);
    global_style.align(FL_ALIGN_LEFT);
    global_style.height(17);
    col_style[1].align(FL_ALIGN_CENTER);
}

void Conversions_List::get_style( Flv_Style &s, int R, int C )
{
    Flv_Table::get_style(s,R,C);            //  Get standard style
    if (R<0) {                              //  Heading/Footing is bold
        s.font((Fl_Font)(s.font()+FL_BOLD));
        s.background(FL_GRAY);              //  Black background
    }
    if (R==-2) {                            //  Row footer exception
        s.background(FL_BLACK);             //  Black background
        s.foreground(FL_WHITE);             //  White text
        s.frame(FL_FLAT_BOX);               //  No box
        s.align(FL_ALIGN_RIGHT);            //  Right aligned
    }
}

void Conversions_List::draw_cell (
    int Offset,
    int &X,int &Y,
    int &W,int &H,
    int R,int C
) {
Flv_Style s;

    get_style(s,R,C);
    Flv_Table::draw_cell(Offset,X,Y,W,H,R,C);
    fl_draw(get_value(R,C),X-Offset,Y,W,H,s.align());
}

static int cw[10];  //  Column width
//  Another way would be to override handle for FL_SIZE.  We could also
//  spend a lot of time setting styles for the columns and returning
//  that, but since we're calculating it anyway, why bother.
int Conversions_List::col_width(int C)
{
static int LW=-1;
int scrollbar_width = (scrollbar.visible()?scrollbar.w():0);
int W = w()- scrollbar_width-1;
int ww, t;

    //  Either always calculate or be sure to check that width
    //  hasn't changed.
    if (W!=LW) {                //  Width change, recalculate
        cw[1] = (W*20)/100;     //  Image Size
        cw[2] = (W*15)/100;     //  Conversion Progress
        for (ww=0,t=1;t<3;t++) {
            ww += cw[t];
        }
        cw[0] = W-ww-1;                 //  ~30% +/- previous rounding errors
        LW = W;
    }
    return cw[C];
}
void Conversions_List::redraw()
{
    if (visible_r()) {
        this->Flv_Table::redraw();
    }
}
