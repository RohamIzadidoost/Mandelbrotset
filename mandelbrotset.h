#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include"defs.h"
#include<math.h>

const int MAX_ITER = 80 ; 

typedef struct _complex
{
    double R , I ; 
} complex;

complex mul(complex a , complex b){
    complex res ; 
    res.R = a.R * b.R - a.I * b.I ; 
    res.I = a.I * b.R + a.R * b.I ; 
    return res ; 
}

complex sum(complex a , complex b){
    complex res = {a.R + b.R , a.I + b.I} ; 
    return res ; 
}

double ABS(complex a){
    return sqrt(a.R * a.R + a.I * a.I) ; 
}

int get_mbs_iter(double x, double y)
{
    int res = 0 ;
    complex c = {x , y} ; 
    complex z = {0 , 0} ;  
    while(res <= MAX_ITER && ABS(z) <= 2){
        res ++ ; 
        z = sum(mul(z , z) , c ) ; 
    }
    return res ; 
}

void hsv_to_rgb(int hue, int min, int max, COLORINDEX* p)
{
    //todo
}

void UpdateImageData(ImageState* state)
{
    printf("UPDING IMAGE DATA\n %d , %d" , state->width , state->height) ; 
    for(int x=0; x<state->width; x++){
        //printf("%d" , x) ; 
        for(int y=0; y<state->height; y++)
        {
            // x,y => dx,dy
            // dx,dy => rx, ry
            // rx = nx * cos(state->angle) + ny * sin(state->angle);
            // ry = -nx * sin(state->angle) + ny * cos(state->angle);
            int rx = x , ry = y ; 
            int iter = get_mbs_iter(rx, ry);
           // printf("x:%d , y:%d , iter:%d \n" , rx , ry , iter) ; 
            state->bmFileData.bmData[y * state->width + x] = iter;
        }
    }
    for(int i=0; i<256; i++)
    {
        //int hue = (int) ((i / 255)/360);
        //printf("%d" , hue) ; 
        // HSV2RGB(hue, 100, 100, &(state->bmFileData.bmHeader.colorIdx[i]));
        state->bmFileData.bmHeader.colorIdx[i].r = 255 ; 
    }
}

void ChangeCenter(ImageState* state, double newcx, double newcy, int steps)
{
    // TODO

    for(int i=0;i<steps; i++)
    {
        // TODO
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoom(ImageState* state, double zoom, int steps)
{
    // TODO
    for(int i=0; i<steps; i++)
    {
        // TODO
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotation(ImageState* state, double angle, int steps)
{
    // TODO
    double angle_step = angle / steps;
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Hold(ImageState* state, int steps)
{
    // TODO
    for(int i=0; i<steps; i++)
    {
        //TODO
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

#endif