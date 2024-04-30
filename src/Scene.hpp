#pragma once

#include "Camera.hpp"
#include "Model.hpp"
#include "Sphere.hpp"
#include "math.hpp"
#include "structs.hpp"

class Scene {
 public:
  Scene();

  void load(const std::vector<std::string> &pFiles);

  bool intersect(const Ray &ray, HitRecord &hitRecord, const float epsilon);

  bool triangleIntersect(const Ray &ray, const Triangle &triangle,
                         HitRecord &hitRecord, const float epsilon);

  bool sphereIntersect(const Ray &ray, const Sphere &sphere,
                       HitRecord &hitRecord, const float epsilon);

  void addPointLight(GLPoint pointLight);
  void addModel(Model model);
  void addSphere(Sphere sphere);

  void setCamera(std::shared_ptr<Camera> cam);
  std::shared_ptr<Camera> getCamera() const;
  GLPoint getViewPoint() const;

  std::vector<Model> &getModels(); // nicht const damit diese modifiziert werden können
  std::vector<Sphere> &getSpheres();
  std::vector<GLPoint> &getPointLights();

 private:
  std::shared_ptr<Camera> mCamera;
  std::vector<GLPoint> mPointLights;
  std::vector<Model> mModels;
  std::vector<Sphere> mSpheres;
};
