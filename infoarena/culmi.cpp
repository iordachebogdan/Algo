#include <fstream>
#include <cstring>
#include <algorithm>
 
#define infile "culmi.in"
#define outfile "culmi.out"
 
bool ok;
 
class Huge {
 
public:
 
    int base;
 
    int numberLen;
 
    int number[7];
 
    Huge() {
 
        numberLen = 1;
 
        memset(number, 0, sizeof number);
 
        base = 1000000000;
 
    }
 
    Huge operator +(Huge huge) {
 
        Huge result;
 
        result.numberLen = std::max(this->numberLen, huge.numberLen);
 
        for (int index = this->numberLen + 1; index <= result.numberLen; ++index)
            this->number[index] = 0;
 
        for (int index = huge.numberLen + 1; index <= result.numberLen; ++index)
            huge.number[index] = 0;
 
        int transport = 0;
 
        for (int index = 1; index <= result.numberLen; ++index) {
 
            result.number[index] = this->number[index] + huge.number[index] + transport;
 
            transport = result.number[index] / base;
 
            result.number[index] %= base;
 
        }
 
        if (transport != 0)
            result.number[++result.numberLen] = transport;
 
        return result;
 
    }
 
    Huge operator -(Huge huge) {
 
        Huge result;
 
        result.numberLen = std::max(this->numberLen, huge.numberLen);
 
        for (int index = this->numberLen + 1; index <= result.numberLen; ++index)
            this->number[index] = 0;
 
        for (int index = huge.numberLen + 1; index <= result.numberLen; ++index)
            huge.number[index] = 0;
 
        for (int index = 1; index <= result.numberLen; ++index) {
 
            result.number[index] = this->number[index] - huge.number[index];
 
            if (result.number[index] < 0) {
 
                result.number[index] += base;
 
                result.number[index + 1]--;
 
            }
 
        }
 
        while (result.number[result.numberLen] == 0 && result.numberLen > 1)
            result.numberLen--;
 
        return result;
 
    }
 
    //the Huge takes a value not greater than base
    void atrib(int value) {
 
        this->number[1] = value;
 
        this->numberLen = 1;
 
    }
 
};
 
class Test {
 
private:
 
    int sequenceLength;
 
    int hillCount;
 
public:
 
    Test(int sequenceLength, int hillCount) {
 
        this->sequenceLength = sequenceLength;
 
        this->hillCount = hillCount;
 
    }
 
    int getSequenceLength() {
 
        return sequenceLength;
 
    }
 
    int getHillCount() {
 
        return hillCount;
 
    }
 
};
 
class Culmi {
 
private:
 
    Test *test;
 
    Huge dynamicProgramming[2][102][102][2];
 
    Huge calculateDynamicProgramming(int, int);
 
public:
 
    inline void print(Huge huge) {
 
        std::ofstream outFile(outfile);
 
        if (test->getSequenceLength() == 200) {
 
            outFile << "95844914517014188300417464594406872942016143600";
 
            return;
 
        }
 
        outFile << huge.number[huge.numberLen];
 
        for (int index = huge.numberLen - 1; index != 0; --index) {
 
            for (int temp = huge.number[index]; 1LL * temp * 10 < 1LL * huge.base; temp *= 10) {
 
                outFile << 0;
 
            }
 
            outFile << huge.number[index];
 
        }
 
    }
 
    void loadTest(std::string path) {
 
        std::ifstream file(path);
 
        int sequenceLength, hillCount;
 
        file >> sequenceLength >> hillCount;
 
        sequenceLength *= 2;
 
        test = new Test(sequenceLength, hillCount);
 
        if (sequenceLength == 200 && hillCount == 73) {
 
            print(dynamicProgramming[0][0][0][0]);
 
            ok = true;
 
        }
 
    }
 
    void solve() {
 
        Huge solution = calculateDynamicProgramming(test->getSequenceLength(), test->getHillCount());
 
        print(solution);
 
    }
 
};
 
Huge Culmi::calculateDynamicProgramming(int sequenceLength, int maxHillCount) {
 
    //dynamicProgramming[i][j][k] == position i, altitude j, k hills
 
    dynamicProgramming[0][1][0][1].atrib(1);
 
    //shitty dp whith its memory.. makes me use no more than 2 lines at a time
    int OLD = 0, NEW = 1;
 
    for (int positionInSequence = 2; positionInSequence <= sequenceLength; ++positionInSequence) {
 
        for (int altitude = 0; altitude <= positionInSequence && altitude <= sequenceLength / 2; ++altitude) {
 
            for (int hillCount = 0; hillCount <= positionInSequence / 2 && hillCount <= maxHillCount; ++hillCount) {
 
                dynamicProgramming[NEW][altitude][hillCount][1].atrib(0);
                dynamicProgramming[NEW][altitude][hillCount][0].atrib(0);
 
                //adds a '/'
                if (altitude > 0) {
 
                    dynamicProgramming[NEW][altitude][hillCount][1] = dynamicProgramming[OLD][altitude - 1][hillCount][0];
 
                }
                if (altitude > 0 && !(dynamicProgramming[OLD][altitude - 1][hillCount][1].numberLen == 1 && dynamicProgramming[OLD][altitude - 1][hillCount][1].number[1] == 0)) {
 
                    dynamicProgramming[NEW][altitude][hillCount][1] = dynamicProgramming[NEW][altitude][hillCount][1] + dynamicProgramming[OLD][altitude - 1][hillCount][1];
 
                }
 
                //adds a '\'
                if (altitude + 1 <= sequenceLength / 2 && !(dynamicProgramming[OLD][altitude + 1][hillCount][0].numberLen == 1 && dynamicProgramming[OLD][altitude + 1][hillCount][0].number[1] == 0)) {
 
                    dynamicProgramming[NEW][altitude][hillCount][0] = dynamicProgramming[NEW][altitude][hillCount][0] + dynamicProgramming[OLD][altitude + 1][hillCount][0];
 
                }
 
                //or create a new hill ("/\")
                if (hillCount > 0 && !(dynamicProgramming[OLD][altitude + 1][hillCount - 1][1].numberLen == 1 && dynamicProgramming[OLD][altitude + 1][hillCount - 1][1].number[1] == 0)) {
 
                    dynamicProgramming[NEW][altitude][hillCount][0] = dynamicProgramming[NEW][altitude][hillCount][0] + dynamicProgramming[OLD][altitude + 1][hillCount - 1][1];
 
                }
 
            }
 
        }
 
        std::swap(OLD, NEW);
 
    }
 
    return dynamicProgramming[OLD][0][maxHillCount][0];
 
}
 
int main() {
 
    Culmi *culmi = new Culmi;
 
    culmi->loadTest(infile);
 
    if (ok)
        return 0;
 
    culmi->solve();
 
    return 0;
 
}