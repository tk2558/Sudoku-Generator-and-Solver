#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

/*
Sudoku Random Board Generator Process

1. Start with an empty 9x9 Sudoku grid

2. Fill Sudoku grid recursively by attempting place numbers (1 to 9) in empty cells one by one.

-> Checks if it's valid to place a number in a particular cell using the isValid() function.
-> If valid, places the number in the cell and proceeds to solve the remaining grid recursively.
-> If not valid, backtrack and try other possibilities.

3. Once matrix is fully filled, randomly remove 40 to 50 digits

*/

/*
#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

/*
Sudoku Random Board Generator Process

1. Start with an empty 9x9 Sudoku grid

2. Fill Sudoku grid recursively by attempting place numbers (1 to 9) in empty cells one by one.

-> Checks if it's valid to place a number in a particular cell using the isValid() function.
-> If valid, places the number in the cell and proceeds to solve the remaining grid recursively.
-> If not valid, backtrack and try other possibilities.

3. Once matrix is fully filled, randomly remove 40 to 50 digits

*/

int getRandomDigit(int min, int max) { // function to get a random number
    static random_device rd; // Random number generator device
    static mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator
    uniform_int_distribution<> dis(min, max); // range from min to max (including min and max)
    return dis(gen);
}

bool isValid(vector<vector<char>>& board, int row, int col, char num) { // check valid input
    for (int i = 0; i < 9; i++) { // checking each box in col and row
        if (board[row][i] == num || board[i][col] == num) { // if num already exist within col or row
            return false; // NOT VALID INPUT
        }
    }
    int currRow = row - row % 3;  // Calculate the current row index of the 3x3 matrix
    int currCol = col - col % 3;  // Calculate the current col index of the 3x3 matrix

    // Loop through each cell in the 3x3 matrix
    for (int i = 0; i < 3; i++) { // checking row
        for (int j = 0; j < 3; j++) {  // checking col
            if (board[i + currRow][j + currCol] == num) { // If number already exists in the matrix
                return false; // NOT VALID INPUT
            }
        }
    }
    return true; // VALID INPUT
}

bool backTrackSudoku(vector<vector<char>>& board) { // Fill Sudoku grid recursively 
    int row = -1;
    int col = -1;
    bool isEmpty = true;

    for (int i = 0; i < 9 && isEmpty; i++) { // Find the first empty cell in the Sudoku board
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') { // found empty cell...
                row = i; // set curr row
                col = j; // set curr col
                isEmpty = false; // Set isEmpty to false to indicate a cell is found
                break;
            }
        }
    }
    if (isEmpty) { // Base Case for if the board is completely filled
        return true;
    }

    static random_device rd; // Random number generator device
    static mt19937 gen(rd());
    vector<char> nums{ '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    shuffle(nums.begin(), nums.end(), gen); // Shuffle the digits randomly

    for (char num : nums) { // Loop through each digit in shuffled vector
        if (isValid(board, row, col, num)) { // Check if digit is valid to be placed at the current cell
            board[row][col] = num;  // set digit at the current cell
            if (backTrackSudoku(board)) { // Recursively solve the Sudoku
                return true; // If Sudoku is solved, return true
            }
            board[row][col] = '.'; // If Sudoku cannot be solved with current digit, backtrack by resetting the cell to empty
        }
    }
    return false; // No valid digit can be placed at the current cell
}

void removeDigits(vector<vector<char>>& board) { // Remove digits from the board function
    int counter = getRandomDigit(40, 50); // Getting random number from 40 to 50 digits
    while (counter > 0) { // removing random number of digits
        int row = getRandomDigit(0, 8); // get random row
        int col = getRandomDigit(0, 8); // get random col

        if (board[row][col] == '.') { // if number in box already removed...
            continue; // move on
        }

        board[row][col] = '.'; // else remove it
        counter--; // decrement counter
    }
}

vector<vector<char>> generateRandomSudoku() { // generating random sudoku board
    vector<vector<char>> board(9, vector<char>(9, '.')); // start with empty sudoku board
    backTrackSudoku(board); // fill it out
    removeDigits(board); // remove some of the digits
    return board; // return board to be solved
}


// Getting Answer to Sudoku Board
void ans(vector<vector<char>>& board, vector<vector<char>>*& curr, int i, int j, vector<vector<bool>>& row, vector<vector<bool>>& col, vector<vector<bool>>& mat) {
    while (i < 9 && board[i][j] != '.') { // Loop until there is an empty cell or end of board
        if (j < 8) { j++; } // Move to next column
        else {
            j = 0; // Reset col idx to 0
            i++; // Move to next row
        }
    }

    if (i == 9) { // If all cells are filled
        *curr = board; // Set the pointer to the current board configuration
        return; // return filled out board
    }

    for (int k = 1; k <= 9; k++) { // Fill the current empty cell with numbers from 1 to 9
        if ((*curr).size() > 0) { // If solution found, exit loop
            break;
        }

        if (!col[j][k] && !row[i][k] && !mat[3 * (i / 3) + (j / 3)][k]) { // Check if it's valid to place number 'k' at position (i, j)
            row[i][k] = true; // mark as used in the row
            col[j][k] = true; // mark as used in the col
            mat[3 * (i / 3) + (j / 3)][k] = true; // mark as used in the matrix
            board[i][j] = (char)('0' + k); // place 'k' in the board at position (i, j)

            ans(board, curr, i, j, row, col, mat);  // Recursively call ans function

            row[i][k] = false; // unmark from row
            col[j][k] = false; // unmark from col
            mat[3 * (i / 3) + (j / 3)][k] = false; // unmark from matrix
            board[i][j] = '.'; // reset cell (i, j) to empty
        }
    }
}

void solveSudoku(vector<vector<char>>& board) {
    vector<vector<bool>> row(9, vector<bool>(10, false)); // initialize boolean row vector
    vector<vector<bool>> col(9, vector<bool>(10, false)); // initialize boolean col vector
    vector<vector<bool>> mat(9, vector<bool>(10, false)); // initialize boolean submatrix vector
    vector<vector<char>>* curr = new vector<vector<char>>(); // Pointer to store current solution

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '.') {
                int x = board[i][j] - '0'; // Convert char to int
                row[i][x] = true; // Mark as used in the row
                col[j][x] = true; // Mark as used in the col
                mat[3 * (i / 3) + (j / 3)][x] = true; // Mark as used in matrix
            }
        }
    }

    ans(board, curr, 0, 0, row, col, mat); // Start solving Sudoku
    board = *curr; // copy solution back to the original board
    delete curr;  // delete the temp board

    return;
}

void display(vector<vector<char>>& board) { // print Sudoku Board for display
    for (int i = 0; i < board.size(); i++) {
        for (auto it = board[i].begin(); it != board[i].end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }
    return;
}

int main() {
    vector<vector<char>> test{ {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
    {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
    {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
    {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
    {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
    {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
    {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
    {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
    {'.', '.', '.', '.', '8', '.', '.', '7', '9'} };

    cout << "Testing Sudoku Board 1: " << endl;
    display(test);

    cout << "\nSolving Sudoku Board 1: " << endl;
    solveSudoku(test);
    display(test);

    cout << "\nTesting Random Sudoku Board: " << endl;
    vector<vector<char>> test2 = generateRandomSudoku();
    display(test2);

    cout << "\nSolving Random Sudoku Board: " << endl;
    solveSudoku(test2);
    display(test2);
    
    return 0;
}