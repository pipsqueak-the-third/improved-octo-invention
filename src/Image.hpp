#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "Color.hpp"

/**
** Repräsentiert ein Bild mit den Maßen mWidth und mHeight
** Der Punkt (0,0) liegt hierbei unten rechts
** Der Wertebereich je Farbkanal liegt zwischen 0.0 und 1.0
**/
class Image {
 public:
  Image(size_t width, size_t height);
  void writeAsPPM(std::string filename);
  void setValue(int x, int y, Color color);
  std::vector<Color> &getValues();
  size_t getWidth() const;
  size_t getHeight() const;

 private:
  size_t mWidth;
  size_t mHeight;
  Color mBackgroundColor = Color(1.0, 1.0, 1.0);
  std::vector<Color> mValues;
};
