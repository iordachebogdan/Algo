#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cmath>
 
#define infile "rubarba.in"
#define outfile "rubarba.out"
 
const int DIM = 100005;
const long double PI = 3.14159265359;
const long double epsilon = 1e-16;
 
int x[DIM], y[DIM], n;
 
long double minimumEnclosingRectangle(long double rotationAngle) {
 
    long double sinus = std::sin(rotationAngle);
    long double cosinus = std::cos(rotationAngle);
 
    long double maxX = cosinus * x[1] - sinus * y[1];
    long double minX = maxX;
 
    long double maxY = sinus * x[1] + cosinus * y[1];
    long double minY = maxY;
 
    for (int i = 2; i <= n; ++i) {
 
        long double newX = cosinus * x[i] - sinus * y[i];
 
        maxX = std::max(maxX, newX);
        minX = std::min(minX, newX);
 
        long double newY = sinus * x[i] + cosinus * y[i];
 
        maxY = std::max(maxY, newY);
        minY = std::min(minY, newY);
 
    }
 
    return (maxX - minX) * (maxY - minY);
 
}
 
int main() {
     
    std::ifstream fin(infile);
    std::ofstream fout(outfile);
 
    fin >> n;
 
    for (int i = 1; i <= n; ++i) {
 
        fin >> x[i] >> y[i];
 
    }
 
    long double left = 0, right = PI * 0.5;
 
    while (right - left >= epsilon) {
 
        long double smallThird = left + (right - left) / 3.0;
        long double bigThird = right - (right - left) / 3.0;
 
        if (minimumEnclosingRectangle(smallThird) < minimumEnclosingRectangle(bigThird)) {
 
            right = bigThird;
 
        }
        else {
 
            left = smallThird;
 
        }
 
    }
 
    fout << std::setprecision(2) << std::fixed << minimumEnclosingRectangle(left);
 
    return 0;
 
}