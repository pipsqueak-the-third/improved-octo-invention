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
  printf("%f", mScene->getSpheres()[0].getMaterial().color.b);
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

    GLVector N = r.normal;
    GLVector L = GLVector(-r.rayDirection(0), -r.rayDirection(1), -r.rayDirection(2));
    L.normalize();
    GLVector R = 2 * (L * N) * (N - L);
    R.normalize();
    GLVector V = GLVector(
        (mScene->getViewPoint() - r.intersectionPoint)(0),
        (mScene->getViewPoint() - r.intersectionPoint)(1),
        (mScene->getViewPoint() - r.intersectionPoint)(2));
    V.normalize();

    const double k_ambient = 0.4;
    const double k_diffuse = 0.4;
    const double k_specular = 0.2;

    double shiny = r.modelId != -1 ? mScene->getModels() [r.modelId].getMaterial().reflection : mScene->getModels()[r.sphereId].getMaterial().reflection;

    double I_i = 1;
    double I_ambient = 0.7;

    double I_total = I_ambient * k_ambient + k_diffuse * I_i * (L * N) + k_specular * I_i * pow((R * V), shiny);

    if (r.modelId != -1){
        r.color = (mScene->getModels()[r.modelId].getMaterial().color);
        r.color *= I_total;
    }
    else if (r.sphereId != -1){
        r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
        r.color *= I_total;
    }
}
