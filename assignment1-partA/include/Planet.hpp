#ifndef LAB01_INCLUDE_SIMPLE_GL_Planet_HPP
#define LAB01_INCLUDE_SIMPLE_GL_Planet_HPP

#pragma once

#include "ShaderPaths.hpp"
#include "QuadTree.hpp"
#include <atlas/utils/Geometry.hpp>
#include <atlas/primitives/Disk.hpp>
#include <atlas/core/Float.hpp>

class Planet : public atlas::utils::Geometry
{
public:
	atlas::math::Vector mPosition;
	float mMass;
	float mRadius;
	atlas::math::Matrix4 mPlanetModel;
	atlas::primitives::Disk mPlanet;


	Planet(atlas::math::Vector mPosition, float mRadius, float mMass):
		mPlanet(mRadius, 40),
		mIntegrator(Integrator::EULER)
	{
		this->mPosition = mPosition;
		this->mRadius = mRadius;
		this->mMass = mMass;

		USING_ATLAS_GL_NS;
		USING_ATLAS_MATH_NS;

		mPlanetModel = glm::translate(atlas::math::Matrix4(1.0f), mPosition);

		// Get the path where our shaders are stored.
		std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

		// Now set up the information for our shaders.
		std::vector<ShaderInfo> shaders
		{
			ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "circle.vs.glsl" },
			ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "circle.fs.glsl" }
		};

		// Create a new shader and add it to our list.
		mShaders.push_back(ShaderPointer(new Shader));

		// Compile the shaders.
		mShaders[0]->compileShaders(shaders);

		// And link them.
		mShaders[0]->linkShaders();

		mUniforms.insert(UniformKey("mvpMat",
			mShaders[0]->getUniformVariable("mvpMat")));

		mPlanet.createBuffers();

		// Disable at the end to avoid mixing shaders.
	//	mShaders[0]->disableShaders();
		
	}
    ~Planet();
	
	void updateGeometry(atlas::utils::Time const& dt) override;
	void updateEulerGeometry(atlas::utils::Time const& dt);
//	void updateRK4Geometry(atlas::utils::Time const& dt);
	

/*	struct State {
		atlas::math::Vector r;
		float theta;
		atlas::math::Vector rdot;
		float thetadot;
	};

	struct Derivative {
		atlas::math::Vector rdot;
		float rdotdot;
		float thetadot;
		float thetadotdot;
	};

/*	Derivative evaluateRK4(State &currentState, Derivative &currentDerivative, float dt, atlas::math::Vector mNormal);
	void integrateRK4(State &currentState, float dt);
	float newThetaAccelRK4(State &mState);
	float newRadialAccelRK4(State &mState);
	void initializeStructs(State &mState, Derivative &mDerivative);
	*/
		
	float getModulus(atlas::math::Vector mVector);
	atlas::math::Vector getNormal(atlas::math::Vector mVector);
	atlas::math::Vector getPerpendicular(atlas::math::Vector mVector);
	

	void renderGeometry(atlas::math::Matrix4 projection,
        atlas::math::Matrix4 view) override;
	void calculateNetAcceleration(QuadTreeNodeData* mOtherPlanet);

	enum class Integrator : int
	{
		EULER = 0,
		VERLET,
		RK4
	};

	
private:
	float mAngPosition = 0.0f;

	atlas::math::Vector mRadialVelocity;
	atlas::math::Vector mTangenVelocity;
	float mAngVelocity = 0.0f;

	atlas::math::Vector radialAccel;
	atlas::math::Vector tangenAccel;

	float rdotdot = 0.0f;
	float thetadotdot = 0.0f;

	Integrator mIntegrator;

};

#endif