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
    bool hit = false;
    int sphere_id = 0;
    int model_id = 0;
    int triangle_id = 0;
    for (Sphere sphere : mSpheres) {
      if (sphereIntersect(ray,sphere, hitRecord, epsilon)){
          hitRecord.sphereId = sphere_id;
          hitRecord.color = mSpheres[sphere_id].getMaterial().color;
          hit = true;
          hitRecord.printColor();
      }
      sphere_id ++;
    }
    for (Model model : mModels) {
      for (Triangle triangle : model.mTriangles) {
        //Transforming Triangle
        Triangle trans_triangle = Triangle();

        trans_triangle.vertex[0] = model.getTransformation() * triangle.vertex[0];
        trans_triangle.vertex[1] = model.getTransformation() * triangle.vertex[1];
        trans_triangle.vertex[2] = model.getTransformation() * triangle.vertex[2];

        trans_triangle.normal = crossProduct(trans_triangle.vertex[0] - trans_triangle.vertex[2],
                                                  trans_triangle.vertex[1] - trans_triangle.vertex[2]);
        trans_triangle.normal.normalize();

        hitRecord.modelId = model_id;
        hitRecord.triangleId = triangle_id;
        hitRecord.color = mModels[model_id].getMaterial().color;

          if(triangleIntersect(ray,trans_triangle, hitRecord, epsilon)) {
              hitRecord.modelId = model_id;
              hitRecord.triangleId = triangle_id;
              hit = true;
          }
        triangle_id++;
      }
      model_id++;
    }
    return hit;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    //New Implementation based on https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
    GLVector edge1 = triangle.vertex[1] - triangle.vertex[0];
    GLVector edge2 = triangle.vertex[2] - triangle.vertex[0];
    GLVector pvec = crossProduct(ray.direction, edge2);

    double det = dotProduct(edge1, pvec);
    if (det < epsilon && det > -epsilon){
        return false;
    }
    double invDet = 1 / det;
    GLVector tvec = ray.origin - triangle.vertex[0];

    double u = dotProduct(tvec,pvec) * invDet;
    if (u < 0 || u > 1){
        return false;
    }
    GLVector qvec = crossProduct(tvec, edge1);
    double v = dotProduct(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1){
        return false;
    }
    double t = dotProduct(edge2,qvec) * invDet;
    if (t <= epsilon || t >= hitRecord.parameter){
        return false;
    }
    hitRecord.parameter = t;
    hitRecord.intersectionPoint = ray.origin + t * ray.direction;
    hitRecord.normal = triangle.normal;
    hitRecord.normal.normalize();
    return true;
}


/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
  // Probably need to change something with the hitRecord. I dunno. Don't fully understand that yet
  double r = sphere.getRadius();
  GLPoint m = sphere.getPosition();
  GLPoint e = ray.origin;
  GLVector v = ray.direction;

  double a = dotProduct(v, v);
  double b = 2 * dotProduct(v, e-m);
  double c = (e-m, e-m).norm2() - pow(r, 2);

  //We obviously remove the possibility of any imaginary values
  double discriminant = b * b - 4 * a * c;
  if (discriminant < epsilon){
      return false;
  }

  double t0 = (-b - sqrt(discriminant))/(2 * a);
  double t1 = (-b + sqrt(discriminant))/(2 * a);

  if (t0 < epsilon || t1 < epsilon){
      return false;
  }
  if (t0 > hitRecord.parameter || t1 > hitRecord.parameter){
      return false;
  }
  double t = t0 < t1 ? t0 : t1;
  hitRecord.parameter = t;
  hitRecord.intersectionPoint = e + t * v;
  hitRecord.rayDirection = ray.direction;

  hitRecord.normal = (e - m).norm() < r ?
          m - hitRecord.intersectionPoint:
          hitRecord.intersectionPoint - m;
  hitRecord.normal.normalize();

  return true;
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