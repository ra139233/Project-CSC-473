#pragma once
#ifndef LAB01_INCLUDE_QuadTree_HPP
#define LAB01_INCLUDE_QuadTree_HPP

#include "atlas/math/Vector.hpp"

class QuadTreeNodeData;
class QuadTreeNode;

typedef void(*QuadTreeNodeDataBlock)(QuadTreeNodeData data);
typedef void(*QuadTreeNodeTraverseBlock) (QuadTreeNode* data);

	typedef struct QuadTreeNodeData {
		atlas::math::Vector centerOfMass;
		float totalMass;
		void *data;
		bool wasThereBefore;
	} QuadTreeNodeData;

	QuadTreeNodeData QuadTreeNodeDataMake(atlas::math::Vector centerOfMass, float mMass, void *data);

	typedef struct BoundingBox {
		atlas::math::Vector topLeft;
		atlas::math::Vector bottomRight;
	} BoundingBox;

	BoundingBox BoundingBoxMake(atlas::math::Vector P0, atlas::math::Vector P1);

class QuadTreeNode {	
public:
	QuadTreeNode *northWest;
	QuadTreeNode *northEast;
	QuadTreeNode *southWest;
	QuadTreeNode *southEast;
	QuadTreeNodeData *PlanetInfo;
	BoundingBox boundary;
	int capacity;
	int count;


	QuadTreeNode(BoundingBox boundary, int capacity);
	~QuadTreeNode();

	static QuadTreeNode* QuadTreeNodeMake(BoundingBox boundary, int capacity);

	void gatherDataWithinBoundary(BoundingBox boundary, QuadTreeNodeDataBlock block);
	void traverse(QuadTreeNodeTraverseBlock block);
	bool insert(QuadTreeNodeData data);
	void subdivide();
	void initializeEmptyNode(QuadTreeNode *empty);

	QuadTreeNodeData getPlanet() const;
	BoundingBox getBoundary() const;
	int getCapacity() const;
};

class QuadTree : public QuadTreeNode {
public:
	QuadTree(QuadTreeNodeData *data, BoundingBox boundary, int capacity);
	static QuadTreeNode* QuadTreeMake(QuadTreeNodeData *data, BoundingBox boundary, int capacity);
};

#endif