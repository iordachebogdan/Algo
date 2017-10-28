#include <fstream>
#include <cstring>
 
#define infile "cabine.in"
#define outfile "cabine.out"
 
//keeps data about an array of cabins
class CabinArray {
 
private:
     
    bool *cabins; //saves the status of a cabin
 
    int cabinCount;
 
public:
 
    CabinArray(int cabinCount) {
 
        this->cabinCount = cabinCount;
 
        cabins = new bool[cabinCount + 5];
 
        memset(cabins, false, cabinCount + 1);
 
    }
 
    void occupyCabin(int cabinIndex) {
 
        cabins[cabinIndex] = true;
 
    }
 
    bool cabinStatus(int cabinIndex) {
 
        return cabins[cabinIndex];
 
    }
 
    //returns the index of the n-th occupied cabin
    int nthOccupiedCabin(int n) {
 
        //tries to occupy first cabin
        if (cabins[1] == false) {
 
            occupyCabin(1);
 
            --n;
 
        }
 
        if (n == 0) {
 
            return 1;
 
        }
 
        //tries to occupy last cabin
        if (cabins[cabinCount] == false) {
 
            occupyCabin(cabinCount);
 
            --n;
 
        }
 
        if (n == 0) {
 
            return cabinCount;
 
        }
 
        for (int cabinIndex = cabinCount - 1; cabinIndex; --cabinIndex) {
 
            if (cabins[cabinIndex] == false && cabins[cabinIndex + 1] == false) {
 
                occupyCabin(cabinIndex);
 
                --n;
 
            }
 
            if (n == 0) {
 
                return cabinIndex;
 
            }
 
        }
 
        for (int cabinIndex = 1; cabinIndex <= cabinCount; ++cabinIndex) {
 
            if (cabins[cabinIndex] == false) {
 
                occupyCabin(cabinIndex);
 
                --n;
 
            }
 
            if (n == 0) {
 
                return cabinIndex;
 
            }
 
        }
 
    }
 
};
 
//solves problem "cabine"
class Cabine {
 
private:
 
    CabinArray *cabinArray;
 
    int solution, occupiedCabinCount;
 
public:
 
    void loadCabinArray(std::string path) {
 
        std::ifstream file(path);
 
        int cabinCount;
 
        file >> cabinCount >> occupiedCabinCount;
 
        cabinArray = new CabinArray(cabinCount);
 
        for (int cabinIndex = 1; cabinIndex <= cabinCount; ++cabinIndex) {
 
            int cabinStatus;
 
            file >> cabinStatus;
 
            if (cabinStatus == 1) {
 
                cabinArray->occupyCabin(cabinIndex);
 
            }
 
        }
 
        file.close();
 
    }
 
    void solve() {
 
        solution = cabinArray->nthOccupiedCabin(occupiedCabinCount);
 
    }
 
    void writeSolution(std::string path) {
 
        std::ofstream file(path);
 
        file << solution;
 
        file.close();
 
    }
 
};
 
int main() {
 
    Cabine cabine;
 
    cabine.loadCabinArray(infile);
     
    cabine.solve();
 
    cabine.writeSolution(outfile);
 
    return 0;
 
}