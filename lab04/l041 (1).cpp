#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <utility>

using namespace std;
using namespace std::chrono;

class Point {
private:
    double xCoord;
    double yCoord;
    double height;
public:
    Point() {
        xCoord = 0.0;
        yCoord = 0.0;
        height = 0;
    }
    Point(double x, double y) {
        xCoord = x;
        yCoord = y;
        height = 0;
    }
    Point(double x, double y, double h) {
        xCoord = x;
        yCoord = y;
        height = h;
    }
    Point(const Point& p) {
        xCoord = p.xCoord;
        yCoord = p.yCoord;
        height = 0;
    }
    double getX() { return xCoord; }
    double getY() { return yCoord; }
    double getHeight() { return height; }
    void setX(double xP) { xCoord = xP; }
    void setY(double yP) { yCoord = yP; }
    void setHeight(double h) { height = h; }
};

class Display {
private:
    int red, green, blue;
public:
    Display() {
        red = 1;
        green = 1;
        blue = 1;
    }
    int getRed() { return red; }
    int getGreen() { return green; }
    int getBlue() { return blue; }
    void setRed(int x) { red = x; }
    void setGreen(int x) { green = x; }
    void setBlue(int x) { blue = x; }
};

class Line {
private:
    Point* A;
    Point* B;
public:
    Line(Point* a, Point* b) {
        A = a;
        B = b;
    }
    Point* getA() { return A; }
    Point* getB() { return B; }
    double getSlope() { return (B->getY() - A->getY()) / (B->getX() - A->getX()); }
};

const int length = 400;
Display arr[length][length] = { };

bool ComparePoints(Point p1, Point p2)
{
    return (p1.getX() < p2.getX());
}

void illuminate(double x, double y) {
    int x1 = (int)(round(x));
    int y1 = (int)(round(y));
    if (!(x >= length || x < 0 || y >= length || y < 0)) {
        arr[x1][y1].setRed(0);
        arr[x1][y1].setGreen(0);
        arr[x1][y1].setBlue(0);
    }
}
void illuminateRed(double x, double y) {
    int x1 = (int)(round(x));
    int y1 = (int)(round(y));
    if (!(x >= length || x < 0 || y >= length || y < 0)) {
        arr[x1][y1].setRed(1);
        arr[x1][y1].setGreen(0);
        arr[x1][y1].setBlue(0);
    }
}

double distance(double x1, double x2, double y1, double y2) {
    double x = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    return sqrt(x);
}

void xRightDown(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = y1;
    int err = delY - delX;
    for (int i = x1; i < x2; i++) {
        illuminate(i, j);
        if (err >= 0) {
            j = j + 1;
            err = err - delX;
        }
        err = err + delY;
    }
}
void xLeftDown(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = y1;
    int err = delY - delX;
    for (int i = x1; i < x2; i++) {
        illuminate(i, j);
        if (err >= 0) {
            j = j - 1;
            err = err - delX;
        }
        err = err + delY;
    }
}
void xRightUp(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = y1;
    int err = delY - delX;
    for (int i = x1; i > x2; i--) {
        illuminate(i, j);
        if (err >= 0) {
            j = j + 1;
            err = err - delX;
        }
        err = err + delY;
    }
}
void xLeftUp(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = y1;
    int err = delY - delX;
    for (int i = x1; i > x2; i--) {
        illuminate(i, j);
        if (err >= 0) {
            j = j - 1;
            err = err - delX;
        }
        err = err + delY;
    }
}

void yRightDown(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = x1;
    int err = delX - delY;
    for (int i = y1; i < y2; i++) {
        illuminate(j, i);
        if (err >= 0) {
            j = j + 1;
            err = err - delY;
        }
        err = err + delX;
    }
}
void yLeftDown(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = x1;
    int err = delX - delY;
    for (int i = y1; i > y2; i--) {
        illuminate(j, i);
        if (err >= 0) {
            j = j + 1;
            err = err - delY;
        }
        err = err + delX;
    }

}
void yRightUp(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = x1;
    int err = delX - delY;
    for (int i = y1; i < y2; i++) {
        illuminate(j, i);
        if (err >= 0) {
            j = j - 1;
            err = err - delY;
        }
        err = err + delX;
    }
}
void yLeftUp(int x1, int x2, int y1, int y2, int delX, int delY) {
    int j = x1;
    int err = delX - delY;
    for (int i = y1; i > y2; i--) {
        illuminate(j, i);
        if (err >= 0) {
            j = j - 1;
            err = err - delY;
        }
        err = err + delX;
    }
}
void line(double x1, double y1, double x2, double y2) {
    int ldelX = x2 - x1;
    int ldelY = y2 - y1;
    double lx1 = x1;
    double ly1 = y1;
    double lx2 = x2;
    double ly2 = y2;
    if (abs(ldelX) >= abs(ldelY)) {
        if (ly2 > ly1 && lx2 > lx1) {
            xRightDown(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (ly2 == ly1) {
            for (int i = lx1; i < lx2; i++)
                illuminate(i, ly1);
        }
        else if (ly2 > ly1 && lx2 < lx1) {
            xRightUp(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (ly2 < ly1 && lx2 > lx1) {
            xLeftDown(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (ly2 < ly1 && lx2 < lx1) {
            xLeftUp(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
    }
    if (abs(ldelY) > abs(ldelX)) {
        if (ly2 > ly1 && lx2 > lx1) {
            yRightDown(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (lx2 == lx1) {
            for (int i = ly1; i < ly2; i++)
                illuminate(lx1, i);
        }
        else if (ly2 > ly1 && lx2 < lx1) {
            yRightUp(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (ly2 < ly1 && lx2 < lx1) {
            yLeftUp(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
        else if (ly2 < ly1 && lx2 > lx1) {
            yLeftDown(lx1, lx2, ly1, ly2, abs(ldelX), abs(ldelY));
        }
    }
}


void drawCircle(bool red, double radius, double x_c, double y_c) {
    int y, xmax, ySq, ySq_new, ty;
    xmax = (int)(radius * 0.70710678);
    y = radius;
    ySq = y * y;
    ty = (2 * y) - 1;
    ySq_new = ySq;

    for (int i = 0; i <= xmax; i++) {
        if ((ySq - ySq_new) >= ty) {
            ySq -= ty;
            y -= 1;
            ty -= 2;
        }
        if (red == false) {
            illuminate((i + x_c), (y + y_c));
            illuminate((i + x_c), (-y + y_c));
            illuminate((-i + x_c), (y + y_c));
            illuminate((-i + x_c), (-y + y_c));
            illuminate((y + x_c), (i + y_c));
            illuminate((y + x_c), (-i + y_c));
            illuminate((-y + x_c), (i + y_c));
            illuminate((-y + x_c), (-i + y_c));
        }
        if (red == true) {
            illuminateRed((i + x_c), (y + y_c));
            illuminateRed((i + x_c), (-y + y_c));
            illuminateRed((-i + x_c), (y + y_c));
            illuminateRed((-i + x_c), (-y + y_c));
            illuminateRed((y + x_c), (i + y_c));
            illuminateRed((y + x_c), (-i + y_c));
            illuminateRed((-y + x_c), (i + y_c));
            illuminateRed((-y + x_c), (-i + y_c));
        }
        ySq_new -= (2 * i) + 3;
    }
}

int writeFile() {
    ofstream myfile("quickhull.ppm");
    if (myfile.is_open())
    {
        myfile << "P3 ";
        myfile << length << " " << length << " " << "1";
        myfile << "\n";
        for (int j = 0; j < length; j++) {
             for (int i = 0; i < length; i++) {
                myfile << arr[i][j].getRed() << " ";
                myfile << arr[i][j].getGreen() << " ";
                myfile << arr[i][j].getBlue() << " ";
            }
            myfile << "\n";
        }
        myfile.close();
    }
    else
        cout << "Unable to open file.";
    return 0;
}

void fillWithBlank() {
    int i, j;
    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            arr[i][j].setRed(1);
            arr[i][j].setGreen(1);
            arr[i][j].setBlue(1);

        }
    }
}

pair<Point, Point> findLeftRight(vector<Point>& pointsV) {
    pair<Point, Point> retPair;
    //left is min, right is max
    retPair.first = pointsV[0];
    retPair.second = pointsV[0];
    for (unsigned int i = 1; i < pointsV.size(); i++) {
        if (pointsV[i].getX() < retPair.first.getX()) {
            retPair.first = pointsV[i];
        }
        if (pointsV[i].getX() > retPair.second.getX()) {
            retPair.second = pointsV[i];
        }
    }
    return retPair;
}

double counterClockWise(Point& A, Point& B, Point& C) {
    return (B.getX() - A.getX()) * (C.getY() - A.getY()) - (C.getX() - A.getX()) * (B.getY() - A.getY());
}

vector<Point> findSet(vector<Point>& pointSet, Point& A, Point& B) {
    vector<Point> retVector1;
    for (unsigned int i = 0; i < pointSet.size(); i++) {
        double CCWCheck = counterClockWise(A, B, pointSet[i]);
        if (CCWCheck < 0 && pointSet[i].getX() != A.getX() && pointSet[i].getX() != B.getX())
            retVector1.push_back(pointSet[i]);
    }
    return retVector1;
}

Point findFurthestPoint(vector<Point>& set, Point& P, Point& Q) {
    double height = 0;
    Point C = Point(set[0].getX(), set[0].getY());
    for (unsigned int i = 0; i < set.size(); i++) {
        double h = abs(counterClockWise(P, Q, set[i])) / distance(P.getX(), Q.getX(), P.getY(), Q.getY());
        if (h > height) {
            C.setX(set[i].getX());
            C.setY(set[i].getY());
            height = h;
        }
    }
    return C;
}

void findHull(vector<Point>& set, Point& P, Point& Q, vector<Point>& cHull) {
    if (set.size() == 0)
        return;
    Point furthest = findFurthestPoint(set, P, Q);
    vector<Point> left = findSet(set, P, furthest);
    findHull(left, P, furthest, cHull);
    cHull.push_back(furthest);
    vector<Point> right = findSet(set, furthest, Q);
    findHull(right, furthest, Q, cHull);
}

vector<Point> quickHull(vector<Point>& pointSet) {
    vector<Point> retVect;
    pair<Point, Point> minMax = findLeftRight(pointSet);
    Point left = minMax.first;
    Point right = minMax.second;
    vector<Point> abRight = findSet(pointSet, left, right);
    vector<Point> baRight = findSet(pointSet, right, left);
    retVect.push_back(left);
    findHull(abRight, left, right, retVect);
    retVect.push_back(right);
    findHull(baRight, right, left, retVect);
    retVect.push_back(left);
    return retVect;
}

vector<Point> part0() {
    vector<Point> points;
    ofstream pointPush;
    pointPush.open("points.txt");
    srand(time(NULL));
    for (int i = 0; i < 60; i++) {
        Point p = Point((rand() / (double)RAND_MAX), (rand() / (double)RAND_MAX));
        pointPush << setprecision(23) << '\t' << (p.getX()) << "  " << (p.getY()) << endl;
        points.push_back(p);
    }
    pointPush.close();
    return points;
}   

void part1() {
    vector<Point> points = part0();
    //fillWithBlank();
    vector<Point> convexHullFinal = quickHull(points);
    for (unsigned int i = 0; i < points.size() ; i++) {
        drawCircle(false, 3, length * points[i].getX(), length * points[i].getY());
    }
    for (unsigned int i = 0; i < convexHullFinal.size(); i++) {
        drawCircle(true, 3, length * convexHullFinal[i].getX(), length * convexHullFinal[i].getY());
    }
    for (unsigned int i = 0;i < convexHullFinal.size() - 1; i++) {
        line(length*convexHullFinal[i].getX(), length*convexHullFinal[i].getY(), length*convexHullFinal[i + 1].getX(), length*convexHullFinal[i + 1].getY());
    }
    writeFile();
}

int main()
{
    part1();
}
