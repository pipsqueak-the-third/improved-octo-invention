
#include "Sphere.hpp"

void Sphere::setMaterial(Material material) {
  mMaterial = Material();
  mMaterial.smooth = material.smooth;
  mMaterial.reflection = material.reflection;
  mMaterial.refraction = material.refraction;
  mMaterial.transparency = material.transparency;
  mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

Material Sphere::getMaterial() const { return mMaterial; }

void Sphere::setPosition(GLPoint position) { mPosition = position; }

GLPoint Sphere::getPosition() const { return mPosition; }

void Sphere::setRadius(double radius) { mRadius = radius; }

double Sphere::getRadius() const { return mRadius; }
