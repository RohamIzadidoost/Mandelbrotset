#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include"defs.h"
#include<math.h>
const double PI=  3.14159265/180.0;
int MAX_ITER = 100; 

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
//printf("I am Sareh");

complex sum(complex a , complex b){
    complex res = {a.R + b.R , a.I + b.I} ; 
    return res ; 
}

double ABS(complex* a){
    return sqrt(a->R * a->R + a->I * a->I) ; 
}

double get_mbs_iter(double x, double y)
{
    int res = 0 ;
    complex c = {x , y} ; 
    complex z = {0 , 0} ;  
    while(res <= MAX_ITER && ABS(&z) <= 2){
        res ++ ; 
        z = sum(mul(z , z) , c ) ; 
    }
    if(res == MAX_ITER + 1) return res ; 
    return (double)res - log(log2(ABS(&z))); //log(log2(abs(z)
}

void hsv_to_rgb(float H, float S, float V, COLORINDEX* p)
{
    float s = S/100.0;
    float v = V/100.0;
    float C = s*v;
    float X = C*(1-fabs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    //printf("RGB : %d , %d , %d \n" , R , G , B) ; 
    p->b = R ; // why it creates a good pic ?
    p->b = B ; 
    p->g = G ; 
}

void UpdateImageData(ImageState* state)
{
    printf("CREATING pic:%d\n" , state->image_count);
    //printf("UPDING IMAGE DATA %d , %d" , state->width , state->height) ; 
    for(int x=0; x<state->width; x++){
        //printf("%d" , x) ; 
        for(int y=0; y<state->height; y++)
        {
            // x,y => dx,dy
            // dx,dy => rx, ry
            // rx = nx * cos(state->angle) + ny * sin(state->angle);
            // ry = -nx * sin(state->angle) + ny * cos(state->angle);
            //RE_START + (x / WIDTH) * (RE_END - RE_START)
            /* 
            double rx = (double)state->minx + (x / (double)(state->width) ) * (double)(state->maxx - state->minx)
             , ry = (double)state->miny + (y / (double)(state->height) ) * (double)(state->maxx - state->miny) ; 
            double nx = cos(state->angle) * rx - sin(state->angle) * ry ; 
            double ny = sin(state->angle) * rx + cos(state->angle) * ry ; 
            */
            double nx = (double)state->minx + (x / (double)(state->width) ) * (double)(state->maxx - state->minx)
             , ny = (double)state->miny + (y / (double)(state->height) ) * (double)(state->maxx - state->miny) ; 
            double rx = nx * cos(PI * state->angle) + ny * sin(PI * state->angle);
            double ry = -nx * sin(PI * state->angle) + ny * cos(PI * state->angle); 
            
            double iter = get_mbs_iter(rx , ry);
           // printf("x:%d , y:%d , iter:%d \n" , rx , ry , iter) ; 
            if(iter == MAX_ITER + 1)
            state->bmFileData.bmData[y * state->width + x] = 0;
            else 
            state->bmFileData.bmData[y * state->width + x] = iter;
            
           // printf("iter: %lf\n" , iter) ; 

            //printf("iter : %d\n" , iter ) ; 
           // printf("x : %d , y: %d" , x , y) ; 
        }
    }

    // I just set i = 2 fixed later  
    for(int i=0; i<256; i++)
    {
        double hue = (int) ((i / 255)/360);
        hue = (double)(i)/255.0 * 360.0 ; 
        //printf("%d" , hue) ; 
        hsv_to_rgb(hue, 100, 100, &(state->bmFileData.bmHeader.colorIdx[i])); 
        //printf("i:%d R:%d , G:%d , B:%d \n" , i , state->bmFileData.bmHeader.colorIdx[i].r , 
       //state->bmFileData.bmHeader.colorIdx[i].g , state->bmFileData.bmHeader.colorIdx[i].b) ; 
    }
    state->bmFileData.bmHeader.bmInfoHeader.biClrUsed = 100;
    state->bmFileData.bmHeader.bmInfoHeader.biClrImportant = 100;
    state->bmFileData.bmHeader.colorIdx[0].r = 0 ; 
    state->bmFileData.bmHeader.colorIdx[0].b = 0 ;
    state->bmFileData.bmHeader.colorIdx[0].g = 0 ;
}

void ChangeCenter(ImageState* state, double newcx, double newcy, int steps)
{
    // TODO
    double xst = (newcx - state->cx) / (double)(steps) ; 
    double yst = (newcy - state->cy) / (double)(steps) ; 
    for(int i=0;i<steps; i++)
    {
        // TODO
        state->cx += xst ; 
        state->cy += yst ; 
        state->minx += xst ; 
        state->maxx += xst ; 
        state->miny += yst ; 
        state->maxy += yst; 
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoom(ImageState* state, double zoom, int steps)
{
    // TODO
    double zst = pow(zoom , 1./steps) ; // zoom step
    //printf("ZST : %lf\n" , zst) ; 
    for(int i=0; i<steps; i++)
    {
        state->minx = state->minx / zst ;  
        state->miny = state->miny / zst ; 
        state->maxx = state->maxx / zst ; 
        state->maxy = state->maxy / zst ; 
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotation(ImageState* state, double angle, int steps)
{
    // TODO
    double angle_step = angle / steps;
    printf("%lf\n" , angle_step) ; 
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Hold(ImageState* state, int steps)
{
    printf("HOLDING FOR %d \n" , steps) ; 
    // TODO
    for(int i=0; i<steps; i++)
    {
        //TODO
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

#endif