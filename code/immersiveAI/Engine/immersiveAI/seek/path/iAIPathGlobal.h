//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathGlobal
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIPathGlobal.h
//-------------------------------------------------------------------
/// @class iAIPathGlobal
/// @author Gavin Bunney
/// @version 1.0
/// @brief Global macros for the Seek :: Path functions.
/// 
/// Holds a collection of the macros used within the various path
/// finding classes.
//-------------------------------------------------------------------
#ifndef _IAIPATHGLOBAL_H_
#define _IAIPATHGLOBAL_H_

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_COLLISION_MASK
/// @brief Collision mask to detect for node clearance.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_COLLISION_MASK			(InteriorObjectType | StaticShapeObjectType | VehicleObjectType | PlayerObjectType | StaticTSObjectType)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_MAX_SLOPE
/// @brief Max slope between two nodes.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_MAX_SLOPE					100.0f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_MAX_SMOOTHED_SLOPE
/// @brief Max slope between path nodes for smoothing.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_MAX_SMOOTHED_SLOPE		50.0f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_NODE_CLEARANCE
/// @brief Amount of clearance in X, Y & Z around a node.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_NODE_CLEARANCE			Point3F(1.0, 1.0, 2.3)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_MOVE_MODIFIER_UNTRAVERSAL
/// @brief MoveModifier for a node to be considered untraversal.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_MOVE_MODIFIER_UNTRAVERSAL	100.0f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_MOVE_MODIFIER_WATER
/// @brief MoveModifier when a node is in water.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_MOVE_MODIFIER_WATER		70.0f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_BUFFER_INTERIOR
/// @brief Amount of clearance around an interior for a grid.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_BUFFER_INTERIOR		Point3F(10.0, 10.0, 0)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_DENSITY_INTERIOR
/// @brief Density of nodes within an interior.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_DENSITY_INTERIOR		2.0f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_DENSITY_TERRAIN
/// @brief Density of nodes on normal terrain.
//-------------------------------------------------------------------
//#define IAIPATHGLOBAL_GRID_DENSITY_TERRAIN		0.1f
#define IAIPATHGLOBAL_GRID_DENSITY_TERRAIN		0.4f

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_RENDER_CLEARANCE
/// @brief Clearance above node position to render the grid.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_RENDER_CLEARANCE		Point3F(0, 0, 0.3f)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_RENDER_NODE_HEIGHT
/// @brief Height of rendered nodes.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_RENDER_NODE_HEIGHT	Point3F(0, 0, 1.0f)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_RENDER_COLOUR
/// @brief Colour of the rendered grid: R, G, B, Alpha
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_RENDER_COLOUR		31, 102, 155, 255

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_RENDER_BOX_COLOUR
/// @brief Colour of the rendered grid bounding box: R, G, B, Alpha
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_RENDER_BOX_COLOUR	255, 0, 0, 255

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_GRID_RENDER_NODE_COLOUR
/// @brief Colour of the rendered nodes on a grid: R, G, B, Alpha
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_GRID_RENDER_NODE_COLOUR	255, 255, 0, 255

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_PATH_RENDER_CLEARANCE
/// @brief Clearance above node to render the path.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_PATH_RENDER_CLEARANCE		Point3F(0, 0, 1.0f)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_PATH_RENDER_NODE_HEIGHT
/// @brief Height of rendered path nodes.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_PATH_RENDER_NODE_HEIGHT	Point3F(0, 0, 1.0f)

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_PATH_RETRY_COUNT
/// @brief Number of retries to perform if a path is not found.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_PATH_RETRY_COUNT			2

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_PATH_SMOOTH_ANGLE_THRESHOLD
/// @brief Angle (degrees) between two points to detect if it can
///        be removed in the path smoothing process. The angle
///        between the two points must be +/- threadhold around 90.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_PATH_SMOOTH_ANGLE_THRESHOLD	15.0

//-------------------------------------------------------------------
/// @def IAIPATHGLOBAL_INVALID_POSITION
/// @brief Used to detect for invalid position nodes.
//-------------------------------------------------------------------
#define IAIPATHGLOBAL_INVALID_POSITION			Point3F(-1000.0f, -1000.0f, -1000.0f)


#endif