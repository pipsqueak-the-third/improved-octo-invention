#include "SolidRenderer.hpp"

//#include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
  // This function is part of the base

  std::cout << "Rendern mittels Raycast gestartet." << std::endl;
  // Berechnung der einzelnen Rows in eigener Methode um die
  // Parallelisierbarkeit zu verbessern

  // Ohne parallelisierung:
  //for(size_t i = 0; i < mImage->getHeight(); ++i ){
  //    computeImageRow( i );
  //}
  //  Parallelisierung mit OpenMP:
  #pragma omp parallel for
  for(size_t i = 0; i < mImage->getHeight(); ++i ){
      computeImageRow( i );
  }
}
/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
    //Rendering for every pixel in the x-axis given the y-axis location (rowNumber)
    for (size_t columnNumber = 0; columnNumber < mImage->getWidth(); columnNumber++) {
        //Found the way to initialize the ray inside the Camera.cpp (55)
        Ray ray = mCamera->getRay((int)columnNumber, (int)rowNumber);
        // initializing the HitRecord as instructed in structs.hpp(20)
        HitRecord hr;
        hr.color = Color(0,0,0);
        hr.parameter = std::numeric_limits<double>::max(); //Initialized to a large value
        hr.modelId = -1;
        hr.sphereId = -1;
        hr.triangleId = -1;
        hr.recursions = 0;

        //Checking intersection
        if (mScene->intersect(ray,hr,EPSILON)){
            shade(hr);
            mImage->setValue((int)columnNumber,(int)rowNumber,hr.color);
        } else {
            mImage->setValue((int)columnNumber,(int)rowNumber,Color(1,1,1));
        }
    }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
    // --- PHONG SHADING ---
    GLVector N = r.normal;

    //light vector
    GLVector L = GLVector(
        mScene->getPointLights()[0](0) - r.intersectionPoint(0),
        mScene->getPointLights()[0](1) - r.intersectionPoint(1),
        mScene->getPointLights()[0](2)  - r.intersectionPoint(2)
    );
    L.normalize();

    //reflection vector
    GLVector R = 2 * (L * N) * N - L;
    R.normalize();

    //viewpoint vector
    GLVector V = GLVector(
        (mScene->getViewPoint() - r.intersectionPoint)(0),
        (mScene->getViewPoint() - r.intersectionPoint)(1),
        (mScene->getViewPoint() - r.intersectionPoint)(2));
    V.normalize();

    const double k_ambient = 0.4;
    const double k_diffuse = 0.4;
    const double k_specular = 0.2;
    //intensity of highlights
    double n = 20;

    //I think this is what needed implementing
    double I_i = 1.0;
    double I_ambient = k_ambient * I_i;
    double I_diffuse = k_diffuse * I_i * std::max(0.0, L * N);
    double I_specular = k_specular * I_i * std::pow(std::max(0.0, R * V), n);

    //I believe this is the correct light intensity equation
    double I_total = I_ambient + I_diffuse + I_specular;
    //--- PHONG SHADING END ---

    // --- SHADOWS ---
    double intensity = 1;
    Ray shadow = Ray();
    GLVector offset = (mScene->getPointLights()[0]) - r.intersectionPoint;
    //I think we were supposed EPSILON
    shadow.origin = GLPoint(
        r.intersectionPoint(0) + (EPSILON * offset(0)),
        r.intersectionPoint(1) + (EPSILON * offset(1)),
        r.intersectionPoint(2) + (EPSILON * offset(2)) );

    shadow.direction = (mScene->getPointLights()[0]) - shadow.origin;
    shadow.direction.normalize();

    HitRecord shadow_hr;
    shadow_hr.color = Color(1,1,1);
    shadow_hr.intersectionPoint = shadow.origin;
    shadow_hr.parameter = sqrt(
        pow(mScene->getPointLights()[0](0) - shadow.origin(0),2)
        + pow(mScene->getPointLights()[0](1) - shadow.origin(1),2)
        + pow(mScene->getPointLights()[0](2) - shadow.origin(2),2)
    );
    shadow_hr.rayDirection = shadow.direction;
    shadow_hr.recursions = 0;
    shadow_hr.modelId = r.modelId;
    shadow_hr.sphereId = r.sphereId;

    if(mScene->intersect(shadow, shadow_hr, EPSILON)) {
        intensity = 0.5;
    }
    // --- SHADOWS END ---

    // --- RAYTRACING ---
    float reflection;
    if (r.modelId != -1) { 
        reflection = mScene->getModels()[r.modelId].getMaterial().reflection;
    } else {
        reflection = mScene->getSpheres()[r.sphereId].getMaterial().reflection;
    }

    Color finalColor(0, 0, 0);
    // Recursion
    if (reflection > 0 && r.recursions < 6) {
        // Calculate direction of reflection ray
        GLVector reflectionDirection = (2 * (V * N) * N) - V;
        reflectionDirection.normalize();

        //init reflection ray
        Ray reflection_ray;
        reflection_ray.direction = reflectionDirection;
        reflection_ray.origin = r.intersectionPoint + (EPSILON * reflectionDirection);

        //intit reflection hr
        HitRecord reflection_hr;
        reflection_hr.color = Color(0,0,0);
        reflection_hr.modelId = -1;
        reflection_hr.sphereId = -1;
        reflection_hr.parameter = std::numeric_limits<double>::max();
        reflection_hr.rayDirection = reflection_ray.direction;
        reflection_hr.recursions = r.recursions + 1;
        reflection_hr.intersectionPoint = r.intersectionPoint;
        reflection_hr.rayDirection = reflection_ray.direction;

        //std::cout << reflection_hr.recursions;

        if (mScene->intersect(reflection_ray, reflection_hr, EPSILON)) {
            shade(reflection_hr);
            finalColor = reflection_hr.color;
            finalColor *= reflection;
        }
    }
    // --- RAYRTRACING END ---

    if (r.modelId != -1){
        Color phongColor = mScene->getModels()[r.modelId].getMaterial().color;
        phongColor *= I_total;
        phongColor *= intensity;
        r.color = phongColor;
        r.color += finalColor;
    }
    else if (r.sphereId != -1){
        Color phongColor = mScene->getSpheres()[r.sphereId].getMaterial().color;
        phongColor *= I_total;
        phongColor *= intensity;
        r.color = phongColor;
        r.color += finalColor;
    }
}
