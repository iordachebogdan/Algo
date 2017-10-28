#include <fstream>
#include <algorithm>
 
#define DIM 500005
#define numerator first
#define denominator second
#define infile "farey.in"
#define outfile "farey.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int n, k;
 
int c[DIM];
 
pair <int, int> fractions[DIM];
 
//Determinam cate fractii sunt mai mici decat (val / n)
int getFractionCount(int val) {
 
    // c[i] = cate fractii cu numitorul i sunt mai mici decat (val / n)
 
    // numaratorul maxim al unei fractii cu numitorul i este tot i => sunt i * val / n fractii(nu neaparat ireductibile)
    //cu numitorul i mai mici decat (val / n);
    for (int i = 2; i <= n; ++i)
        c[i] = i * val / n;
 
    //Le eliminam pe cele reductibile cu ajutorul unui algoritm tip ciur.
    for (int i = 2; i <= n; ++i) {
 
        //scadem c[i] din toti c[j] cu j % i == 0
        for (int j = i + i; j <= n; j += i)
            c[j] -= c[i];
 
    }
 
    int sol = 0;
 
    for (int i = 2; i <= n; ++i)
        sol += c[i];
 
    return sol;
 
}
 
bool cmp(pair<int, int> a, pair<int, int> b) {
 
    //le sortam dupa valoarea fractiei si in caz de egalitate dupa numarator
    if (a.numerator * b.denominator == a.denominator * b.numerator)
        return (a.numerator < b.numerator);
 
    return (a.numerator * b.denominator < a.denominator * b.numerator);
 
}
 
// Determinam a K fractie dintr-o secventa Farey de ordin N.
int main() {
 
    fin >> n >> k;
 
    int left = 1, right = n - 1;
 
    //Cautam binar intre 1 -> (n - 1) numarul x pentru care ( (x / n) < (a Ka fractie) < (x + 1 / n) )
    while (left <= right) {
 
        int middle = (left + right) / 2;
 
        if (getFractionCount(middle) < k)
            left = middle + 1;
        else
            right = middle - 1;
 
    }
 
    k -= getFractionCount(right);
 
    //Problema se reduce la a determina a Ka fractie ireductibila din intervalul (right / n, (right + 1) / n)
 
    int fractionCount = 0;
 
    //In intervalul fixat de noi poate exista maxim o fractie cu numitorul i in intervalul [2, n] si ii determinam numitorul
    for (int i = 2; i <= n; ++i){
 
        int numerator = i * (right + 1) / n;
 
        //verificam ca fractia sa fie numerator / i sa fie mai mare decat (right / n)
        if (numerator * n > right * i)
            // If so, il adaugam intr-un vector
            fractions[++fractionCount] = make_pair(numerator, i);
 
    }
 
    //Sortam fractiile
    sort(fractions + 1, fractions + fractionCount + 1, cmp);
 
    fractions[0] = make_pair(1, 1);
 
    for (int i = 1; i <= fractionCount; ++i) {
 
        //daca fractia curenta difera de cea precedenta inseamna ca este ireductibila si o numaram
        if (fractions[i - 1].numerator * fractions[i].denominator != fractions[i - 1].denominator * fractions[i].numerator)
            k--;
 
        //daca am gasit k fractii ireductibile diferite afisam fractia
        if (k == 0) {
 
            fout << fractions[i].numerator << " " << fractions[i].denominator << "\n";
 
            break;
 
        }
 
    }
 
    return 0;
}
 