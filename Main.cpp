// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Trim includes
#include "Point.h"
#include "CImg.h"
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <stack> 

using namespace cimg_library;
using namespace std;

unsigned int w = 800;
unsigned int h = 800;

CImg<unsigned char> maze(w, h, 1, 3, 255);
CImgDisplay dsp(w, h, "Maze", 0);
const unsigned char red[] = { 0, 0, 0 };

Point** intiGrid(int n);

void itterativeGen(Point** grid, stack<Point> genStack, int n);

void draw_line(cimg_library::CImg<uint8_t>& image,
	const int x1, const int y1,
	const int x2, const int y2,
	const uint8_t* const color,
	const unsigned int line_width);

int main()
{
	Point** grid;
	stack<Point> mazeGenStack;
	int n = 0;

	//Get Maze Size From User
	cout << "Enter Size of Maze: ";
	cin >> n;

	grid = intiGrid(n);

	//Choose random starting point (awful random method, with large n values you will see repeat patterns, needs rework)
	srand(time(NULL));
	Point startPoint = grid[rand() % n][rand() % n];
	mazeGenStack.push(startPoint);

	dsp.display(maze);
	itterativeGen(grid, mazeGenStack, n);

	dsp.render(maze);
	dsp.paint();

	Sleep(100000);
}


Point** intiGrid(int n) {
	//Create 2d Array of Points to represent grid
	Point** grid = new Point * [n];
	for (int i = 0; i < n; i++) {
		grid[i] = new Point[n];
	}

	//Inti Grid
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			grid[i][j] = Point(i, j);
		}
	}

	return grid;
}

void itterativeGen(Point** grid, stack<Point> genStack, int n) {
	srand(time(NULL));
	//Loop while stack isn't empty 
	while (!genStack.empty()) {

		//Pop cell off of stack
		Point cell = genStack.top();
		genStack.pop();

		//Get cells pos
		int pX = cell.x;
		int pY = cell.y;

		//Set cell as visited
		cell.visited = true;
		grid[pX][pY].visited = true;


		//Get all unvisted neighbors
		vector<Point> neighbors;
		neighbors.clear(); // needed?


		//Ugly, needs a rework. Checks if neighbors are valid and/or visited
		if ((pX - 1) >= 0 && !grid[pX - 1][pY].visited)
			neighbors.push_back(grid[pX - 1][pY]);

		if ((pY + 1) < n && !grid[pX][pY + 1].visited)
			neighbors.push_back(grid[pX][pY + 1]);

		if ((pX + 1) < n && !grid[pX + 1][pY].visited)
			neighbors.push_back(grid[pX + 1][pY]);

		if ((pY - 1) >= 0 && !grid[pX][pY - 1].visited)
			neighbors.push_back(grid[pX][pY - 1]);

		//Shuffle list of unvisted neighbors
		random_shuffle(neighbors.begin(), neighbors.end());

		if (neighbors.empty() == FALSE) {
			//Push current cell back to stack 
			genStack.push(cell);

			//Grab our "random" neighbor cell
			Point neighborCell = neighbors.back();
			neighbors.pop_back();

			//TODO: REMOVE WALL BETWEEN CELL AND NEIGHBOR

			draw_line(maze, cell.dispX, cell.dispY, neighborCell.dispX, neighborCell.dispY, red, 1);

			//Displays the "steps" of our maze at x intervals (ms)
			dsp.render(maze);
			dsp.paint();

			neighborCell.visited = true;
			grid[neighborCell.x][neighborCell.y].visited = true;

			genStack.push(neighborCell);
		}
	}
}


//~~~ IMPORTED ~~~
void draw_line(cimg_library::CImg<uint8_t>& image,
	const int x1, const int y1,
	const int x2, const int y2,
	const uint8_t* const color,
	const unsigned int line_width)
{
	if (x1 == x2 && y1 == y2) {
		return;
	}
	// Convert line (p1, p2) to polygon (pa, pb, pc, pd)
	const double x_diff = std::abs(x1 - x2);
	const double y_diff = std::abs(y1 - y2);
	const double w_diff = line_width / 2.0;

	// Triangle between pa and p1: x_adj^2 + y_adj^2 = w_diff^2
	// Triangle between p1 and p2: x_diff^2 + y_diff^2 = length^2 
	// Similar triangles: y_adj / x_diff = x_adj / y_diff = w_diff / length
	// -> y_adj / x_diff = w_diff / sqrt(x_diff^2 + y_diff^2) 
	const int x_adj = y_diff * w_diff / std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));
	const int y_adj = x_diff * w_diff / std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));

	// points are listed in clockwise order, starting from top-left
	cimg_library::CImg<int> points(4, 2);
	points(0, 0) = x1 - x_adj;
	points(0, 1) = y1 + y_adj;
	points(1, 0) = x1 + x_adj;
	points(1, 1) = y1 - y_adj;
	points(2, 0) = x2 + x_adj;
	points(2, 1) = y2 - y_adj;
	points(3, 0) = x2 - x_adj;
	points(3, 1) = y2 + y_adj;

	image.draw_polygon(points, color);
}