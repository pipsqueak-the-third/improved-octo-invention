#pragma once

#include "Material.hpp"
#include "math.hpp"
#include "structs.hpp"

class Model {
 public:
  Model();

  void setMaterial(Material material);

  // Setter für die Transformationen
  void setRotation(GLVector rotation);
  void setTranslation(GLVector translation);
  void setScale(GLVector scale);

  // Die Dreiecke aus denen das Modell besteht
  std::vector<Triangle> mTriangles;

  // Das Material des Modells
  Material getMaterial() const;

  GLMatrix getTransformation() const;

 private:
  // Aufgabenblatt 2, Aufgabe 2: Nutzen Sie diese Matrizen für die Transformationen
  GLVector mRotation;
  GLVector mTranslation;
  GLVector mScale;

  Material mMaterial;
  // Die Transformationsmatrix des Modells
  GLMatrix mMatrix;
  void updateMatrix();
};
