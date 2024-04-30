#include "WireframeRenderer.hpp"
#include "stack"
/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2)
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    //Aliases for easier access to (x1,y2) and (x2,y2) coordinates
    int x1 = (int)(p1(0));
    int y1 = (int)(p1(1));
    int x2 = (int)(p2(0));
    int y2 = (int)(p2(1));

    //Delta experiences no change per quadrant
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    //Here we define the quadrant that it finds itself in
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dy - dx;

    while (x1 != x2 || y1 != y2) {
        //Here taken into consideration that x is flipped as explained in Image.hpp (12)
        mImage->setValue((int) mImage->getWidth() - x1, y1, color);
        int e2 = 2 * err;

        if (e2 < dy) {
            x1 += sx;
            err += dy;
        }
        if (e2 > -dx) {
            y1 += sy;
            err -= dx;
        }
    }
}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
    //aliases for width and height defined
    int width = (int)(mImage->getWidth());
    int height = (int)(mImage->getHeight());

    //Implementation of the Flood Fill Algorithm https://de.wikipedia.org/wiki/Floodfill
    std::stack<GLPoint> stack;
    stack.push(seed);

    while (!stack.empty()) {
        GLPoint toppy = stack.top();
        stack.pop();

        int x = (int)(toppy(0));
        int y = (int)(toppy(1));

        // Check if the current point is within the image bounds
        if (x >= 0 && x < width && y >= 0 && y < height) {

            // Get the color of the current pixel using info from Image.cpp (40)
            Color currentColor = mImage->getValues()[y * width + x];

            // Check if the current pixel hasn't been visited and is not the borderColor
            if (currentColor!=(fillColor) && currentColor!=(borderColor)){

                // Fill the current pixel with the fillColor
                mImage->setValue(x, y, fillColor);

                // Push neighboring points onto the stack
                stack.emplace(x + 1, y,0);
                stack.emplace(x - 1, y,0);
                stack.emplace(x, y + 1,0);
                stack.emplace(x, y - 1,0);
            }
        }
    }
}
