#include "Model.hpp"

// Konstruktor
Model::Model()  {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */

  mTranslation(0) = 0;
  mTranslation(1) = 0;
  mTranslation(2) = 0;

  mScale(0) = 1;
  mScale(1) = 1;
  mScale(2) = 1;

  mRotation(0) = 0;
  mRotation(1) = 0;
  mRotation(2) = 0;
}

// Setter für das Material
void Model::setMaterial(Material material) {
  mMaterial = Material();
  mMaterial.smooth = material.smooth;
  mMaterial.reflection = material.reflection;
  mMaterial.refraction = material.refraction;
  mMaterial.transparency = material.transparency;
  mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

/* Aufgabenblatt 2, Aufgabe 3: Implementieren Sie die vier Methoden für die Transformationen hier */
void Model::setTranslation(GLVector translation) {
  mTranslation = translation;
  updateMatrix();
}

void Model::setScale(GLVector scale) {
  mScale = scale;
  updateMatrix();
}

void Model::setRotation(GLVector rotation) {
  mRotation = rotation;
  updateMatrix();
}

void Model::updateMatrix() {
  GLMatrix scaleMatrix;
  GLMatrix translationMatrix;
  GLMatrix rotationX;
  GLMatrix rotationY;
  GLMatrix rotationZ;

  float cosX = cos(mRotation(0)), sinX = sin(mRotation(0));
  float cosY = cos(mRotation(1)), sinY = sin(mRotation(1));
  float cosZ = cos(mRotation(2)), sinZ = sin(mRotation(2));

  //Translation
  translationMatrix.setValue(0, 3, mTranslation(0));
  translationMatrix.setValue(1, 3, mTranslation(1));
  translationMatrix.setValue(2, 3, mTranslation(2));

  //Scaling
  scaleMatrix.setValue(0, 0, mScale(0));
  scaleMatrix.setValue(1, 1, mScale(1));
  scaleMatrix.setValue(2, 2, mScale(2));

  //Rotation
  rotationX.setValue(1, 1, cosX);
  rotationX.setValue(1, 2, -sinX);
  rotationX.setValue(2, 1, sinX);
  rotationX.setValue(2, 2, cosX);

  rotationY.setValue(0, 0, cosY);
  rotationY.setValue(0, 2, sinY);
  rotationY.setValue(2, 0, -sinY);
  rotationY.setValue(2, 2, cosY);

  rotationZ.setValue(0, 0, cosZ);
  rotationZ.setValue(0, 1, -sinZ);
  rotationZ.setValue(1, 0, sinZ);
  rotationZ.setValue(1, 1, cosZ);

  mMatrix = matrixMultMatrix(
    matrixMultMatrix(
      matrixMultMatrix(
        matrixMultMatrix(
          translationMatrix, rotationZ
        ), rotationY
      ), rotationX
    ), scaleMatrix
  );
}

GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
