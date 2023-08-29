#include <fstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <list>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <unordered_map>


using namespace std;
using namespace std::chrono;


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

class Segment {
private:
    Point a, b;
public:
    Segment() {
        a.setX(0);
        a.setY(0);
        b.setX(0);
        b.setY(0);
    }
    Segment(Point x, Point y) {
        a.setX(x.getX());
        a.setY(x.getY());
        b.setX(y.getX());
        b.setY(y.getY());
    }
    Point getA() { return a; }
    Point getB() { return b; }
    double getDistance() {
        double temp = (b.getX() - a.getX()) * (b.getX() - a.getX()) + (b.getY() - a.getY()) * (b.getY() - a.getY());
        return sqrt(temp);
    }
    void setA(Point p) {
        a.setX(p.getX());
        a.setY(p.getY());
    }
    void setB(Point p) {
        b.setX(p.getX());
        b.setY(p.getY());
    }
};

class SubSquare {
private:
    unsigned long long int xGrid;
    unsigned long long int yGrid;
public:
    SubSquare(unsigned long long int x, unsigned long long int y) {
        xGrid = x;
        yGrid = y;
    }
    int getX() const { return xGrid; }
    int getY() const { return yGrid; }
    void setX(int x) { xGrid = x; }
    void setY(int y) { yGrid = y; }
    bool operator==(const SubSquare& p) const {
        if ((xGrid == p.xGrid) && (yGrid == p.yGrid))
            return true;
        return false;
    }
};
class MyHashFunction {
public:
    size_t operator()(const SubSquare& p) const {
        return (hash<unsigned long long int>()(p.getX())) ^ (hash<unsigned long long int>()(p.getY()));
    }
};

list<Point> list1;
Display arr[length][length] = { };
vector<Point> vector1; // part1/2
vector<Point> vector2; //part3
vector<Point> vector3; //part4
Segment* recFin = new Segment();
Segment* bruteFin = new Segment();
Segment* compFin = new Segment();
Segment* randFin = new Segment();

bool ComparePoints(Point p1, Point p2)
{
    return (p1.getX() < p2.getX());
}

bool compareYPoints(Point p1, Point p2) {
    return (p1.getY() < p2.getY());
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

void readFromFile(string filename, int l) {
    ifstream inData(filename);
    string line;
    while (getline(inData, line))
    {
        int ind = line.find(" ");
        double x = std::stod(line.substr(0, ind));
        double y = std::stod(line.substr(ind + 2, line.size()));
        Point* temp = new Point(x, y);
        if (l == 0)
            list1.push_back(*temp);
        if (l == 1)
            vector1.push_back(*temp);
        if (l == 2)
            vector2.push_back(*temp);
        if (l == 3)
            vector3.push_back(*temp);
    }
    inData.close();
}

Segment* bruteForce() {
    Segment* mins = new Segment(list1.front(), list1.back());
    list<Point>::iterator it;

    for (it = list1.begin(); it != list1.end(); it++) {
        list<Point>::iterator it2 = it;
        for (++it2; it2 != list1.end(); it2++) {
            double temp = distance(it->getX(), it2->getX(), it->getY(), it2->getY());
            if (temp < mins->getDistance()) {
                mins->setA(*it);
                mins->setB(*it2);
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

Segment* calcStrip(vector<Point> v, Segment* d, int l, int r) {
    Segment* ret = new Segment();
    ret->setA(d->getA());
    ret->setB(d->getB());
    for (int i = l; i < r; ++i)
        for (int j = i + 1; j <= r && abs(v[j].getX() - v[i].getX()) < d->getDistance(); ++j)
            if (distance(v[i].getX(), v[j].getX(), v[i].getY(), v[j].getY()) < d->getDistance())
            {
                ret->setA(v[i]);
                ret->setB(v[j]);
            }

    return ret;
}

Segment* calcCompStrip(vector<Point> v, Segment* d, int l, int r) {
    Segment* ret = new Segment();
    ret->setA(d->getA());
    ret->setB(d->getB());

    vector<Point> temp;
    int mid = (l + r) / 2;
    for (int i = l; i < r; i++) {
        if (abs(v[i].getX() - v[mid].getX()) < d->getDistance())
            temp.push_back(v[i]);
    }
    sort(temp.begin(), temp.end(), compareYPoints);

    for (int i = 0; i < (int)temp.size(); ++i) {
        for (int j = (i + 1); (j < (i + 16)) && (j < (int)temp.size()) && (temp[j].getY() - temp[i].getY()) < d->getDistance(); ++j)
            if (distance(temp[i].getX(), temp[j].getX(), temp[i].getY(), temp[j].getY()) < ret->getDistance()) {
                ret->setA(temp[i]);
                ret->setB(temp[j]);
            }
    }
    return ret;
}

Segment* mergeSort(vector<Point> v, int left, int right, int c) {
    if (right - left == 1) {
        Segment* m = new Segment(v[left], v[right]);
        return m;
    }
    else if (right - left == 2) {
        Segment* ret = new Segment(v[left], v[left + 1]);
        Segment* s2 = new Segment(v[left + 1], v[right]);
        Segment* s3 = new Segment(v[left], v[right]);
        if (ret->getDistance() > s2->getDistance()) {
            ret->setA(v[left + 1]);
            ret->setB(v[right]);
        }
        if (ret->getDistance() > s3->getDistance()) {
            ret->setA(v[right]);
            ret->setB(v[left]);
        }
        return ret;
    }
    else {
        int mid = left + (right - left) / 2;
        Segment* leftD = mergeSort(v, left, mid, c);
        Segment* rightD = mergeSort(v, mid + 1, right, c);
        Segment* d = new Segment();
        if (leftD->getDistance() >= rightD->getDistance()) {
            d->setA(rightD->getA());
            d->setB(rightD->getB());
        }
        else {
            d->setA(leftD->getA());
            d->setB(leftD->getB());
        }
        Segment* ret1 = new Segment();
        if (c == 0)
            ret1 = calcStrip(v, d, left, right);
        if (c == 1)
            ret1 = calcCompStrip(v, d, left, right);
        return ret1;
    }
}

void knuth() {
    for (int i = 0; i < (int)vector3.size(); i++) {
        int n = (int)((rand() % (vector3.size() - i)) + i);
        if (n < (int)vector3.size())
            swap(vector3[i], vector3[n]);
    }
}

Segment* randomize() {

    double delta = distance(vector3[0].getX(), vector3[1].getX(), vector3[0].getY(), vector3[1].getY());

    unordered_map<SubSquare, Point, MyHashFunction> umap;
    SubSquare k1 =  SubSquare(vector3[0].getX() / (delta / 2), vector3[0].getY() / (delta / 2));
    SubSquare k2 =  SubSquare(vector3[1].getX() / (delta / 2), vector3[1].getY() / (delta / 2));
    umap.insert({ k1, vector3[0] });
    umap.insert({ k2, vector3[1] });

    Segment* ret = new Segment();
    ret->setA(vector3[0]);
    ret->setB(vector3[1]);
    for (int i = 2; i < (int)vector3.size(); i++) {
        SubSquare squareST =  SubSquare(vector3[i].getX() / (delta / 2), vector3[i].getY() / (delta / 2));
        bool flag = false;
        for (int a = squareST.getX() - 2; a <= squareST.getX() + 2; a++) {
            for (int j = squareST.getY() - 2;  j<= squareST.getY() + 2; j++) {
                SubSquare tempSq =  SubSquare(a, j);
                if (umap.count(tempSq)) {
                    if(!((umap[squareST].getX() == umap[tempSq].getX()) && (umap[squareST].getY() == umap[tempSq].getY()))) {
                    double temp = distance(umap[tempSq].getX(), umap[squareST].getX(), umap[tempSq].getY(), umap[squareST].getY());
                    if (temp <= delta) {
                        delta = temp;
                        ret->setA(umap[squareST]);
                        ret->setB(umap[tempSq]);
                        flag = true;
                    }
                    }
                }
            }
        }
        if (flag == true) {
            umap.clear();
            for (int k = 0; k < i; k++) {
                SubSquare squareST2 =  SubSquare(vector3[k].getX() / (delta / 2), vector3[k].getY() / (delta / 2));
                umap.insert({ squareST2, vector3[k] });
            }
        }
        else
            umap.insert({ squareST, vector3[i] });
    }
        return ret;
    }


void part0() {
    string x;
    cout << "Do you want to generate points? (yes or no)";
    cin >> x;
    srand(time(NULL));
    if (x == "yes") {
        double y;
        cout << "How many points would you like to generate?";
        cin >> y;
        ofstream points;
        points.open("points.txt");
        for (int i = 0; i < y; i++) {
            Point* temp = new Point((rand() / (double)RAND_MAX), (rand() / (double)RAND_MAX));
            points << setprecision(23) << (temp->getX()) << "  " << (temp->getY()) << endl;
        }
        points.close();
    }
}

int part1() {
    readFromFile("points.txt", 0);
    fillWithBlank();
    auto start = high_resolution_clock::now();
    Segment* smallestDists = new Segment();
    smallestDists = bruteForce();
    auto stop = high_resolution_clock::now();
    list<Point>::iterator it;
    for (it = list1.begin(); it != list1.end(); ++it) {
        drawCircle(false, 3, length * it->getX(), length * it->getY());
        drawCircle(false, 2, length * it->getX(), length * it->getY());
    }
    drawCircle(true, 3, length * smallestDists->getA().getX(), length * smallestDists->getA().getY());
    drawCircle(true, 3, length * (smallestDists)->getB().getX(), length * (smallestDists)->getB().getY());
    drawCircle(true, 2, length * smallestDists->getA().getX(), length * smallestDists->getA().getY());
    drawCircle(true, 2, length * (smallestDists)->getB().getX(), length * (smallestDists)->getB().getY());

    auto duration = duration_cast<microseconds>(stop - start);
    writeFile();
    bruteFin->setA(smallestDists->getA());
    bruteFin->setB(smallestDists->getB());
    return duration.count();
}

int part2() {
    readFromFile("points.txt", 1);

    auto begin = high_resolution_clock::now();
    sort(vector1.begin(), vector1.end(), ComparePoints);

    Segment* dist = mergeSort(vector1, 0, vector1.size() - 1, 0);

    auto end = high_resolution_clock::now();
    auto ti = duration_cast<microseconds>(end - begin);
    recFin->setA(dist->getA());
    recFin->setB(dist->getB());
    return ti.count();
}

int part3() {
    readFromFile("points.txt", 2);
    auto a = high_resolution_clock::now();
    sort(vector2.begin(), vector2.end(), ComparePoints);
    Segment* dist = mergeSort(vector2, 0, vector2.size() - 1, 1);
    auto b = high_resolution_clock::now();
    auto ab = duration_cast<microseconds>(b - a);
    compFin->setA(dist->getA());
    compFin->setB(dist->getB());
    return ab.count();
}

int part4() {
    readFromFile("points.txt", 3);
    auto s = high_resolution_clock::now();
    knuth();
    Segment* dist = randomize();
    auto e = high_resolution_clock::now();
    auto se = duration_cast<microseconds>(e - s);
    randFin->setA(dist->getA());
    randFin->setB(dist->getB());
    return se.count();
}


int main()
{
    part0();
    //  int bruteTime = part1();
    //int recTime = part2();
    int compTime = part3();
    int randTime = part4();
    /* cout << setprecision(23) << "Coordinate 1 (brute force): " << "(" << bruteFin->getA().getX() << ", " <<
         bruteFin->getA().getY() << ") Coordinate 2 (brute force): (" << bruteFin->getB().getX() << ", " << bruteFin->getB().getY() << ")" << endl;
     cout << setprecision(23) << "Distance (brute force): " << bruteFin->getDistance() << endl;
     cout << "Time taken for brute force " << bruteTime << endl;*/
    /*cout << setprecision(23) << "Coordinate 1 (recursive): " << "(" << recFin->getA().getX() << ", " <<
        recFin->getA().getY() << ") Coordinate 2 (recursive): (" << recFin->getB().getX() << ", " << recFin->getB().getY() << ")" << endl;
    cout << setprecision(23) << "Distance (recursive): " << recFin->getDistance() << endl;
    cout << "Time taken for recursive " << recTime << endl;*/
    cout << setprecision(23) << "Coordinate 1 (Complete recursive): " << "(" << compFin->getA().getX() << ", " <<
        compFin->getA().getY() << ") Coordinate 2 (Complete recursive): (" << compFin->getB().getX() << ", " << compFin->getB().getY() << ")" << endl;
    cout << setprecision(23) << "Distance (complete recursive): " << compFin->getDistance() << endl;
    cout << "Time taken for Complete recursive " << compTime << endl;
    cout << setprecision(23) << "Coordinate 1 (Randomize): " << "(" << randFin->getA().getX() << ", " <<
        randFin->getA().getY() << ") Coordinate 2 (Randomize): (" << randFin->getB().getX() << ", " << randFin->getB().getY() << ")" << endl;
    cout << setprecision(23) << "Distance (Randomize): " << randFin->getDistance() << endl;
    cout << "Time taken for Randomize " << randTime << endl;
    ofstream results;
    results.open("results.txt");
    /*  results << setprecision(23) << "Coordinate 1 (brute force): " << "(" << bruteFin->getA().getX() << ", " <<
          bruteFin->getA().getY() << ") Coordinate 2 (brute force): (" << bruteFin->getB().getX() << ", " << bruteFin->getB().getY() << ")" << endl;
      results << setprecision(23) << "Distance (brute force): " << bruteFin->getDistance() << endl;
      results << "Time taken for brute force " << bruteTime << endl;*/
      /* results << setprecision(23) << "Coordinate 1 (recursive): " << "(" << recFin->getA().getX() << ", " <<
           recFin->getA().getY() << ") Coordinate 2 (recursive): (" << recFin->getB().getX() << ", " << recFin->getB().getY() << ")" << endl;
       results << setprecision(23) << "Distance (recursive): " << recFin->getDistance() << endl;
       results << "Time taken for recursive " << recTime << endl; */
    results << setprecision(23) << "Coordinate 1 (Complete recursive): " << "(" << compFin->getA().getX() << ", " <<
        compFin->getA().getY() << ") Coordinate 2 (Complete recursive): (" << compFin->getB().getX() << ", " << compFin->getB().getY() << ")" << endl;
    results << setprecision(23) << "Distance (complete recursive): " << compFin->getDistance() << endl;
    results << "Time taken for Complete recursive " << compTime << endl;
    results << setprecision(23) << "Coordinate 1 (Randomize): " << "(" << randFin->getA().getX() << ", " <<
        randFin->getA().getY() << ") Coordinate 2 (Randomize): (" << randFin->getB().getX() << ", " << randFin->getB().getY() << ")" << endl;
    results << setprecision(23) << "Distance (Randomize): " << randFin->getDistance() << endl;
    results << "Time taken for Randomize " << randTime << endl;
    results.close();
}
