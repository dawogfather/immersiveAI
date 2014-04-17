//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathMap
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIPathNode.h
//-------------------------------------------------------------------
/// @class iAIPathNode
/// @author Gavin Bunney
/// @version 1.0
/// @brief Represents a single path finding node.
/// 
/// A point within the game world, consisting of a location, id, 
/// neighbours and various path finding variables.
//-------------------------------------------------------------------
#ifndef _IAIPATHNODE_H_
#define _IAIPATHNODE_H_

class iAIPathNode {

	friend class iAIPathMap;
	friend class iAIPathGrid;
	friend class iAIPath;
	friend class iAIPathFind;

public:

	//-------------------------------------------------------------------
	/// @fn iAIPathNode() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIPathNode() { }

	//-------------------------------------------------------------------
	/// @fn ~iAIPathNode() 
	/// @brief Default deconstructor.
	//-------------------------------------------------------------------
	~iAIPathNode();

	//-------------------------------------------------------------------
	/// @fn iAIPathNode(const Point3F position, iAIPathGrid* pathGrid, 
	///                 const U16 idX, const U16 idY) 
	/// @brief Constructor to create node at specified position.
	///
	/// @param position Point in world coords to create the node.
	/// @param pathGrid Pointer to grid which the node is contained in.
	/// @param idX ID in X of the node within the grid.
	/// @param idY ID in Y of the node within the grid.
	//-------------------------------------------------------------------
	iAIPathNode(const Point3F position, iAIPathGrid* pathGrid, const U16 idX, const U16 idY);

	//-------------------------------------------------------------------
	/// @fn bool isClear()
	/// @brief Checks if the node is in a valid position, clear of
	///	       obstructions.
	///
	/// @return Node clear of all obstructions.
	//-------------------------------------------------------------------
	bool isClear();

	//-------------------------------------------------------------------
	/// @fn bool addNeighbour(iAIPathNode* neighbour)
	/// @brief Adds a neighbour to this node's neighbour list.
	///
	/// @return Neighbour added to node neighbour list.
	//-------------------------------------------------------------------
	bool addNeighbour(iAIPathNode* neighbour);

	//-------------------------------------------------------------------
	/// @fn bool hasNeighbour(iAIPathNode* neighbour)
	/// @brief Checks if the node has the parsed neighbour.
	///
	/// @return Neighbour in this node's neighbour list.
	//-------------------------------------------------------------------
	bool hasNeighbour(iAIPathNode* neighbour);

	//-------------------------------------------------------------------
	/// @fn bool removeNeighbour(iAIPathNode* neighbour)
	/// @brief Removes the specified neighbour.
	///
	/// @return Neighbour was removed successfully.
	//-------------------------------------------------------------------
	bool removeNeighbour(iAIPathNode* neighbour);

	//-------------------------------------------------------------------
	/// @fn void bool isNeighbourValid(const Point3F neighbourPosition)
	/// @brief Checks if a neighbour is accessible from this node.
	///
	/// @param neighbourPosition The position of the neighbour to check.
	/// @return Neighbour in valid position
	//-------------------------------------------------------------------
	bool isNeighbourValid(const Point3F neighbourPosition);

	//-------------------------------------------------------------------
	/// @var Point3F mPosition
	/// @brief Position of the node in world coordinates.
	//-------------------------------------------------------------------
	Point3F mPosition;

	//-------------------------------------------------------------------
	/// @var U16 mIdX
	/// @brief ID in X within the node's grid.
	//-------------------------------------------------------------------
	U16 mIdX;

	//-------------------------------------------------------------------
	/// @var U16 mIdY
	/// @brief ID in Y within the node's grid.
	//-------------------------------------------------------------------
	U16 mIdY;

	//-------------------------------------------------------------------
	/// @var Vector<iAIPathNode*> mNeighbours
	/// @brief Vector of the nodes neighbours. Expect 8 neighbours, but
	///        if edge of area or obstructed, will be less.
	//-------------------------------------------------------------------
	Vector<iAIPathNode*> mNeighbours;

	//-------------------------------------------------------------------
	/// @var iAIPathGrid* mParentGrid
	/// @brief Pointer to the grid which the node is contained within.
	//-------------------------------------------------------------------
	iAIPathGrid* mParentGrid;

	//-------------------------------------------------------------------
	/// @fn void updateMoveModifier()
	/// @brief Updates the node's move modifer, based on its position.
	//-------------------------------------------------------------------
	void updateMoveModifier();

	//-------------------------------------------------------------------
	/// @var F32 mMoveModifier
	/// @brief Utilised in A* algorithm; the level of difficulty at this
	//         node. 100.0f is untraversal, 0.0f is easiest.
	//-------------------------------------------------------------------
	F32 mMoveModifier;

	//-------------------------------------------------------------------
	/// @var F32 mFitness
	/// @brief Utilised in A* algorithm; fitness of this node from a
	///        start node.
	//-------------------------------------------------------------------
	F32 mFitness;

	//-------------------------------------------------------------------
	/// @var F32 mLowestCostFromStart
	/// @brief Utilised in A* algorithm; lowest cost from the start node.
	//-------------------------------------------------------------------
	F32 mLowestCostFromStart;

	//-------------------------------------------------------------------
	/// @var F32 mHeuristicCostToGoal
	/// @brief Utilised in A* algorithm; best guess cost to goal from this
	///        node.
	//-------------------------------------------------------------------
	F32 mHeuristicCostToGoal;

	//-------------------------------------------------------------------
	/// @var iAIPathNode* mParent
	/// @brief Utilised in A* algorithm; the node traversed previously to
	///        reach this node.
	//-------------------------------------------------------------------
	iAIPathNode* mParent;

	//-------------------------------------------------------------------
	/// @var bool mOpen
	/// @brief Utilised in A* algorithm; the node requires checking.
	//-------------------------------------------------------------------
	bool mOpen;

	//-------------------------------------------------------------------
	/// @var bool mClosed
	/// @brief Utilised in A* algorithm; if node has been checked, set as
	///        closed.
	//-------------------------------------------------------------------
	bool mClosed;
};

#endif