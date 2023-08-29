#include <fstream>
#include <iostream> 
#include <iomanip>
#include <iostream>
#include <math.h>
#include <list>
#include <iterator>
#include <string>

using namespace std;

const int length = 800;

class Point {
private:
    double xCoord;
    double yCoord;
public:
    Point() {
        xCoord = 0.0;
        yCoord = 0.0;
    }
    Point(double x, double y) {
        xCoord = x;
        yCoord = y;
    }

    Point(const Point& p) {
        xCoord = p.xCoord;
        yCoord = p.yCoord;
    }
    Point operator+(const Point& p) {
        Point a;
        a.xCoord = p.xCoord;
        a.yCoord = p.yCoord;
        return a;
    }
    double getX() { return xCoord; }
    double getY() { return yCoord; }
    void setX(double xP) { xCoord = xP; }
    void setY(double yP) { yCoord = yP; }
};

class Display {
private:
    int red, green, blue;
public:
    Display() {
        red = 0;
        green = 0;
        blue = 0;
    }
    int getRed() { return red; }
    int getGreen() { return green; }
    int getBlue() { return blue; }
    void setRed(int x) { red = x; }
    void setGreen(int x) { green = x; }
    void setBlue(int x) { blue = x; }
};

list<Point> list1;
Display arr[length][length] = { };

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

void readFromFile(string filename) {
    ifstream inData(filename);
    string line;
    while (getline(inData, line)) 
    {
        int ind = line.find(" ");
        double x = std::stod(line.substr(0, ind));
        double y = std::stod(line.substr(ind + 2, line.size()));

        Point* temp = new Point(x, y);
        list1.push_back(*temp);
    }
    inData.close();
}

Point* bruteForce() {
    double checkMin = distance(list1.front().getX(), list1.back().getX(), list1.front().getY(), list1.back().getY());
    Point* mins = new Point[2];
    list<Point>::iterator it;
    
    for (it = list1.begin(); it != list1.end(); it++) {
        list<Point>::iterator it2 = it; 
        for (++it2; it2 != list1.end(); it2++) {
            double temp = distance(it->getX(), it2->getX(), it->getY(), it2->getY());
            if (temp < checkMin) {
                checkMin = temp;
                mins[0].setX(it->getX());
                mins[0].setY(it->getY());
                mins[1].setX(it2->getX());
                mins[1].setY(it2->getY());
            }
        }
    }
    return mins;
}

int writeFile() {
    ofstream myfile("output.ppm");
    if (myfile.is_open())
    {
        myfile << "P3 ";
        myfile << length << " " << length << " " << "1";
        myfile << "\n";
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < length; j++) {
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
        for (j = 0; j < (length); j++) {
            arr[i][j].setRed(1);
            arr[i][j].setGreen(1);
            arr[i][j].setBlue(1);

        }
    }
}

void part0() {
    string x;
    cout << "Do you want to generate points? (yes or no)";
    cin >> x;
    srand(time(NULL));
    if (x == "yes") {
        ofstream points;
        points.open("points.txt");
        for (int i = 0; i < 60; i++) {
            Point* temp = new Point((rand() / (double)RAND_MAX), (rand() / (double)RAND_MAX));
            points << setprecision(23) << '\t' << (temp->getX()) << "  " << (temp->getY()) << endl;
        }
        points.close();
    }
}

void part1() {
    readFromFile("points.txt");
    fillWithBlank();
    Point* smallestDists = new Point[2]();
    smallestDists = bruteForce();

    list<Point>::iterator it;
    for (it = list1.begin(); it != list1.end(); ++it) {
        drawCircle(false, 3, length * it->getX(), length * it->getY());
        drawCircle(false, 2, length * it->getX(), length * it->getY());
    }
    drawCircle(true, 3, length * smallestDists->getX(), length * smallestDists->getY());
    drawCircle(true, 3, length * (smallestDists + 1)->getX(), length * (smallestDists + 1)->getY());
    drawCircle(true, 2, length * smallestDists->getX(), length * smallestDists->getY());
    drawCircle(true, 2, length * (smallestDists + 1)->getX(), length * (smallestDists + 1)->getY());

    writeFile();

    cout << setprecision(23) << "Coordinate 1: " << "(" << smallestDists->getX() << ", " << smallestDists->getY() << ") Coordinate 2: (" << (smallestDists + 1)->getX() << ", " << (smallestDists + 1)->getY() << ")" << endl;
    double min = distance((smallestDists->getX()), (smallestDists + 1)->getX(), smallestDists->getY(), (smallestDists + 1)->getY()); 
    cout << "min " << min << endl;

}

int main()
{
    part0();
    part1();
}