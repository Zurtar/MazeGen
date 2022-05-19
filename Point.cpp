#include "Point.h"
#include <iostream>

Point::Point()
{
}

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;

	this->dispX = (x + 1) * 10;
	this->dispY = (y + 1) * 10;
}

int Point::getX() {
	return this->x;
}

int Point::getY() {
	return this->y;
}

void Point::setX(int x) {
	this->x = x;
}

void Point::setY(int y) {
	this->y = y;
}