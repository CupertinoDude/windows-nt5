//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Microsoft Corporation.  All Rights Reserved.
//
//  File:       d3dx8mesh.h
//  Content:    D3DX mesh types and functions
//
//////////////////////////////////////////////////////////////////////////////

#include "d3dx8.h"

#ifndef __D3DX8MESH_H__
#define __D3DX8MESH_H__

#include "dxfile.h"     // defines LPDIRECTXFILEDATA

// {A69BA991-1F7D-11d3-B929-00C04F68DC23}
DEFINE_GUID(IID_ID3DXBaseMesh,
0xa69ba991, 0x1f7d, 0x11d3, 0xb9, 0x29, 0x0, 0xc0, 0x4f, 0x68, 0xdc, 0x23);

// {9D84AC46-6B90-49a9-A721-085C7A3E3DAE}
DEFINE_GUID(IID_ID3DXMesh, 
0x9d84ac46, 0x6b90, 0x49a9, 0xa7, 0x21, 0x8, 0x5c, 0x7a, 0x3e, 0x3d, 0xae);

// {15981AA8-1A05-48e3-BBE7-FF5D49654E3F}
DEFINE_GUID(IID_ID3DXPMesh, 
0x15981aa8, 0x1a05, 0x48e3, 0xbb, 0xe7, 0xff, 0x5d, 0x49, 0x65, 0x4e, 0x3f);

// {BC3BBDE2-1F7D-11d3-B929-00C04F68DC23}
DEFINE_GUID(IID_ID3DXSPMesh, 
0xbc3bbde2, 0x1f7d, 0x11d3, 0xb9, 0x29, 0x0, 0xc0, 0x4f, 0x68, 0xdc, 0x23);

// {82A53844-F322-409e-A2E9-992E1104069D}
DEFINE_GUID(IID_ID3DXSkinMesh, 
0x82a53844, 0xf322, 0x409e, 0xa2, 0xe9, 0x99, 0x2e, 0x11, 0x4, 0x6, 0x9d);


// Mesh options - lower 3 bytes only, upper byte used by _D3DXMESHOPT option flags
enum _D3DXMESH {
    D3DXMESH_32BIT                  = 0x001, // If set, then use 32 bit indices, if not set use 16 bit indices.
    D3DXMESH_DONOTCLIP              = 0x002, // Use D3DUSAGE_DONOTCLIP for VB & IB.
    D3DXMESH_POINTS                 = 0x004, // Use D3DUSAGE_POINTS for VB & IB. 
    D3DXMESH_RTPATCHES              = 0x008, // Use D3DUSAGE_RTPATCHES for VB & IB. 
    D3DXMESH_NPATCHES               = 0x4000,// Use D3DUSAGE_NPATCHES for VB & IB. 
    D3DXMESH_VB_SYSTEMMEM           = 0x010, // Use D3DPOOL_SYSTEMMEM for VB. Overrides D3DXMESH_MANAGEDVERTEXBUFFER
    D3DXMESH_VB_MANAGED             = 0x020, // Use D3DPOOL_MANAGED for VB. 
    D3DXMESH_VB_WRITEONLY           = 0x040, // Use D3DUSAGE_WRITEONLY for VB.
    D3DXMESH_VB_DYNAMIC             = 0x080, // Use D3DUSAGE_DYNAMIC for VB.
    D3DXMESH_VB_SOFTWAREPROCESSING = 0x8000, // Use D3DUSAGE_SOFTWAREPROCESSING for VB.
    D3DXMESH_IB_SYSTEMMEM           = 0x100, // Use D3DPOOL_SYSTEMMEM for IB. Overrides D3DXMESH_MANAGEDINDEXBUFFER
    D3DXMESH_IB_MANAGED             = 0x200, // Use D3DPOOL_MANAGED for IB.
    D3DXMESH_IB_WRITEONLY           = 0x400, // Use D3DUSAGE_WRITEONLY for IB.
    D3DXMESH_IB_DYNAMIC             = 0x800, // Use D3DUSAGE_DYNAMIC for IB.
    D3DXMESH_IB_SOFTWAREPROCESSING= 0x10000, // Use D3DUSAGE_SOFTWAREPROCESSING for IB.

    D3DXMESH_VB_SHARE               = 0x1000, // Valid for Clone* calls only, forces cloned mesh/pmesh to share vertex buffer

    D3DXMESH_USEHWONLY              = 0x2000, // Valid for ID3DXSkinMesh::ConvertToBlendedMesh

    // Helper options
    D3DXMESH_SYSTEMMEM              = 0x110, // D3DXMESH_VB_SYSTEMMEM | D3DXMESH_IB_SYSTEMMEM
    D3DXMESH_MANAGED                = 0x220, // D3DXMESH_VB_MANAGED | D3DXMESH_IB_MANAGED
    D3DXMESH_WRITEONLY              = 0x440, // D3DXMESH_VB_WRITEONLY | D3DXMESH_IB_WRITEONLY
    D3DXMESH_DYNAMIC                = 0x880, // D3DXMESH_VB_DYNAMIC | D3DXMESH_IB_DYNAMIC
    D3DXMESH_SOFTWAREPROCESSING   = 0x18000, // D3DXMESH_VB_SOFTWAREPROCESSING | D3DXMESH_IB_SOFTWAREPROCESSING

;begin_internal
    D3DXMESH_VALIDBITS              = 0x1cfff,
    D3DXMESH_VALIDCLONEBITS         = 0x1dfff,
    D3DXMESH_VALIDBLENDEDBITS       = 0x1efff,
;end_internal
};

// option field values for specifying min value in D3DXGeneratePMesh and D3DXSimplifyMesh
enum _D3DXMESHSIMP
{
    D3DXMESHSIMP_VERTEX   = 0x1,
    D3DXMESHSIMP_FACE     = 0x2,

;begin_internal
    D3DXMESHSIMP_VALIDBITS =0x03
;end_internal
};

enum _MAX_FVF_DECL_SIZE
{
    MAX_FVF_DECL_SIZE = 20
};

typedef struct ID3DXBaseMesh *LPD3DXBASEMESH;
typedef struct ID3DXMesh *LPD3DXMESH;
typedef struct ID3DXPMesh *LPD3DXPMESH;
typedef struct ID3DXSPMesh *LPD3DXSPMESH;
typedef struct ID3DXSkinMesh *LPD3DXSKINMESH;

typedef struct _D3DXATTRIBUTERANGE
{
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
} D3DXATTRIBUTERANGE;

typedef D3DXATTRIBUTERANGE* LPD3DXATTRIBUTERANGE;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
struct D3DXMATERIAL
{
    D3DMATERIAL8  MatD3D;
    LPSTR         pTextureFilename;
};
typedef struct D3DXMATERIAL *LPD3DXMATERIAL;
#ifdef __cplusplus
}
#endif //__cplusplus

typedef struct _D3DXATTRIBUTEWEIGHTS
{
    FLOAT Position;
    FLOAT Boundary;
    FLOAT Normal;
    FLOAT Diffuse;
    FLOAT Specular;
    FLOAT Tex[8];
} D3DXATTRIBUTEWEIGHTS;

typedef D3DXATTRIBUTEWEIGHTS* LPD3DXATTRIBUTEWEIGHTS;

enum _D3DXWELDEPSILONSFLAGS
{
    D3DXWELDEPSILONS_WELDALL = 0x1,              // weld all vertices marked by adjacency as being overlapping

    D3DXWELDEPSILONS_WELDPARTIALMATCHES = 0x2,   // if a given vertex component is within epsilon, modify partial matched 
                                                    // vertices so that both components identical AND if all components "equal"
                                                    // remove one of the vertices
    D3DXWELDEPSILONS_DONOTREMOVEVERTICES = 0x4,  // instructs weld to only allow modifications to vertices and not removal
                                                    // ONLY valid if D3DXWELDEPSILONS_WELDPARTIALMATCHES is set
                                                    // useful to modify vertices to be equal, but not allow vertices to be removed
};

typedef struct _D3DXWELDEPSILONS
{
    FLOAT SkinWeights;
    FLOAT Normal;
    FLOAT Tex[8];
    DWORD Flags;
} D3DXWELDEPSILONS;

typedef D3DXWELDEPSILONS* LPD3DXWELDEPSILONS;


#undef INTERFACE
#define INTERFACE ID3DXBaseMesh

DECLARE_INTERFACE_(ID3DXBaseMesh, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBaseMesh
    STDMETHOD(DrawSubset)(THIS_ DWORD AttribId) PURE;
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ DWORD Declaration[MAX_FVF_DECL_SIZE]) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE8* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER8* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER8* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(GetAttributeTable)(
                THIS_ D3DXATTRIBUTERANGE *pAttribTable, DWORD* pAttribTableSize) PURE;

    STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ CONST DWORD* pPRep, DWORD* pAdjacency) PURE;
    STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ CONST DWORD* pAdjacency, DWORD* pPRep) PURE;
    STDMETHOD(GenerateAdjacency)(THIS_ FLOAT Epsilon, DWORD* pAdjacency) PURE;
};


#undef INTERFACE
#define INTERFACE ID3DXMesh

DECLARE_INTERFACE_(ID3DXMesh, ID3DXBaseMesh)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBaseMesh
    STDMETHOD(DrawSubset)(THIS_ DWORD AttribId) PURE;
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ DWORD Declaration[MAX_FVF_DECL_SIZE]) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE8* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER8* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER8* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(GetAttributeTable)(
                THIS_ D3DXATTRIBUTERANGE *pAttribTable, DWORD* pAttribTableSize) PURE;

    STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ CONST DWORD* pPRep, DWORD* pAdjacency) PURE;
    STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ CONST DWORD* pAdjacency, DWORD* pPRep) PURE;
    STDMETHOD(GenerateAdjacency)(THIS_ FLOAT Epsilon, DWORD* pAdjacency) PURE;

    // ID3DXMesh
    STDMETHOD(LockAttributeBuffer)(THIS_ DWORD Flags, DWORD** ppData) PURE;
    STDMETHOD(UnlockAttributeBuffer)(THIS) PURE;
    STDMETHOD(Optimize)(THIS_ DWORD Flags, CONST DWORD* pAdjacencyIn, DWORD* pAdjacencyOut, 
                     DWORD* pFaceRemap, LPD3DXBUFFER *ppVertexRemap,  
                     LPD3DXMESH* ppOptMesh) PURE;
    STDMETHOD(OptimizeInplace)(THIS_ DWORD Flags, CONST DWORD* pAdjacencyIn, DWORD* pAdjacencyOut, 
                     DWORD* pFaceRemap, LPD3DXBUFFER *ppVertexRemap) PURE;

};


#undef INTERFACE
#define INTERFACE ID3DXPMesh

DECLARE_INTERFACE_(ID3DXPMesh, ID3DXBaseMesh)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBaseMesh
    STDMETHOD(DrawSubset)(THIS_ DWORD AttribId) PURE;
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ DWORD Declaration[MAX_FVF_DECL_SIZE]) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE8* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER8* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER8* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD Flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(GetAttributeTable)(
                THIS_ D3DXATTRIBUTERANGE *pAttribTable, DWORD* pAttribTableSize) PURE;

    STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ CONST DWORD* pPRep, DWORD* pAdjacency) PURE;
    STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ CONST DWORD* pAdjacency, DWORD* pPRep) PURE;
    STDMETHOD(GenerateAdjacency)(THIS_ FLOAT Epsilon, DWORD* pAdjacency) PURE;

    // ID3DXPMesh
    STDMETHOD(ClonePMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3D, LPD3DXPMESH* ppCloneMesh) PURE;
    STDMETHOD(ClonePMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3D, LPD3DXPMESH* ppCloneMesh) PURE;
    STDMETHOD(SetNumFaces)(THIS_ DWORD Faces) PURE;
    STDMETHOD(SetNumVertices)(THIS_ DWORD Vertices) PURE;
    STDMETHOD_(DWORD, GetMaxFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetMinFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetMaxVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetMinVertices)(THIS) PURE;
    STDMETHOD(Save)(THIS_ IStream *pStream, LPD3DXMATERIAL pMaterials, DWORD NumMaterials) PURE;

    STDMETHOD(Optimize)(THIS_ DWORD Flags, DWORD* pAdjacencyOut, 
                     DWORD* pFaceRemap, LPD3DXBUFFER *ppVertexRemap,  
                     LPD3DXMESH* ppOptMesh) PURE;

    STDMETHOD(OptimizeBaseLOD)(THIS_ DWORD Flags, DWORD* pFaceRemap) PURE;
    STDMETHOD(TrimByFaces)(THIS_ DWORD NewFacesMin, DWORD NewFacesMax, DWORD *rgiFaceRemap, DWORD *rgiVertRemap) PURE;
    STDMETHOD(TrimByVertices)(THIS_ DWORD NewVerticesMin, DWORD NewVerticesMax, DWORD *rgiFaceRemap, DWORD *rgiVertRemap) PURE;

    STDMETHOD(GetAdjacency)(THIS_ DWORD* pAdjacency) PURE;
};


#undef INTERFACE
#define INTERFACE ID3DXSPMesh

DECLARE_INTERFACE_(ID3DXSPMesh, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXSPMesh
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ DWORD Declaration[MAX_FVF_DECL_SIZE]) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE8* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3D, DWORD *pAdjacencyOut, DWORD *pVertexRemapOut, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3DDevice, DWORD *pAdjacencyOut, DWORD *pVertexRemapOut, LPD3DXMESH* ppCloneMesh) PURE;
    STDMETHOD(ClonePMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE8 pD3D, DWORD *pVertexRemapOut, LPD3DXPMESH* ppCloneMesh) PURE;
    STDMETHOD(ClonePMesh)(THIS_ DWORD Options, 
                CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3D, DWORD *pVertexRemapOut, LPD3DXPMESH* ppCloneMesh) PURE;
    STDMETHOD(ReduceFaces)(THIS_ DWORD Faces) PURE;
    STDMETHOD(ReduceVertices)(THIS_ DWORD Vertices) PURE;
    STDMETHOD_(DWORD, GetMaxFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetMaxVertices)(THIS) PURE;
    STDMETHOD(GetVertexAttributeWeights)(THIS_ LPD3DXATTRIBUTEWEIGHTS pVertexAttributeWeights) PURE;
    STDMETHOD(GetVertexWeights)(THIS_ FLOAT *pVertexWeights) PURE;
};

#define UNUSED16 (0xffff)
#define UNUSED32 (0xffffffff)

// ID3DXMesh::Optimize options - upper byte only, lower 3 bytes used from _D3DXMESH option flags
enum _D3DXMESHOPT {
    D3DXMESHOPT_COMPACT       = 0x01000000,
    D3DXMESHOPT_ATTRSORT      = 0x02000000,
    D3DXMESHOPT_VERTEXCACHE   = 0x04000000,
    D3DXMESHOPT_STRIPREORDER  = 0x08000000,
    D3DXMESHOPT_IGNOREVERTS   = 0x10000000,  // optimize faces only, don't touch vertices
    D3DXMESHOPT_SHAREVB       = 0x20000000,  
;begin_internal
    D3DXMESHOPT_VALIDBITS     = 0x3f000000,

    D3DXMESHOPTINT_ALREADYATTRSORTED = 0x40000000,
    D3DXMESHOPTINT_INVERTFACEREMAP   = 0x80000000,
    D3DXMESHOPTINT_VALIDBITS         = 0xff000000,
;end_internal
};

// Subset of the mesh that has the same attribute and bone combination.
// This subset can be rendered in a single draw call
typedef struct _D3DXBONECOMBINATION
{
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
    DWORD* BoneId;
} D3DXBONECOMBINATION, *LPD3DXBONECOMBINATION;


#undef INTERFACE
#define INTERFACE ID3DXSkinMesh

DECLARE_INTERFACE_(ID3DXSkinMesh, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXMesh
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ DWORD Declaration[MAX_FVF_DECL_SIZE]) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE8* ppDevice) PURE;
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER8* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER8* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD flags, BYTE** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(LockAttributeBuffer)(THIS_ DWORD flags, DWORD** ppData) PURE;
    STDMETHOD(UnlockAttributeBuffer)(THIS) PURE;
    // ID3DXSkinMesh
    STDMETHOD_(DWORD, GetNumBones)(THIS) PURE;
    STDMETHOD(GetOriginalMesh)(THIS_ LPD3DXMESH* ppMesh) PURE;
    STDMETHOD(SetBoneInfluence)(THIS_ DWORD bone, DWORD numInfluences, CONST DWORD* vertices, CONST FLOAT* weights) PURE;
    STDMETHOD_(DWORD, GetNumBoneInfluences)(THIS_ DWORD bone) PURE;
    STDMETHOD(GetBoneInfluence)(THIS_ DWORD bone, DWORD* vertices, FLOAT* weights) PURE;
    STDMETHOD(GetMaxVertexInfluences)(THIS_ DWORD* maxVertexInfluences) PURE;
    STDMETHOD(GetMaxFaceInfluences)(THIS_ DWORD* maxFaceInfluences) PURE;

    STDMETHOD(ConvertToBlendedMesh)(THIS_ DWORD Options, 
                                    CONST LPDWORD pAdjacencyIn, 
                                    LPDWORD pAdjacencyOut,
                                    DWORD* pNumBoneCombinations, 
                                    LPD3DXBUFFER* ppBoneCombinationTable, 
                                    DWORD* pFaceRemap, 
                                    LPD3DXBUFFER *ppVertexRemap, 
                                    LPD3DXMESH* ppMesh) PURE;

    STDMETHOD(ConvertToIndexedBlendedMesh)(THIS_ DWORD Options, 
                                           CONST LPDWORD pAdjacencyIn, 
                                           DWORD paletteSize, 
                                           LPDWORD pAdjacencyOut, 
                                           DWORD* pNumBoneCombinations, 
                                           LPD3DXBUFFER* ppBoneCombinationTable, 
                                           DWORD* pFaceRemap, 
                                           LPD3DXBUFFER *ppVertexRemap, 
                                           LPD3DXMESH* ppMesh) PURE;

    STDMETHOD(GenerateSkinnedMesh)(THIS_ DWORD Options, 
                                   FLOAT minWeight, 
                                   CONST LPDWORD pAdjacencyIn, 
                                   LPDWORD pAdjacencyOut, 
                                   DWORD* pFaceRemap, 
                                   LPD3DXBUFFER *ppVertexRemap, 
                                   LPD3DXMESH* ppMesh) PURE;
    STDMETHOD(UpdateSkinnedMesh)(THIS_ CONST D3DXMATRIX* pBoneTransforms, CONST D3DXMATRIX* pBoneInvTransforms, LPD3DXMESH pMesh) PURE;
};

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI 
    D3DXCreateMesh(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        CONST DWORD *pDeclaration, 
        LPDIRECT3DDEVICE8 pD3D, 
        LPD3DXMESH* ppMesh);

HRESULT WINAPI 
    D3DXCreateMeshFVF(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        DWORD FVF, 
        LPDIRECT3DDEVICE8 pD3D, 
        LPD3DXMESH* ppMesh);

HRESULT WINAPI 
    D3DXCreateSPMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST LPD3DXATTRIBUTEWEIGHTS pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        LPD3DXSPMESH* ppSMesh);

// clean a mesh up for simplification, try to make manifold
HRESULT WINAPI
    D3DXCleanMesh(
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacencyIn,
    LPD3DXMESH* ppMeshOut,
    DWORD* pAdjacencyOut,
    LPD3DXBUFFER* ppErrorsAndWarnings);

HRESULT WINAPI
    D3DXValidMesh(
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacency,
    LPD3DXBUFFER* ppErrorsAndWarnings);

HRESULT WINAPI 
    D3DXGeneratePMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST LPD3DXATTRIBUTEWEIGHTS pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXPMESH* ppPMesh);

HRESULT WINAPI 
    D3DXSimplifyMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST LPD3DXATTRIBUTEWEIGHTS pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXMESH* ppMesh);

HRESULT WINAPI 
    D3DXComputeBoundingSphere(
        PVOID pPointsFVF, 
        DWORD NumVertices, 
        DWORD FVF,
        D3DXVECTOR3 *pCenter, 
        FLOAT *pRadius);

HRESULT WINAPI 
    D3DXComputeBoundingBox(
        PVOID pPointsFVF, 
        DWORD NumVertices, 
        DWORD FVF,
        D3DXVECTOR3 *pMin, 
        D3DXVECTOR3 *pMax);

HRESULT WINAPI 
    D3DXComputeNormals(
        LPD3DXBASEMESH pMesh,
        DWORD *pAdjacency);

HRESULT WINAPI 
    D3DXCreateBuffer(
        DWORD NumBytes, 
        LPD3DXBUFFER *ppBuffer);


HRESULT WINAPI
    D3DXLoadMeshFromX(
        LPSTR pFilename, 
        DWORD Options, 
        LPDIRECT3DDEVICE8 pD3D, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

HRESULT WINAPI 
    D3DXSaveMeshToX(
        LPSTR pFilename,
        LPD3DXMESH pMesh,
        CONST DWORD* pAdjacency,
        CONST LPD3DXMATERIAL pMaterials,
        DWORD NumMaterials,
        DWORD Format
        );

HRESULT WINAPI 
    D3DXCreatePMeshFromStream(
        IStream *pStream, 
        DWORD Options,
        LPDIRECT3DDEVICE8 pD3DDevice, 
        LPD3DXBUFFER *ppMaterials,
        DWORD* pNumMaterials,
        LPD3DXPMESH *ppPMesh);

HRESULT WINAPI
    D3DXCreateSkinMesh(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD NumBones,
        DWORD Options, 
        CONST DWORD *pDeclaration, 
        LPDIRECT3DDEVICE8 pD3D, 
        LPD3DXSKINMESH* ppSkinMesh);

HRESULT WINAPI
    D3DXCreateSkinMeshFVF(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD NumBones,
        DWORD Options, 
        DWORD FVF, 
        LPDIRECT3DDEVICE8 pD3D, 
        LPD3DXSKINMESH* ppSkinMesh);

HRESULT WINAPI
    D3DXCreateSkinMeshFromMesh(
        LPD3DXMESH pMesh,
        DWORD numBones,
        LPD3DXSKINMESH* ppSkinMesh);

HRESULT WINAPI 
    D3DXLoadMeshFromXof(
        LPDIRECTXFILEDATA pXofObjMesh, 
        DWORD Options, 
        LPDIRECT3DDEVICE8 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

HRESULT WINAPI
    D3DXLoadSkinMeshFromXof(
        LPDIRECTXFILEDATA pxofobjMesh, 
        DWORD Options,
        LPDIRECT3DDEVICE8 pD3D,
        LPD3DXBUFFER* ppAdjacency,
        LPD3DXBUFFER* ppMaterials,
        DWORD *pMatOut,
        LPD3DXBUFFER* ppBoneNames,
        LPD3DXBUFFER* ppBoneTransforms,
        LPD3DXSKINMESH* ppMesh);

HRESULT WINAPI
    D3DXTessellateNPatches(
        LPD3DXMESH pMeshIn,             
        CONST DWORD* pAdjacencyIn,             
        FLOAT NumSegs,                    
        BOOL  QuadraticInterpNormals,     // if false use linear intrep for normals, if true use quadratic
        LPD3DXMESH *ppMeshOut,
        LPD3DXBUFFER *ppAdjacencyOut);

UINT WINAPI
    D3DXGetFVFVertexSize(DWORD FVF);

HRESULT WINAPI
    D3DXDeclaratorFromFVF(
        DWORD FVF,
        DWORD Declaration[MAX_FVF_DECL_SIZE]);

HRESULT WINAPI
    D3DXFVFFromDeclarator(
        CONST DWORD *pDeclarator,
        DWORD *pFVF);

HRESULT WINAPI 
    D3DXWeldVertices(
        CONST LPD3DXMESH pMesh,         
        LPD3DXWELDEPSILONS pEpsilons,                 
        CONST DWORD *pAdjacencyIn, 
        DWORD *pAdjacencyOut,
        DWORD* pFaceRemap, 
        LPD3DXBUFFER *ppVertexRemap);

typedef struct _D3DXINTERSECTINFO
{
    DWORD FaceIndex;                // index of face intersected
    FLOAT U;                        // Barycentric Hit Coordinates    
    FLOAT V;                        // Barycentric Hit Coordinates
    FLOAT Dist;                     // Ray-Intersection Parameter Distance
} D3DXINTERSECTINFO, *LPD3DXINTERSECTINFO;


HRESULT WINAPI
    D3DXIntersect(
        LPD3DXBASEMESH pMesh,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array

HRESULT WINAPI
    D3DXIntersectSubset(
        LPD3DXBASEMESH pMesh,
        DWORD AttribId,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array


HRESULT WINAPI D3DXSplitMesh
    (
    CONST LPD3DXMESH pMeshIn,         
    CONST DWORD *pAdjacencyIn, 
    CONST DWORD MaxSize,
    CONST DWORD Options,
    DWORD *pMeshesOut,
    LPD3DXBUFFER *ppMeshArrayOut,
    LPD3DXBUFFER *ppAdjacencyArrayOut,
    LPD3DXBUFFER *ppFaceRemapArrayOut,
    LPD3DXBUFFER *ppVertRemapArrayOut
    );

BOOL D3DXIntersectTri 
(
    CONST D3DXVECTOR3 *p0,           // Triangle vertex 0 position
    CONST D3DXVECTOR3 *p1,           // Triangle vertex 1 position
    CONST D3DXVECTOR3 *p2,           // Triangle vertex 2 position
    CONST D3DXVECTOR3 *pRayPos,      // Ray origin
    CONST D3DXVECTOR3 *pRayDir,      // Ray direction
    FLOAT *pU,                       // Barycentric Hit Coordinates
    FLOAT *pV,                       // Barycentric Hit Coordinates
    FLOAT *pDist);                   // Ray-Intersection Parameter Distance

BOOL WINAPI
    D3DXSphereBoundProbe(
        CONST D3DXVECTOR3 *pCenter,
        FLOAT Radius,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);

BOOL WINAPI 
    D3DXBoxBoundProbe(
        CONST D3DXVECTOR3 *pMin, 
        CONST D3DXVECTOR3 *pMax,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);

enum _D3DXERR {
    D3DXERR_CANNOTMODIFYINDEXBUFFER     = MAKE_DDHRESULT(2900),
    D3DXERR_INVALIDMESH                 = MAKE_DDHRESULT(2901),
    D3DXERR_CANNOTATTRSORT              = MAKE_DDHRESULT(2902),
    D3DXERR_SKINNINGNOTSUPPORTED        = MAKE_DDHRESULT(2903),
    D3DXERR_TOOMANYINFLUENCES           = MAKE_DDHRESULT(2904),
    D3DXERR_INVALIDDATA                 = MAKE_DDHRESULT(2905),
    D3DXERR_LOADEDMESHASNODATA          = MAKE_DDHRESULT(2906),
};


#define D3DX_COMP_TANGENT_NONE 0xFFFFFFFF

HRESULT WINAPI D3DXComputeTangent(LPD3DXMESH InMesh,
                                 DWORD TexStage,
                                 LPD3DXMESH OutMesh,
                                 DWORD TexStageUVec,
                                 DWORD TexStageVVec,
                                 DWORD Wrap,
                                 DWORD *Adjacency);

HRESULT WINAPI
D3DXConvertMeshSubsetToSingleStrip
(
    LPD3DXBASEMESH MeshIn,
    DWORD AttribId,
    DWORD IBOptions,
    LPDIRECT3DINDEXBUFFER8 *ppIndexBuffer,
    DWORD *pNumIndices
);

HRESULT WINAPI
D3DXConvertMeshSubsetToStrips
(
    LPD3DXBASEMESH MeshIn,
    DWORD AttribId,
    DWORD IBOptions,
    LPDIRECT3DINDEXBUFFER8 *ppIndexBuffer,
    DWORD *pNumIndices,
    LPD3DXBUFFER *ppStripLengths,
    DWORD *pNumStrips
);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__D3DX8MESH_H__

