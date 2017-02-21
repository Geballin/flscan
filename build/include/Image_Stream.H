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
#ifndef IMAGE_STREAM_H
#define IMAGE_STREAM_H

#define MagickEpsilon  1.0e-12
#define MagickPI  3.14159265358979323846264338327950288419716939937510
#define Max(x,y)  (((x) > (y)) ? (x) : (y))
#define Min(x,y)  (((x) < (y)) ? (x) : (y))

extern double Box(double x);
extern double Bessel(double x);
extern double Blackman(double x);
extern double Catrom(double x);
extern double Cubic(double x);
extern double Gaussian(double x);
extern double Hanning(double x);
extern double Hamming(double x);
extern double Hermite(double x);
extern double Sinc(double x);
extern double Lanczos(double x);
extern double Mitchell(double x);
extern double Quadratic(double x);
extern double Triangle(double x);

typedef struct _ContributionInfo
{
  union {
      int    pixel;
      int    x_offset;
  };
  int    y_offset;
  double weight;
} ContributionInfo;

typedef struct _FilterInfo
{
    double (*function)(double);
    double support;
} FilterInfo;

static const FilterInfo filters_[] = {
    { Box      , 0.0    },
    { Box      , 0.0    },
    { Box      , 0.5    },
    { Triangle , 1.0    },
    { Hermite  , 1.0    },
    { Hanning  , 1.0    },
    { Hamming  , 1.0    },
    { Blackman , 1.0    },
    { Gaussian , 1.25   },
    { Quadratic, 1.5    },
    { Cubic    , 2.0    },
    { Catrom   , 2.0    },
    { Mitchell , 2.0    },
    { Lanczos  , 3.0    },
    { Bessel   , 3.2383 },
    { Sinc     , 4.0    }
};

typedef double  Vector[3];
typedef Vector  Matrix[3];
typedef long    IVector[3];
typedef IVector IMatrix[3];

class Image_Stream
{
private:
    unsigned char *imst_data_;
    int imst_x_;
    int imst_y_;
    int imst_w_;
    int imst_h_;
    int imst_bpp_;
    int imst_offset_;
    bool imst_color_;
    int imst_depth_;

    Matrix mat_;
    Matrix old_mat_;
    Matrix undo_mat_;

    IMatrix imat_;
    IMatrix old_imat_;
    IMatrix undo_imat_;

    int filter_;
    double blur_;

    double clip_x_;
    double clip_y_;
    double clip_w_;
    double clip_h_;

    long old_dst_w_;
    long old_dst_h_;
    int old_buf_size_;
    int last_frame_;

    bool update_height();
    unsigned char *get_pixel(long x,long y);

public:
    enum {
        FRAME_GRAY=0,
        FRAME_RGB,
        FRAME_RED,
        FRAME_GREEN,
        FRAME_BLUE
    };
    enum {
        UNDEFINED_FILTER=0,
        POINT_FILTER,
        BOX_FILTER,
        TRIANGLE_FILTER,
        HERMITE_FILTER,
        HANNING_FILTER,
        HAMMING_FILTER,
        BLACKMAN_FILTER,
        GAUSSIAN_FILTER,
        QUADRATIC_FILTER,
        CUBIC_FILTER,
        CATROM_FILTER,
        MITCHELL_FILTER,
        LANCZOS_FILTER,
        BESSEL_FILTER,
        SINC_FILTER
    };
    Image_Stream();
    Image_Stream(Image_Stream& i);
    ~Image_Stream();
    bool begin(int image_width,int bit_depth,bool iscolor);
    void rewind();
    bool add_row_data(unsigned char *row_data,int data_len,int frame);
    void reset();
    const double ** matrix() { return (const double **)mat_; }
    void matrix(const Matrix m);
    void matrix(const double**);
    unsigned char *data() { return imst_data_; }
    int depth() { return imst_depth_; }
    bool iscolor() { return (imst_color_==1); }
    void measure(int& ww,int &hh) { ww=imst_w_; hh=imst_h_; }
    bool serialize (
        unsigned char **out,
        int out_w,
        int out_h,
        void (*begin_image)(int width,int height,bool iscolor),
        void (*add_row)(unsigned char *buf)
    );
    void undo_rotate();
    void rotate(double angle);
    void rotate_cw();
    void rotate_ccw();
    void flip_vertical();
    void flip_horizontal();
    void clip(double x,double y,double w,double h);
    void filter(int f) { filter_=(f>=0 && f<=SINC_FILTER)?f:UNDEFINED_FILTER; }
    int filter() { return filter_; }
    double blur() { return blur_; }
    void blur(double b) { blur_=b; }
};

#endif /* IMAGE_STREAM_H */
