#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
 
using namespace std;
 
int freq[32005], v[32005], bin[32005];
 
ofstream fout("invsort.out");
 
void reverseBoth(int left, int right) {
 
    fout << left << ' ' << right << '\n';
 
    for (int i = left, j = right; i < j; ++i, --j) {
 
        swap(bin[i], bin[j]);
        swap(v[i], v[j]);
 
    }
 
}
 
void sortBin(int left, int right) {
 
    if (left >= right)
        return;
 
    if (right - left == 1) {
 
        if (bin[left] > bin[right])
            reverseBoth(left, right);
 
        return;
 
    }
 
    int mid = (left + right) / 2;
 
    sortBin(left, mid - 1);
    sortBin(mid, right);
 
    int revLeft = mid - 1;
    while (revLeft >= left && bin[revLeft] == 1) revLeft--;
    revLeft++;
 
    int revRight = mid;
    while (revRight <= right && bin[revRight] == 0) revRight++;
    revRight--;
 
    if (revLeft == mid || revRight == mid - 1)
        return;
 
    reverseBoth(revLeft, revRight);
 
}
 
void sortV(int left, int right) {
 
    if (left >= right)
        return;
 
    if (right - left == 1) {
 
        if (v[left] > v[right])
            reverseBoth(left, right);
 
        return;
 
    }
 
    int mid = (left + right) / 2;
 
    for (int i = left; i <= right; ++i)
        bin[i] = (v[i] < mid ? 0 : 1);
 
    sortBin(left, right);
 
    sortV(left, mid - 1);
    sortV(mid, right);
 
}
 
int main() {
 
    ifstream fin("invsort.in");
 
    int n;
    fin >> n;
 
    freq[0] = 1;
    for (int i = 1; i <= n; ++i) {
 
        fin >> v[i];
        freq[v[i] + 1]++;
 
    }
 
    for (int i = 1; i <= 32000; ++i)
        freq[i] += freq[i - 1];
 
    for (int i = 1; i <= n; ++i)
        v[i] = (++freq[v[i]]) - 1;
 
    sortV(1, n);
 
    return 0;
 
}