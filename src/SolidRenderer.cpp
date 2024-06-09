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
  /*
  for(size_t i = 0; i < mImage->getHeight(); ++i ){
      computeImageRow( i );
  }
  */
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
    for (int i = 0; i < mImage->getWidth(); i++) {
        //Found the way to initialize the ray inside the Camera.cpp (55)
        Ray ray = mCamera->getRay(i, (int)rowNumber);
        // initializing the HitRecord as instructed in structs.hpp(20)
        HitRecord hr;
        //Not sure how to initialize Color properly
        hr.color = Color(1,1,1);
        hr.parameter = i - mCamera->getEyePoint()(1);
        hr.triangleId = -1;
        hr.sphereId = -1;
        //Checking intersection
        if (mScene->intersect(ray,hr,EPSILON)){
            mImage->setValue(i, (int)rowNumber, hr.color);
        } else {
            mImage->setValue(i,(int)rowNumber,hr.color);
        }
    }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
}
