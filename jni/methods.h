#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
   int    r;
   int    c;
} point_t;


typedef struct
{
    point_t s;
    point_t e;
    int a;
    float f;
} square_t;

typedef struct
{
    square_t outer;
    square_t inner;
    int w_half;
    int w;
    int h;
}eye_t;



typedef struct
{
    square_t outer;
    square_t middle;
    square_t inner;
    int w_i;
    int w_m;
    int w_o;
}ring_t;


inline float area(const float *img,point_t size,point_t start,point_t end,point_t offset){
    // use like in numpy including start excluding end
    // 0-based indecies
    // this is because the integral image has w+1 and h+1
    return    img[(offset.r + end.r  ) * size.c + offset.c + end.c]
            + img[(offset.r + start.r) * size.c + offset.c + start.c]
            - img[(offset.r + start.r) * size.c + offset.c + end.c]
            - img[(offset.r + end.r  ) * size.c + offset.c + start.c];
    }

inline eye_t make_eye(int h){
    int w = 3*h;
    eye_t eye;
    eye.h = h;
    eye.w = w;
    eye.outer.s = (point_t){0,0};
    eye.outer.e = (point_t){w,w};
    eye.inner.s = (point_t){h,h};
    eye.inner.e = (point_t){h+h,h+h};
    eye.inner.a = h*h;
    eye.outer.a = w*w;
    eye.outer.f =  1.0/eye.outer.a;
    eye.inner.f =  -1.0/eye.inner.a;
    eye.w_half = w/2;
    return eye;
}

inline ring_t make_ring(int w){
    ring_t ring;
    ring.w_i = w;
    ring.w_m = 3*w;
    ring.w_o = 9*w;
    ring.outer.s = (point_t){0,0};
    ring.outer.e = (point_t){9*w,9*w};
    ring.middle.s = (point_t){3*w,3*w};
    ring.middle.e = (point_t){6*w,6*w};
    ring.inner.s = (point_t){4*w,4*w};
    ring.inner.e = (point_t){5*w,5*w};
    ring.inner.a = w*w;
    ring.middle.a = 9*w*w;
    ring.outer.a = 81*w*w;
    ring.outer.f =  -.5/ring.outer.a;
    ring.middle.f =  1.0/ring.middle.a;
    ring.inner.f =  -.5/ring.inner.a;
    return ring;
}

void filter(const float *img, const int rows, const int cols, int * x_pos,int *y_pos,int *width, int min_w,int max_w,float *response);
void ring_filter(const float *img, const int rows, const int cols, int * x_pos,int *y_pos,int *width, float *response);
