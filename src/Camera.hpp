#pragma once

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"
#include "math.hpp"
#include "structs.hpp"

class Camera {
 public:
  Camera();

  void setEyePoint(const GLPoint &pos);

  GLPoint getEyePoint() const;

  void setViewDirection(const GLVector &view);

  void setUp(const GLVector &up);

  void makeOrtho();

  void setSize(size_t width, size_t height);

  void update();

  Ray getRay(int x, int y) const;

  void print();

  int getWidth() const;

  int getHeight() const;

  GLMatrix getViewMatrix() const;

  GLMatrix getInvViewMatrix() const;

 private:
  GLMatrix mViewTransform;
  GLMatrix mProjectionTransform;  // orthographic
  GLMatrix mWindowTransform;

  GLMatrix mInvViewTransform;

  GLMatrix mWorldToScreenTransform;

  GLMatrix mWindow;

  int mWidth;
  int mHeight;
};
