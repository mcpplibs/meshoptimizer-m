// mcpplibs.mwrap.meshoptimizer — meshoptimizer, the mesh optimization library, as a C++
// module. Every exported name is the upstream global from <meshoptimizer.h>, unchanged: the C
// functions and the header's C++ overloads over index types under one name each.
// Written by tools/surface.py from the header (tools/check_exports.py holds the two together).
module;
#include <meshoptimizer.h>

export module mcpplibs.mwrap.meshoptimizer;

export import mcpplibs.mwrap.meshoptimizer.macros;

// structs, the class, enums
export using ::meshopt_Stream;
export using ::meshopt_EncodeExpMode;
export using ::meshopt_VertexCacheStatistics;
export using ::meshopt_VertexFetchStatistics;
export using ::meshopt_OverdrawStatistics;
export using ::meshopt_CoverageStatistics;
export using ::meshopt_Meshlet;
export using ::meshopt_Bounds;
export using ::meshopt_Allocator;

// enumerators
export using ::meshopt_EncodeExpSeparate;
export using ::meshopt_EncodeExpSharedVector;
export using ::meshopt_EncodeExpSharedComponent;
export using ::meshopt_EncodeExpClamped;
export using ::meshopt_SimplifyLockBorder;
export using ::meshopt_SimplifySparse;
export using ::meshopt_SimplifyErrorAbsolute;
export using ::meshopt_SimplifyPrune;
export using ::meshopt_SimplifyRegularize;
export using ::meshopt_SimplifyPermissive;
export using ::meshopt_SimplifyRegularizeLight;
export using ::meshopt_SimplifyPreserveFolds;
export using ::meshopt_SimplifyErrorClamped;
export using ::meshopt_SimplifyVertex_Lock;
export using ::meshopt_SimplifyVertex_Protect;
export using ::meshopt_SimplifyVertex_Priority;
export using ::meshopt_TangentCompatible;
export using ::meshopt_TangentZeroFallback;
export using ::meshopt_RemeshShell;
export using ::meshopt_RemeshSolve;

// functions
export using ::meshopt_generateVertexRemap;
export using ::meshopt_generateVertexRemapMulti;
export using ::meshopt_generateVertexRemapCustom;
export using ::meshopt_remapVertexBuffer;
export using ::meshopt_remapIndexBuffer;
export using ::meshopt_filterIndexBuffer;
export using ::meshopt_filterIndexBufferMulti;
export using ::meshopt_generateShadowIndexBuffer;
export using ::meshopt_generateShadowIndexBufferMulti;
export using ::meshopt_generatePositionRemap;
export using ::meshopt_generateAdjacencyIndexBuffer;
export using ::meshopt_generateTessellationIndexBuffer;
export using ::meshopt_generateProvokingIndexBuffer;
export using ::meshopt_optimizeVertexCache;
export using ::meshopt_optimizeVertexCacheStrip;
export using ::meshopt_optimizeVertexCacheFifo;
export using ::meshopt_optimizeOverdraw;
export using ::meshopt_optimizeVertexFetch;
export using ::meshopt_optimizeVertexFetchRemap;
export using ::meshopt_encodeIndexBuffer;
export using ::meshopt_encodeIndexBufferBound;
export using ::meshopt_encodeIndexVersion;
export using ::meshopt_decodeIndexBuffer;
export using ::meshopt_decodeIndexVersion;
export using ::meshopt_encodeIndexSequence;
export using ::meshopt_encodeIndexSequenceBound;
export using ::meshopt_decodeIndexSequence;
export using ::meshopt_encodeMeshlet;
export using ::meshopt_encodeMeshletBound;
export using ::meshopt_decodeMeshlet;
export using ::meshopt_decodeMeshletRaw;
export using ::meshopt_encodeVertexBuffer;
export using ::meshopt_encodeVertexBufferBound;
export using ::meshopt_encodeVertexBufferLevel;
export using ::meshopt_encodeVertexVersion;
export using ::meshopt_decodeVertexBuffer;
export using ::meshopt_decodeVertexVersion;
export using ::meshopt_decodeFilterOct;
export using ::meshopt_decodeFilterQuat;
export using ::meshopt_decodeFilterExp;
export using ::meshopt_decodeFilterColor;
export using ::meshopt_encodeFilterOct;
export using ::meshopt_encodeFilterQuat;
export using ::meshopt_encodeFilterExp;
export using ::meshopt_encodeFilterColor;
export using ::meshopt_simplify;
export using ::meshopt_simplifyWithAttributes;
export using ::meshopt_simplifyWithUpdate;
export using ::meshopt_simplifySloppy;
export using ::meshopt_simplifyPrune;
export using ::meshopt_simplifyPoints;
export using ::meshopt_simplifyScale;
export using ::meshopt_stripify;
export using ::meshopt_stripifyBound;
export using ::meshopt_unstripify;
export using ::meshopt_unstripifyBound;
export using ::meshopt_analyzeVertexCache;
export using ::meshopt_analyzeVertexFetch;
export using ::meshopt_analyzeOverdraw;
export using ::meshopt_analyzeCoverage;
export using ::meshopt_buildMeshlets;
export using ::meshopt_buildMeshletsScan;
export using ::meshopt_buildMeshletsBound;
export using ::meshopt_buildMeshletsFlex;
export using ::meshopt_buildMeshletsSpatial;
export using ::meshopt_optimizeMeshlet;
export using ::meshopt_optimizeMeshletLevel;
export using ::meshopt_computeClusterBounds;
export using ::meshopt_computeMeshletBounds;
export using ::meshopt_computeSphereBounds;
export using ::meshopt_extractMeshletIndices;
export using ::meshopt_partitionClusters;
export using ::meshopt_spatialSortRemap;
export using ::meshopt_spatialSortTriangles;
export using ::meshopt_spatialClusterPoints;
export using ::meshopt_opacityMapMeasure;
export using ::meshopt_opacityMapRasterize;
export using ::meshopt_opacityMapEntrySize;
export using ::meshopt_opacityMapCompact;
export using ::meshopt_generateTangents;
export using ::meshopt_generateNormals;
export using ::meshopt_remesh;
export using ::meshopt_quantizeHalf;
export using ::meshopt_quantizeFloat;
export using ::meshopt_dequantizeHalf;
export using ::meshopt_computePositionExponent;
export using ::meshopt_setAllocator;
export using ::meshopt_quantizeUnorm;
export using ::meshopt_quantizeSnorm;
