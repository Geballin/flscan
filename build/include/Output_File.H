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

#ifndef FL_OUTPUT_FILE_H
#define FL_OUTPUT_FILE_H

#include <Threads.H>
#include <Image_Stream.H>
extern "C" {
    #define HAVE_PROTOTYPES
    #include <jpeglib.h>
    #include <png.h>
}

class OutputImageFile
{
private:
    void (*progressCb_)(int);
public:
    virtual ~OutputImageFile() {}
    virtual const char *filter()=0;
    virtual const char *extension()=0;
    virtual void set_quality(int quality)=0;
    virtual void set_smoothing(int smoothing)=0;
    virtual void set_optimization(int optimization)=0;
    virtual void set_out_file(const char *s)=0;
    virtual void begin_image(int w,int h,bool iscolor)=0;
    virtual void add_row(unsigned char *line)=0;
    virtual void flush_image()=0;
    void set_progressCb(void (*cb)(int));
    void progressCb(int prog);
};

class Output_JPEG_Thread: public Thread
{
private:
    char *scratchfile_;

public:
    Output_JPEG_Thread(char *filein);
    ~Output_JPEG_Thread();
    void thread();
};

class Output_JPEG: public OutputImageFile
{
private:
    char *outfile_;
    char scratchfile_[19];
    FILE *pfscratch_;
    // unsigned char *image_data_;
    int image_w_;
    int image_h_;
    // int image_pntr_;
    int image_iscolor_;
    int image_next_scanline_;
    int image_quality_;
    int image_smoothing_;
    int image_optimization_;

public:
    Output_JPEG();
    ~Output_JPEG();
    const char *filter();
    const char *extension();
    void begin_image(int w,int h,bool iscolor);
    void add_row(unsigned char *line);
    void flush_image();
    void set_quality(int quality);
    void set_smoothing(int smoothing);
    void set_optimization(int optimization);
    void set_out_file(const char *s);
};
class Output_PNG_Thread: public Thread
{
private:
    char *scratchfile_;

public:
    Output_PNG_Thread(char *filein);
    ~Output_PNG_Thread();
    void thread();
};

class Output_PNG: public OutputImageFile
{
private:
    char *outfile_;
    char scratchfile_[19];
    FILE *pfscratch_;
    // unsigned char *image_data_;
    int image_w_;
    int image_h_;
    // int image_pntr_;
    int image_iscolor_;
    int image_next_scanline_;
    int image_quality_;
    int image_smoothing_;
    int image_optimization_;

public:
    Output_PNG();
    ~Output_PNG();
    const char *filter();
    const char *extension();
    void begin_image(int w,int h,bool iscolor);
    void add_row(unsigned char *line);
    void flush_image();
    void set_quality(int quality);
    void set_smoothing(int smoothing);
    void set_optimization(int optimization);
    void set_out_file(const char *s);
};
class Output_File: public Image_Stream
{
private:
    int type_;
    OutputImageFile *formats_[1];

public:
    enum {
        JPEG=0,
        PNG,
        /* PNM,
        RAW,
        TIFF, */
        LAST_TYPE
    };
    Output_File();
    ~Output_File();
    void type(int t);
    OutputImageFile *format(int t);
    OutputImageFile *format();
    int type();
    const char *filter();
    const char *extension();
    void write(const char *outfile,int geom,void (*progressCb)(int));
};

extern void readJPEG (
    const char *name,
    bool (*read_cb)(unsigned char *,int,int,int,int)
);
extern void readPNG (
    const char *name,
    bool (*read_cb)(unsigned char *,int,int,int,int)
);

#endif /* FL_OUTPUT_FILE_H */
