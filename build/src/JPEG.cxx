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
#include <Conversions_List.H>

#include <sys/mman.h>

extern void FlScanUpdateDirectory(void);

Output_JPEG_Thread::Output_JPEG_Thread(char *filein)
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
Output_JPEG_Thread::~Output_JPEG_Thread()
{
    if (scratchfile_) {
      free(scratchfile_);
    }
    printf("Deleting Thread\n");
}
void Output_JPEG_Thread::thread()
{
int w,h,iscolor,quality,smoothing,optimization,rowlen;
unsigned char *data;
struct jpeg_compress_struct cinfo_;
struct jpeg_error_mgr jerr_;
JSAMPROW row_pointer[1];
char outputfile[1024];
FILE *pfout,*pfscratch;
int sequence_number;
Image_Conversion *pconv;
int now,max,progress,last_progress;
char *filein;

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
            "%d,%d,%d,%d,%d,%d,%d,%s\n",
            &w,&h,&iscolor,
            &quality,
            &smoothing,
            &optimization,
            &sequence_number,
            outputfile
        );
        if ((pfout=fopen(outputfile,"wb"))) {
            cinfo_.err = jpeg_std_error(&jerr_);
            jpeg_create_compress(&cinfo_);
            jpeg_stdio_dest(&cinfo_,pfout);
            cinfo_.image_width = w;
            cinfo_.image_height = h;
            if (iscolor) {
                cinfo_.input_components = 3;
                cinfo_.in_color_space = JCS_RGB;
            } else {
                cinfo_.input_components = 1;
                cinfo_.in_color_space = JCS_GRAYSCALE;
            }
            rowlen=w*cinfo_.input_components;
            jpeg_set_defaults(&cinfo_);
            jpeg_set_quality(&cinfo_,quality,TRUE);
            cinfo_.smoothing_factor = smoothing;
            cinfo_.optimize_coding = optimization;
            jpeg_start_compress(&cinfo_,TRUE);
            if ((data = new unsigned char[rowlen])) {
                now=0;
                max=cinfo_.image_height;
//                FL_LOCK_BEGIN();
//                    sequence_number=FlScanConversionArea->add (
//                        new Image_Conversion(outputfile,w,h)
//                    );
//                FL_LOCK_END((void*)0);
                while (
                    !feof(pfscratch) && 
                    now<max
                ) {
                    fread(data,1,rowlen,pfscratch);
                    row_pointer[0] = data;
                    jpeg_write_scanlines(&cinfo_,row_pointer,1);
                    now=cinfo_.next_scanline;
                    progress=(int)((100*now)/max);
                    if (progress!=last_progress) {
                        FL_LOCK_BEGIN();
                            pconv=FlScanConversionArea->get(sequence_number);
                            if (pconv) {
                                pconv->progress(progress);
                                pconv->visualize(Image_Conversion::PROGRESS);
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
                jpeg_finish_compress(&cinfo_);
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
            }
            jpeg_destroy_compress(&cinfo_);
        }
        fclose(pfscratch);
        unlink(filein);
    }
    FL_TOGGLE_THREAD();
}
Output_JPEG::Output_JPEG()
{
    FL_LOCK_BEGIN();
        scratchfile_[0]='\0';
    FL_LOCK_END((void*)0);
    pfscratch_=0;
    image_w_=0;
    image_h_=0;
    image_iscolor_=0;
    image_next_scanline_=0;
    image_quality_=100;
    image_smoothing_=100;
    image_optimization_=1;
    outfile_=0;
    // image_data_=0;
    // image_pntr_=0;
}
Output_JPEG::~Output_JPEG()
{
    if (outfile_) {
      free(outfile_);
    }
    outfile_=0;
    // if (image_data_) {
        // delete image_data_;
    // }
    // image_data_=0;
}
const char *Output_JPEG::filter()
{
    return "*.{jpg|jpeg}";
}
const char *Output_JPEG::extension()
{
    return ".jpg";
}
void Output_JPEG::begin_image(int w,int h,bool iscolor)
{
int fscratch, sequence_number;
Image_Conversion *pconv;

    FL_LOCK_BEGIN();
        strcpy(scratchfile_,"Output_JPEG_XXXXXX");
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
            "%d,%d,%d,%d,%d,%d,%d,%s\n",
            w,h,iscolor,
            image_quality_,
            image_smoothing_,
            image_optimization_,
            sequence_number,
            outfile_
        );
/******************************************************************************
        // if (image_data_) {
            // delete image_data_;
        // }
        // image_data_=new unsigned char[w*((iscolor)?3:1)*h];
        image_data_=0;
        image_pntr_=0;
        image_data_ = (unsigned char *)mmap (
            (void *)image_data_,
            w*((iscolor)?3:1)*h,
            PROT_WRITE,
            MAP_PRIVATE,
            fscratch,
            0
        );
 *****************************************************************************/
    }
}
void Output_JPEG::add_row(unsigned char *line)
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
void Output_JPEG::flush_image()
{
    if (pfscratch_) {
/******************************************************************************
        int image_size;
        image_size=image_w_*((image_iscolor_)?3:1)*image_h_;
        // fwrite(image_data_,1,image_size,pfscratch_);
        // delete image_data_;
        // image_data_=0;
        msync(image_data_,image_size,MS_SYNC);
        munmap(image_data_,image_size);
 *****************************************************************************/
        fclose(pfscratch_);
        new Output_JPEG_Thread(scratchfile_);
    }
}
void Output_JPEG::set_quality(int quality)
{
    image_quality_=quality;
}
void Output_JPEG::set_smoothing(int smoothing)
{
    image_smoothing_=smoothing;
}
void Output_JPEG::set_optimization(int optimization)
{
    image_optimization_=optimization;
}
void Output_JPEG::set_out_file(const char *s)
{
    if (outfile_) {
      free(outfile_);
    }
    outfile_=0;
    if (s) {
        outfile_=strdup(s);
    }
}
void readJPEG (
    const char *name,
    bool (*read_cb)(unsigned char *,int,int,int,int)
) {
struct jpeg_decompress_struct cinfo;
struct jpeg_error_mgr jerr;
FILE *infile;
long width,height,components;

    if (read_cb) {
        if (!name || !(infile=fopen(name,"rb"))) {
            fprintf(stderr, "can't open %s\n", name);
            return;
        }
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, infile);
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);
        width=cinfo.output_width;
        components=cinfo.output_components;
        read_cb(NULL,width,8,(components==3),0);
        height = cinfo.output_height;
        unsigned char *ibuffer = new unsigned char[width*components];
        if (ibuffer) {
            for (int i=height; i--; ) {
                jpeg_read_scanlines(&cinfo,&ibuffer,1);
                read_cb(
                    ibuffer,
                    width,width*components,
                    (components==3),
                    1
                );
            }
            jpeg_finish_decompress(&cinfo);
            delete ibuffer;
        }
        jpeg_destroy_decompress(&cinfo);
        read_cb(NULL,0,0,0,2);
        fclose(infile);
    }
}
