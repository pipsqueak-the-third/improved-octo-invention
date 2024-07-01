#include "Camera.hpp"

Camera::Camera() { mViewTransform = GLMatrix(); }

void Camera::setEyePoint(const GLPoint &pos) {
  mViewTransform.setColumn(3, pos);
}

GLPoint Camera::getEyePoint() const {
  GLVector col = mViewTransform.getColumn(3);
  return GLPoint(col(0), col(1), col(2));
}

void Camera::setViewDirection(const GLVector &view) {
  mViewTransform.setColumn(2, view);
  makeOrtho();
}

void Camera::setUp(const GLVector &up) {
  mViewTransform.setColumn(1, up);
  makeOrtho();
}

void Camera::makeOrtho() {
  GLVector up = mViewTransform.getColumn(1);
  up.normalize();
  GLVector view = mViewTransform.getColumn(2);
  view.normalize();
  mViewTransform.setColumn(0, crossProduct(up, view));

  mInvViewTransform = mViewTransform;
  mInvViewTransform.inverse();
}

void Camera::setSize(size_t width, size_t height) {
  mWidth = width;
  mHeight = height;

  mProjectionTransform.setColumn(0, GLVector(2.0 / (double(mWidth)), 0, 0));
  mProjectionTransform.setColumn(1, GLVector(0, 2.0 / double(mHeight), 0));
  mProjectionTransform.setColumn(2, GLVector(0, 0, 0));
  mProjectionTransform.setColumn(3, GLVector(0, 0, -1.0));

  mWindowTransform.setColumn(0, GLVector(double(width) / 2, 0.0, 0.0));
  mWindowTransform.setColumn(1, GLVector(0.0, double(height) / 2, 0.0));
  mWindowTransform.setColumn(2, GLVector(0.0, 0.0, 0.5));
  mWindowTransform.setColumn(
      3, GLVector(double(width) / 2.0, double(height) / 2.0, 0.0));
}

void Camera::update() {
    // compute inverse matrices
}

Ray Camera::getRay(int x, int y) const {
  Ray r;
  r.origin = GLPoint(0, 0, 0) + mViewTransform.getColumn(3);

  double aspectRatio = double(mWidth) / double(mHeight);

  r.direction =
      (mViewTransform.getColumn(2) +
       mViewTransform.getColumn(0) * (2.0 * (x - mWidth / 2) / (double)mWidth) +
       mViewTransform.getColumn(1) *
           (2.0 * (y - mHeight / 2) / (double)mHeight));
  
  // Antialiasing für Ray casting / tracing. Zur Nutzung einkommentieren.
  /*double jitterx = (drand48() - 0.5) / (double) mWidth;
  double jittery = (drand48() - 0.5)/ (double) mHeight;
  r.direction = r.direction + mViewTransform.getColumn( 0 ) * jitterx + mViewTransform.getColumn( 1 ) * jittery;
  */
 
  r.direction(0) *= aspectRatio;
  r.direction.normalize(); 

  return r;
}

void Camera::print() {
  std::cout << mViewTransform << std::endl;
  std::cout << "Window: " << mWidth << "x" << mHeight << std::endl;
}

int Camera::getWidth() const { return mWidth; }

int Camera::getHeight() const { return mHeight; }

GLMatrix Camera::getViewMatrix() const { return mViewTransform; }

GLMatrix Camera::getInvViewMatrix() const { return mInvViewTransform; }