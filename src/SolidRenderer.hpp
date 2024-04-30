#pragma once

#include "Camera.hpp"
#include "Image.hpp"
#include "Scene.hpp"

class SolidRenderer {
 public:
  SolidRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Image> image,
                std::shared_ptr<Camera> camera)
      : mImage(image), mScene(scene), mCamera(camera) {}

  void renderRaycast();

  void computeImageRow(size_t rowNumber);

  void shade(HitRecord &r);

  std::shared_ptr<Image> mImage;
  std::shared_ptr<Scene> mScene;
  std::shared_ptr<Camera> mCamera;
};
