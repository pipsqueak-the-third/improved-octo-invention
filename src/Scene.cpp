#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

Scene::Scene() {}

/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 *  (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    for (int sphere_id = 0; sphere_id < mSpheres.size(); sphere_id++) {
      if (sphereIntersect(ray, mSpheres[sphere_id], hitRecord, epsilon)){
          hitRecord.sphereId = sphere_id;
          hitRecord.color = mSpheres[sphere_id].getMaterial().color;
          hitRecord.print();
          return true;
      }
    }
    for (int model_id = 0; model_id < mModels.size(); model_id++) {
      for (int triangle_id = 0; triangle_id < mModels[model_id].mTriangles.size(); triangle_id++) {
        if(triangleIntersect(ray, mModels[model_id].mTriangles[triangle_id], hitRecord, epsilon)) {
            hitRecord.modelId = model_id;
            hitRecord.triangleId = triangle_id;
            hitRecord.print();
            return true;
        }
      }
    }
    return false; // Platzhalter; entfernen bei der Implementierung
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    //Setting up Barycentric Coordinates to determine whether the point lies inside the triangle
    GLVector v0 = (GLVector)(triangle.vertex[1] - triangle.vertex[0]);
    GLVector v1 = (GLVector)(triangle.vertex[2] - triangle.vertex[1]);
    GLVector v2 = (GLVector)(triangle.vertex[2] - triangle.vertex[0]);

    //Found this implementation and decided it makes the most sense
    //https://ceng2.ktu.edu.tr/~cakir/files/grafikler/Texture_Mapping.pdf
    float d00 = (float) dotProduct(v0,v0);
    float d01 = (float) dotProduct(v0,v1);
    float d02 = (float) dotProduct(v0,v2);
    float d11 = (float) dotProduct(v1,v1);
    float d12 = (float) dotProduct(v1,v2);

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d02 - d01 * d12) / denom;
    float w = (d00 * d12 - d01 * d02) / denom;

    //Changed from the previous implementation (written below), into one that uses epsilon to fix inconsistencies
    //return (v >= 0) && (w >= 0) && (v + w <= 1);
    return (v >= -epsilon) && (w >= -epsilon) && (v + w <= 1 + epsilon);
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
  // Probably need to change something with the hitRecord. I dunno. Don't fully understand that yet
  GLPoint m = sphere.getPosition();
  float r = (float)sphere.getRadius();
  GLPoint e = ray.origin;
  GLVector v = ray.direction;

  float a = dotProduct(v, v);
  float b = -2 * dotProduct(v, e-m);
  float c = dotProduct(e-m, e-m) * pow(r, 2);

  float t = b * b - 4*a*c;

  //Added epsilon as a small fix
  return (t >= -epsilon);
}

/**
** Liest die Modelle (deren Dateinamen in pFiles stehen) ein und speichert sie
*in mModels
**/
void Scene::load(const std::vector<std::string> &pFiles) {
  std::cout << "Laden der PLY Dateien:" << std::endl;
  // Für alle Objekte (Objekt meint hier das selbe wie Model)
  for (int obj_nr = 0; obj_nr < pFiles.size(); obj_nr++) {
    std::cout << "\tModel-Index: " << obj_nr << std::endl;
    // Assimp übernimmt das Einlesen der ply-Dateien
    Assimp::Importer importer;
    const aiScene *assimpScene = importer.ReadFile(
        pFiles[obj_nr], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);
    
      if( !assimpScene)
        {
          std::cout << importer.GetErrorString() << std::endl;
        }
      assert(assimpScene);
    auto meshes = assimpScene->mMeshes;
    // Neues Model erstellen
    Model model = Model();

    // Für alle Meshes des Models
    for (int i = 0; i < assimpScene->mNumMeshes; i++) {
      std::cout << "\t\tMesh-Index: " << i << " (" << meshes[i]->mNumFaces
                << " Faces)" << std::endl;
      auto faces = meshes[i]->mFaces;
      auto vertices = meshes[i]->mVertices;

      // Für alle Faces einzelner Meshes
      for (int j = 0; j < meshes[i]->mNumFaces; j++) {
        // Dreieck konstruieren und nötige Werte berechnen
        Triangle tri;
        assert(faces[j].mNumIndices == 3);
        tri.vertex[0] = GLPoint(vertices[faces[j].mIndices[0]].x,
                                vertices[faces[j].mIndices[0]].y,
                                vertices[faces[j].mIndices[0]].z);
        tri.vertex[1] = GLPoint(vertices[faces[j].mIndices[1]].x,
                                vertices[faces[j].mIndices[1]].y,
                                vertices[faces[j].mIndices[1]].z);
        tri.vertex[2] = GLPoint(vertices[faces[j].mIndices[2]].x,
                                vertices[faces[j].mIndices[2]].y,
                                vertices[faces[j].mIndices[2]].z);
        GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                       tri.vertex[2] - tri.vertex[0]);
        normal.normalize();
        tri.normal = normal;
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
        model.mTriangles.push_back(tri);
      }
    }
    // Immer das gleiche Material für das Model setzen
    Material material;
    material.color = Color(0.00, 1.00, 0.00);
    model.setMaterial(material);
    // Jedes Model zum Vector der Models der Scene hinzufügen
    mModels.push_back(model);
  }

  std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) { mCamera = cam; }

std::shared_ptr<Camera> Scene::getCamera() const { return mCamera; }

GLPoint Scene::getViewPoint() const {
  if (mCamera)
    return mCamera->getEyePoint();
  else {
    std::cerr << "No Camera set to get view point from." << std::endl;
    return GLPoint(0, 0, 0);
  }
}



void Scene::addPointLight(GLPoint pointLight) {
  mPointLights.push_back(pointLight);
}

void Scene::addModel(Model model) { mModels.push_back(model); }

void Scene::addSphere(Sphere sphere) { mSpheres.push_back(sphere); }

std::vector<Model> &Scene::getModels() { return mModels; }

std::vector<Sphere> &Scene::getSpheres() { return mSpheres; }

std::vector<GLPoint> &Scene::getPointLights() { return mPointLights; }
