#pragma once
#include <stdint.h>
#include <fstream>

#define W3D_MAKE_VERSION(major,minor)   (((major) << 16) | (minor))
#define W3D_GET_MAJOR_VERSION(ver)      ((ver)>>16)
#define W3D_GET_MINOR_VERSION(ver)      ((ver) & 0xFFFF)
#define W3D_NAME_LEN    16
#define W3D_TYPE_LEN    32

namespace libw3d
{
	class Chunk
	{
	public:
		enum Type : uint32_t
		{
			MESH = 0x00000000,  // Mesh definition
			VERTICES = 0x00000002,  // array of vertices (array of W3dVectorStruct's)
			VERTICES_COPY = 0x00000C00,  // array of vertices (array of W3dVectorStruct's for BFME)
			VERTEX_NORMALS = 0x00000003,  // array of normals  (array of W3dVectorStruct's)
			VERTEX_NORMALS_COPY = 0x00000C01,  // array of normals  (array of W3dVectorStruct's for BFME)

			MESH_USER_TEXT = 0x0000000C,  // Text from the MAX comment field (Null terminated string)
			VERTEX_INFLUENCES = 0x0000000E,  // Mesh Deformation vertex connections (array of W3dVertInfStruct's)
			MESH_HEADER3 = 0x0000001F,  // Mesh header contains general info about the mesh. (W3dMeshHeader3Struct)
			TRIANGLES = 0x00000020,  // New improved triangles chunk (array of W3dTriangleStruct's)
			VERTEX_SHADE_INDICES = 0x00000022,  // shade indexes for each vertex (array of uint32's)

			PRELIT_UNLIT = 0x00000023,  // optional unlit material chunk wrapper
			PRELIT_VERTEX = 0x00000024,  // optional vertex-lit material chunk wrapper
			PRELIT_LIGHTMAP_MULTI_PASS = 0x00000025,  // optional lightmapped multi-pass material chunk wrapper
			PRELIT_LIGHTMAP_MULTI_TEXTURE = 0x00000026,  // optional lightmapped multi-texture material chunk wrapper

			MATERIAL_INFO = 0x00000028,  // materials information, pass count, etc (contains W3dMaterialInfoStruct)
			SHADERS = 0x00000029,  // shaders (array of W3dShaderStruct's)

			VERTEX_MATERIALS = 0x0000002A,  // wraps the vertex materials
			VERTEX_MATERIAL = 0x0000002B,
			VERTEX_MATERIAL_NAME = 0x0000002C,  // vertex material name (NULL-terminated string)
			VERTEX_MATERIAL_INFO = 0x0000002D,  // W3dVertexMaterialStruct
			VERTEX_MAPPER_ARGS0 = 0x0000002E,  // Null-terminated string
			VERTEX_MAPPER_ARGS1 = 0x0000002F,  // Null-terminated string

			TEXTURES = 0x00000030,  // wraps all of the texture info
			TEXTURE = 0x00000031,  // wraps a texture definition
			TEXTURE_NAME = 0x00000032,  // texture filename (NULL-terminated string)
			TEXTURE_INFO = 0x00000033,  // optional W3dTextureInfoStruct

			MATERIAL_PASS = 0x00000038,  // wraps the information for a single material pass
			VERTEX_MATERIAL_IDS = 0x00000039,  // single or per-vertex array of uint32 vertex material indices (check chunk size)
			SHADER_IDS = 0x0000003A,  // single or per-tri array of uint32 shader indices (check chunk size)
			DCG = 0x0000003B,  // per-vertex diffuse color values (array of W3dRGBAStruct's)
			DIG = 0x0000003C,  // per-vertex diffuse illumination values (array of W3dRGBStruct's)
			SCG = 0x0000003E,  // per-vertex specular color values (array of W3dRGBStruct's)

			TEXTURE_STAGE = 0x00000048,  // wrapper around a texture stage.
			TEXTURE_IDS = 0x00000049,  // single or per-tri array of uint32 texture indices (check chunk size)
			STAGE_TEXCOORDS = 0x0000004A,  // per-vertex texture coordinates (array of W3dTexCoordStruct's)
			PER_FACE_TEXCOORD_IDS = 0x0000004B,  // indices to STAGE_TEXCOORDS, (array of Vector3i)

			NORMALMAP_INFO = 0x00000050,  // material/texture 1st marker flag for BFME2 (???)
			NORMALMAP_FLAG1 = 0x00000051,  // material/texture 2nd marker flag for BFME2 (???)
			NORMALMAP_FLAG2 = 0x00000052,  // material/texture structure  of W3dNormMapHeaderStruct for BFME2
			NORMALMAP_FLAG3 = 0x00000053,  // array of texture structures of W3dNormMapEntryStruct for BFME2

			DEFORM = 0x00000058,  // mesh deform or 'damage' information.
			DEFORM_SET = 0x00000059,  // set of deform information
			DEFORM_KEYFRAME = 0x0000005A,  // a keyframe of deform information in the set
			DEFORM_DATA = 0x0000005B,  // deform information about a single vertex

			PS2_SHADERS = 0x00000080,  // Shader info specific to the Playstation 2.
			AABTREE = 0x00000090,  // Axis-Aligned Box Tree for hierarchical polygon culling
			AABTREE_HEADER,                                // catalog of the contents of the AABTree
			AABTREE_POLYINDICES,                           // array of uint32 polygon indices with count=mesh.PolyCount
			AABTREE_NODES,                                 // array of W3dMeshAABTreeNode's with count=aabheader.NodeCount

			HIERARCHY = 0x00000100,  // hierarchy tree definition
			HIERARCHY_HEADER,
			PIVOTS,
			PIVOT_FIXUPS,                                      // only needed by the exporter...

			ANIMATION = 0x00000200,  // hierarchy animation data
			ANIMATION_HEADER,
			ANIMATION_CHANNEL,                                 // channel of vectors
			BIT_CHANNEL,                                       // channel of boolean values (e.g. visibility)

			COMPRESSED_ANIMATION = 0x00000280,  // compressed hierarchy animation data
			COMPRESSED_ANIMATION_HEADER,                       // describes playback rate, number of frames, and type of compression
			COMPRESSED_ANIMATION_CHANNEL,                      // compressed channel, format dependent on type of compression
			COMPRESSED_BIT_CHANNEL,                            // compressed bit stream channel, format dependent on type of compression
			COMPRESSED_ANIMATION_BFME2DATA,                    // compressed animation data for BFME2 ??? dependent on type of compression

			MORPH_ANIMATION = 0x000002C0,  // hierarchy morphing animation data (morphs between poses, for facial animation)
			MORPHANIM_HEADER,                                  // W3dMorphAnimHeaderStruct describes playback rate, number of frames, and type of compression
			MORPHANIM_CHANNEL,                                 // wrapper for a channel
			MORPHANIM_POSENAME,                            // name of the other anim which contains the poses for this morph channel
			MORPHANIM_KEYDATA,                             // morph key data for this channel
			MORPHANIM_PIVOTCHANNELDATA,                        // uin32 per pivot in the htree, indicating which channel controls the pivot

			HMODEL = 0x00000300,  // blueprint for a hierarchy model
			HMODEL_HEADER,                                     // Header for the hierarchy model
			NODE,                                              // render objects connected to the hierarchy
			COLLISION_NODE,                                    // collision meshes connected to the hierarchy
			SKIN_NODE,                                         // skins connected to the hierarchy
			OBSOLETE_HMODEL_AUX_DATA,                          // extension of the hierarchy model header
			OBSOLETE_SHADOW_NODE,                              // shadow object connected to the hierarchy

			LODMODEL = 0x00000400,  // blueprint for an LOD model. This is simply a
			LODMODEL_HEADER,                                   // collection of 'n' render objects, ordered in terms
			LOD,                                               // of their expected rendering costs. (highest is first)

			COLLECTION = 0x00000420,  // collection of render object names
			COLLECTION_HEADER,                                 // general info regarding the collection
			COLLECTION_OBJ_NAME,                               // contains a string which is the name of a render object
			PLACEHOLDER,                                       // contains information about a 'dummy' object that will be instanced later
			TRANSFORM_NODE,                                    // contains the filename of another w3d file that should be transformed by this node

			POINTS = 0x00000440,  // array of W3dVectorStruct's. May appear in meshes, hmodels, lodmodels, or collections.

			LIGHT = 0x00000460,  // description of a light
			LIGHT_INFO,                                        // generic light parameters
			SPOT_LIGHT_INFO,                                   // extra spot light parameters
			NEAR_ATTENUATION,                                  // optional near attenuation parameters
			FAR_ATTENUATION,                                   // optional far attenuation parameters

			EMITTER = 0x00000500,  // description of a particle emitter
			EMITTER_HEADER,                                    // general information such as name and version
			EMITTER_USER_DATA,                                 // user-defined data that specific loaders can switch on
			EMITTER_INFO,                                      // generic particle emitter definition
			EMITTER_INFOV2,                                    // generic particle emitter definition (version 2.0)
			EMITTER_PROPS,                                     // Key-frameable properties
			OBSOLETE_EMITTER_COLOR_KEYFRAME,                   // structure defining a single color keyframe
			OBSOLETE_EMITTER_OPACITY_KEYFRAME,                 // structure defining a single opacity keyframe
			OBSOLETE_EMITTER_SIZE_KEYFRAME,                    // structure defining a single size keyframe
			EMITTER_LINE_PROPERTIES,                           // line properties, used by line rendering mode
			EMITTER_ROTATION_KEYFRAMES,                        // rotation keys for the particles
			EMITTER_FRAME_KEYFRAMES,                           // frame keys (u-v based frame animation)
			EMITTER_BLUR_TIME_KEYFRAMES,                       // length of tail for line groups

			AGGREGATE = 0x00000600,  // description of an aggregate object
			AGGREGATE_HEADER,                                  // general information such as name and version
			AGGREGATE_INFO,                                // references to 'contained' models
			TEXTURE_REPLACER_INFO,                             // information about which meshes need textures replaced
			AGGREGATE_CLASS_INFO,                              // information about the original class that created this aggregate

			HLOD = 0x00000700,  // description of an HLod object (see HLodClass)
			HLOD_HEADER,                                       // general information such as name and version
			HLOD_LOD_ARRAY,                                    // wrapper around the array of objects for each level of detail
			HLOD_SUB_OBJECT_ARRAY_HEADER,                  // info on the objects in this level of detail array
			HLOD_SUB_OBJECT,                               // an object in this level of detail array
			HLOD_AGGREGATE_ARRAY,                              // array of aggregates, contains SUB_OBJECT_ARRAY_HEADER and SUB_OBJECT_ARRAY
			HLOD_PROXY_ARRAY,                                  // array of proxies, used for application-defined purposes, provides a name and a bone.

			BOX = 0x00000740,  // defines an collision box render object (W3dBoxStruct)
			SPHERE,
			RING,

			NULL_OBJECT = 0x00000750,  // defines a NULL object (W3dNullObjectStruct)

			LIGHTSCAPE = 0x00000800,  // wrapper for lights created with Lightscape.
			LIGHTSCAPE_LIGHT,                                  // definition of a light created with Lightscape.
			LIGHT_TRANSFORM,                               // position and orientation (defined as right-handed 4x3 matrix transform W3dLightTransformStruct).

			DAZZLE = 0x00000900,  // wrapper for a glare object. Creates halos and flare lines seen around a bright light source
			DAZZLE_NAME,                                       // null-terminated string, name of the dazzle (typical w3d object naming: "container.object")
			DAZZLE_TYPENAME,                                   // null-terminated string, type of dazzle (from dazzle.ini)

			SOUNDROBJ = 0x00000A00,  // description of a sound render object
			SOUNDROBJ_HEADER,                                  // general information such as name and version
			SOUNDROBJ_DEFINITION,                              // chunk containing the definition of the sound that is to play
		};

		Type ChunkType;

		virtual void Load(std::ifstream& fin, uint32_t chunksize);
	};
}