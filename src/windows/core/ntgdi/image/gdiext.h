
/*++

Copyright (c) 1996-1999  Microsoft Corporation

Module Name

   gdiext.h

Abstract:

    GDI exensions for transparent blt, blending, gradient fill

Author:

   Mark Enstrom   (marke)  23-Jun-1996

Enviornment:

   User Mode

Revision History:

--*/


typedef LONG  PTFX8;
typedef USHORT COLOR16;

typedef struct _VERTEX
{
    PTFX8   x;
    PTFX8   y;
    COLOR16 Red;
    COLOR16 Green;
    COLOR16 Blue;
    COLOR16 Alpha;
}VERTEX,*PVERTEX;

BOOL
TransparentImage(
    HDC      hdcDest,
    LONG     DstX,
    LONG     DstY,
    LONG     DstCx,
    LONG     DstCy,
    HANDLE   hSrc,
    LONG     SrcX,
    LONG     SrcY,
    LONG     SrcCx,
    LONG     SrcCy
    );


BOOL
AlphaImage(
    HDC      hdcDest,
    LONG     DstX,
    LONG     DstY,
    LONG     DstCx,
    LONG     DstCy,
    HANDLE   hSrc,
    LONG     SrcX,
    LONG     SrcY,
    LONG     SrcCx,
    LONG     SrcCy,
    ULONG    fAlpha
    );

BOOL
StretchImage(
    HDC      hdcDest,
    LONG     DstX,
    LONG     DstY,
    LONG     DstCx,
    LONG     DstCy,
    HANDLE   hSrc,
    LONG     SrcX,
    LONG     SrcY,
    LONG     SrcCx,
    LONG     SrcCy
    );


BOOL
TriangleMesh(
    HDC       hdc,
    PVERTEX   pVertex,
    ULONG     nVertex,
    PULONG    pMesh,
    ULONG     nCount
    );







