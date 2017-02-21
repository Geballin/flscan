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

extern void FlScanUpdateDirectory(void);

void png_error_fn(png_structp data, png_const_charp errmsg)
{
    fprintf(stderr,"PNG ERROR: %s\n",errmsg);
}
void png_warning_fn(png_structp data, png_const_charp errmsg)
{
    fprintf(stderr,"PNG WARNING: %s\n",errmsg);
}

Output_PNG_Thread::Output_PNG_Thread(char *filein)
{
    if (filein) {
        scratchfile_=strdup(filein);
        if (scratchfile_) {
#ifdef THREADED_SAVING
            create();
#else
            thread();
#endif
        }
    }
}
Output_PNG_Thread::~Output_PNG_Thread()
{
    if (scratchfile_) {
        delete scratchfile_;
    }
    printf("Deleting Thread\n");
}
void Output_PNG_Thread::thread()
{
int w,h,iscolor,quality,rowlen;
unsigned char *data;
png_structp png_ptr;
png_infop info_ptr;
char outputfile[1024];
FILE *pfout,*pfscratch;
int sequence_number;
Image_Conversion *pconv;
int now,max,progress,last_progress;
char *filein;
png_color_8 sig_bit;

    FL_TOGGLE_THREAD();
    filein=0;
    FL_LOCK_BEGIN();
        if (scratchfile_ && scratchfile_[0]!='\0') {
            filein=strdup(scratchfile_);
        }
    FL_LOCK_END((void*)0);
    if (filein && (pfscratch=fopen(filein,"rb"))) {
        fscanf (
            pfscratch,
            "%d,%d,%d,%d,%d,%s\n",
            &w,&h,&iscolor,
            &quality,
            &sequence_number,
            outputfile
        );
        if ((pfout=fopen(outputfile,"wb"))) {
            if (
                (
                    png_ptr = png_create_write_struct (
                        PNG_LIBPNG_VER_STRING,
                        0,
                        png_error_fn,
                        png_warning_fn
                    )
                ) &&
                (
                    info_ptr = png_create_info_struct (
                        png_ptr
                    )
                )
            ) {
                png_init_io(png_ptr,pfout);
                png_set_IHDR (
                    png_ptr,
                    info_ptr,
                    w, h,
                    8, (iscolor) ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_GRAY,
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_BASE,
                    PNG_FILTER_TYPE_BASE
                );
                sig_bit.gray = 8;
                sig_bit.red = 8;
                sig_bit.green = 8;
                sig_bit.blue = 8;
                sig_bit.alpha = 0;
                png_set_sBIT(png_ptr,info_ptr,&sig_bit);
                /*- Optionally write comments into the image -*
                 text_ptr[0].key = "Title";
                 text_ptr[0].text = "Mona Lisa";
                 text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
                 text_ptr[1].key = "Author";
                 text_ptr[1].text = "Leonardo DaVinci";
                 text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
                 text_ptr[2].key = "Description";
                 text_ptr[2].text = "<long text>";
                 text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
                 png_set_text(png_ptr, info_ptr, text_ptr, 2);
                -*/
                png_write_info(png_ptr,info_ptr);
                rowlen=w*((iscolor)?3:1);
                if ((data = new unsigned char[rowlen])) {
                    now=0;
                    max=h;
//                    FL_LOCK_BEGIN();
//                        sequence_number=FlScanConversionArea->add (
//                            new Image_Conversion(outputfile,w,h)
//                        );
//                    FL_LOCK_END((void*)0);
                    while (
                        !feof(pfscratch) && 
                        now<max
                    ) {
                        fread(data,1,rowlen,pfscratch);
                        png_write_row(png_ptr,data);
                        now++;
                        progress=(int)((100*now)/max);
                        if (progress!=last_progress) {
                            FL_LOCK_BEGIN();
                                pconv=FlScanConversionArea->get (
                                    sequence_number
                                );
                                if (pconv) {
                                    pconv->progress(progress);
                                    pconv->visualize (
                                        Image_Conversion::PROGRESS
                                    );
                                    FlScanConversionArea->row(sequence_number);
                                    FlScanConversionArea->redraw();
                                }
                            FL_LOCK_END((void*)0);
                            last_progress=progress;
                        }
                    }
                    delete data;
                    FL_LOCK_BEGIN();
                        pconv=FlScanConversionArea->get(sequence_number);
                        if (pconv) {
                            pconv->progress(100);
                            pconv->message("Flushing");
                            pconv->visualize(Image_Conversion::MESSAGE);
                            FlScanConversionArea->row(sequence_number);
                            FlScanConversionArea->redraw();
                        }
                    FL_LOCK_END((void*)0);
                    png_write_end(png_ptr,info_ptr);
                }
                png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
                FL_LOCK_BEGIN();
                    pconv=FlScanConversionArea->get(sequence_number);
                    if (pconv) {
                        pconv->message("Completed");
                        pconv->visualize(Image_Conversion::MESSAGE);
                        FlScanConversionArea->row(sequence_number);
                        FlScanConversionArea->redraw();
                        FlScanUpdateDirectory();
                    }
                FL_LOCK_END((void*)0);
            } else {
                if (info_ptr) {
                    png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
                }
            }
            fclose(pfout);
        }
        fclose(pfscratch);
        unlink(filein);
    }
    FL_TOGGLE_THREAD();
}
Output_PNG::Output_PNG()
{
    FL_LOCK_BEGIN();
        scratchfile_[0]='\0';
    FL_LOCK_END((void*)0);
    pfscratch_=0;
    image_w_=0;
    image_h_=0;
    image_iscolor_=0;
    image_next_scanline_=0;
    image_quality_=9;
    image_smoothing_=0;
    image_optimization_=0;
    outfile_=0;
    // image_data_=0;
    // image_pntr_=0;
}
Output_PNG::~Output_PNG()
{
    if (outfile_) {
        delete outfile_;
    }
    outfile_=0;
    // if (image_data_) {
        // delete image_data_;
    // }
    // image_data_=0;
}
char *Output_PNG::filter()
{
    return "*.png";
}
char *Output_PNG::extension()
{
    return ".png";
}
void Output_PNG::begin_image(int w,int h,bool iscolor)
{
int fscratch, sequence_number;
Image_Conversion *pconv;

    FL_LOCK_BEGIN();
        strcpy(scratchfile_,"Output_PNG_XXXXXX");
        fscratch=mkstemp(scratchfile_);
    FL_LOCK_END((void*)0);
    if (
        fscratch && 
        (pfscratch_=fdopen(fscratch,"w+")) &&
        outfile_ && outfile_[0]
    ) {
        image_w_=w;
        image_h_=h;
        image_iscolor_=iscolor;
        image_next_scanline_=0;
        FL_LOCK_BEGIN();
            sequence_number=FlScanConversionArea->add (
                new Image_Conversion(outfile_,w,h)
            );
            pconv=FlScanConversionArea->get(sequence_number);
            if (pconv) {
                pconv->message("Dumping");
                pconv->visualize(Image_Conversion::MESSAGE);
                FlScanConversionArea->row(sequence_number);
                FlScanConversionArea->redraw();
            }
        FL_LOCK_END((void*)0);
        fprintf (
            pfscratch_,
            "%d,%d,%d,%d,%d,%s\n",
            w,h,iscolor,
            image_quality_,
            sequence_number,
            outfile_
        );
    }
}
void Output_PNG::add_row(unsigned char *line)
{
//static int last_progress=0;
int progress;
int rowlen;

    if (pfscratch_ && line /* && image_data_ */) {
        rowlen=image_w_*((image_iscolor_)?3:1);
        if (image_next_scanline_<image_h_) {
            fwrite(line,1,rowlen,pfscratch_);
            // memcpy((void*)(image_data_+image_pntr_),(void*)line,rowlen);
            // image_pntr_+=rowlen;
        }
        image_next_scanline_++;
        progress=(int)((100*image_next_scanline_)/image_h_);
        progressCb(progress);
//        if (progress!=last_progress) {
//            progressCb(progress);
//            last_progress=progress;
//        }
//    } else {
//        last_progress=0;
    }
}
void Output_PNG::flush_image()
{
    if (pfscratch_) {
        fclose(pfscratch_);
        new Output_PNG_Thread(scratchfile_);
    }
}
void Output_PNG::set_quality(int quality)
{
    image_quality_=quality;
}
void Output_PNG::set_smoothing(int smoothing)
{
    image_smoothing_=smoothing;
}
void Output_PNG::set_optimization(int optimization)
{
    image_optimization_=optimization;
}
void Output_PNG::set_out_file(const char *s)
{
    if (outfile_) {
        delete outfile_;
    }
    outfile_=0;
    if (s) {
        outfile_=strdup(s);
    }
}
void readPNG (
    const char *name,
    bool (*read_cb)(unsigned char *,int,int,int,int)
) {
png_structp png_ptr;
png_infop info_ptr;
png_uint_32 width, height;
int bit_depth, color_type;
FILE *infile;
int iscolor;

    if (read_cb) {
        if (!name || !(infile=fopen(name,"rb"))) {
            fprintf(stderr, "can't open %s\n", name);
            return;
        }
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,0,0);
        if (png_ptr == NULL) {
            fclose(infile);
            return;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_read_struct(&png_ptr,NULL,NULL);
            fclose(infile);
            return;
        }
        png_init_io(png_ptr,infile);
        png_read_info(png_ptr,info_ptr);
        png_get_IHDR (
            png_ptr,
            info_ptr,
            &width,&height,
            &bit_depth,&color_type,
            NULL,NULL,NULL
        );
        if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8) {
            png_set_expand(png_ptr);
        }
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand(png_ptr);
        }
        if (bit_depth == 16) {
            png_set_strip_16(png_ptr);
        }
        if (bit_depth < 8) {
            png_set_packing(png_ptr);
        }
        png_read_update_info(png_ptr,info_ptr);
        png_get_IHDR (
            png_ptr,
            info_ptr,
            &width,&height,
            &bit_depth,&color_type,
            NULL,NULL,NULL
        );
        iscolor = (
            color_type!=PNG_COLOR_TYPE_GRAY       &&
            color_type!=PNG_COLOR_TYPE_GRAY_ALPHA
        );
        read_cb(NULL,width,bit_depth,iscolor,0);
        int  rowbytes = png_get_rowbytes(png_ptr,info_ptr);
        unsigned char *ibuffer = new unsigned char[rowbytes];
        if (ibuffer) {
            for (int i=height; i--; ) {
                png_read_row(png_ptr,ibuffer,NULL);
                read_cb(
                    ibuffer,
                    width,rowbytes,
                    iscolor,
                    1
                );
            }
            png_read_end(png_ptr,NULL);
            delete ibuffer;
        }
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        read_cb(NULL,0,0,0,2);
        fclose(infile);
    }
}
