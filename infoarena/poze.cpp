#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>
 
#define infile "poze.in"
#define outfile "poze.out"
 
//The Parser class can be used to optimize the reading of data from a specified path (or with little changes from the console)
class Parser {
 
private:
 
    char *buffer; //a string containing a contiguos array of chars from the input file, from this the numerical data would be extracted
    int maxLenBuffer; //the maximum length of the buffer
    int currentBufferIndex; //the current position in the buffer being processed
 
    std::fstream file; //the stream used to accesses the path
 
public:
 
    //no default constructor because the file path and the maxLenBuffer are relevant
    Parser(std::string path, int maxLenBuffer) {
 
        this->maxLenBuffer = maxLenBuffer;
 
        this->file.open(path, std::ios::in);
 
        buffer = new char[maxLenBuffer];
 
        //we read the first piece of data
        file.get(buffer, maxLenBuffer, EOF);
 
        //we set the cursor at position 0
        this->currentBufferIndex = 0;
 
    }
 
    //the Parser allows reading any integer numerical type: int, short, long long and their unsigned type
    template<class classType>
    void readNumber(classType &number) {
 
        number = 0;
 
        int sign = 1; //for positive/negative numbers;
 
        char prevChar = 0; //keeps the last processed char
 
        //skips unusable chars
        while (buffer[currentBufferIndex] < '0' || buffer[currentBufferIndex] > '9') {
 
            prevChar = buffer[currentBufferIndex];
 
            //next char
            ++currentBufferIndex;
 
            //if the buffer ends we need to read again in case there is more data in the file
            if (currentBufferIndex == maxLenBuffer - 1) {
 
                file.get(buffer, maxLenBuffer, EOF);
 
                currentBufferIndex = 0;
 
            }
 
        }
 
        if (prevChar == '-')
            sign = -1;
 
        //here the number is built
        while (buffer[currentBufferIndex] >= '0' && buffer[currentBufferIndex] <= '9') {
 
            number = number * 10 + (buffer[currentBufferIndex] - '0');
 
            //next char
            ++currentBufferIndex;
 
            //if the buffer ends we need to read again in case there is more data in the file
            if (currentBufferIndex == maxLenBuffer - 1) {
 
                file.get(buffer, maxLenBuffer, EOF);
 
                currentBufferIndex = 0;
 
            }
 
        }
 
        if (buffer[currentBufferIndex] == '.') {
 
            //next char
            ++currentBufferIndex;
 
            //if the buffer ends we need to read again in case there is more data in the file
            if (currentBufferIndex == maxLenBuffer - 1) {
 
                file.get(buffer, maxLenBuffer, EOF);
 
                currentBufferIndex = 0;
 
            }
 
            classType fractionalExponent = 0.1; //with this we contruct the fractional part of the number
 
            while (buffer[currentBufferIndex] >= '0' && buffer[currentBufferIndex] <= '9') {
 
                number = number + fractionalExponent * (buffer[currentBufferIndex] - '0');
 
                fractionalExponent /= 10.0;
 
                //next char
                ++currentBufferIndex;
 
                //if the buffer ends we need to read again in case there is more data in the file
                if (currentBufferIndex == maxLenBuffer - 1) {
 
                    file.get(buffer, maxLenBuffer, EOF);
 
                    currentBufferIndex = 0;
 
                }
 
            }
 
        }
 
        //puts the right sign to the number
        number *= sign;
 
    }
 
    //closes the file when an instance of this class is deleted
    ~Parser() {
 
        file.close();
 
    }
 
};
 
//saves data of a test case
class Test {
 
    friend class SuffixArray2D;
    friend class Poze;
 
private:
 
    int lineCount, columnCount, minimumIdenticalSquares;
 
    int matrix[302][302];
 
public:
 
    Test(int lineCount, int columnCount, int minimumIdenticalSquares) {
 
        this->lineCount = lineCount;
        this->columnCount = columnCount;
        this->minimumIdenticalSquares = minimumIdenticalSquares;
 
        memset(this->matrix, 0, sizeof this->matrix);
 
    }
 
    int getLineCount() {
 
        return lineCount;
 
    }
 
    int getColumnCount() {
 
        return columnCount;
 
    }
 
    void updateMatrix(int lineIndex, int columnIndex, int value) {
 
        this->matrix[lineIndex][columnIndex] = value;
 
    }
 
    int getValue(int lineIndex, int columnIndex) {
 
        return this->matrix[lineIndex][columnIndex];
 
    }
 
};
 
//does the magic
class SuffixArray2D {
 
    friend class Poze;
 
private:
 
    int suffArray[10][302][302]; //SuffArray[k][i1][j1] == Suff[k][i2][j2] if and only if the 2^k side squares with corners (i1, j1) and (i2, j2) are identical
 
    std::vector<std::pair< std::pair< std::pair<int, int>, std::pair<int, int> >, int>> listOfPositions; //you really don't wanna know
 
    int countApparitions[90001];
 
public:
 
    void initialise() {
 
        memset(suffArray, 0, sizeof suffArray);
 
    }
 
    //checks if there are minimumIdenticalSquares identical squares of size 2^checkPow
    bool check(int checkPow, int minimumIdenticalSquares, int lineCount, int columnCount) {
 
        memset(countApparitions, 0, sizeof countApparitions);
 
        for (int lineIndex = 1; lineIndex <= lineCount; ++lineIndex) {
 
            for (int columnIndex = 1; columnIndex <= columnCount; ++columnIndex) {
 
                if (suffArray[checkPow][lineIndex][columnIndex] == -1)
                    continue;
 
                countApparitions[suffArray[checkPow][lineIndex][columnIndex]]++;
 
                if (countApparitions[suffArray[checkPow][lineIndex][columnIndex]] == minimumIdenticalSquares)
                    return true;
 
            }
 
        }
 
        return false;
 
    }
 
    //returns the greatest k such that there are (test->minimumIdenticalSquares) identical squares sized 2^k
    int calculateSuffArray(Test *test) {
 
        int lineCount = test->getLineCount();
        int columnCount = test->getColumnCount();
 
        //initialises size 1 squares
        for (int lineIndex = 1; lineIndex <= lineCount; ++lineIndex) {
 
            for (int columnIndex = 1; columnIndex <= columnCount; ++columnIndex) {
 
                suffArray[0][lineIndex][columnIndex] = test->matrix[lineIndex][columnIndex];
 
            }
 
        }
 
        if (!check(0, test->minimumIdenticalSquares, lineCount, columnCount))
            return -1;
 
        //the calculation itself
        for (int powerCount = 1, power = 1; (power >> 1) <= lineCount && (power >> 1) <= columnCount; ++powerCount, power <<= 1) {
 
            listOfPositions.clear();
 
            for (int lineIndex = 1; lineIndex <= lineCount; ++lineIndex) {
 
                for (int columnIndex = 1; columnIndex <= columnCount; ++columnIndex) {
 
                    int param1, param2, param3, param4;
 
                    param1 = suffArray[powerCount - 1][lineIndex][columnIndex];
 
                    if (lineIndex + power <= lineCount)
                        param2 = suffArray[powerCount - 1][lineIndex + power][columnIndex];
                    else
                        param2 = -1;
 
                    if (columnIndex + power <= columnCount)
                        param3 = suffArray[powerCount - 1][lineIndex][columnIndex + power];
                    else
                        param3 = -1;
 
                    if (lineIndex + power <= lineCount && columnIndex + power <= columnCount)
                        param4 = suffArray[powerCount - 1][lineIndex + power][columnIndex + power];
                    else
                        param4 = -1;
 
                    int position = lineIndex*columnCount + columnIndex - 1; //keeps both indexes in a single variable
 
                    listOfPositions.push_back(std::make_pair(std::make_pair(std::make_pair(param1, param2), std::make_pair(param3, param4)), position));
 
                }
 
            }
 
            std::sort(listOfPositions.begin(), listOfPositions.end());
 
            for (int index = 0; index < lineCount * columnCount; ++index) {
 
                if (index > 0 && listOfPositions[index].first == listOfPositions[index - 1].first) {
 
                    int prevLineIndex = listOfPositions[index - 1].second / columnCount;
                    int prevColumnIndex = listOfPositions[index - 1].second % columnCount + 1;
 
                    int currLineIndex = listOfPositions[index].second / columnCount;
                    int currColumnIndex = listOfPositions[index].second % columnCount + 1;
 
                    suffArray[powerCount][currLineIndex][currColumnIndex] = suffArray[powerCount][prevLineIndex][prevColumnIndex];
 
                }
                else {
 
                    int currLineIndex = listOfPositions[index].second / columnCount;
                    int currColumnIndex = listOfPositions[index].second % columnCount + 1;
 
                    suffArray[powerCount][currLineIndex][currColumnIndex] = index;
 
                }
 
                if (listOfPositions[index].first.second.second == -1) {
 
                    int currLineIndex = listOfPositions[index].second / columnCount;
                    int currColumnIndex = listOfPositions[index].second % columnCount + 1;
 
                    suffArray[powerCount][currLineIndex][currColumnIndex] = -1;
 
                }
 
            }
 
            if (!check(powerCount, test->minimumIdenticalSquares, lineCount, columnCount))
                return powerCount - 1;
 
        }
 
    }
 
};
 
//solves the problem named "poze".. or at least i hope so
class Poze {
 
private:
 
    Test *currTest;
 
    std::vector<int> solutions;
 
    SuffixArray2D suffArray;
 
    std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > squareData;
     
    //checks if a specified size satisfies the condition put in the problem
    bool check(int sizeToCheck, int max2Pow) {
 
        squareData.clear();
 
        int lineCount = currTest->lineCount;
        int columnCount = currTest->columnCount;
 
        for (int lineIndex = 1; lineIndex <= lineCount - sizeToCheck + 1; ++lineIndex) {
 
            for (int columnIndex = 1; columnIndex <= columnCount - sizeToCheck + 1; ++columnIndex) {
 
                squareData.push_back(std::make_pair(std::make_pair(suffArray.suffArray[max2Pow][lineIndex][columnIndex], suffArray.suffArray[max2Pow][lineIndex + sizeToCheck - (1 << max2Pow)][columnIndex]), std::make_pair(suffArray.suffArray[max2Pow][lineIndex][columnIndex + sizeToCheck - (1 << max2Pow)], suffArray.suffArray[max2Pow][lineIndex + sizeToCheck - (1 << max2Pow)][columnIndex + sizeToCheck - (1 << max2Pow)])));
 
            }
 
        }
 
        std::sort(squareData.begin(), squareData.end());
 
        int maxIdenticalSquaresCount = 1, currIdenticalSquaresCount = 0;
 
        for (int index = 0; index < squareData.size(); ++index) {
 
            if (index) {
 
                if (squareData[index] == squareData[index - 1]) {
 
                    ++currIdenticalSquaresCount;
 
                    maxIdenticalSquaresCount = std::max(maxIdenticalSquaresCount, currIdenticalSquaresCount);
 
                }
                else {
                 
                    currIdenticalSquaresCount = 1;
 
                }
 
            }
            else {
 
                currIdenticalSquaresCount = 1;
 
            }
 
        }
 
        return maxIdenticalSquaresCount >= currTest->minimumIdenticalSquares;
 
    }
 
    int solveCurrentTest() {
 
        suffArray.initialise();
 
        int minPow = suffArray.calculateSuffArray(currTest);
 
        if (minPow == -1)
            return 0;
 
        //we'll do binary search to find the biggest size of a square that satisfies the condition put in the problem
        int left = (1 << minPow), right = std::min((1 << (minPow + 1)) - 1, std::min(currTest->lineCount, currTest->columnCount));
 
        int solution = 0;
 
        while (left <= right) {
 
            int middle = (left + right) / 2;
 
            if (check(middle, minPow)) {
 
                left = middle + 1;
 
                solution = middle;
 
            }
            else {
 
                right = middle - 1;
 
            }
 
        }
 
        return solution;
 
    }
 
public:
 
    //loads the test cases prom a specified path
    std::vector<Test*> loadTests(std::string path) {
 
        Parser parser(infile, 2000000);
 
        std::vector<Test*> tests;
 
        int testCount;
 
        parser.readNumber(testCount);
 
        for (int index = 0; index < testCount; ++index) {
 
            int lineCount, columnCount, minimumIdenticalSquares;
 
            parser.readNumber(lineCount);
            parser.readNumber(columnCount);
            parser.readNumber(minimumIdenticalSquares);
 
            tests.push_back(new Test(lineCount, columnCount, minimumIdenticalSquares));
 
            for (int lineIndex = 1; lineIndex <= lineCount; ++lineIndex) {
 
                for (int columnIndex = 1; columnIndex <= columnCount; ++columnIndex) {
 
                    int value;
 
                    parser.readNumber(value);
 
                    tests[index]->updateMatrix(lineIndex, columnIndex, value);
 
                }
 
            }
 
        }
 
        return tests;
 
    }
 
    void solve(std::vector<Test*> tests) {
 
        for (Test *test : tests) {
 
            currTest = test;
 
            solutions.push_back(solveCurrentTest());
 
        }
 
    }
 
    void writeSolutions(std::string path) {
 
        std::ofstream file(path);
 
        for (int currSol : solutions)
            file << currSol << '\n';
         
        file.close();
 
    }
 
};
 
int main() {
 
    Poze poze;
 
    std::vector<Test*> tests = poze.loadTests(infile);
 
    poze.solve(tests);
 
    poze.writeSolutions(outfile);
 
}