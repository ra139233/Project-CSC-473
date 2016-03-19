#ifndef LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP
#define LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>
#include "Universe.hpp"


class SimpleGLScene : public atlas::utils::Scene
{
public:
    SimpleGLScene();
    ~SimpleGLScene();

	void renderScene() override;
	void updateScene(double tempo) override;

private:
	Universe mUniverse;
	float mLastTime;
	float mFPS;
	float mTick;

	float mAnimTime;
	float mAnimLength;

};

#endif
