// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//    Gamma correction LUT stuff.
//    Functions to draw patches (by post) directly to screen.
//    Functions to blit a block to the screen.
//
//-----------------------------------------------------------------------------
#include "z_zone.h"
#include "i_video.h"

#include "i_system.h"
#include "r_local.h"

#include "doomdef.h"
#include "doomdata.h"

#include "m_bbox.h"
#include "m_swap.h"

#include "v_video.h"

#define ytab(y) (((y)<<8)+((y)<<6))

extern uint32_t*      palarray;
extern surface_t*    _dc;
extern void *bufptr;

int                  usegamma;

// Now where did these came from?
byte  __attribute__((aligned(8))) gammatable[5][256] =
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
     17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
     81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
     97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
     144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
     160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
     176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
     192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
     208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

    {2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,
     32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,
     56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,
     78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
     99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,
     130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
     146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,
     161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,
     175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,
     190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,
     205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,
     219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,
     233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,
     247,248,249,250,251,252,252,253,254,255},

    {4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,
     43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,
     70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,
     94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,
     144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,
     160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,
     174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,
     188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,
     202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,
     216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,
     229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,
     242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,
     255},

    {8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,
     57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,
     86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,
     108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,
     141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
     155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,
     169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,
     183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,
     195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,
     207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,
     219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,
     231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,
     242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,
     253,253,254,254,255},

    {16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,
     78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,
     107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
     142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
     156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,
     169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,
     182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,
     193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,
     204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,
     214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,
     224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,
     234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,
     243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,
     251,252,252,253,254,254,255,255}
};


//
// V_MarkRect
//
void V_MarkRect(int x, int y, int width, int height)
{
    //M_AddToBox (dirtybox, x, y);
    //M_AddToBox (dirtybox, x+width-1, y+height-1);
}

//
// V_CopyRect
//
void V_CopyRect( int srcx, int srcy, int srcscrn, int width, int height, int destx, int desty, int destscrn)
{
}

//
// V_DrawPatch
// Masks a column based masked pic to the screen.
//
//__attribute__ ((optimize(1)))
void V_DrawPatch ( int x, int y, patch_t* patch )
{
    int          count;
    int          col; 
    column_t*    column; 
    byte*        source; 
    int          w; 

    y -= SHORT(patch->topoffset); 
    x -= SHORT(patch->leftoffset); 
    col = 0; 
    w = SHORT(patch->width); 

#ifdef RANGECHECK 
    if (x<0
    ||x+SHORT(patch->width) >SCREENWIDTH
    || y<0
    || y+SHORT(patch->height)>SCREENHEIGHT )
    {
      I_Error("Patch at %d,%d exceeds LFB\n", x,y );
      // No I_Error abort - what is up with TNT.WAD?
      //fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
      //return;
    }
#endif 

    uint16_t*    desttop;
    uint16_t*    dest;

    desttop = (uint16_t*)((uintptr_t)bufptr + (uintptr_t)((((y)*SCREENWIDTH)+x)*2));

    for ( ; col<w ; x++, col++, desttop++)
    { 
        column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 

        // step through the posts in a column 
        while (column->topdelta != 0xff ) 
        { 
            source = (byte *)column + 3; 
            dest = (uint16_t*)((uintptr_t)desttop + (uintptr_t)(column->topdelta*SCREENWIDTH*2)); 
            count = column->length; 

            while (count--) 
            { 
                uint16_t next_spot = palarray[*source++];
                *dest = next_spot;
                dest += SCREENWIDTH; 
            } 
            column = (column_t *)(  (byte *)column + column->length + 4 ); 
        } 
    }
}


void V_DrawPatchBuf ( int x, int y, patch_t* patch, uint16_t *buf)
{
    int          count;
    int          col; 
    column_t*    column; 
    byte*        source; 
    int          w; 

    y -= SHORT(patch->topoffset); 
    x -= SHORT(patch->leftoffset); 
    col = 0; 
    w = SHORT(patch->width); 

#ifdef RANGECHECK 
    if (x<0
    ||x+SHORT(patch->width) >SCREENWIDTH
    || y<0
    || y+SHORT(patch->height)>SCREENHEIGHT )
    {
      I_Error("Patch at %d,%d exceeds LFB\n", x,y );
      // No I_Error abort - what is up with TNT.WAD?
      //fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
      //return;
    }
#endif 

    uint16_t*    desttop;
    uint16_t*    dest;

    desttop = (uint16_t*)((uintptr_t)buf + (uintptr_t)((((y)*SCREENWIDTH)+x)*2));

    for ( ; col<w ; x++, col++, desttop++)
    { 
        column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 

        // step through the posts in a column 
        while (column->topdelta != 0xff ) 
        { 
            source = (byte *)column + 3; 
            dest = (uint16_t*)((uintptr_t)desttop + (uintptr_t)(column->topdelta*SCREENWIDTH*2)); 
            count = column->length; 

            while (count--) 
            { 
                uint16_t next_spot = palarray[*source++];
                *dest = next_spot;
                dest += SCREENWIDTH; 
            } 
            column = (column_t *)(  (byte *)column + column->length + 4 ); 
        } 
    }
}


//
// V_DrawPatchFlipped
// Masks a column based masked pic to the screen.
// Flips horizontally, e.g. to mirror face.
//
void V_DrawPatchFlipped ( int x, int y, patch_t* patch )
{
    int          count;
    int          col;
    column_t*    column;
    byte*        source;
    int          w;
    uint16_t*    desttop;
    uint16_t*    dest;

    y -= SHORT(patch->topoffset);
    x -= SHORT(patch->leftoffset);
    col = 0;
    w = SHORT(patch->width);

#ifdef RANGECHECK 
    if (x<0
    ||x+SHORT(patch->width) >SCREENWIDTH
    || y<0
    || y+SHORT(patch->height)>SCREENHEIGHT )
    {
      I_Error("Patch at %d,%d exceeds LFB\n", x,y );
      // No I_Error abort - what is up with TNT.WAD?
      //fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
      //return;
    }
#endif 

    desttop = (uint16_t*)((uintptr_t)bufptr + (uintptr_t)((((y)*SCREENWIDTH)+x)*2));

    for ( ; col<w ; x++, col++)
    {
        column = (column_t *)((byte *)patch + LONG(patch->columnofs[w-1-col]));

        // step through the posts in a column
        while (column->topdelta != 0xff )
        {
            y+=column->topdelta;

            source = (byte *)column + 3;
            dest = (uint16_t*)((uintptr_t)desttop + (uintptr_t)(column->topdelta*SCREENWIDTH*2)); 
            count = column->length;

            while (count--)
            {
                uint16_t mapped_spot = palarray[*source++];
                *dest = mapped_spot;
                dest += SCREENWIDTH; 
            }

            column = (column_t *)(  (byte *)column + column->length + 4 );
        }
    }
}


//
// V_DrawBlock
// Draw a linear block of pixels into the view buffer.
//
void
V_DrawBlock
( int		x,
  int		y,
  int		width,
  int		height,
  uint16_t*		src )
{
    // hack for f_wipe
    memcpy(bufptr, src, 320*200*2);
}


//
// V_GetBlock
// Gets a linear block of pixels from the view buffer.
//
void
V_GetBlock
( int		x,
  int		y,
  int		width,
  int		height,
  uint16_t*		dest )
{
    // hack for f_wipe
    memcpy(dest, bufptr, 320*200*2);
}


//
// V_Init
//
void V_Init (void)
{
}
