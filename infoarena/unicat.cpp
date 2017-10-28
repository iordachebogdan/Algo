/*
    Palindromic Tree Implementation - Bogdan Iordache 2015
*/
#include <fstream>
#include <algorithm>
#include <cstring>
 
#define DIM 500005
#define infile "unicat.in"
#define outfile "unicat.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int mask, ans;
 
char a[DIM];
 
struct pal {
 
    int len, mask;
 
    pal *suffLink, *next[26];
 
    pal() {
 
        len = mask = 0;
 
        suffLink = NULL;
 
        for (int i = 0; i < 26; ++i)
            next[i] = NULL;
 
    }
 
} *root1, *root2, *suff;
 
void initTree(void){
 
    root1 = new pal;
 
    root1->len = -1;
    root1->suffLink = root1;
 
    root2 = new pal;
 
    root2->len = 0;
    root2->suffLink = root1;
 
    suff = root2;
 
}
 
void addLetter(int pos) {
 
    int letter = a[pos] - 'a';
 
    pal *crtSuff = suff;
 
    while (true) {
 
        int crtLen = crtSuff->len;
 
        if (pos - crtLen - 1 > 0 && a[pos - crtLen - 1] == a[pos])
            break;
 
        crtSuff = crtSuff->suffLink;
 
    }
 
    if (crtSuff->next[letter] != NULL) {
 
        //The palindrome already exists in the tree.
 
        suff = crtSuff->next[letter];
 
        suff->mask |= mask;
 
        return;
 
    }
 
    //Else we have to create the node.
 
    crtSuff->next[letter] = new pal;
 
    pal *node = crtSuff->next[letter];
 
    suff = node;
 
    node->mask |= mask;
 
    node->len = crtSuff->len + 2;
 
    if (node->len == 1) {
 
        node->suffLink = root2;
 
        return;
 
    }
 
 
    crtSuff = crtSuff->suffLink;
 
    while (true) {
 
        int crtLen = crtSuff->len;
 
        if (pos - crtLen - 1 > 0 && a[pos - crtLen - 1] == a[pos])
            break;
 
        crtSuff = crtSuff->suffLink;
 
    }
 
    node->suffLink = crtSuff->next[letter];
 
}
 
void DFS(pal *node) {
 
    if (node->mask == 3 && node != root1 && node != root2)
        ++ans;
 
    node->mask = -1;
 
    for (int i = 0; i < 26; ++i) {
 
        if (node->next[i] == NULL)
            continue;
 
        if (node->next[i]->mask == -1)
            continue;
 
        DFS(node->next[i]);
 
    }
 
}
 
int main() {
 
    fin >> a + 1;
 
    int n = strlen(a + 1);
 
    mask = 1;
 
    initTree();
 
    for (int i = 1; i <= n; ++i) {
 
        addLetter(i);
 
    }
 
    fin >> a + 1;
 
    n = strlen(a + 1);
 
    mask = 2;
 
    suff = root2;
 
    for (int i = 1; i <= n; ++i) {
 
        addLetter(i);
 
    }
 
    DFS(root1);
 
    DFS(root2);
 
    fout << ans;
 
    return 0;
}
 