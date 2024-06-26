#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <complex>
#include <unistd.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <random>

#include "Color.hpp"
#include "GLPoint.hpp"
#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"
    int main(int argc, char **argv) {
    // Dimensionen des Ergebnisbildes im Konstruktor setzen
    std::shared_ptr<Image> img = std::make_shared<Image>(640, 480);
    // Verwendete Modelle festlegen
    std::vector<std::string> path_vector;
    path_vector.emplace_back("../data/bunny/bunny_scaled.ply");
    path_vector.emplace_back("../data/basicObjects/cube_scaled.ply");
    path_vector.emplace_back("../data/basicObjects/cube_scaled.ply");
    path_vector.emplace_back("../data/basicObjects/cube_scaled.ply");
    path_vector.emplace_back("../data/basicObjects/cube_scaled.ply");
    path_vector.emplace_back("../data/basicObjects/cube_scaled.ply");
    // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
    auto scene = std::make_shared<Scene>();
    scene->load(path_vector);

    /* Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer
    WireframeRenderer wf = WireframeRenderer(scene, img);
    Color color(0, 0, 0);
    */
    /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier
    //Placing point c in the Center

    int x0 = (int) img->getWidth() / 2;
    int y0 = (int) img->getHeight() / 2;
    GLPoint c = GLPoint(x0, y0, 0);
    int a = 200;

    //Generating lines spanning every Quadrant with Euler's Formula where r*exp(iπ)=r * (cosπ-isinπ)
    for (int n = 0; n < 16; n++) {
        double angle = n * M_PI / 8;
        double re = x0 + a * cos(angle);
        double im = y0 + a * sin(angle);
        GLPoint z = GLPoint(re, im, 0);
        wf.drawBresenhamLine(c, z, color);
        //Closing the cirle
        angle = (n+1) * M_PI / 8;
        GLPoint p = GLPoint(x0 + a * cos(angle), y0 + a * sin(angle), 0);
        wf.drawBresenhamLine(z, p, color);
    }
    */
    /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier

    //Random Number Generator (set outside to prevent issues experienced before)
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0, 1);

    for (int n = 0;n < 16; n++) {
      // Equation obtained solving for 0.5 * o * Zn
      double angle = (2 * n + 1) * M_PI / 16;
      double re = x0 - 0.5 * a * cos(angle);
      double im = y0 + 0.5 * a * sin(angle);

      //Randomly generated RGB values
      float r = distribution(generator);
      float g = distribution(generator);
      float b = distribution(generator);
      Color fillColor(r,g,b);

      //Seed Fill Generation
      GLPoint seed = GLPoint(re,im, 0);
      wf.seedFillArea(seed, color, fillColor);
    }
    */

    /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */
    std::vector<Model>& models = scene->getModels();
    Model& bunny = models[0];
    Model& cube1 = models[1];
    Model& cube2 = models[2];
    Model& cube3 = models[3];
    Model& cube4 = models[4];
    Model& cube5 = models[5];

    /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */

    /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
    // Diese Einstellungen beziehen sich auf den world space
    // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
    // den ViewSpace benötigen, da die Strahlen für Raycasting und Raytracing im
    // World space definiert sind. Modelle müssen also lediglich in den World
    // space überführt werden

    /* Aufgabenblatt 3: kommentieren Sie die Zeilen wieder ein, die eine Kamera erzeugen und zur Scene hinzufügen*/
    auto cam = std::make_shared<Camera>();
    GLPoint eye = GLPoint(0.0, 0.0, 200.0);
    cam->setEyePoint(eye);
    cam->setUp(GLVector(0.0, 1.0, 0.0));
    GLVector viewDirection = GLVector(0.0, 0.0, -1.0);
    viewDirection.normalize();
    cam->setViewDirection(viewDirection);
    cam->setSize(img->getWidth(), img->getHeight());
    scene->setCamera(cam);

    /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fügen Sie diese zur Szene hinzu*/
    Sphere sphere1 = Sphere();
    sphere1.setPosition(GLPoint(-150, 50, -30));
    sphere1.setRadius(50.0);
    scene->addSphere(sphere1);

    Sphere sphere2 = Sphere();
    sphere2.setPosition(GLPoint(150, -50, -30));
    sphere2.setRadius(50.0);
    scene->addSphere(sphere2);

    /* Aufgabenblatt 4: Setzen Sie materialeigenschaften für die Kügeln und die Modelle. Die Materialeigenschaften für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */
    Material bunny_material,
    cube1_material, cube2_material, cube3_material, cube4_material, cube5_material,
    sphere1_material, sphere2_material;

    bunny_material.reflection = 1.0;

    sphere1_material.reflection = 1,0;
    sphere2_material.reflection = 1.0;
    /* Aufgabenblatt 3: (Wenn nötig) Transformationen der Modelle im World space, sodass sie von der Kamera gesehen werden könnnen. Die nötigen Transformationen für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */

    bunny.setTranslation(GLVector(0,-15,-20));
    bunny.setScale(GLVector(1,1,1));
    bunny.setRotation(GLVector(0,110,0));

    cube1.setTranslation(GLVector(-60,-50,5));
    cube1.setScale(GLVector(1,1,1));
    cube1.setRotation(GLVector(0,0,0));

    cube2.setTranslation(GLVector(60,50,-50));
    cube2.setScale(GLVector(1,1,1));
    cube2.setRotation(GLVector(0,0,0));

    cube3.setTranslation(GLVector(-80,10,-100));
    cube3.setScale(GLVector(1,1,1));
    cube3.setRotation(GLVector(0,0,0));

    cube4.setTranslation(GLVector(0,-100,0));
    cube4.setScale(GLVector(500,0.01,500));
    cube4.setRotation(GLVector(0,0,0));

    cube5.setTranslation(GLVector(0,0,-200));
    cube5.setScale(GLVector(500,500,500));
    cube5.setRotation(GLVector(0,0,0));

    //Setting Materials 1xBunny, 4x Cubes, 2x Spheres
    bunny_material.color = Color(1,0,1);
    bunny.setMaterial(bunny_material);

    cube1_material.color = Color(0,0,1);
    cube1.setMaterial(cube1_material);

    cube2_material.color = Color(0,1,0);
    cube2.setMaterial(cube2_material);

    cube3_material.color = Color(1,0,0);
    cube3.setMaterial(cube3_material);

    cube4_material.color = Color(0.9,0.9,0.9);
    cube4.setMaterial(cube4_material);

    cube5_material.color = Color(1, 1, 1);
    cube5.setMaterial(cube5_material);

    sphere1_material.color = Color(0,1,1);
    scene->getSpheres()[0].setMaterial(sphere1_material);

    sphere2_material.color = Color(1,1,0);
    scene->getSpheres()[1].setMaterial(sphere2_material);

    /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/

    /* Aufgabenblatt 4 Fügen Sie ein Licht zur Szene hinzu */
    scene->addPointLight(GLPoint (0,0,200));

    /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mit einem shared_ptr) und rufen sie die Funktion renderRaycast auf */

    std::shared_ptr<SolidRenderer> sr = std::make_shared<SolidRenderer>(scene, img, cam);
    sr->renderRaycast();

    //wf.renderScene(color);

    // Schreiben des Bildes in Datei
    if (argc > 1) {
        img->writeAsPPM(argv[1]);
        std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                  << img->getHeight() << " in Datei " << argv[1] << " geschrieben."
                  << std::endl;
    } else {
        std::cerr
            << "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
            << std::endl;
    }

    return 0;
}
