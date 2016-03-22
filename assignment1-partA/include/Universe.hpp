#pragma once
#ifndef LAB01_INCLUDE_Universe_HPP
#define LAB01_INCLUDE_Universe_HPP

#include <atlas/utils/Geometry.hpp>
#include "Planet.hpp"
#include "QuadTree.hpp"
#include "ShaderPaths.hpp"
#include <time.h>


#define MASS 5000
#define RADIUS 1
#define BOUNDARY 500
#define RATIO 0.5

class Universe : public atlas::utils::Geometry
{
public:
	int numPlanets;
	Planet** Planets;


	Universe(int numPlanets)
	{
		this->numPlanets = numPlanets;
		Planets = new Planet*[numPlanets];
		float borderMin = - BOUNDARY;
		float borderMax = BOUNDARY;
		int minMass = 10;
		int maxMass = 50;
		srand(static_cast <unsigned> (time(0)));

		for (int i = 0; i < numPlanets; i++)
		{
			atlas::math::Vector mPosition = getRandomPosition(borderMin, borderMax);
			int mMass = getRandomMass(minMass, maxMass);
			int radius = mMass / 10;
			Planets[i] = new Planet(mPosition, RADIUS, MASS);		
		}

		mInitialBox.topLeft.x = - (BOUNDARY + 5);
		mInitialBox.topLeft.y = BOUNDARY + 5;
		mInitialBox.bottomRight.x = BOUNDARY + 5;
		mInitialBox.bottomRight.y = - (BOUNDARY + 5);
	}

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const &dt) override;
	
	void calculatePlanetsAcceleration(Planet* mReference, QuadTreeNode* mNode);
	float getModulus(atlas::math::Vector mVector);

	QuadTreeNode* mTree;
	QuadTreeNode* buildTree();
	BoundingBox mInitialBox;
	

atlas::math::Vector getRandomPosition(float min, float max) {

		atlas::math::Vector Position;

		float x = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		Position.x = x;
		float y = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		Position.y = y;
		Position.z = 0;

		return Position;


};

int getRandomMass(int min, int max) {
	 
	int output = min + (rand() % (int)(max - min + 1));

	return output;
}

private:
	
};

#endif