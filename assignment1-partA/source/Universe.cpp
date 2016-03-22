#include "Universe.hpp"
#include "ShaderPaths.hpp"
#include "Planet.hpp"
#include "QuadTree.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <atlas/core/Float.hpp>
#include <iostream>

void Universe::renderGeometry(atlas::math::Matrix4 projection,
	atlas::math::Matrix4 view)
{
	for (int i = 0; i < numPlanets; i++) {
		Planets[i]->renderGeometry(projection, view);

//		std::cout << std::to_string(Planets[i]->mRadialVelocity.x) << std::endl;
//		std::cout << std::to_string(Planets[i]->mRadialVelocity.y) << std::endl;
//		std::cout << "\n" << std::endl;
	}

//	std::cout << "------------ END PLANETS POSITION ----------- \n\n" << std::endl;
}


void Universe::updateGeometry(atlas::utils::Time const &dt) {

	mTree = buildTree();
	Planet* mReferencePlanet;
	QuadTreeNode* mActualNode = mTree;

	for (int i = 0; i < numPlanets; i++) {
		mReferencePlanet = Planets[i];
		calculatePlanetsAcceleration(mReferencePlanet, mActualNode);
	}

	for (int i = 0; i < numPlanets; i++) {
		Planets[i]->updateGeometry(dt);
	}
	
	mTree->~QuadTreeNode();
}

void Universe::calculatePlanetsAcceleration(Planet* mReference, QuadTreeNode* mCurrentNode) {
	
	if (mCurrentNode->northWest == NULL) { //My node is a leaf
		if (mCurrentNode->PlanetInfo->centerOfMass != atlas::math::Vector(0.0f) && mCurrentNode->PlanetInfo->centerOfMass != mReference->mPosition) //The node isn't empty and isn't the same planet as mReference
			mReference->calculateNetAcceleration(mCurrentNode->PlanetInfo);

		return;

	}

	else {

		float boundingBoxSide, distancePlanets;
		boundingBoxSide = abs(mCurrentNode->boundary.bottomRight.x - mCurrentNode->boundary.topLeft.x);
		distancePlanets = getModulus(mCurrentNode->PlanetInfo->centerOfMass - mReference->mPosition);

		if (boundingBoxSide / distancePlanets < RATIO) {
			mReference->calculateNetAcceleration(mCurrentNode->PlanetInfo);
			return;
		}

		else {
			calculatePlanetsAcceleration(mReference, mCurrentNode->northWest);
			calculatePlanetsAcceleration(mReference, mCurrentNode->northEast);
			calculatePlanetsAcceleration(mReference, mCurrentNode->southWest);
			calculatePlanetsAcceleration(mReference, mCurrentNode->southEast);

		}
	}
}

QuadTreeNode* Universe::buildTree() {

	QuadTreeNodeData* mPlanets = new QuadTreeNodeData[numPlanets];

	float x =  sizeof(mPlanets) / sizeof(QuadTreeNodeData);
	int y = sizeof(mPlanets);
	int z = sizeof(QuadTreeNodeData);

	for (int i = 0; i < numPlanets; i++) {
		mPlanets[i] = QuadTreeNodeDataMake(Planets[i]->mPosition, Planets[i]->mMass, Planets[i]);			
	}

	QuadTreeNode* mRoot;
	mRoot = QuadTree::QuadTreeMake(mPlanets, mInitialBox, 1);
	
	return mRoot;
}

float Universe::getModulus(atlas::math::Vector mVector) {

	return sqrt((mVector.x * mVector.x) + (mVector.y * mVector.y));

}