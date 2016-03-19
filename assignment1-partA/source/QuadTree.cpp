#include "ShaderPaths.hpp"
#include "QuadTree.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <atlas/core/Float.hpp>
#include <iostream>

QuadTreeNodeData QuadTreeNodeDataMake(atlas::math::Vector mPosition, float mMass, void *data) {

	QuadTreeNodeData newNodeData;

	newNodeData.centerOfMass = mPosition;
	newNodeData.totalMass = mMass;
	newNodeData.wasThereBefore = false;

	return newNodeData;
}

BoundingBox BoundingBoxMake(atlas::math::Vector p0, atlas::math::Vector p1) {

	BoundingBox newBox;

	newBox.topLeft = p0;
	newBox.bottomRight = p1;

	return newBox;
}

bool BoundingBoxContainsData(BoundingBox Box, QuadTreeNodeData PlanetInfo) {

	bool X = PlanetInfo.centerOfMass.x > Box.topLeft.x && PlanetInfo.centerOfMass.x < Box.bottomRight.x;
	bool Y= PlanetInfo.centerOfMass.y < Box.topLeft.y && PlanetInfo.centerOfMass.y > Box.bottomRight.y;

	return X && Y;
}

bool BoundingBoxIntersectsBoundingBox(BoundingBox boundary, BoundingBox test) {

	return (boundary.topLeft.x <= test.bottomRight.x && boundary.bottomRight.x >= test.topLeft.x && boundary.topLeft.y <= test.bottomRight.y && boundary.bottomRight.y >= test.topLeft.y);

}

QuadTreeNode::QuadTreeNode(BoundingBox Box, int cap) {

	this->northWest = NULL;
	this->northEast = NULL;
	this->southWest = NULL;
	this->southEast = NULL;

	this->boundary = Box;
	this->capacity = cap;
	this->count = 0;
	this->PlanetInfo = (QuadTreeNodeData *)malloc(sizeof(QuadTreeNodeData) * cap);

}

QuadTreeNode::~QuadTreeNode() {

	if (this->northWest != NULL)
		delete this->northWest;

	if (this->northEast != NULL)
		delete this->northEast;

	if (this->southWest != NULL)
		delete this->southWest;

	if (this->southEast != NULL)
		delete this->southEast;

	for (int i = 0; i < this->count; i++)
		free(this->PlanetInfo[i].data);
	
}

QuadTreeNode* QuadTreeNode::QuadTreeNodeMake(BoundingBox Box, int capacity) {

	QuadTreeNode *node = new QuadTreeNode(Box, capacity);
	return node;

}

void QuadTreeNode::gatherDataWithinBoundary(BoundingBox boundary, QuadTreeNodeDataBlock block) {

	// If range is not contained in the node's boundingBox then bail
	if (!BoundingBoxIntersectsBoundingBox(this->boundary, boundary)) {
		return;
	}

	for (int i = 0; i < this->count; i++) {
		// Gather points contained in range
		if (BoundingBoxContainsData(boundary, this->PlanetInfo[i])) {
			block(this->PlanetInfo[i]);
		}
	}

	// Bail if node is leaf
	if (this->northWest == NULL) {
		return;
	}

	// Otherwise traverse down the tree
	this->northWest->gatherDataWithinBoundary(boundary, block);
	this->northEast->gatherDataWithinBoundary(boundary, block);
	this->southWest->gatherDataWithinBoundary(boundary, block);
	this->southEast->gatherDataWithinBoundary(boundary, block);

}

void QuadTreeNode::traverse(QuadTreeNodeTraverseBlock block) {

		block(this);

	if (this->northWest == NULL) {
		return;
	}

	this->northWest->traverse(block);
	this->northEast->traverse(block);
	this->southWest->traverse(block);
	this->southEast->traverse(block);

}

bool QuadTreeNode::insert(QuadTreeNodeData data) {

	// Return if our coordinate is not in the boundingBox
	if (!BoundingBoxContainsData(this->boundary, data)) {
		return false;
	}

	// Add the coordinate to the points array.
	if (this->count < this->capacity) {
		this->PlanetInfo[this->count++] = data;
		this->PlanetInfo->wasThereBefore = true;
		return true;
	}

	// Check to see if the current node is a leaf, if it is, split.
	if (this->northWest == NULL) {
		this->subdivide();
		this->insert(this->PlanetInfo[0]);
	}

	// Traverse the tree
	if (this->northWest->insert(data)) {
		if (!data.wasThereBefore) {
			this->PlanetInfo->centerOfMass = ((this->PlanetInfo->centerOfMass * this->PlanetInfo->totalMass) + (data.centerOfMass * data.totalMass)) / (data.totalMass + this->PlanetInfo->totalMass);
			this->PlanetInfo->totalMass += data.totalMass;
			data.wasThereBefore = true;
		}

		return true;
	}
	if (this->northEast->insert(data)) {
		if (!data.wasThereBefore) {
			this->PlanetInfo->centerOfMass = ((this->PlanetInfo->centerOfMass * this->PlanetInfo->totalMass) + (data.centerOfMass * data.totalMass)) / (data.totalMass + this->PlanetInfo->totalMass);
			this->PlanetInfo->totalMass += data.totalMass;
			data.wasThereBefore = true;
		}
		return true;
	}
	if (this->southWest->insert(data)) {
		if (!data.wasThereBefore) {
			this->PlanetInfo->centerOfMass = ((this->PlanetInfo->centerOfMass * this->PlanetInfo->totalMass) + (data.centerOfMass * data.totalMass)) / (data.totalMass + this->PlanetInfo->totalMass);
			this->PlanetInfo->totalMass += data.totalMass;
			data.wasThereBefore = true;
		}

		return true;
	}
	if (this->southEast->insert(data)) {
		if (!data.wasThereBefore) {
			this->PlanetInfo->centerOfMass = ((this->PlanetInfo->centerOfMass * this->PlanetInfo->totalMass) + (data.centerOfMass * data.totalMass)) / (data.totalMass + this->PlanetInfo->totalMass);
			this->PlanetInfo->totalMass += data.totalMass;
			data.wasThereBefore = true;
		}

		return true;
	}
	// Default. Was unable to add the node.
	return false;

}

void QuadTreeNode::subdivide() {

	BoundingBox box = this->boundary;

	// Spit the quadrant into four equal parts.
	atlas::math::Vector MidCoordinates;
	MidCoordinates.x = (box.bottomRight.x + box.topLeft.x) / 2.0;
	MidCoordinates.y = (box.bottomRight.y + box.topLeft.y) / 2.0;
	
	// Create the north west bounding box.
	BoundingBox northWest = BoundingBoxMake(box.topLeft, MidCoordinates);
	this->northWest = new QuadTreeNode(northWest, this->capacity);
	initializeEmptyNode(this->northWest);

	// Create the north east bounding box.
	atlas::math::Vector NETopLeft;
	atlas::math::Vector NEBottomRight;
	
	NETopLeft.x = MidCoordinates.x;
	NETopLeft.y = box.topLeft.y;

	NEBottomRight.x = box.bottomRight.x;
	NEBottomRight.y = MidCoordinates.y;

	BoundingBox northEast = BoundingBoxMake(NETopLeft, NEBottomRight);
	this->northEast = new QuadTreeNode(northEast, this->capacity);
	initializeEmptyNode(this->northEast);

	// Create the south west bounding box.
	atlas::math::Vector SWTopLeft;
	atlas::math::Vector SWBottomRight;

	SWTopLeft.x = box.topLeft.x;
	SWTopLeft.y = MidCoordinates.y;

	SWBottomRight.x = MidCoordinates.x;
	SWBottomRight.y = box.bottomRight.y;

	BoundingBox southWest = BoundingBoxMake(SWTopLeft, SWBottomRight);
	this->southWest = new QuadTreeNode(southWest, this->capacity);
	initializeEmptyNode(this->southWest);
	
	// Create the south east bounding box.
	BoundingBox southEast = BoundingBoxMake(MidCoordinates, box.bottomRight);
	this->southEast = new QuadTreeNode(southEast, this->capacity);
	initializeEmptyNode(this->southEast);
}

void QuadTreeNode::initializeEmptyNode(QuadTreeNode *empty) {

	empty->PlanetInfo->centerOfMass = atlas::math::Vector(0.0f);
	empty->PlanetInfo->totalMass = 0.0f;
	empty->PlanetInfo->wasThereBefore = false;

}


QuadTreeNodeData QuadTreeNode::getPlanet() const {

	return this->PlanetInfo[0];

}

BoundingBox QuadTreeNode::getBoundary() const {

	return this->boundary;

}

int QuadTreeNode::getCapacity() const {

	return this->capacity;

}

QuadTree::QuadTree(QuadTreeNodeData *data, BoundingBox boundary, int capacity) : QuadTreeNode(boundary, capacity) {

	for (int i = 0; i < sizeof(QuadTreeNodeData) / sizeof(data); i++) {
		this->insert(data[i]);
	}

}

QuadTreeNode* QuadTree::QuadTreeMake(QuadTreeNodeData * data, BoundingBox boundary, int capacity)
{
	QuadTree *tree = new QuadTree(data, boundary, capacity);
	return tree;
}

