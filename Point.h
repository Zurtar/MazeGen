#pragma once
class Point
{
public:
	int dispX, dispY;
	int x, y;
	bool visited = false;

	Point();

	Point(int x, int y);

	int getX();

	int getY();

	void setX(int x);

	void setY(int y);
};

