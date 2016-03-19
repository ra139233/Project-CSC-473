#include "SimpleGLScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>



SimpleGLScene::SimpleGLScene() :
	mLastTime(0.0f),
	mFPS(60.0f),
	mTick(1.0f / mFPS),
	mAnimTime(0.0f),
	mAnimLength(3.0f),
	mUniverse(3)

{
    // Initialize the matrices to identities.
    mProjection = glm::perspective(glm::radians(45.0f), (float)16 / (float)9, 0.1f, 100.0f);
    mView = glm::lookAt(
		               glm::vec3(0, 0, 100), // Camera is at (0,0,15), in World Space
		               glm::vec3(0, 0, 0), // and looks at the origin
		               glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

}

SimpleGLScene::~SimpleGLScene()
{ }

void SimpleGLScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Tell our scene to render
	mUniverse.renderGeometry(mProjection, mView);
}

void SimpleGLScene::updateScene(double t) {

	mTime.currentTime = t;
	mTime.totalTime += t;

	if (atlas::core::geq((mTime.currentTime - mLastTime), mTick)) {

		mLastTime += mTick;
		mTime.deltaTime = mTick;
		mAnimTime += mTick;

		mUniverse.updateGeometry(mTime);

	//	atlas::math::Vector mLinSpringPos = mCircles.updateLinearGeometry(mTime);
//		float mAngSpringPos = mCircles.updateAngularGeometry(mTime);
		//mPlanets.updateGeometry(mTime);

	//	mSpring.updateSpringGeometry(mLinSpringPos, mAngSpringPos);
	}




}