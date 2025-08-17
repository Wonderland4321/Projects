#ifndef SUDOKU_HEADER_
#define SUDOKU_HEADER_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <limits>

class SudokuBase {
protected:
    std::vector<std::vector<int> > grid;
    friend std::ostream &operator <<(std::ostream &, const SudokuBase&);
    friend std::istream &operator >>(std::istream &, SudokuBase&);
public:
    SudokuBase();
    virtual void displaygrid() const;
    virtual std::vector<std::vector<int> >& getGrid();
    virtual void countSoln(int &number);
    virtual bool isValidMove(int row, int col, int num) const;
};

class SudokuSolver : virtual public SudokuBase {
protected:
    bool solved;
    friend std::istream &operator >>(std::istream &, SudokuSolver&);
public:
    SudokuSolver();
    bool solvable();
    SudokuSolver(std::vector<std::vector<int> > &v);
    bool isComplete();
    bool solve();
};

class SudokuGenerator : virtual public SudokuBase {
protected:
    std::vector<std::vector<int> > solution;
    int guessNum[9];
    int gridPos[81];
    friend std::istream &operator >>(std::istream &, SudokuGenerator&);
public:
    SudokuGenerator();
    void createSeed();
    void fillEmptyDiagonalBox(int);
    void generatePuzzle();
};

class SudokuGame : public SudokuGenerator, public SudokuSolver {
    friend std::istream &operator >>(std::istream &, SudokuGame&);
    public:
        SudokuGame();
        bool playerMove();
};

#endif
