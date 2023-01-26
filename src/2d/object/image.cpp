#include "image.h"

void Image::draw(int offsetX, int offsetY) {
    imageData.draw(TwoDimensionalObject::coordinates.x + offsetX, TwoDimensionalObject::coordinates.y + offsetY);
}

void Image::rotate90() {
    imageData.rotate90(1);
}