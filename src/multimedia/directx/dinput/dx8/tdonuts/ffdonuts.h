//-----------------------------------------------------------------------------
// File: ffdonuts.h
//
// Desc: DirectInput ForceFeedback version of Donuts game
//
// Copyright (C) 1995-1999 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef FFDONUTS_H
#define FFDONUTS_H


// Program states
enum{ PS_SPLASH, PS_ACTIVE, PS_BEGINREST, PS_REST };

// Game object types
enum{ OBJ_DONUT=0, OBJ_PYRAMID, OBJ_CUBE, OBJ_SPHERE, OBJ_SHIP, OBJ_BULLET };




// Object dimensions and fixed properties
#define MAX_SCREEN_X      (g_dwScreenWidth-1)
#define MAX_SCREEN_Y      (g_dwScreenHeight-1)

#define DONUT_WIDTH    64
#define DONUT_HEIGHT   64
#define PYRAMID_WIDTH  32
#define PYRAMID_HEIGHT 32
#define SPHERE_WIDTH   16
#define SPHERE_HEIGHT  16
#define CUBE_WIDTH     16
#define CUBE_HEIGHT    16
#define SHIP_WIDTH     32
#define SHIP_HEIGHT    32
#define BULLET_WIDTH    3
#define BULLET_HEIGHT   3

#define MAX_DONUT_FRAME   30
#define MAX_PYRAMID_FRAME 40
#define MAX_SPHERE_FRAME  40
#define MAX_CUBE_FRAME    40
#define MAX_SHIP_FRAME    40
#define MAX_BULLET_FRAME  400

#define BULLET_XOFFSET   304
#define BULLET_YOFFSET    0

#define NO_PLAYER        -1




//-----------------------------------------------------------------------------
// Name: struct DisplayObject
// Desc: A game object that goes in the display list
//-----------------------------------------------------------------------------
struct DisplayObject
{
    DisplayObject* next;     // Link to next object
    DisplayObject* prev;     // Link to previous object
	BOOL   bVisible;         // Whether the object is visible
    SHORT  type;             // Type of object
    FLOAT  posx, posy;       // X and y position
    FLOAT  velx, vely;       // X and y velocity (pixels/second)
    FLOAT  frame;            // Current animation frame
    FLOAT  delay;            // Frame/second
    RECT   rcSrc, rcDst;     // Source and destination rects
    LPDIRECTDRAWSURFACE7 pddsSurface; // Surface containing bitmap
	int    Player;			 //which player the pbject belongs to -- only for bullets and ships
};


FLOAT Dirx[40] =
{
    0.000000f,
    0.156434f,
    0.309017f,
    0.453991f,
    0.587785f,
    0.707107f,
    0.809017f,
    0.891007f,
    0.951057f,
    0.987688f,
    1.000000f,
    0.987688f,
    0.951057f,
    0.891007f,
    0.809017f,
    0.707107f,
    0.587785f,
    0.453990f,
    0.309017f,
    0.156434f,
    0.000000f,
    -0.156435f,
    -0.309017f,
    -0.453991f,
    -0.587785f,
    -0.707107f,
    -0.809017f,
    -0.891007f,
    -0.951057f,
    -0.987688f,
    -1.000000f,
    -0.987688f,
    -0.951056f,
    -0.891006f,
    -0.809017f,
    -0.707107f,
    -0.587785f,
    -0.453990f,
    -0.309017f,
    -0.156434f
};

FLOAT Diry[40] =
{
    -1.000000f,
    -0.987688f,
    -0.951057f,
    -0.891007f,
    -0.809017f,
    -0.707107f,
    -0.587785f,
    -0.453990f,
    -0.309017f,
    -0.156434f,
    0.000000f,
    0.156434f,
    0.309017f,
    0.453991f,
    0.587785f,
    0.707107f,
    0.809017f,
    0.891007f,
    0.951057f,
    0.987688f,
    1.000000f,
    0.987688f,
    0.951057f,
    0.891006f,
    0.809017f,
    0.707107f,
    0.587785f,
    0.453990f,
    0.309017f,
    0.156434f,
    0.000000f,
    -0.156435f,
    -0.309017f,
    -0.453991f,
    -0.587785f,
    -0.707107f,
    -0.809017f,
    -0.891007f,
    -0.951057f,
    -0.987688f
};




//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
HRESULT InitializeGame( HWND hWnd );
HRESULT InitializeSound( HWND hWnd );
HRESULT InitializeInput( HWND hWnd );
VOID    DestroyGame();
VOID    DestroySound();
VOID    DestroyInput();


VOID    UpdateFrame(LPDIRECTDRAWSURFACE7 lpddsConfigDlg = NULL);
VOID    CleanupAndDisplayError( CHAR* strError );
HRESULT RestoreSurfaces();
BOOL    IsDisplayListEmpty();
BOOL    IsDisplayListEmptyExceptShips();
VOID    InitializeShips();
VOID    AdvanceLevel();
VOID    UpdateDisplayList();
VOID    DrawDisplayList(LPDIRECTDRAWSURFACE7 lpConfigDlgSurf = NULL);
FLOAT   rnd( FLOAT low=-1.0f, FLOAT high=1.0f );
VOID    CheckForHits();
VOID    BltBitmappedText( CHAR* strScore, int x, int y );
HRESULT DisplaySplashScreen();
VOID    EraseScreen();
HRESULT FlipScreen();
VOID    DisplayLevelIntroScreen( DWORD dwLevel );

DisplayObject* CreateObject( SHORT type, FLOAT x, FLOAT y, FLOAT vx, FLOAT vy, int Player );
VOID    AddToList( DisplayObject* pObject );
VOID    DeleteFromList( DisplayObject* pObject );

VOID   GetDeviceInput(DWORD input[]);

#endif
