#include "Image.hpp"

// http://netpbm.sourceforge.net/doc/ppm.html
void Image::writeAsPPM(std::string filename) {
  // Skalierung und Maximalwert je Farbkanal
  double scale = 255.0;
  int maxPossibleValue = 255;

  // Headerinformationen in Datei schreiben
  std::ofstream of(filename);
  of << "P3\n"
    // << "# result\n"
     << mWidth << " " << mHeight << "\n"
     << maxPossibleValue << "\n";

  for (size_t ii = 0; ii < mValues.size(); ++ii) {
    int i = mWidth * mHeight - ii - 1;
    of << (int)(scale * mValues[i].r) << " " << (int)(scale * mValues[i].g)
       << " " << (int)(scale * mValues[i].b) << "\n";
  }

  of.flush();
}

Image::Image(size_t width, size_t height) {
  mWidth = width;
  mHeight = height;
  mValues = std::vector<Color>(width * height);
  std::fill(mValues.begin(), mValues.end(), mBackgroundColor);
}

std::vector<Color> &Image::getValues() { return mValues; }

/**
** Setzt den Pixelwert an der Stelle (x,y) auf die Farbe color
** macht nichts wenn (x,y) außerhalb der Dimension des Bildes liegt
**/
void Image::setValue(int x, int y, Color color) {
  if (x < mWidth && y < mHeight) {
    mValues[y * mWidth + x] = color;
  }
}

size_t Image::getWidth() const { return mWidth; }

size_t Image::getHeight() const { return mHeight; }
