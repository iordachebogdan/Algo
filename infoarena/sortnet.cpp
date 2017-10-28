#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stack>
 
using namespace std;
 
FILE* fin = fopen("sortnet.in", "r");
FILE* fout = fopen("sortnet.out", "w");
 
const int dim = 35;
 
int n, m;
 
struct Comparator {
    int other, value;
} a[dim][dim];
 
inline int LSB(int x) {
    return (x&(-x));
}
 
inline bool IsSorted(int x) {
    return !((x + LSB(x)) & ((1 << n) - 1));
}
 
int trace(int pos, int val) {
 
    for (int i = 1; i <= m; ++i) {
 
        a[i][pos].value = val;
        int other = a[i][pos].other;
 
        if (a[i][other].value == 0 && pos < other)
            pos = other;
        else if (a[i][other].value == 1 && pos > other)
            pos = other;
 
    }
 
    return pos;
 
}
 
int main() {
 
    fscanf(fin, "%d%d", &n, &m);
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= (n >> 1); ++j) {
 
            int x, y;
            fscanf(fin, " <%d,%d>", &x, &y);
 
            --x, --y;
 
            a[i][x].other = y;
            a[i][y].other = x;
 
        }
    }
 
    stack<int> st;
    for (int i = n - 1; i >= 0; --i) st.push(i);
 
    int sol = 1, output = 0;
    while (!st.empty()) {
 
        int pos = st.top();
        st.pop();
 
        output ^= (1 << trace(pos, a[1][pos].value ^ 1));
 
        if (IsSorted(output)) sol++;
 
        for (int i = pos - 1; i >= 0; --i)
            st.push(i);
 
    }
 
    fprintf(fout, "%d\n", sol);
 
    return 0;
 
}