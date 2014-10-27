    /*
(*)~----------------------------------------------------------------------------------
 Pupil - eye tracking platform
 Copyright (C) 2012-2014  Pupil Labs

 Distributed under the terms of the CC BY-NC-SA License.
 License details are in the file license.txt, distributed as part of this software.
----------------------------------------------------------------------------------~(*)
*/

#include "methods.h"

void filter(const float *img, const int rows, const int cols, int * x_pos,int *y_pos,int *width, int min_w,int max_w,float *response)
// Algorithm based on:
// Robust real-time pupil tracking in highly off-axis images
// Lech Świrski Andreas Bulling Neil A. Dodgson
// Computer Laboratory, University of Cambridge, United Kingdom
// Eye Tracking Research & Applications 2012
{
    point_t img_size = {rows,cols};
    int min_h = min_w/3;
    int max_h = max_w/3;
    int h, i, j;
    float best_response = -10000;
    point_t best_pos ={0,0};
    int best_h = 0;
    int h_step = 4;
    int step = 5;

    for (h = min_h; h < max_h; h+=h_step)
        {
            eye_t eye = make_eye(h);
            // printf("inner factor%f outer.factor %f center %i \n",eye.inner.f,eye.outer.f,(int)eye.w_half );
            for (i=0; i<rows-eye.w; i +=step)
            {
                for (j=0; j<cols-eye.w; j+=step)
                {
                    // printf("|%2.0f",img[i * cols + j]);
                    point_t offset = {i,j};
                    float response =  eye.outer.f*area(img,img_size,eye.outer.s,eye.outer.e,offset)
                                     +eye.inner.f*area(img,img_size,eye.inner.s,eye.inner.e,offset);
                    // printf("| %5.2f ",response);
                    if(response > best_response){
                        // printf("!");
                        best_response = response;
                        best_pos = (point_t){i,j};
                        // printf("%i %i", (int)best_pos.r,(int)best_pos.c);
                        best_h = eye.h;
                    }
                }
                // printf("\n");
            }
        }



    // now we refine the search at pixel resolution This hole part can be commented out if needed
    point_t window_lower = {MAX(0,best_pos.r-step+1),MAX(0,best_pos.c-step+1)};
    point_t window_upper = {MIN(img_size.r,best_pos.r+step),MIN(img_size.c,best_pos.c+step)};
    for (h = best_h-h_step+1; h < best_h+h_step; h+=1)
        {
            eye_t eye = make_eye(h);
            // printf("inner factor%f outer.factor %f center %i \n",eye.inner.f,eye.outer.f,(int)eye.w_half );
            for (i=window_lower.r; i<MIN(window_upper.r,img_size.r-eye.w); i +=1)
            {
                for (j=window_lower.c; j<MIN(window_upper.c,img_size.c-eye.w); j +=1)
                {

                    // printf("|%2.0f",img[i * cols + j]);
                    point_t offset = {i,j};
                    float response =  eye.outer.f*area(img,img_size,eye.outer.s,eye.outer.e,offset)
                                     +eye.inner.f*area(img,img_size,eye.inner.s,eye.inner.e,offset);
                    // ikiuprintf("| %5.2f ",response);
                    if(response > best_response){
                        // printf("!");
                        best_response = response;
                        best_pos = (point_t){i,j};
                        // printf("%i %i", (int)best_pos.r,(int)best_pos.c);
                        best_h = eye.h;
                    }
                }
                // printf("\n");
            }
        }

    *x_pos = (int)best_pos.r;
    *y_pos = (int)best_pos.c;
    *width = best_h*3;
    *response = best_response;
    }


void ring_filter(const float *img, const int rows, const int cols, int * x_pos,int *y_pos,int *width, float *response)
// Algorithm based on:
// Robust real-time pupil tracking in highly off-axis images
// Lech Świrski Andreas Bulling Neil A. Dodgson
// Computer Laboratory, University of Cambridge, United Kingdom
// Eye Tracking Research & Applications 2012
{
    point_t img_size = {rows,cols};
    int min_h = 6;
    int max_h = 50;
    int h, i, j;
    float best_response = -10000;
    point_t best_pos ={0,0};
    int best_h = 0;
    int h_step = 4;
    int step = 5;

    for (h = min_h; h < max_h; h+=h_step)
        {
            ring_t ring = make_ring(h);
            // printf("inner factor%f outer.factor %f center %i \n",ring.inner.f,ring.outer.f,(int)ring.w_o_half );
            for (i=0; i<rows-ring.w_o; i +=step)
            {
                for (j=0; j<cols-ring.w_o; j+=step)
                {
                    // printf("|%2.0f",img[i * cols + j]);
                    point_t offset = {i,j};
                    float response =  ring.outer.f*area(img,img_size,ring.outer.s,ring.outer.e,offset)
                                     +ring.middle.f*area(img,img_size,ring.middle.s,ring.middle.e,offset)
                                     +ring.inner.f*area(img,img_size,ring.inner.s,ring.inner.e,offset);
                    // printf("| %5.2f ",response);
                    if(response > best_response){
                        // printf("!");
                        best_response = response;
                        best_pos = (point_t){i,j};
                        // printf("%i %i", (int)best_pos.r,(int)best_pos.c);
                        best_h = ring.w_o;
                    }
                }
                // printf("\n");
            }
        }



    // now we refine the search at pixel resolution This hole part can be commented out if needed
    point_t window_lower = {MAX(0,best_pos.r-step+1),MAX(0,best_pos.c-step+1)};
    point_t window_upper = {MIN(img_size.r,best_pos.r+step),MIN(img_size.c,best_pos.c+step)};
    for (h = best_h-h_step+1; h < best_h+h_step; h+=1)
        {
            ring_t ring = make_ring(h);
            // printf("inner factor%f outer.factor %f center %i \n",ring.inner.f,ring.outer.f,(int)ring.w_o_half );
            for (i=window_lower.r; i<MIN(window_upper.r,img_size.r-ring.w_o); i +=1)
            {
                for (j=window_lower.c; j<MIN(window_upper.c,img_size.c-ring.w_o); j +=1)
                {

                    // printf("|%2.0f",img[i * cols + j]);
                    point_t offset = {i,j};
                     float response = ring.outer.f*area(img,img_size,ring.outer.s,ring.outer.e,offset)
                                     +ring.middle.f*area(img,img_size,ring.middle.s,ring.middle.e,offset)
                                     +ring.inner.f*area(img,img_size,ring.inner.s,ring.inner.e,offset);
                    // printf("| %5.2f ",response);
                    // ikiuprintf("| %5.2f ",response);
                    if(response > best_response){
                        // printf("!");
                        best_response = response;
                        best_pos = (point_t){i,j};
                        // printf("%i %i", (int)best_pos.r,(int)best_pos.c);
                        best_h = ring.w_o;
                    }
                }
                // printf("\n");
            }
        }


    // point_t start = {0,0};
    // point_t end = {1,1};
    // printf("FULL IMG SUM %1.0f\n",img[(img_size.r-1) * img_size.c + (img_size.c-1)] );
    // printf("AREA:%f\n",area(img,img_size,start,end,(point_t){0,0}));
    *x_pos = (int)best_pos.r;
    *y_pos = (int)best_pos.c;
    *width = best_h;
    *response = best_response;
    }

