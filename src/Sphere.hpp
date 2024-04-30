#pragma once

#include "Material.hpp"
#include "math.hpp"

// Relevant für Aufgabenblatt 3

class Sphere {
 public:
  Sphere() : mPosition(GLPoint(0, 0, 0)), mRadius(1) {}

  Sphere(GLPoint position, double radius)
      : mPosition(position), mRadius(radius) {}

  void setMaterial(Material material);
  Material getMaterial() const;

  void setPosition(GLPoint position);
  GLPoint getPosition() const;

  void setRadius(double radius);
  double getRadius() const;

  // Die Transformationsmatrix des Modells

 private:
  GLPoint mPosition;
  double mRadius;

  // Das Material der Sphere
  Material mMaterial;
};
