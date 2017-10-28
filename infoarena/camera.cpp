#include <fstream>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <vector>
 
using namespace std;
 
ifstream fin("camera.in");
ofstream fout("camera.out");
 
const double eps = 1e-5;
 
struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }
};
 
int n;
 
vector< pair<Point, Point> > edges;
vector<Point> polygon;
 
inline double det(const Point &a, const Point &b, const Point &c) {
 
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
 
}
 
inline int sign(double x) {
 
    if (x > -eps && x < eps)
        return 0;
    if (x > 0)
        return 1;
    return -1;
 
}
 
Point intersect(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
 
    double a1 = p2.y - p1.y;
    double b1 = p1.x - p2.x;
    double c1 = p2.x*p1.y - p1.x*p2.y;
     
    double a2 = p4.y - p3.y;
    double b2 = p3.x - p4.x;
    double c2 = p4.x*p3.y - p3.x*p4.y;
 
    double div = a1*b2 - a2*b1;
    double x = (b1*c2 - b2*c1) / div;
    double y = (a2*c1 - a1*c2) / div;
 
    return Point(x, y);
 
}
 
vector<Point> aux;
inline void cutPolygon(const Point &a, const Point &b) {
 
    aux.clear();
 
    for (unsigned int i = 0; i + 1 < polygon.size(); ++i) {
 
        if (sign(det(polygon[i], a, b)) * sign(det(polygon[i + 1], a, b)) < 0) {
 
            Point temp = intersect(polygon[i], polygon[i + 1], a, b);
 
            if (sign(det(polygon[i], a, b)) > 0) {
                aux.push_back(polygon[i]);
                aux.push_back(temp);
            }
            else {
                aux.push_back(temp);
                aux.push_back(polygon[i + 1]);
            }
 
            continue;
 
        }
 
        if (sign(det(polygon[i], a, b)) >= 0)
            aux.push_back(polygon[i]);
 
    }
 
    aux.push_back(aux[0]);
 
    polygon = aux;
 
}
 
int main() {
 
    fin >> n;
 
    Point p0, prev;
    fin >> p0.x >> p0.y;
    prev = p0;
 
    double maxX = p0.x, minX = p0.x;
    double maxY = p0.y, minY = p0.y;
 
    for (int i = 2; i <= n; ++i) {
 
        Point curr;
        fin >> curr.x >> curr.y;
 
        maxX = max(maxX, curr.x);
        minX = min(minX, curr.x);
        maxY = max(maxY, curr.y);
        minY = min(minY, curr.y);
 
        edges.push_back(make_pair(prev, curr));
 
        prev = curr;
 
    }
 
    edges.push_back(make_pair(prev, p0));
 
    double aria = 0;
    for (unsigned int i = 0; i < edges.size(); ++i)
        aria += det(Point(0, 0), edges[i].first, edges[i].second);
 
    if (aria < 0) {
 
        for (unsigned int i = 0; i < edges.size(); ++i)
            swap(edges[i].first, edges[i].second);
        for (int i = 0, j = edges.size() - 1; i < j; ++i, --j)
            swap(edges[i], edges[j]);
 
    }
 
    polygon.push_back(Point(minX, minY));
    polygon.push_back(Point(maxX, minY));
    polygon.push_back(Point(maxX, maxY));
    polygon.push_back(Point(minX, maxY));
    polygon.push_back(Point(minX, minY));
 
    for (unsigned int i = 0; i < edges.size(); ++i) {
 
        cutPolygon(edges[i].first, edges[i].second);
 
    }
 
    aria = 0;
    for (unsigned int i = 0; i + 1 < polygon.size(); ++i)
        aria += det(Point(0, 0), polygon[i], polygon[i + 1]);
    aria /= 2.0;
 
    if (aria < 0)
        aria = -aria;
 
    fout << setprecision(2) << fixed << aria << '\n';
 
    return 0;
 
}