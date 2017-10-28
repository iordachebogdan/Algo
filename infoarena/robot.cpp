#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <cmath>
#include <iomanip>
#include <queue>
 
using namespace std;
 
ifstream fin("robot.in");
ofstream fout("robot.out");
 
const int inf = 1000000000;
const double inff = 100000000000000000LL;
const double eps = 1e-7;
 
struct Point {
 
    int x, y;
    Point() {}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
 
    void move(int moveX, int moveY) {
        x += moveX;
        y += moveY;
    }
 
};
 
struct Comp {
    bool operator()(const Point &a, const Point &b){
        return (a.x == b.x ? a.y > b.y : a.x > b.x);
    }
};
 
vector< vector<Point> > polygons;
vector<Point> robot, points;
Point o;
 
vector< pair<int, double> > *graph;
 
inline int det(const Point &a, const Point &b, const Point &c) {
 
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
 
}
 
inline int sign(int x) {
 
    if (x < 0)
        return -1;
    if (x == 0)
        return 0;
    return 1;
 
}
 
inline double dist(const Point &a, const Point &b) {
 
    int d = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    return sqrt(d * 1.0);
 
}
 
Point p0;
inline bool comp(const Point &a, const Point &b) {
 
    int d = det(p0, a, b);
    return (!d ? dist(p0, a) > dist(p0, b) : d > 0);
 
}
 
inline bool comp2(const Point &a, const Point &b) {
 
    return dist(p0, a) < dist(p0, b);
 
}
 
Point temp[100005]; int nr = 0;
vector<Point> convexHull(vector<Point> polygon) {
 
    int pos = 0;
    for (unsigned int i = 1; i < polygon.size(); ++i) {
        if ((polygon[pos].y > polygon[i].y) || (polygon[pos].y == polygon[i].y && polygon[pos].x > polygon[i].x))
            pos = i;
    }
 
    swap(polygon[0], polygon[pos]);
    p0 = polygon[0];
 
    nr = 0;
    for (int i = 1; i < (int)polygon.size(); ++i)
        temp[++nr] = polygon[i];
    sort(temp + 1, temp + nr + 1, comp);
 
    for (int i = 1; i < (int)polygon.size(); ++i)
        polygon[i] = temp[i];
 
    nr = 2;
    temp[1] = (polygon[0]), temp[2] = (polygon[1]);
    for (int i = 2; i < (int)polygon.size() && det(polygon[0], polygon[1], polygon[i]) == 0; ++i)
        temp[++nr] = (polygon[i]);
 
    sort(temp + 1, temp + nr + 1, comp2);
 
    for (unsigned int i = 0; i < nr; ++i)
        polygon[i] = temp[i + 1];
 
    vector<Point> hull;
    hull.push_back(polygon[0]), hull.push_back(polygon[1]);
 
    for (int i = 2; i < (int)polygon.size(); ++i) {
 
        while (hull.size() >= 2 && det(hull[hull.size() - 2], hull[hull.size() - 1], polygon[i]) < 0)
            hull.pop_back();
        hull.push_back(polygon[i]);
 
    }
 
    return hull;
 
}
 
void transformPolygon(vector<Point> &polygon) {
 
    vector<Point> newPolygon;
    set<Point, Comp> mySet;
    for (unsigned int i = 0; i < polygon.size(); ++i) {
 
        for (unsigned int j = 0; j < robot.size(); ++j) {
 
            Point temp = o;
            temp.move(polygon[i].x - robot[j].x, polygon[i].y - robot[j].y);
            if (mySet.find(temp) == mySet.end())
                mySet.insert(temp);
 
        }
 
    }
 
    for (set<Point>::iterator it = mySet.begin(); it != mySet.end(); ++it)
        newPolygon.push_back(*it);
 
    polygon = convexHull(newPolygon);
 
}
 
inline double modul(double x) {
    return (x > 0 ? x : -x);
}
 
inline bool between(double x, double a, double b) {
 
    if (x > a + eps && x < b - eps)
        return true;
    return false;
 
}
 
pair<double, double> intersectLines(Point a, Point b, Point c, Point d) {
 
    int a1 = b.y - a.y;
    int b1 = a.x - b.x;
    int c1 = (a.x * (a.y - b.y) + a.y * (b.x - a.x));
 
    int a2 = d.y - c.y;
    int b2 = c.x - d.x;
    int c2 = (c.x * (c.y - d.y) + c.y * (d.x - c.x));
 
    return make_pair(((double)1.0*c2*b1 - (double)1.0*c1*b2) / ((double)1.0*a1*b2 - (double)1.0*a2*b1), ((double)1.0*c2*a1 - (double)1.0*c1*a2) / ((double)1.0*b1*a2 - (double)1.0*b2*a1));
 
}
 
bool intersectSegPolyg(Point p1, Point p2, vector<Point> polygon) {
 
    int negative = 0, positive = 0;
 
    for (int i = 0; i < (int)polygon.size(); ++i) {
 
        int d = det(p1, p2, polygon[i]);
 
        if (d < 0)
            negative++;
        else if (d > 0)
            positive++;
 
    }
 
    if (!negative || !positive)
        return false;
 
    pair<double, double> pp1(inff, inff), pp2(inff, inff);
    polygon.push_back(polygon[0]);
    for (int i = 0; i < (int)polygon.size() - 1; ++i) {
 
        int d1 = det(p1, p2, polygon[i]);
        int d2 = det(p1, p2, polygon[i + 1]);
 
        if (sign(d1) * sign(d2) == -1) {
            if (pp1.first == inff)
                pp1 = intersectLines(p1, p2, polygon[i], polygon[i + 1]);
            else
                pp2 = intersectLines(p1, p2, polygon[i], polygon[i + 1]);
        }
        else if (sign(d1) * sign(d2) == 0) {
            pair<double, double> aux;
            if (d1 == 0)
                aux = make_pair(polygon[i].x, polygon[i].y);
            else
                aux = make_pair(polygon[i + 1].x, polygon[i + 1].y);
 
            if (modul(aux.first - pp1.first) < eps && modul(aux.second - pp1.second) < eps)
                continue;
            else if (pp1.first == inff)
                pp1 = aux;
            else
                pp2 = aux;
        }
 
        if (pp2.first != inff)
            break;
 
    }
 
    if (p1.x == p2.x) {
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        swap(pp1.first, pp1.second);
        swap(pp2.first, pp2.second);
    }
 
    if (p1.x > p2.x)
        swap(p1, p2);
    if (pp1.first > pp2.first)
        swap(pp1, pp2);
 
    if (between(p1.x * 1.0, pp1.first, pp2.first))
        return true;
    if (between(p2.x * 1.0, pp1.first, pp2.first))
        return true;
    if (between(pp1.first, p1.x * 1.0, p2.x * 1.0))
        return true;
    if (between(pp2.first, p1.x * 1.0, p2.x * 1.0))
        return true;
 
    if (modul(p1.x - pp1.first) < eps && modul(p2.x - pp2.first) < eps)
        return true;
 
    return false;
 
}
 
vector<double> dp;
vector<bool> inQue;
void bellmanFord(void) {
 
    dp.resize(points.size() + 1, inff);
    inQue.resize(points.size() + 1, false);
 
    dp[0] = 0;
 
    queue<int> que;
    que.push(0);
 
    while (!que.empty()) {
 
        int node = que.front();
        inQue[node] = false;
 
        for (vector< pair<int, double> >::iterator edge = graph[node].begin(); edge != graph[node].end(); ++edge) {
 
            if (dp[edge->first] <= dp[node] + edge->second)
                continue;
 
            dp[edge->first] = dp[node] + edge->second;
 
            if (!inQue[edge->first]) {
                inQue[edge->first] = true;
                que.push(edge->first);
            }
 
        }
 
        que.pop();
 
    }
 
}
 
int main() {
 
    int n;
    fin >> n;
 
    polygons.push_back(vector<Point>(n, Point()));
    o = Point(inf, inf);
    for (int i = 0; i < n; ++i) {
 
        fin >> polygons[0][i].x >> polygons[0][i].y;
 
        o.x = min(o.x, polygons[0][i].x);
        o.y = min(o.y, polygons[0][i].y);
 
    }
 
    robot = polygons[0];
 
    int m;
    fin >> m;
 
    for (int i = 1; i <= m; ++i) {
 
        int len;
        fin >> len;
 
        polygons.push_back(vector<Point>(len, Point()));
        for (int j = 0; j < len; ++j)
            fin >> polygons[i][j].x >> polygons[i][j].y;
 
        transformPolygon(polygons[i]);
 
    }
 
    points.push_back(o);
    points.push_back(Point());
    fin >> points.back().x >> points.back().y;
 
    for (int i = 1; i <= m; ++i) {
 
        for (unsigned int j = 0; j < polygons[i].size(); ++j)
            points.push_back(polygons[i][j]);
 
    }
 
    graph = new vector< pair<int, double> >[points.size() + 1];
    for (unsigned int i = 0; i < points.size(); ++i) {
 
        for (unsigned int j = i + 1; j < points.size(); ++j) {
 
            bool ok = false;
            for (int k = 1; k <= m; ++k) {
                if (intersectSegPolyg(points[i], points[j], polygons[k])){
                    ok = true;
                    break;
                }
            }
 
            if (ok)
                continue;
 
            graph[i].push_back(make_pair(j, dist(points[i], points[j])));
            graph[j].push_back(make_pair(i, dist(points[i], points[j])));
 
        }
 
    }
 
    bellmanFord();
 
    if (dp[1] == inff)
        fout << -1;
    else
        fout << setprecision(4) << fixed << dp[1];
 
 
    return 0;
 
}
 