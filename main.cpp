#include "project.hpp"
using namespace std;

int main(){
    SudokuGame game;
    char c;
    bool quit = false;
    game.generatePuzzle();
    cout << "Generated Sudoku Puzzle\n";
    cout << game;
    cout << "\nWelcome to Sudoku! Enter numbers to solve.\n\n";
    while(true){
        cout << game;
        if (game.isComplete()){
            cout << "Congratulations! You've completed the puzzle!\n";
            break;
        }
        try{
            if (!game.playerMove()){
                quit = true;
                break;
            }
        }
        catch(const exception& e){
            cout << "Exception: " << e.what();
        }
    }
    if (quit){
        game.solve();
        cout << "\nSolution:\n";
        cout << game;
    }
}