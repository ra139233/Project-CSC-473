#include "Planet.hpp"
#include "ShaderPaths.hpp"

#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>
#include <iostream>

float G = 0.00066742f;


Planet::~Planet()
{
  
}

void Planet::renderGeometry(atlas::math::Matrix4 projection,
    atlas::math::Matrix4 view)
{
	// Enable the shaders.
	mShaders[0]->enableShaders();

	auto mvpMat = projection * view * mPlanetModel;
	glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);

	mPlanet.drawPrimitive();

    // Disable them.
	mShaders[0]->disableShaders();
}

void Planet::calculateNetAcceleration(QuadTreeNodeData * mOtherPlanet)
{
	atlas::math::Vector r = (mOtherPlanet->centerOfMass - mPosition);
	atlas::math::Vector mNormalRadius = getNormal(r);
	float rModulus = getModulus(r);

	thetadotdot = -(2.0f * (mAngVelocity * getModulus(mRadialVelocity)) / rModulus);
	tangenAccel += thetadotdot * getPerpendicular(mNormalRadius) * rModulus;

	rdotdot = mAngVelocity * mAngVelocity * rModulus - ((G * mOtherPlanet->totalMass) / (rModulus * rModulus));
	radialAccel += rdotdot * mNormalRadius;

}



void Planet::updateGeometry(atlas::utils::Time const& t) {

	if (mIntegrator == Integrator::EULER)
		updateEulerGeometry(t);

/*	else if (mIntegrator == Integrator::VERLET)
		updateVerletGeometry(t);

	else if (mIntegrator == Integrator::RK4)
		updateRK4Geometry(t);*/

}

void Planet::updateEulerGeometry(atlas::utils::Time const& t) {
	
	mAngVelocity += thetadotdot * t.deltaTime;
	mAngPosition += mAngVelocity * t.deltaTime;
	
	mTangenVelocity += (tangenAccel * t.deltaTime); // v = w r  . w = tangenAccel * dt
	mPosition +=  mTangenVelocity * t.deltaTime;

	mModel = glm::translate(atlas::math::Matrix4(1.0f), mPosition);


	
	mRadialVelocity += radialAccel * t.deltaTime;
	mPosition += mRadialVelocity * t.deltaTime;

	mModel = glm::translate(atlas::math::Matrix4(1.0f), mPosition);
	
}


/*void Planet::updateRK4Geometry(atlas::utils::Time const& t) {
		
	State currentState;

	currentState.r = mPosition2;
	currentState.rdot = mRadialVelocity;
	currentState.theta = mAngPosition2;
	currentState.thetadot = mAngVelocity;

	integrateRK4(currentState, t.deltaTime); 

}

void Planet::integrateRK4(State &currentState, float dt) {

	Derivative d1, d2, d3, d4;
	atlas::math::Vector mNormalRadius = getNormal(mPosition2 - mPosition1);
	atlas::math::Vector mTangenVec = getPerpendicular(mNormalRadius);
	float rModulus = getModulus(mPosition2 - mPosition1);

	//	initializeStructs(currentState, d0);

	d1 = evaluateRK4(currentState, Derivative(), 0.0f, mNormalRadius);
	d2 = evaluateRK4(currentState, d1, dt * 0.5f, mNormalRadius);
	d3 = evaluateRK4(currentState, d2, dt * 0.5f, mNormalRadius);
	d4 = evaluateRK4(currentState, d3, dt, mNormalRadius);

	//	------------------------	T H E T A -------------------------------------------------------

	//				V E L O C I T Y
	float dthetadt = (1.0f / 6.0f) * (d1.thetadot + 2.0f * (d2.thetadot + d3.thetadot) + d4.thetadot);

	//				A C C E L E R A T I O N
	thetadotdot += (1.0f / 6.0f) * (d1.thetadotdot + 2.0f * (d2.thetadotdot + d3.thetadotdot) + d4.thetadotdot);


	//				N E W	P O S I T I O N S
	currentState.theta = currentState.theta + dthetadt * dt;
	mAngPosition2 = currentState.theta;

	//				N E W   V E L O C I T I E S
	currentState.thetadot = currentState.thetadot + thetadotdot * dt;
	mAngVelocity = currentState.thetadot;
	
	mTangenVelocity += mTangenVec * (thetadotdot * dt * rModulus);



	//	------------------------	R A D I A L  -------------------------------------------------------

	//				V E L O C I T Y
	atlas::math::Vector drdt = (1.0f / 6.0f) * (d1.rdot + 2.0f * (d2.rdot + d3.rdot) + d4.rdot);

	//				A C C E L E R A T I O N
	rdotdot += (1.0f / 6.0f) * (d1.rdotdot + 2.0f * (d2.rdotdot + d3.rdotdot) + d4.rdotdot);


	//				N E W	P O S I T I O N S
	currentState.r = currentState.r + drdt * dt;
	
	//				N E W   V E L O C I T I E S
	currentState.rdot = currentState.rdot + (rdotdot * dt) * mNormalRadius;
	mRadialVelocity = currentState.rdot;

	//	mRadialVelocity = getModulus(mRadialVelocity) * mNormalRadius;
	//mPosition2 += mRadialVelocity * dt;

	std::cout << "rdotdot: " + std::to_string(rdotdot) + "\n" << std::endl;
	//std::cout << "angVel: " + std::to_string((mAngVelocity)) + "\n" << std::endl;
	//std::cout << "radVel: " + std::to_string(mRadialVelocity.x) + " " + std::to_string(mRadialVelocity.y) << std::endl;
	
	mPosition2 += mRadialVelocity * dt;
	mPosition2 += mTangenVelocity * dt;

	mModel2 = glm::translate(atlas::math::Matrix4(1.0f), mPosition2); */


//}

/*
Planet::Derivative Planet::evaluateRK4(State &currentState, Derivative &currentDerivative, float dt, atlas::math::Vector mNormal)
{
	State mState;
	mState.theta = currentState.theta + (currentDerivative.thetadot * dt); // x = xo + vt
	mState.r = currentState.r + (currentDerivative.rdot * dt);	// x = xo + vt

	mState.thetadot = currentState.thetadot + (currentDerivative.thetadotdot * dt); // v = vo + at
	mState.rdot = currentState.rdot + (currentDerivative.rdotdot * dt) * mNormal; // v = vo + at

	Derivative mDerivative;
	mDerivative.thetadot = mState.thetadot;
	mDerivative.rdot = mState.rdot;

	mDerivative.thetadotdot = newThetaAccelRK4(mState);
	mDerivative.rdotdot = newRadialAccelRK4(mState);

	return mDerivative;
}

float Planet::newThetaAccelRK4(State &mState) {

	float thetadotdot = -(2.0f * (mState.thetadot * getModulus(mState.rdot)) / getModulus(mState.r - mPosition1));

	return thetadotdot;

}

float Planet::newRadialAccelRK4(State &mState) {

	atlas::math::Vector rMod = mState.r - mPosition1;

	float rdotdot = (mState.thetadot * mState.thetadot * getModulus(rMod)) - ((G * mMass1) / getModulus(rMod * rMod));

	return rdotdot;
}*/

float Planet::getModulus(atlas::math::Vector mVector) {

	return sqrt((mVector.x * mVector.x) + (mVector.y * mVector.y));

}

atlas::math::Vector Planet::getNormal(atlas::math::Vector mVector) {

	if (mVector == atlas::math::Vector(0.0f, 0.0f, 0.0f)) //prevent divisions by 0
		return mVector;

	else {

		atlas::math::Vector temp;
		temp.x = mVector.x / sqrt((mVector.x * mVector.x) + (mVector.y * mVector.y));
		temp.y = mVector.y / sqrt((mVector.x * mVector.x) + (mVector.y * mVector.y));
		temp.z = 0.0f;

		return temp;
	}

}

atlas::math::Vector Planet::getPerpendicular(atlas::math::Vector mVector) {

	atlas::math::Vector temp;
	
	temp.x = -mVector.y;
	temp.y = mVector.x;
	
	return temp;

}


