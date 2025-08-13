#include "project.hpp"
using namespace std;

bool FindUnassignedLocation(vector<vector<int> > grid, int &row, int &col){
    for (row = 0; row < 9; row++){
        for (col = 0; col < 9; col++){
            if (grid[row][col] == 0)
                return true;
        }
    }
    return false;
}

SudokuBase::SudokuBase():grid(9, vector<int>(9, 0)){}

void SudokuBase::displaygrid() const {
    for (int j = 0; j< 9; j++) {
        for (int k = 0; k<9; k++) {
            if (grid[j][k])
                cout << grid[j][k] << " ";
            else 
                cout << "  ";
            if(k==2 or k==5)
                cout << "| "; 
        }
        if(j==2 or j==5)
            cout<<endl<<"------$-------$------";   
        cout << endl;
    }
}

std::vector<std::vector<int> >& SudokuBase::getGrid(){
    return grid;
}

bool SudokuBase::isValidMove(int row, int col, int num) const {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num || grid[i][col] == num ||
            grid[(row / 3) * 3 + i / 3][(col / 3) * 3 + i % 3] == num) {
            return false;
        }
    }
    return true;
}

ostream& operator <<(ostream &out,const SudokuBase &s){
    s.displaygrid();    
    return out;
}

istream& operator >>(istream &in, SudokuBase &s){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if (!(in >> s.grid[i][j])){
                cout << "Enter integers!\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                j--; continue;
            }
        }
    }
    return in;
}

SudokuSolver::SudokuSolver():SudokuBase(){
    solved = false;
}

SudokuSolver::SudokuSolver(vector<vector<int> >& v){
    grid = v;
    solved = false;
    int row, col;
    if (solvable() && !FindUnassignedLocation(grid, row, col))
       solved = true;
}

bool SudokuSolver::solvable(){
    int temp;
    for(int x = 0; x<9; x++){
        for(int y = 0; y<9; y++){
            if(temp = grid[x][y]){
                grid[x][y] = 0;
                if (isValidMove(x, y, temp))
                    grid[x][y] = temp;
                else{
                    grid[x][y] = temp;
                    return false;
                }
                
            }
        }
    }
    int num = 0;
    countSoln(num);
    if (num>1)
        return false;
    return true;
}

bool SudokuSolver::solve(){
    if (solved)
        return true;
    int row, col;
    if (FindUnassignedLocation(grid, row, col)) {
        for (int num = 1; num <= 9; num++) {
            if (isValidMove(row, col, num)) {
                grid[row][col] = num;
                if (solve()) return true;
                grid[row][col] = 0;
            }
        }
        return false;
    }
    solved = true;
    return true;
}

bool SudokuSolver::isComplete(){
    return solved;
}

istream& operator >>(istream &in, SudokuSolver &s){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if (!(in >> s.grid[i][j])){
                cout << "Enter integers!\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                j--; continue;
            }
        }
    }
    int row, col;
    if(s.solvable() && !FindUnassignedLocation(s.grid, row, col)){
        s.solved = true;
    }
    return in;
}

SudokuGenerator::SudokuGenerator():SudokuBase(), solution(9, vector<int>(9,0)){
    for(int i=0;i<81;i++){
        gridPos[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(gridPos, gridPos + 81, g);
    
    for(int i=0;i<9;i++){
        guessNum[i]=i+1;
    }

    shuffle(guessNum, guessNum + 9, g);
}

istream& operator >>(istream &in, SudokuGenerator &s){
    throw runtime_error("SudokuGenerator does not take input!");
    return in;
}

void SudokuGenerator::fillEmptyDiagonalBox(int idx){
    int start = idx * 3;
    random_device rd;
    mt19937 g(rd());
    shuffle(guessNum, guessNum + 9, g);
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j)
        grid[start+i][start+j] = guessNum[i*3+j];
    }
}

void SudokuGenerator::createSeed(){
    fillEmptyDiagonalBox(0);
    fillEmptyDiagonalBox(1);
    fillEmptyDiagonalBox(2);

    SudokuSolver s(grid);
    s.solve();
    grid = s.getGrid();

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
        solution[i][j] = grid[i][j];
        }
    }
}

void SudokuBase::countSoln(int &number)
{
    int row, col;

    if(!FindUnassignedLocation(grid, row, col)){
        number++;
        return ;
    }
    for(int i=1;i<10 && number<2;i++){
        if( isValidMove(row, col, i) ){
            grid[row][col] = i;
            countSoln(number);
        }
        grid[row][col] = 0;
    }
}

void SudokuGenerator::generatePuzzle()
{
    createSeed();
    int count = 0;
    for(int i=0;i<81;i++)
    {
        int x = (gridPos[i])/9;
        int y = (gridPos[i])%9;
        int temp = grid[x][y];
        grid[x][y] = 0;

        int check=0;
        countSoln(check);
        if(check!=1)
        {
            grid[x][y] = temp;
        }
        if (++count >= 30)
            break;
    }
}

SudokuGame::SudokuGame():SudokuGenerator(){
    solved = false;
}

istream& operator >>(istream &in, SudokuGame &s){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if (!(in >> s.grid[i][j])){
                cout << "Enter integers!\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                j--; continue;
            }
        }
    }
    int row, col;
    if (!s.solvable()){
        throw runtime_error("This is not a valid sudoku puzzle!");
    }
    if(!FindUnassignedLocation(s.grid, row, col)){
        s.solved = true;
    }
    return in;
}

bool SudokuGame::playerMove(){
    if(solved){
        cout << "The puzzle is already solved.\n";
        return true;
    }
    int row, col, num;
    cout << "Enter -1 if you want to quit.\n";
    cout << "Enter row (1-9), column (1-9), and number (1-9): ";
    if(!(cin >> row)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Enter integers only!\n");
    }

    if (row == -1)
        return false;
    
    if (!(cin >> col >> num)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Enter integers only!\n");
    }

    if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
        cout << "Invalid input. Please use numbers 1-9.\n";
        return true;
    }

    row--;
    col--;

    if (grid[row][col] != 0)
        cout << "Cell is already filled. Try a different cell.\n";

    else if (num == solution[row][col]) {
        grid[row][col] = num;
        cout << "Move accepted.\n";
        int x, y;
        if (!FindUnassignedLocation(grid, x, y))
            solved = true;
    }

    else 
        cout << "Wrong move. Try again.\n";

    return true;
}