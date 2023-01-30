#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include"defs.h"
#include<math.h>
const double PI=  3.14159265/180.0;
int MAX_ITER = 200; 

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

double ABS(complex* a){
    return sqrt(a->R * a->R + a->I * a->I) ; 
}

// complex julia_begin_const = {-0.835 , 0.232} ; 
complex julia_begin_const = {0 , 0} ;
bool JULIA = false ;


double get_mbs_iter(double x, double y)
{
    int res = 0 ;
    // complex c = {x , y} ; 
    // complex z = {0 , 0} ;  
    complex c;
    complex z = {x , y} ;  
    if(JULIA){
        c = julia_begin_const ; 
        //z = (complex){x , y} ;
    }else{
        c = (complex){x , y} ; 
        //z = (complex){x , y} ;      
    }
     
    while(res <= MAX_ITER && ABS(&z) <= 2){
        res ++ ; 
        z = sum(mul(z , z) , c ) ; 
    }
    if(res == MAX_ITER + 1) return res ; 
    // printf("res : %d , ABS(z): %lf , log(abs(z)): %lf  , log2: %lf , log(log2): %lf \n " 
    // , res , ABS(&z) , log(ABS(&z)) , log2(ABS(&z)) , log(log2(ABS(&z)))) ; 
    //return (double)res - log2(ABS(&z)) * ABS(&z) * ABS(&z) * log(ABS(&z))  ; 
    return (double)res - log(log2(ABS(&z))); //log(log2(abs(z)
}

void hsv_to_rgb(float H, float S, float V, COLORINDEX* p)
{
    //printf("%lf\n" , H) ; 
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
     p->b = R ; // why it creates a good pic ? bbg
     p->b = B ; 
     p->g = G ; 

    
    // p->r = G ;
    // p->b = G ; 
    // p->g = G ;  
    // //p->r = B ; 
    //lprintf("R %d, G %d , B:%d \n" , R , G , B );
}

int color_number(float H, float S, float V, COLORINDEX* p)
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
    return G;
}

double RC = 1.4 , BC = 0.5 , GC = 0.6 ; // red_const , blue_const , green_const

void do_coloring(int cn , COLORINDEX* p){
    p -> b = BC * cn ; 
    p -> r = RC * cn ; 
    p -> g = GC * cn ; 
}

void HSV_to_rgb(float H, float S, float V, COLORINDEX* p){
    printf("Hvalue: %lf\n" , H) ;  
    return ;
    if(H < 120){
        p->b = H/120.0 * 255.0 ;
    }else if(H < 240){
        p -> r = (H - 120) / 120.0 * 255.0 ; 
    }else{
        p ->g = (H - 240) / 120.0 * 255.0 ; 
    }
}

void UpdateImageData(ImageState* state)
{
    printf("CREATING pic:%d , Center: %lf,%lf,X: %lf,%lf,Y: %lf,%lf, COlval: %lf,ColGoal: %lf,Colstep: %lf\n" ,
     state->image_count , state->cx , state->cy,
    state->minx , state->maxx , state->miny , state->maxy , state -> colval , state->colgoal , state -> colstep);
    if(state -> colgoal != state->colval){
        state->colval += state ->colstep; 
    }
    if(state -> colBC_goal != BC){
        BC += state->colBC_step ; 
    }
    if(state -> colGC_goal != GC){
        GC += state->colGC_step ; 
    }
    if(state -> colRC_goal != RC){
        RC += state->colRC_step ; 
    }
    for(int x=0; x<state->width; x++){
        //printf("%d" , x) ; 
        for(int y=0; y<state->height; y++)
        {
            double nx = (double)state->minx + ((double)x / (double)(state->width) ) * (double)(state->maxx - state->minx)
             , ny = (double)state->miny + ((double)y / (double)(state->height) ) * (double)(state->maxy - state->miny) ; 
            double rx = nx * cos(PI * state->angle) + ny * sin(PI * state->angle);
            double ry = -nx * sin(PI * state->angle) + ny * cos(PI * state->angle); 
            
            double iter = get_mbs_iter(rx , ry);
           // printf("x:%d , y:%d , iter:%d \n" , rx , ry , iter) ; 
            if(iter == MAX_ITER + 1 )
            state->bmFileData.bmData[y * state->width + x] = 0;
            else 
            state->bmFileData.bmData[y * state->width + x] = iter/MAX_ITER * 256.0;

            //printf("bmData: %d , iter: %lf\n" ,state->bmFileData.bmData[y * state->width + x] , iter ) ; 
        } 
    }

    for(int i=0; i<256; i++)
    {
        double hue = (int) ((i / 255)/360);
        hue = (double)(i)/255.0 * 360.0 ; 
        int H = ((int)hue + 200) % 360 ; 
        int Hint = (int) hue ; 
        // HSV_to_rgb(hue , 100 , state->colval , &(state->bmFileData.bmHeader.colorIdx[i])); 
        // hsv_to_rgb(hue , 100 , state->colval , &(state->bmFileData.bmHeader.colorIdx[i])); 
        int colnumber = color_number(hue , 100 , state->colval , &(state->bmFileData.bmHeader.colorIdx[i])) ; 
        do_coloring(colnumber , &(state->bmFileData.bmHeader.colorIdx[i])) ; 
    }
    state->bmFileData.bmHeader.bmInfoHeader.biClrUsed = 100;
    state->bmFileData.bmHeader.bmInfoHeader.biClrImportant = 100;
    state->bmFileData.bmHeader.colorIdx[0].r = 0 ; 
    state->bmFileData.bmHeader.colorIdx[0].b = 0 ;
    state->bmFileData.bmHeader.colorIdx[0].g = 0 ;
}
void ChangeColVal(ImageState* state, double goal, int steps)
{
    state -> colgoal = goal ; 
    state -> colstep = goal - state->colval ; 
    state -> colstep /= (double)steps ; 
}
void ChangeColoring(ImageState* state, double newRC , double newBC , double newGC , int steps){
    state -> colBC_goal = newBC ; 
    state -> colGC_goal = newGC ; 
    state -> colRC_goal = newRC ;
    state -> colBC_step = (newBC - BC)/(double)steps; 
    state -> colGC_step = (newGC - GC)/(double)steps ;
    state -> colRC_step = (newRC - RC)/(double)steps ;  
}
void BuildJulia(ImageState* state, double NewRconst, double NewIconst, int steps)
{
    // TODO
    JULIA ^= 1 ;
    double Rst = (double)(NewRconst - julia_begin_const.R) / (double)(steps) ; 
    double Ist = (double)(NewIconst - julia_begin_const.I) / (double)(steps) ; 
    for(int i=0;i<steps; i++)
    {
        // TODO
        julia_begin_const.I += Ist ; 
        julia_begin_const.R += Rst ;  
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}
void ChangeZoom(ImageState* state, double zoom, int steps)
{
    // TODO
    double zst = pow(zoom , 1./steps) ; // zoom step
    printf("ZST : %lf\n" , zst) ; 
    for(int i=0; i<steps; i++)
    {
        // state->minx = state->minx / zst ;  
        // state->miny = state->miny / zst ; 
        // state->maxx = state->maxx / zst ; 
        // state->maxy = state->maxy / zst ; 
        // state->cx = state->cx / zst ; 
        // state->cy = state->cy / zst ; 
        double flx = state->maxx - state->minx ; //first len x 
        double fly = state->maxy - state->miny ; 
        double dx = flx - flx / zst; // delta x
        double dy = fly - fly / zst;
        state->minx = state->minx + dx/2 ;  
        state->miny = state->miny + dy/2 ; 
        state->maxx = state->maxx - dx/2 ; 
        state->maxy = state->maxy - dy/2 ;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}
void ZoomJul(ImageState* state, double NewRconst, double NewIconst ,double zoom, int steps ){
    printf("ZOOMJULING : %lf , %lf , %lf , %d : \n" , NewRconst , NewIconst , zoom , steps) ; 
    printf("STATE:%d , Center: %lf , %lf , X: %lf , %lf, Y: %lf , %lf , COlval: %lf , ColGoal: %lf ,Colstep: %lf\n" ,
     state->image_count , state->cx , state->cy,
    state->minx , state->maxx , state->miny , state->maxy , state -> colval , state->colgoal , state -> colstep);
    JULIA = 1 ; 
    double Rst = (double)(NewRconst - julia_begin_const.R) / (double)(steps) ; 
    double Ist = (double)(NewIconst - julia_begin_const.I) / (double)(steps) ; 
    double zst = pow(zoom , 1./steps) ;
    for(int i=0;i<steps; i++)
    {
        // TODO
        julia_begin_const.I += Ist ; 
        julia_begin_const.R += Rst ;  
        double flx = state->maxx - state->minx ; //first len x 
        double fly = state->maxy - state->miny ; 
        double dx = flx - flx / zst; // delta x
        double dy = fly - fly / zst;
        state->minx = state->minx + dx/2 ;  
        state->miny = state->miny + dy/2 ; 
        state->maxx = state->maxx - dx/2 ; 
        state->maxy = state->maxy - dy/2 ;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
    printf("STATE:%d ,COlval: %lf , ColGoal: %lf ,Colstep: %lf\n" ,
     state->image_count , state -> colval , state->colgoal , state -> colstep);
}

void ChangeCenter(ImageState* state, double newcx, double newcy, int steps)
{
    // TODO
    double xst = (double)(newcx - state->cx) / (double)(steps) ; 
    double yst = (double)(newcy - state->cy) / (double)(steps) ; 
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
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

#endif