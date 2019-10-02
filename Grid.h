#include <iostream>
#include <fstream>

using namespace std;

class Grid
{
  public:
  //Both the constructor and overloaded constructor
  Grid(int rows, int columns, float density, int gameMode);

  Grid(int rows, int columns, int gameMode, string lastGen);

  //Changing user's input file name into a text file to access
  Grid(string userFileName, int gameMode);

  //Desconstructor
  ~Grid();


  //Methods declarations:
  void makeGrid();
  string getString();
  void numPopulation(string inString);
  int numNeighbors(int coordX, int coordY);
  string createString();
  void initializeGrid(string popString);
  string getNextGen();
  bool isNeighbor(int coordX, int coordY, int origX, int origY);
  bool isOnBorder(int coordX, int coordY);
  void writeFile(string userFile, string writeString);
  bool correctColumns(string* &fileLines, int countLine, int columns);
  string readFile(string userFile);


  //Variable Declarations
  int rows;
  int columns;
  float density;
  string gridString;
  string popString;
  int population;
  int countPop;
  int gameMode;
  string userAnswer;
  ifstream userFile;


  char** theGrid;

};
