#include<stdio.h>
#include<stdlib.h>
#include"defs.h"
#include"utils.h"
#include"mandelbrotset.h"

int main(int argc, char** argv)
{
    printf("Be name khoda") ; 
    Config cfg; memset(&cfg, 0, sizeof(Config));
    ImageState state; memset(&state, 0, sizeof(ImageState));

    printf("processing ARGS") ; 
    
    state.width = cfg.width ;
    state.height = cfg.height ; 
    state.angle = 0 ; 
    state.colgoal = 100 ; 
    state.colval = 100 ; 
    state.colstep = 0 ; 

    ProcessArgs(argc, argv, &cfg, &state);
    
    printf("Args procceed!!!!\n") ; 
    
    UpdateImageData(&state);
    WriteBitmapFile(state.image_count++, & state.bmFileData);

    for(int i=0;i<cfg.CmdCount; i++)
    {
        printf("IN COMMAND %d: type = %d \n" ,i ,  cfg.Commands[i].cmd) ; 
        switch (cfg.Commands[i].cmd)
        {
            case ct_Center:
                ChangeCenter(&state, cfg.Commands[i].cx, cfg.Commands[i].cy, cfg.Commands[i].steps);
                break;
            
            case ct_Zoom:
                ChangeZoom(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps);
                break;

            case ct_Rotate:
                ChangeRotation(&state, cfg.Commands[i].angle, cfg.Commands[i].steps);
                break;

            case ct_Hold:
                Hold(&state, cfg.Commands[i].steps);
                break;
            case ct_Julia:
                BuildJulia(&state , cfg.Commands[i].rconst , cfg.Commands[i].iconst , cfg.Commands[i].steps);
                break;;
            case ct_ZoomJul:
                ZoomJul(&state , cfg.Commands[i].rconst , cfg.Commands[i].iconst , cfg.Commands[i].zoom , cfg.Commands[i].steps);
            case ct_ColVal:
                ChangeColVal(&state , cfg.Commands[i].colgoal , cfg.Commands[i].steps) ; 
            case ct_NewColoring:
                ChangeColoring(&state , cfg.Commands[i].NewcolRC , cfg.Commands[i].NewcolBC , cfg.Commands[i].NewcolGC , cfg.Commands[i].steps) ; 

        }
    }
    return 0;
}