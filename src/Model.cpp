
#include "Model.hpp"

// Konstruktor
Model::Model() {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
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


GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
