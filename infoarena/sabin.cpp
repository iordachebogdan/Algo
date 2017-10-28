#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
 
#define infile "sabin.in"
#define outfile "sabin.out"
 
//saves the data of a book
class Book {
 
public:
 
    char bookTitle[33];
 
    Book(char *newBookTitle) {
 
        memset(this->bookTitle, 0, sizeof this->bookTitle);
 
        for (int charIndex = 0; newBookTitle[charIndex]; ++charIndex)
            this->bookTitle[charIndex] = newBookTitle[charIndex];
 
    }
 
    int bookTitleLength() {
         
        return strlen(this->bookTitle);
     
    }
 
};
 
//saves the data of a shelf/the special shelf
class Shelf {
 
private:
 
    std::vector<Book> shelf;
 
    char mergedShelf[600];
 
public:
 
    Shelf(std::vector<Book> newShelf) {
 
        for (Book currentBook : newShelf) {
 
            this->shelf.push_back(currentBook);
 
        }
 
    }
 
    //gets the book from a specified position
    Book getBook(int bookIndex) {
 
        return shelf[bookIndex - 1];
 
    }
 
    //returns the number of books on this shelf
    int booksOnShelfCount() {
 
        return this->shelf.size();
 
    }
 
    //returns a string formed by the merged titles of the books on the shelf
    char *mergeShelf() {
         
        memset(mergedShelf, 0, sizeof mergedShelf);
 
        int bookIndex = 0;
 
        for (Book book : shelf) {
 
            ++bookIndex;
 
            for (int charIndex = 0; book.bookTitle[charIndex]; ++charIndex)
                mergedShelf[charIndex * booksOnShelfCount() + bookIndex - 1] = book.bookTitle[charIndex];
 
        }
 
        return mergedShelf;
 
    }
 
};
 
//saves the data of a query
class Query {
 
public:
 
    int maxPrefixLength;
 
    std::vector<int> specialShelfBookIndexes;
 
};
 
//saves the data of the test case
class Test {
 
private:
 
    int shelfCount;
    int bookCount, specialShelfBookCount;
    int bookTitleLength;
    int queryCount;
 
    std::vector<Query> queries;
 
    std::vector<Shelf> shelves;
 
    Shelf *specialShelf;
 
public:
 
    Test(int shelfCount, int bookCount, int specialShelfBookCount, int bookTitleLength, int queryCount) {
 
        this->shelfCount = shelfCount;
        this->bookCount = bookCount;
        this->specialShelfBookCount = specialShelfBookCount;
        this->bookTitleLength = bookTitleLength;
        this->queryCount = queryCount;
 
        shelves.clear();
        queries.clear();
 
    }
 
    int getShelfCount() {
 
        return shelfCount;
 
    }
 
    //returns the number of books assigned to a shelf
    int getBookCount() {
 
        return bookCount;
 
    }
 
    //returns the number of books assigned to the special shelf
    int getSpecialShelfBookCount() {
 
        return specialShelfBookCount;
 
    }
 
    //returns the length of the title of a book
    int getBookTitleLength() {
 
        return bookTitleLength;
 
    }
 
    //returns the number of queries
    int getQueryCount() {
 
        return queryCount;
 
    }
 
    //returns a query with a specified index
    Query *getQuery(int queryIndex) {
 
        return &queries[queryIndex - 1];
 
    }
 
    //returns a shelf with a specified index
    Shelf *getShelf(int shelfIndex) {
 
        return &shelves[shelfIndex - 1];
 
    }
 
    //returns the special shelf
    Shelf *getSpecialShelf() {
 
        return specialShelf;
 
    }
 
    //adds a shelf
    void addShelf(Shelf shelf) {
 
        shelves.push_back(shelf);
 
    }
 
    //adds a query
    void addQuery(Query query) {
 
        queries.push_back(query);
 
    }
 
    //adds the special shelf
    void addSpecialShelf(Shelf *SpecialShelf) {
 
        specialShelf = SpecialShelf;
 
    }
 
};
 
class Trie {
 
private:
 
    struct TrieNode{
 
        //shows how many times this node has been visited
        int visitCount;
 
        //26 children for every letter of the alphabet
        TrieNode *children[26];
 
        TrieNode() {
 
            visitCount = 0;
 
            memset(children, NULL, sizeof children);
 
        }
 
    } *trieRoot;
 
public:
 
    Trie() {
 
        this->trieRoot = new TrieNode;
 
    }
 
    TrieNode *getTrieRoot() {
 
        return trieRoot;
 
    }
 
    //inserts a string into the Trie
    void insertString(char *stringToInsert, TrieNode *node) {
 
        node->visitCount++;
 
        if (*stringToInsert == 0)
            return;
 
        TrieNode *nextNode = node->children[*stringToInsert - 'a'];
 
        if (nextNode == NULL) {
 
            node->children[*stringToInsert - 'a'] = nextNode = new TrieNode;
 
        }
 
        insertString(stringToInsert + 1, nextNode);
 
    }
 
    //returns the number of strings in the Trie that have the longest common prefix with stringToCheck at least minimumCommonPrefix
    int queryTrie(char *stringToCheck, int charIndex, int minimumCommonPrefix, TrieNode *node) {
 
        if (charIndex == minimumCommonPrefix) {
 
            return node->visitCount;
 
        }
 
        TrieNode *nextNode = node->children[*stringToCheck - 'a'];
 
        if (nextNode == NULL) {
 
            return 0;
        }
        else {
 
            return queryTrie(stringToCheck + 1, charIndex + 1, minimumCommonPrefix, nextNode);
 
        }
     
    }
 
};
 
//solves the problem "sabin"
class Sabin {
 
private:
 
    Test *test;
 
    Trie trie;
 
    std::vector<int> solutions;
 
    //inserts a shelf into the trie
    void buildTrie(int shelfIndex) {
 
        Shelf *shelf = test->getShelf(shelfIndex);
 
        char *stringToInsert = shelf->mergeShelf();
 
        trie.insertString(stringToInsert, trie.getTrieRoot());
 
    }
 
    //solves a single query
    int solveThisQuery(int queryIndex) {
 
        Query *query = test->getQuery(queryIndex);
 
        Shelf *shelf, *specialShelf = test->getSpecialShelf();
 
        std::vector<Book> booksForQuery;
 
        for (int bookIndex : query->specialShelfBookIndexes) {
 
            booksForQuery.push_back(specialShelf->getBook(bookIndex));
 
        }
 
        shelf = new Shelf(booksForQuery);
 
        char *stringForQuery = shelf->mergeShelf();
 
        int minimumCommonPrefix = query->maxPrefixLength * test->getBookCount();
 
        return trie.queryTrie(stringForQuery, 0, minimumCommonPrefix, trie.getTrieRoot()) - trie.queryTrie(stringForQuery, 0, minimumCommonPrefix + test->getBookCount(), trie.getTrieRoot());
 
    }
 
public:
 
    //load the test from a specified path
    void loadTest(std::string path) {
 
        std::ifstream file(path);
 
        int shelfCount;
        int bookCount, specialShelfBookCount;
        int bookTitleLength;
        int queryCount;
 
        char bookTitle[33];
 
        file >> shelfCount >> bookCount >> specialShelfBookCount >> bookTitleLength >> queryCount;
 
        test = new Test(shelfCount, bookCount, specialShelfBookCount, bookTitleLength, queryCount);
 
        //gets the books from the shelves
        for (int shelfIndex = 1; shelfIndex <= shelfCount; ++shelfIndex) {
 
            std::vector<Book> shelf;
 
            for (int bookIndex = 1; bookIndex <= bookCount; ++bookIndex) {
 
                file >> bookTitle;
 
                Book *newBook = new Book(bookTitle);
 
                shelf.push_back(*newBook);
 
            }
 
            test->addShelf(*(new Shelf(shelf)));
 
        }
         
        //gets the books from the special shelf
        std::vector<Book> specialShelf;
 
        for (int bookIndex = 1; bookIndex <= specialShelfBookCount; ++bookIndex) {
 
            file >> bookTitle;
 
            Book *newBook = new Book(bookTitle);
 
            specialShelf.push_back(*newBook);
 
        }
 
        test->addSpecialShelf(new Shelf(specialShelf));
 
        for (int queryIndex = 1; queryIndex <= queryCount; ++queryIndex) {
 
            Query query;
 
            file >> query.maxPrefixLength;
 
            for (int index = 1; index <= bookCount; ++index) {
 
                int bookIndex;
 
                file >> bookIndex;
 
                query.specialShelfBookIndexes.push_back(bookIndex);
 
            }
 
            test->addQuery(query);
 
        }
 
    }
 
    //solves the test case
    void solve() {
 
        int shelfCount = test->getShelfCount();
 
        for (int shelfIndex = 1; shelfIndex <= shelfCount; ++shelfIndex) {
 
            buildTrie(shelfIndex);
 
        }
 
        int queryCount = test->getQueryCount();
 
        for (int queryIndex = 1; queryIndex <= queryCount; ++queryIndex) {
 
            solutions.push_back(solveThisQuery(queryIndex));
 
        }
 
    }
 
    //writes the solution to a specified path
    void writeSolution(std::string path) {
 
        std::ofstream file(path);
 
        for (int solution : solutions)
            file << solution << '\n';
 
    }
 
};
 
 
int main() {
 
    //makes an instance of the class Sabin
    Sabin sabin;
 
    //loads the test case
    sabin.loadTest(infile);
 
    //solves the queries
    sabin.solve();
 
    //writes the solutions
    sabin.writeSolution(outfile);
 
    return 0;
 
}