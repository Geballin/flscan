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

#include <Image_Stream.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_BUFF_ROWS 1

static void transform_point(Matrix,double,double,double&,double&);
#ifdef USE_INTEGER
static void transform_point(IMatrix,long,long,long&,long&);
#endif
static void matrix_bounds(Matrix,double&,double&,double&,double&);
static void mult_matrix(const Matrix,Matrix);
static void identity_matrix(Matrix);
static void translate_matrix(Matrix,double,double);
static void scale_matrix(Matrix,double,double);
static void rotate_matrix(Matrix,double);
static double determinate(Matrix);
static void cofactor(Matrix,Matrix);
static void invert(Matrix,Matrix);

static unsigned char black[3]={0x00,0x00,0x00};

static void transform_point(Matrix m,double x,double y,double& nx,double& ny)
{
double xx, yy, ww;

    xx = m[0][0] * x + m[0][1] * y + m[0][2];
    yy = m[1][0] * x + m[1][1] * y + m[1][2];
    ww = m[2][0] * x + m[2][1] * y + m[2][2];

    if (!ww) {
        ww = 1.0;
    }

    nx = xx / ww;
    ny = yy / ww;
}

#ifdef USE_INTEGER
static void transform_point(IMatrix m,long x,long y,long& nx,long& ny)
{
long xx, yy, ww;

    xx = m[0][0] * x + m[0][1] * y + m[0][2];
    yy = m[1][0] * x + m[1][1] * y + m[1][2];
#if 0
    ww = m[2][0] * x + m[2][1] * y + m[2][2];
    if (!ww) {
        ww = 1;
    }
    nx = xx / ww;
    ny = yy / ww;
#else
    ww = m[2][2];
    nx = xx >> ww;
    ny = yy >> ww;
#endif
}
#endif

static int matrix_cmp(const void *a, const void *b)
{
double c,d;

    c=*(double*)a;
    d=*(double*)b;
    return (c<d)?-1:(c>d)?+1:0;
}

static void matrix_bounds(Matrix m,double& mx,double& my,double& xm,double& ym)
{
double p[2][4];

    p[0][0] = p[0][2] = mx;
    p[1][0] = p[1][1] = my;
    p[0][1] = p[0][3] = xm;
    p[1][2] = p[1][3] = ym;
    transform_point(m,p[0][0],p[1][0],p[0][0],p[1][0]);
    transform_point(m,p[0][1],p[1][1],p[0][1],p[1][1]);
    transform_point(m,p[0][2],p[1][2],p[0][2],p[1][2]);
    transform_point(m,p[0][3],p[1][3],p[0][3],p[1][3]);
    qsort((void *)p[0],4,sizeof(double),matrix_cmp);
    qsort((void *)p[1],4,sizeof(double),matrix_cmp);
    mx = p[0][0];
    my = p[1][0];
    xm = p[0][3];
    ym = p[1][3];
}

static void mult_matrix(const Matrix m1,Matrix m2)
{
Matrix result;
int i, j, k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result [i][j] = 0.0;
            for (k = 0; k < 3; k++) {
                result [i][j] += m1 [i][k] * m2[k][j];
            }
        }
    }
    /*  copy the result into matrix 2  */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m2 [i][j] = result [i][j];
        }
    }
}
static void identity_matrix(Matrix m)
{
int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

static void translate_matrix(Matrix m,double x,double y)
{
Matrix trans;

    identity_matrix (trans);
    trans[0][2] = x;
    trans[1][2] = y;
    mult_matrix (trans, m);
}

static void scale_matrix(Matrix m,double x,double y)
{
Matrix scale;

    identity_matrix (scale);
    scale[0][0] = x;
    scale[1][1] = y;
    mult_matrix (scale, m);
}

static void rotate_matrix(Matrix m,double theta)
{
Matrix rotate;
double cos_theta, sin_theta;

    cos_theta = cos (theta);
    sin_theta = sin (theta);

    identity_matrix (rotate);
    rotate[0][0] = cos_theta;
    rotate[0][1] = -sin_theta;
    rotate[1][0] = sin_theta;
    rotate[1][1] = cos_theta;
    mult_matrix (rotate, m);
}

/*  find the determinate for a 3x3 matrix  */
static double determinate(Matrix m)
{
int i;
double det = 0;

    for (i = 0; i < 3; i ++) {
        det += m[0][i] * m[1][(i+1)%3] * m[2][(i+2)%3];
        det -= m[2][i] * m[1][(i+1)%3] * m[0][(i+2)%3];
    }
    return det;
}

/*  find the cofactor matrix of a matrix  */
static void cofactor(Matrix m,Matrix m_cof)
{
int i, j;
int x1, y1;
int x2, y2;

    x1 = y1 = x2 = y2 = 0;

    for (i = 0; i < 3; i++) {
        switch (i) {
            case 0 : y1 = 1; y2 = 2; break;
            case 1 : y1 = 0; y2 = 2; break;
            case 2 : y1 = 0; y2 = 1; break;
        }
        for (j = 0; j < 3; j++) {
            switch (j) {
                case 0 : x1 = 1; x2 = 2; break;
                case 1 : x1 = 0; x2 = 2; break;
                case 2 : x1 = 0; x2 = 1; break;
            }
            m_cof[i][j] = (m[x1][y1] * m[x2][y2] - m[x1][y2] * m[x2][y1]) *
              (((i+j) % 2) ? -1 : 1);
        }
    }
}

/*  find the inverse of a 3x3 matrix  */
static void invert(Matrix m,Matrix m_inv)
{
double det = determinate (m);
int i, j;

    if (det == 0.0) {
        return;
    }
    /*  Find the cofactor matrix of m, store it in m_inv  */
    cofactor (m, m_inv);

    /*  divide by the determinate  */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m_inv[i][j] = m_inv[i][j] / det;
        }
    }
}

bool Image_Stream::update_height()
{
bool ok=true;
size_t extra_size, offset;

    imst_x_=0;
    imst_y_++;
    if (imst_y_>=imst_h_) {
        offset = imst_depth_*imst_w_*imst_h_;
        extra_size = imst_depth_*MIN_BUFF_ROWS*imst_w_;
        imst_h_+=MIN_BUFF_ROWS;
        imst_data_=(unsigned char*)realloc (
            (void*)imst_data_,
            offset+extra_size
        );
        if (!imst_data_) {
            ok=false;
        } else {
            memset(imst_data_+offset,0xff,extra_size);
        }
    }
    return ok;
}
Image_Stream::Image_Stream()
{
    blur_=1.0;
    filter_=UNDEFINED_FILTER;
    imst_data_=0;
    clip_x_ = clip_y_ = 0.0;
    clip_w_ = clip_h_ = 1.0;
    reset();
    identity_matrix(mat_);
}
Image_Stream::Image_Stream(Image_Stream& i)
{
    imst_data_=0;
    reset();
    matrix(i.matrix());
}
void Image_Stream::matrix(const Matrix m)
{
    identity_matrix(mat_);
    mult_matrix(m,mat_);
}
void Image_Stream::matrix(const double **m)
{
    matrix(*(Matrix*)m);
}
Image_Stream::~Image_Stream() {
    if (imst_data_) {
      free(imst_data_);
    }
    imst_data_=nullptr;
}
bool Image_Stream::begin(int image_width,int bit_depth,bool iscolor) {
    reset();
    imst_w_=image_width;
    imst_bpp_=bit_depth;
    imst_color_=(iscolor)?1:0;
    imst_depth_=(iscolor)?3:1;
    imst_h_=0;
    imst_y_=-1;
    imst_x_=0;
    return update_height();
}
void Image_Stream::rewind() {
    imst_offset_=0;
    imst_y_=-1;
    imst_x_=0;
}
bool Image_Stream::add_row_data(unsigned char *row_data,int data_len,int frame) {
int i,j;
unsigned char mask;

    if (
        imst_w_ >  0 && 
        frame   >= 0 && 
        frame   <= 4 && 
        ( ( imst_bpp_==1 && frame!=FRAME_RGB ) || imst_bpp_==8 )
    ) {
        switch (frame) {
            case FRAME_GRAY:
                if (imst_bpp_==1) {
                    for (i=0;i<data_len;i++) {
                        for (j=7;j>=0;j++) {
                            mask=(row_data[i]&(1<<j))?0x00:0xff;
#if 1
                            imst_data_[imst_offset_++] = mask;
#else
                            imst_data_[imst_offset_++] = mask;
                            imst_data_[imst_offset_++] = mask;
                            imst_data_[imst_offset_++] = mask;
#endif
                            if (++imst_x_ >= imst_w_) {
                                if (!update_height()) {
                                    return false;
                                }
                                break; // skip padding bits
                            }
                        }
                    }
                } else {
                    for (i=0;i<data_len;i++) {
#if 1
                        imst_data_[imst_offset_++] = row_data[i];
#else
                        imst_data_[imst_offset_++] = row_data[i];
                        imst_data_[imst_offset_++] = row_data[i];
                        imst_data_[imst_offset_++] = row_data[i];
#endif
                        if (++imst_x_ >= imst_w_ && !update_height()) {
                            return false;
                        }
                    }
                }
                break;
            case FRAME_RGB:
                for (i=0;i<data_len;++i) {
                    imst_data_[imst_offset_++] = row_data[i];
                    if ((imst_offset_%3) == 0) {
                        if (++imst_x_ >= imst_w_ && !update_height()) {
                            return false;
                        }
                    }
                }
                break;
            case FRAME_RED:
            case FRAME_GREEN:
            case FRAME_BLUE:
                if (frame!=last_frame_) {
                    imst_offset_ += (frame-FRAME_RED);
                    last_frame_=frame;
                }
                if (imst_bpp_==1) {
                    for (i=0;i<data_len;i++) {
                        mask = row_data[i];
                        for (j=0;i<8;j++) {
                            imst_data_[imst_offset_++] = (mask&1)?0xff:0x00;
                            imst_offset_ += 3;
                            mask >>= 1;
                            if (++imst_x_ >= imst_w_ && !update_height()) {
                                return false;
                            }
                        }
                    }
                } else {
                    for (i=0;i<data_len;i++) {
                        imst_data_[imst_offset_] = row_data[i];
                        imst_offset_ += 3;
                        imst_x_++;
                        if (imst_x_ >= imst_w_ && !update_height()) {
                            return false;
                        }
                    }
                }
                break;
        }
    }
    return true;
}
void Image_Stream::reset()
{
Matrix lm = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

    last_frame_=-2;
    old_dst_w_=0;
    old_dst_h_=0;
    identity_matrix(undo_mat_);
    mult_matrix(lm,old_mat_);
    old_buf_size_=0;
    imst_x_=0;
    imst_y_=0;
    imst_w_=0;
    imst_h_=0;
    imst_offset_=0;
    imst_bpp_=0;
    if (imst_data_) {
      free(imst_data_);
    }
    imst_data_=nullptr;
    // identity_matrix(mat_);
}
void Image_Stream::undo_rotate()
{
Matrix tmat;
Matrix lm = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

    // Swap mat_ and undo_mat_ to undo last rotation (or to redo :-)
    identity_matrix(tmat);
    mult_matrix(mat_,tmat);
    identity_matrix(mat_);
    mult_matrix(undo_mat_,mat_);
    identity_matrix(undo_mat_);
    mult_matrix(tmat,undo_mat_);
    identity_matrix(old_mat_);
    mult_matrix(lm,old_mat_); // force redraw !
}
void Image_Stream::rotate(double angle)
{
int giri;
double rot;

    if (angle<0.0) {
        giri=(int)(fabs(angle)/360.0) + 1;
        rot=(acos(-1.0)*(angle+(giri*360.0)))/180.0;
    } else {
        giri=(int)(angle/360.0);
        rot=(acos(-1.0)*(angle-(giri*360.0)))/180.0;
    }
    identity_matrix(undo_mat_);
    mult_matrix(mat_,undo_mat_);
    rotate_matrix(mat_,rot);
}
void Image_Stream::rotate_cw()
{
    rotate(+90.0);
}
void Image_Stream::rotate_ccw()
{
    rotate(-90.0);
}
void Image_Stream::flip_vertical()
{
Matrix vflip;

    identity_matrix(vflip);
    vflip[1][1] = -1.0;
    mult_matrix(vflip,mat_);
}
void Image_Stream::flip_horizontal()
{
Matrix hflip;

    identity_matrix(hflip);
    hflip[0][0] = -1.0;
    mult_matrix(hflip,mat_);
}
void Image_Stream::clip(double x,double y,double w,double h)
{
    clip_x_=(x>=0 && x<=1)?x:0.0;
    clip_y_=(y>=0 && y<=1)?y:0.0;
    clip_w_=(w>=0 && w<=1)?w:1.0;
    clip_h_=(h>=0 && h<=1)?h:1.0;
}
#define get_pixel_c(x,y) (((x)>=0)&&((y)>=0)&&((x)<imst_w_)&&((y)<imst_h_)) ? \
                           &imst_data_[((y)*imst_w_+(x))*3]:black

#define get_pixel_g(x,y) (((x)>=0)&&((y)>=0)&&((x)<imst_w_)&&((y)<imst_h_)) ? \
                           &imst_data_[((y)*imst_w_+(x))]:black

bool Image_Stream::serialize (
    unsigned char **out_row,
    int out_w,
    int out_h,
    void (*begin_image)(int width,int height,bool iscolor),
    void (*add_row)(unsigned char *row)
) {
int X,Y,W,H,src_max,out_min;
double xscale,yscale;
int buf_size;
long src_x,src_y,dst_x,dst_y,dst_w,dst_h;
bool refresh=false;
double xmin,ymin,xmax,ymax;
Matrix tmat,mat;
#if USE_INTEGER
IMatrix imat;
#else
double ddx,ddy,dsx,dsy;
#endif
bool use_magick_zoom=false;
int contribution_x, contribution_y, size, n;
double density, scale_factor, support, center, start, end;
ContributionInfo *x_contribution = nullptr;
ContributionInfo *y_contribution = nullptr;
ContributionInfo *contribution   = nullptr;

    if (!add_row) {
        return refresh;
    }
    /*
     * Preparazione della matrice di trasformazione
     */
    identity_matrix(tmat);
    scale_matrix(tmat,imst_w_,imst_h_);
    mult_matrix(mat_,tmat);
    xmin=0.0; ymin=0.0; xmax=1.0; ymax=1.0;
    matrix_bounds(tmat,xmin,ymin,xmax,ymax);
    W=(long)fabs(xmax-xmin);
    H=(long)fabs(ymax-ymin);

    xscale=1.0;
    src_max=(W>H)?W:H;
    if (out_w<=0 || out_h<=0) {
        out_w=out_h=src_max;
    }
    out_min=(out_w<out_h)?out_w:out_h;
    if (src_max && out_min) {
        xscale=(double)src_max/out_min;
    }
    yscale=xscale;

    translate_matrix(tmat,(-xmin)/xscale,(-ymin)/yscale);

    X=(long)((double)(W*clip_x_)/xscale);
    Y=(long)((double)(H*clip_y_)/yscale);
    dst_w=(long)((double)(W*clip_w_)/xscale);
    dst_h=(long)((double)(H*clip_h_)/yscale);

    identity_matrix(mat);
    invert(tmat,mat);
    scale_matrix(mat,imst_w_*xscale,imst_h_*yscale);
    /*
     * Allocazione del buffer di memorizzazione di una singola linea
     * dell'immagine
     */
    buf_size=dst_w*imst_depth_;
    if (buf_size>0 && (!*out_row || buf_size!=old_buf_size_)) {
        delete *out_row;
        *out_row = new unsigned char[buf_size];
        old_buf_size_=buf_size;
        refresh=true;
    }
    if (*out_row) {
        if (
            mat[0][0] != old_mat_[0][0] ||
            mat[0][1] != old_mat_[0][1] ||
            mat[0][2] != old_mat_[0][2] ||
            mat[1][0] != old_mat_[1][0] ||
            mat[1][1] != old_mat_[1][1] ||
            mat[1][2] != old_mat_[1][2] ||
            mat[2][0] != old_mat_[2][0] ||
            mat[2][1] != old_mat_[2][1] ||
            mat[2][2] != old_mat_[2][2] ||
            dst_w     != old_dst_w_     ||
            dst_h     != old_dst_h_     ||
            refresh
        ) {
            if (begin_image) {
                begin_image(dst_w,dst_h,imst_color_);
            }
#if USE_INTEGER
            for (int i=0;i<3;i++) {
                for (int j=0;j<3;j++) {
                    imat[i][j]=(long)(mat[i][j]*(1<<20));
                }
            }
            imat[2][2]=20;
#endif
#define FILTER filters_[filter_]
            use_magick_zoom=false;
            if (filter()!=UNDEFINED_FILTER && xscale!=1.0) {
                scale_factor = blur_*Max(xscale,1.0);
                support = Max(scale_factor*FILTER.support,0.5);
                if (support<=0.5) {
                    // reduce to point sampling
                    support=0.5+MagickEpsilon;
                    scale_factor = 1.0;
                }
                size   = (int)(support*2+3);
                x_contribution = new ContributionInfo[size];
                y_contribution = new ContributionInfo[size];
                contribution   = new ContributionInfo[size*size];
                center = 0.0;
                start  = center - support;
                end    = center + support;
                if (x_contribution && y_contribution && contribution) {
                    // zoom X contribution
                    density = 0.0;
                    n = 0;
                    for (int i=(int)rint(start);i<(int)rint(end);i++) {
                        x_contribution[n].pixel =i;
                        x_contribution[n].weight=FILTER.function (
                            (i-center+0.5)/scale_factor
                        );
                        x_contribution[n].weight /= scale_factor;
                        density += x_contribution[n].weight;
                        n++;
                    }
                    contribution_x = n;
                    density = (density==0.0) ? 1.0 : 1.0/density;
                    for (int i=0; i<n; i++) {
                        // normalize
                        x_contribution[i].weight*=density;
                    }
                    // zoom Y contribution
                    density = 0.0;
                    n = 0;
                    for (int i=(int)rint(start);i<(int)rint(end);i++) {
                        y_contribution[n].pixel =i;
                        y_contribution[n].weight=FILTER.function (
                            (i-center+0.5)/scale_factor
                        );
                        y_contribution[n].weight /= scale_factor;
                        density += y_contribution[n].weight;
                        n++;
                    }
                    contribution_y = n;
                    density = (density==0.0) ? 1.0 : 1.0/density;
                    for (int i=0; i<n; i++) {
                        // normalize
                        y_contribution[i].weight*=density;
                    }
                    n=0;
                    for (int j=0;j<contribution_y;j++) {
                        for (int i=0;i<contribution_x;i++) {
                            contribution[n].x_offset=x_contribution[i].pixel;
                            contribution[n].y_offset=y_contribution[j].pixel;
                            contribution[n].weight  =x_contribution[i].weight *
                                                     y_contribution[j].weight;
                            n++;
                        }
                    }
                    use_magick_zoom=true;
                }
            }
            refresh=true;
            for (dst_y=Y;dst_y<dst_h+Y;dst_y++) {
                for (dst_x=X;dst_x<dst_w+X;dst_x++) {
                    int d = (dst_x-X)*imst_depth_;
#if USE_INTEGER
                    transform_point(imat,dst_x,dst_y,src_x,src_y);
#else
                    ddx=(double)dst_x;
                    ddy=(double)dst_y;
                    transform_point(mat,ddx,ddy,dsx,dsy);
                    src_x=(long)dsx;
                    src_y=(long)dsy;
#endif
#define _PIX(x) (unsigned char)((int)(x))

                    if (use_magick_zoom) {
                        double r=0.0;
                        double g=0.0;
                        double b=0.0;
                        unsigned char *pix;
                        long yrow=0,yoff=0;
                        n=0;
                        if (imst_color_) {
#undef get_pixel
#define get_pixel(x,y,r) (((x)>=0)&&((y)>=0)&&((x)<imst_w_)&&((y)<imst_h_)) ? \
                         &imst_data_[((r)+(x))*3]:black

                            for (int j=0;j<contribution_y;j++) {
                                yoff = contribution[n].y_offset + src_y;
                                yrow = yoff*imst_w_;
                                for (int i=0;i<contribution_x;i++) {
                                    pix=get_pixel (
                                        contribution[n].x_offset + src_x,
                                        yoff,yrow
                                    );
                                    r += contribution[n].weight * pix[0];
                                    g += contribution[n].weight * pix[1];
                                    b += contribution[n].weight * pix[2];
                                    n++;
                                }
                            }
                            (*out_row)[d+0] = _PIX((r<0)?0:(r>255)?255:r+0.5);
                            (*out_row)[d+1] = _PIX((g<0)?0:(g>255)?255:g+0.5);
                            (*out_row)[d+2] = _PIX((b<0)?0:(b>255)?255:b+0.5);
                        } else {
#undef get_pixel
#define get_pixel(x,y,r) (((x)>=0)&&((y)>=0)&&((x)<imst_w_)&&((y)<imst_h_)) ? \
                         &imst_data_[(r)+(x)]:black

                            for (int j=0;j<contribution_y;j++) {
                                yoff=contribution[n].y_offset + src_y;
                                yrow = yoff*imst_w_;
                                for (int i=0;i<contribution_x;i++) {
                                    pix=get_pixel (
                                        contribution[n].x_offset + src_x,
                                        yoff,yrow
                                    );
                                    g += contribution[n].weight * pix[0];
                                    n++;
                                }
                            }
                            (*out_row)[d] = _PIX((g<0)?0:(g>255)?255:g+0.5);
                        }
                    } else {
                        if (false) { /*- Pixel Interpolation -*/
                            unsigned char *pix;
                            if (imst_color_) {
                                pix=get_pixel_c(src_x,src_y);
                                (*out_row)[d+0] = pix[0];
                                (*out_row)[d+1] = pix[1];
                                (*out_row)[d+2] = pix[2];
                            } else {
                                pix=get_pixel_g(src_x,src_y);
                                (*out_row)[d] = pix[0];
                            }
                        } else if (true) { /*- Bilinear Interpolation -*/
                            unsigned char *A, *B, *C, *D;

                            double x = dsx - (float)src_x;
                            double y = dsy - (float)src_y;
                            if (imst_color_) {
                                A=get_pixel_c(src_x  ,src_y  );
                                B=get_pixel_c(src_x+1,src_y  );
                                C=get_pixel_c(src_x  ,src_y+1);
                                D=get_pixel_c(src_x+1,src_y+1);
                                double r = (A[0]*(1.0-x)+B[0]*(x))*(1.0-y) +
                                           (C[0]*(1.0-x)+D[0]*(x))*(y);
                                double g = (A[1]*(1.0-x)+B[1]*(x))*(1.0-y) +
                                           (C[1]*(1.0-x)+D[1]*(x))*(y);
                                double b = (A[2]*(1.0-x)+B[2]*(x))*(1.0-y) +
                                           (C[2]*(1.0-x)+D[2]*(x))*(y);
                                (*out_row)[d+0] = _PIX(r);
                                (*out_row)[d+1] = _PIX(g);
                                (*out_row)[d+2] = _PIX(b);
                            } else {
                                A=get_pixel_g(src_x  ,src_y  );
                                B=get_pixel_g(src_x+1,src_y  );
                                C=get_pixel_g(src_x  ,src_y+1);
                                D=get_pixel_g(src_x+1,src_y+1);
                                double g = (A[1]*(1.0-x)+B[1]*(x))*(1.0-y) +
                                           (C[1]*(1.0-x)+D[1]*(x))*(y);
                                (*out_row)[d] = _PIX(g);
                            }
                        }
                    }
                }
                add_row((unsigned char *)(*out_row));
            }
            identity_matrix(old_mat_);
            mult_matrix(mat,old_mat_);
            old_dst_w_ = dst_w;
            old_dst_h_ = dst_h;
        }
        refresh = true;
    }
    if (x_contribution) {
        delete x_contribution;
    }
    if (y_contribution) {
        delete y_contribution;
    }
    if (contribution) {
        delete contribution;
    }
    return refresh;
}
