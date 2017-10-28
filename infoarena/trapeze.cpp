#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
 
#define DIM 1000
#define MAX 100
#define infile "trapeze.in"
#define outfile "trapeze.out"
 
using namespace std;
 
bool vis[MAX];
 
int primes[DIM];
 
int dp[2][DIM][DIM], temp[DIM], aux[DIM];
 
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
 
void Multiply(int A[], int B[], int C[]) {
    int t = 0;
    C[0] = A[0] + B[0] - 1;
    for (int i = 1; i <= A[0] + B[0]; ++i)
        C[i] = 0;
    for (int i = 1; i <= A[0]; i++)
        for (int j = 1; j <= B[0]; j++)
            C[i + j - 1] += A[i] * B[j];
    for (int i = 1; i <= C[0]; i++)
    {
        t = (C[i] += t) / 10;
        C[i] %= 10;
    }
    if (t)
        C[++C[0]] = t;
}
 
int comp(int A[], int B[]) {
    if (A[0] == -1)
        return 1;
    if (B[0] == -1)
        return -1;
    if (A[0] > B[0])
        return 1;
    if (A[0] < B[0])
        return -1;
    for (int i = A[0]; i; --i){
        if (A[i] < B[i])
            return -1;
        if (A[i] > B[i])
            return 1;
    }
    return 0;
}
 
int main() {
 
    ifstream fin(infile);
    ofstream fout(outfile);
 
    int k;
 
    fin >> k;
 
    int n = 0;
 
    for (int i = 2; i < MAX; ++i) {
 
        if (!vis[i] && i != 2)
            primes[++n] = i;
         
        if (vis[i])
            continue;
 
        for (int j = i + i; j < MAX; j += i)
            vis[j] = true;
 
    }
 
    int OLD = 1, NEW = 0;
 
    dp[OLD][1][0] = dp[OLD][1][1] = 1;
 
    for (int i = 2; i < DIM; ++i)
        dp[OLD][i][0] = -1;
 
    for (int i = 1; i <= n; ++i) {
 
        for (int j = 1; j <= k; ++j)
            atrib(dp[NEW][j], dp[OLD][j]);
 
        temp[0] = temp[1] = 1;
         
        for (int j = 1; j <= k; ++j) {
 
            multiply(temp, primes[i]);
 
            for (int index = 1; index * (j + 1) <= k; ++index) {
             
                if (dp[OLD][index][0] <= 0) {
 
                    continue;
 
                }
 
                Multiply(dp[OLD][index], temp, aux);
 
                if (comp(aux, dp[NEW][index * (j + 1)]) < 0) {
 
                    atrib(dp[NEW][index * (j + 1)], aux);
 
                }
             
            }
 
        }
 
        swap(OLD, NEW);
 
    }
 
    for (int i = dp[OLD][k][0]; i; --i)
        fout << dp[OLD][k][i];
 
    return 0;
 
}