#include <fstream>
#include <algorithm>
 
#define DIM 100
#define LEN 10000
#define infile "resturi.in"
#define outfile "resturi.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int p[DIM], r[DIM];
 
int res[DIM][LEN], crtProd[DIM], temp[DIM];
 
void atrib(int A[], int B[]) {
    A[0] = B[0];
    for (int i = 1; i <= A[0]; ++i)
        A[i] = B[i];
}
 
void multiply(int A[], int x) {
    int t = 0;
    for (int i = 1; i <= A[0]; ++i) {
        A[i] = A[i] * x + t;
        t = A[i] / 10;
        A[i] %= 10;
    }
    while (t) {
        A[++A[0]] = t % 10;
        t /= 10;
    }
}
 
void add(int A[], int B[]) {
    int t = 0;
    if (B[0]>A[0]) {
        for (int i = A[0] + 1; i <= B[0]; ++i)
            A[i]=0;
        A[0] = B[0];
    }
    else
        for (int i = B[0] + 1; i <= A[0]; ++i)
            B[i]=0;
    for (int i = 1; i <= A[0]; i++) {
        A[i] += B[i] + t;
        t = A[i] / 10;
        A[i] %= 10;
    }
  if (t)
    A[++A[0]] = t;
}
 
int modulo(int A[], int x) {
 
    int ret = 0;
 
    for (int i = A[0]; i; --i) {
 
        ret = (ret*10 + A[i]) % x;
 
    }
 
    return ret;
 
}
 
int main() {
 
    int testCount;
 
    fin >> testCount;
 
    while (testCount--) {
 
        int n;
 
        fin >> n;
 
        for (int i = 1; i <= n; ++i)
            fin >> p[i] >> r[i];
 
        res[1][0] = 0;
 
        while(r[1]) {
 
            res[1][++res[1][0]] = r[1] % 10;
 
            r[1] /= 10;
 
        }
 
        crtProd[0] = 0;
 
        while(p[1]) {
 
            crtProd[++crtProd[0]] = p[1] % 10;
 
            p[1] /= 10;
 
        }
 
        for (int i = 2; i <= n; ++i) {
 
            temp[0] = 0;
 
            atrib(res[i], res[i - 1]);
 
            for (int x = 0; x < p[i]; ++x) {
 
                add(res[i], temp);
 
                if (modulo(res[i], p[i]) == r[i])
                    break;
 
                atrib(temp, crtProd);
 
            }
 
            multiply(crtProd, p[i]);
 
        }
 
        if (res[n][0] == 0)
            fout << "0";
 
        for(int i = res[n][0]; i; --i)
            fout << res[n][i];
 
        fout << '\n';
 
    }
 
    return 0;
}