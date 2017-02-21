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

#include <flscan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Output_File.H>

static OutputImageFile *currentOutputImageFile=0;

static void Output_File_setCurrentOutputImageFile(OutputImageFile *ip)
{
    currentOutputImageFile=ip;
}
static void Output_File_begin_image(int w,int h,bool iscolor)
{
    if (currentOutputImageFile) {
        currentOutputImageFile->begin_image(w,h,iscolor);
    }
}
static void Output_File_add_row(unsigned char *buf)
{
    if (currentOutputImageFile) {
        currentOutputImageFile->add_row(buf);
    }
}

Output_File::Output_File():Image_Stream()
{
    formats_[JPEG] = new Output_JPEG();
    formats_[PNG] = new Output_PNG();
}
Output_File::~Output_File()
{
}
void Output_File::type(int t)
{
    type_=(t>=0 && t<Output_File::LAST_TYPE)?t:Output_File::JPEG;
}
OutputImageFile *Output_File::format(int t)
{
    if (t>=0 && t<Output_File::LAST_TYPE) {
        return formats_[t];
    }
    return 0;
}
OutputImageFile *Output_File::format()
{
    return formats_[type_];
}
int Output_File::type()
{
    return type_;
}
const char *Output_File::filter()
{
    if (formats_[type_]) {
        return formats_[type_]->filter();
    }
    return 0;
}
const char *Output_File::extension()
{
    if (formats_[type_]) {
        return formats_[type_]->extension();
    }
    return 0;
}
void Output_File::write(const char *outfile,int geom,void (*progressCb)(int))
{
unsigned char *rowbuff=0;
FILE *pf;

    pf=0;
    if (formats_[type_]) {
        formats_[type_]->set_progressCb(progressCb);
        formats_[type_]->set_out_file(outfile);
        Output_File_setCurrentOutputImageFile(formats_[type_]);
        Output_File_add_row(0);
        serialize (
            &rowbuff,
            geom,geom,
            Output_File_begin_image,
            Output_File_add_row
        );
        // reset();
        formats_[type_]->flush_image();
    }
    if (rowbuff) {
        delete rowbuff;
    }
    rowbuff=0;
}
void OutputImageFile::set_progressCb(void (*progressCb)(int))
{
    progressCb_=progressCb;
}
void OutputImageFile::progressCb(int prog)
{
    if (progressCb_) {
        progressCb_(prog);
    }
}
