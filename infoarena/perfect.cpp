#include fstream
#include vector
#include algorithm
#include cstring
 
#define infile perfect.in
#define outfile perfect.out
 
stdofstream outFile(outfile);
 
we need Huge numbers for this problem
class Huge {
 
private
 
    int numberLength;
 
    stdvectorint number;
 
public
 
    Huge() {
 
        numberLength = 0;
 
        number.clear();
 
    }
 
    converts an int to Huge type
    void convertInt(int numberToConvert) {
 
        numberLength = 0;
 
        number.clear();
 
        while (numberToConvert != 0) {
 
            ++numberLength;
 
            number.push_back(numberToConvert);
 
            numberToConvert = 10;
 
        }
     
    }
 
    the sum operator
    Huge operator + (Huge hugeToAdd) {
 
        Huge sum;
 
        sum.numberLength = stdmax(hugeToAdd.numberLength, this-numberLength);
 
        sum.number.resize(stdmax(hugeToAdd.numberLength, this-numberLength), 0);
 
        this-number.resize(sum.numberLength, 0);
        hugeToAdd.number.resize(sum.numberLength, 0);
 
        int transport = 0;
 
        for (int index = 0; index  sum.numberLength; ++index) {
 
            sum.number[index] = this-number[index] + hugeToAdd.number[index] + transport;
 
            transport = sum.number[index]  10;
 
            sum.number[index] %= 10;
 
        }
 
        if (transport != 0) {
 
            sum.number.push_back(transport);
 
            sum.numberLength++;
 
        }
 
        return sum;
 
    }
 
    the multiply operator
    Huge operator  (Huge multiplier) {
 
        Huge product;
 
        product.numberLength = multiplier.numberLength + this-numberLength - 1;
 
        product.number.resize(multiplier.numberLength + this-numberLength - 1, 0);
 
        for (int thisIndex = 0; thisIndex  this-numberLength; ++thisIndex) {
 
            for (int multiplierIndex = 0; multiplierIndex  multiplier.numberLength; ++multiplierIndex) {
 
                product.number[thisIndex + multiplierIndex] += this-number[thisIndex]  multiplier.number[multiplierIndex];
 
            }
 
        }
 
        int transport = 0;
 
        for (int productIndex = 0; productIndex  product.numberLength; ++productIndex) {
 
            transport = (product.number[productIndex] += transport)  10;
 
            product.number[productIndex] %= 10;
 
        }
 
        if (transport != 0) {
 
            product.number.push_back(transport);
 
            product.numberLength++;
 
        }
 
        return product;
 
    }
 
    writes a Huge to the outFile
    void print() {
 
        for (int index = numberLength - 1; index = 0; --index) {
 
            outFile  number[index];
 
        }
 
    }
 
};
 
a class inherited by all 3 special graphs (see problem description)
class SpecialGraph {
 
protected
 
    int order;
 
};
 
an A-type graph (see problem description)
class AGraph  public SpecialGraph {
 
private
 
    Huge fibonacci;
 
public
 
    AGraph(int order) {
 
        this-order = order;
 
        this-fibonacci = new Huge[order + 5];
 
    }
 
    returns the number of different perfect matchings in an A-type graph
    Huge countPerfectMatchings() {
 
        the answer is F(order) where F is the fibonacci function
 
        fibonacci[0].convertInt(1);
        fibonacci[1].convertInt(1);
 
        for (int orderIndex = 2; orderIndex = order; ++orderIndex) {
 
            fibonacci[orderIndex] = fibonacci[orderIndex - 1] + fibonacci[orderIndex - 2];
 
        }
 
        return fibonacci[order];
 
    }
 
};
 
an B-type graph (see problem description)
class BGraph  public SpecialGraph {
 
private
 
    Huge fibonacci;
 
public
 
    BGraph(int order) {
 
        this-order = order;
 
        this-fibonacci = new Huge[order + 5];
 
    }
 
    returns the number of different perfect matchings in an B-type graph
    Huge countPerfectMatchings() {
 
        the answer is 2F(order)F(order - 1) where F is the fibonacci function
 
        fibonacci[0].convertInt(1);
        fibonacci[1].convertInt(1);
 
        for (int orderIndex = 2; orderIndex = order; ++orderIndex) {
 
            fibonacci[orderIndex] = fibonacci[orderIndex - 1] + fibonacci[orderIndex - 2];
 
        }
 
        Huge huge2;
 
        huge2.convertInt(2);
 
        return huge2  fibonacci[order]  fibonacci[order - 1];
 
    }
 
};
 
an C-type graph (see problem description)
class CGraph  public SpecialGraph {
 
private
 
    Huge huge2;
 
public
 
    CGraph(int order) {
     
        this-order = order;
 
        this-huge2.convertInt(2);
     
    }
 
    returns the number of different perfect matchings in an C-type graph
    Huge countPerfectMatchings() {
 
        the answer is 2 ^ (order(order + 1)  2)
 
        Huge counter;
 
        counter.convertInt(1);
 
        for (int index = 1; index = (order  (order + 1)  2)  2; ++index) {
 
            counter = counter  huge2;
 
        }
 
        counter = counter  counter;
 
        if ((order  (order + 1)  2) % 2 == 1) {
 
            counter = counter  huge2;
 
        }
 
        return counter;
 
    }
 
};
 
saves data of a test case
class Test {
 
private
 
    int specialGraphOrder;
 
    char specialGraphType;
 
public
 
    Test(char specialGraphType, int specialGraphOrder) {
 
        this-specialGraphOrder = specialGraphOrder;
 
        this-specialGraphType = specialGraphType;
 
    }
 
    int getSpecialGraphOrder() {
 
        return specialGraphOrder;
 
    }
 
    char getSpecialGraphType() {
 
        return specialGraphType;
 
    }
 
};
 
solves problem perfect
class Perfect {
 
private
 
    Test test;
 
    Huge solution;
 
public
 
    loads the test case from a specified path
    void loadTest(stdstring path) {
 
        stdifstream file(path);
 
        int specialGraphOrder;
        char specialGraphType;
 
        file  specialGraphType  specialGraphOrder;
 
        test = new Test(specialGraphType, specialGraphOrder);
 
    }
 
    solves the problem
    void solve() {
 
        AGraph aGraph(test-getSpecialGraphOrder());
 
        BGraph bGraph(test-getSpecialGraphOrder());
 
        CGraph cGraph(test-getSpecialGraphOrder());
 
        switch (test-getSpecialGraphType()) {
 
        case 'A'
 
            solution = aGraph.countPerfectMatchings();
 
            break;
 
        case 'B'
 
            solution = bGraph.countPerfectMatchings();
 
            break;
 
        case 'C'
 
            solution = cGraph.countPerfectMatchings();
 
            break;
 
        }
 
    }
 
    writes the solution to the output file
    void writeSolution(){
 
        solution.print();
 
    }
 
};
 
int main() {
 
    Perfect perfect;
 
    perfect.loadTest(infile);
 
    perfect.solve();
 
    perfect.writeSolution();
 
    return 0;
 
}