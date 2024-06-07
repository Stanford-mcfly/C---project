#include <bits/stdc++.h>
#include "avl.h"

using namespace std;

#define UNASSIGNED 0
#define N 9


class Graph
{
public:
    Graph(int size);
    void addEdge(int v, int w);
    bool isSafe(int v, int num, vector<vector<int>> &board);
    bool solveSudoku(vector<vector<int>> &board);
    void printadj();

public:
    int V;
    vector<vector<int>> adj;

    void getAdjList(int row, int col, vector<int> &adjList);
    bool dfs(vector<vector<int>> &board, int v);
    int getVertex(int row, int col);
};

Graph::Graph(int size) : V(size)
{
    adj.resize(V);
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            vector<int> adjList;
            getAdjList(row, col, adjList);
            for (int v : adjList)
            {
                addEdge(getVertex(row, col), v);
            }
        }
    }
}

void Graph::printadj(){
    for(int i=0;i<adj.size();i++){
        cout<<"Square ["<<i/N<<"] [" << i%N << "]"<<" : " << endl;
        for(int j=0;j<adj[i].size();j++){
            cout<<adj[i][j]/N<<" " << adj[i][j]%N << " | ";
        }
        cout<<endl << endl;
    }
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

bool Graph::isSafe(int v, int num, vector<vector<int>> &board)
{
    int row = v / N;
    int col = v % N;

    for (int i : adj[v])
    {
        int r = i / N;
        int c = i % N;
        if (board[r][c] == num)
        {
            return false;
        }
    }
    return true;
}

bool Graph::solveSudoku(vector<vector<int>> &board)
{
    return dfs(board, 0);
}

bool Graph::dfs(vector<vector<int>> &board, int v)
{
    if (v == N * N)
    {
        return true;
    }

    int row = v / N;
    int col = v % N;

    if (board[row][col] != UNASSIGNED)
    {
        return dfs(board, v + 1);
    }

    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(v, num, board))
        {
            board[row][col] = num;
            if (dfs(board, v + 1))
            {
                return true;
            }
            board[row][col] = UNASSIGNED; 
        }
    }
    return false; 
}

void Graph::getAdjList(int row, int col, vector<int> &adjList)
{
    int gridRow = row / 3;
    int gridCol = col / 3;

    for (int i = 0; i < N; i++)
    {
        adjList.push_back(getVertex(row, i));
        adjList.push_back(getVertex(i, col));
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            adjList.push_back(getVertex(3 * gridRow + i, 3 * gridCol + j));
        }
    }
}

int Graph::getVertex(int row, int col)
{
    return row * N + col;
}

class SudokuSolver
{
public:
    SudokuSolver();
    void generateRandomSudoku(vector<vector<int>> &board, vector<vector<int>> &solution);
    void printBoard(const vector<vector<int>> &board);
    void storeSolution(const vector<vector<int>> &board, const string &name, double timeTaken);
    bool isBoardCorrect(const vector<vector<int>> &board, const vector<vector<int>> &solution);
    bool solveSudoku(vector<vector<int>> &board);
    void revealCellValue(vector<vector<int>> &board, int row, int col);
    void explainCellValidity(const vector<vector<int>> &board, int row, int col, int num);
    bool fillNextLogicalStep(vector<vector<int>> &board, const vector<vector<int>> &solution);
    void displayScoreboard(AVLTree &tree);
    void printadj();

private:
    Graph graph;
};

SudokuSolver::SudokuSolver() : graph(N * N) {}

void SudokuSolver::generateRandomSudoku(vector<vector<int>> &board, vector<vector<int>> &solution)
{
   
    board.assign(N, vector<int>(N, UNASSIGNED));

 
    solveSudoku(board);
    solution = board; 


    srand(time(0));              
    int count = rand() % 25 + 40; 

    vector<vector<int>> tempBoard = board; 

    while (count > 0)
    {
        int row = rand() % N;
        int col = rand() % N;

        if (board[row][col] != UNASSIGNED)
        {
            int temp = board[row][col];
            board[row][col] = UNASSIGNED;

         
            vector<vector<int>> copy = board;
            if (!solveSudoku(copy))
            {
                board[row][col] = temp;
            }
            else
            {
                count--;
            }
        }
    }
}

void SudokuSolver::printadj(){
    graph.printadj();
}

void SudokuSolver::printBoard(const vector<vector<int>> &board)
{
    for (int i = 0; i < N; i++)
    {
        if (i % 3 == 0 && i != 0)
        {
            cout << "------+-------+------" << endl;
        }
        for (int j = 0; j < N; j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                cout << "| ";
            }
            cout << (board[i][j] == UNASSIGNED ? "." : to_string(board[i][j])) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void SudokuSolver::storeSolution(const vector<vector<int>> &board, const string &name, double timeTaken)
{
    ofstream outfile("sudoku_solutions.txt", ios::app);
    if (!outfile)
    {
        cerr << "Unable to open file sudoku_solutions.txt";
        return;
    }

    outfile << "Name: " << name << endl;
    outfile << "Time taken: " << timeTaken << " seconds" << endl;
    outfile << "Solution:" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << board[i][j] << " ";
        }
        outfile << endl;
    }
    outfile << "------------------------------------------" << endl;

    outfile.close();
}

bool SudokuSolver::isBoardCorrect(const vector<vector<int>> &board, const vector<vector<int>> &solution)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (board[i][j] != solution[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool SudokuSolver::solveSudoku(vector<vector<int>> &board)
{
    return graph.solveSudoku(board);
}

void SudokuSolver::revealCellValue(vector<vector<int>> &board, int row, int col)
{
    auto solution = board;
    solveSudoku(solution);
    if (board[row][col] == UNASSIGNED)
    {
        board[row][col] = solution[row][col];
        cout << "Revealed value at (" << row + 1 << ", " << col + 1 << "): " << solution[row][col] << endl;
    }
    else
    {
        cout << "Cell (" << row + 1 << ", " << col + 1 << ") is already filled with: " << board[row][col] << endl;
    }
}

void SudokuSolver::explainCellValidity(const vector<vector<int>> &board, int row, int col, int num)
{   
    bool valid = true;
    string explanation = "Explanation for number " + to_string(num) + " at cell (" + to_string(row + 1) + ", " + to_string(col + 1) + "): ";

    for (int i = 0; i < N; i++)
    {
        if (board[row][i] == num)
        {
            explanation += "Number found in the same row at column " + to_string(i + 1) + ". ";
            valid = false;
        }
    }

    
    for (int i = 0; i < N; i++)
    {
        if (board[i][col] == num)
        {
            explanation += "Number found in the same column at row " + to_string(i + 1) + ". ";
            valid = false;
        }
    }

    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[startRow + i][startCol + j] == num)
            {
                explanation += "Number found in the 3x3 grid. ";
                valid = false;
            }
        }
    }

    if (valid)
    {
        explanation += "The number is valid.";
    }

    cout << explanation << endl;
}

bool SudokuSolver::fillNextLogicalStep(vector<vector<int>> &board, const vector<vector<int>> &solution)
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (board[row][col] == UNASSIGNED)
            {
                
                if (solution[row][col] != board[row][col])
                {
                    board[row][col] = solution[row][col];
                    cout << "Next logical step: placed " << solution[row][col] << " at cell (" << row + 1 << ", " << col + 1 << ")" << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void SudokuSolver::displayScoreboard(AVLTree &tree)
{
    int choice;
    cout << "Do you want to display scores below a certain time limit?\n1. Yes\n2. No\nEnter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        double timeLimit;
        cout << "Enter the time limit (in seconds): ";
        cin >> timeLimit;

        cout << "Scoreboard (Time <= " << timeLimit << " seconds):" << endl;
        tree.inOrderTraversal(timeLimit);
    }
    else if (choice == 2)
    {
        cout << "Scoreboard (All scores):" << endl;
        tree.inOrderTraversal();
    }
    else
    {
        cout << "Invalid choice." << endl;
    }
}

void loadResultsToAVLTree(AVLTree &tree)
{
    ifstream infile("sudoku_solutions.txt");
    if (!infile)
    {
        cerr << "Unable to open file sudoku_solutions.txt" << endl;
        return;
    }

    string line, name;
    double timeTaken;

    while (getline(infile, line))
    {
        if (line.find("Name: ") == 0)
        {
            name = line.substr(6);
        }
        else if (line.find("Time taken: ") == 0)
        {
            timeTaken = stod(line.substr(12, line.find(" seconds") - 12));
            tree.insert(name, timeTaken);
        }
    }

    infile.close();
}

int main()
{
    SudokuSolver solver;
    vector<vector<int>> board(N, vector<int>(N, UNASSIGNED));
    vector<vector<int>> solution(N, vector<int>(N, UNASSIGNED));
    ifstream infile("sudoku.txt");
    if (!infile)
    {
        cerr << "Unable to open file sudoku.txt";
        exit(1);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            infile >> board[i][j];
        }
    }
    infile.close();

    int choice;
    string name;
    AVLTree tree;

    loadResultsToAVLTree(tree);

    do 
    {  cout << endl << endl << endl << endl << endl;
       cout << "\033[1;32m" // Start green color
         << " ######  ##     ## ########   #######  ##    ## ##     ##              ######   #######  ##       ##     ## ######## ########  \n"
         << "##    ## ##     ## ##     ## ##     ## ##   ##  ##     ##             ##    ## ##     ## ##       ##     ## ##       ##     ## \n"
         << "##       ##     ## ##     ## ##     ## ##  ##   ##     ##             ##       ##     ## ##       ##     ## ##       ##     ## \n"
         << " ######  ##     ## ##     ## ##     ## #####    ##     ##              ######  ##     ## ##       ##     ## ######   ########  \n"
         << "      ## ##     ## ##     ## ##     ## ##  ##   ##     ##                   ## ##     ## ##        ##   ##  ##       ##   ##   \n"
         << "##    ## ##     ## ##     ## ##     ## ##   ##  ##     ##             ##    ## ##     ## ##         ## ##   ##       ##    ##  \n"
         << " ######   #######  ########   #######  ##    ##  #######               ######   #######  ########    ###    ######## ##     ## \n"
         << "\033[0m"; // Reset color
         cout << endl << endl << endl << endl << endl;
        cout << "Menu:\n1. Generate Random Sudoku\n2. Solve Sudoku from File\n3. Show Scoreboard\n4. Reveal Cell Value\n5. Explain Cell Validity\n6. Show Next Logical Step\n7. Show adjacency list \n8. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            solver.generateRandomSudoku(board, solution);
            cout << "Random Sudoku generated:" << endl;
            solver.printBoard(board);
            cout << "Press Enter to start the timer...";
            cin.ignore();
            cin.get(); 
            auto start = chrono::high_resolution_clock::now();
            cout << "Timer started. Enter your solution (row by row):" << endl;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    cin >> board[i][j];
                }
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            if (solver.isBoardCorrect(board, solution))
            {
                cout << "Congratulations! You solved the Sudoku correctly." << endl;
                cout << "Enter your name: ";
                cin >> name;
                solver.storeSolution(board, name, duration.count());
                tree.insert(name, duration.count());
                cout << "Solution time: " << duration.count() << " seconds. Saved successfully!" << endl;
            }
            else
            {
                cout << "The solution is incorrect. Please try again." << endl;
            }
            break;
        }
        case 2:
        {   cout << "Enter the file name : " ;
            string file;
            cin >> file;
            ifstream infile(file);
            if (!infile)
            {
                cerr << "Unable to open file sudoku.txt";
                break;
            }
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    infile >> board[i][j];
                }
            }
            infile.close();
            cout << "Sudoku loaded from file:" << endl;
            auto temp = board;
            solver.printBoard(board);
            auto start = chrono::high_resolution_clock::now();
            if (solver.solveSudoku(temp))
            {   auto end = chrono::high_resolution_clock::now();
                cout << "Sudoku solved:" << endl;
                solver.printBoard(temp);
               chrono::duration<double> duration = end - start;
                cout << "Time taken: " << duration.count() << " seconds" << endl;
            }
            else
            {
                cout << "No solution exists for the Sudoku." << endl;
            }
            break;
        }
        case 3:
        {
            solver.displayScoreboard(tree);
            break;
        }
        case 4:
        {
            int row, col;
            cout << "Enter row and column (1-9) to reveal cell value: ";
            cin >> row >> col;
            if (row >= 1 && row <= 9 && col >= 1 && col <= 9)
            {
                solver.revealCellValue(board, row - 1, col - 1);
            }
            else
            {
                cout << "Invalid row or column number." << endl;
            }
            break;
        }
        case 5:
        {
            int row, col, num;
            cout << "Enter row, column, and number to check validity: ";
            cin >> row >> col >> num;
            if (row >= 1 && row <= 9 && col >= 1 && col <= 9 && num >= 1 && num <= 9)
            {
                solver.explainCellValidity(board, row - 1, col - 1, num);
            }
            else
            {
                cout << "Invalid input. Row and column numbers should be between 1 and 9, and the number should be between 1 and 9." << endl;
            }
            break;
        }
        case 6:
        {
            solution = board;
            solver.solveSudoku(solution);
            if (solver.fillNextLogicalStep(board, solution))
            {
                cout << "Next logical step filled." << endl;
            }
            else
            {
                cout << "No logical step available." << endl;
            }
            break;
        }
        case 7:
        {
            cout << "Printing the adjacency list : " << endl;
            solver.printadj();
            break;
        }
        case 8:
        {
            cout << "Exiting program..." << endl;
            break;
        }
        default:
        {
            cout << "Invalid choice. Please enter a number between 1 and 7." << endl;
            break;
        }
        }
    } while (choice != 8);

    return 0;
}
