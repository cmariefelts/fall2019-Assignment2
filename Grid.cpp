#include "Grid.h"
#include <iostream>
#include <algorithm> //Used when shuffling the grid string
#include <fstream> //Used for input/output files

using namespace std;

//Constructor
Grid::Grid(int row, int column, float d, int gm)
{
  rows = row;
  columns = column;
  density = d;
  theGrid = new char*[rows];
  gameMode = gm;

  //Initialize makeGrid() and createString()
  makeGrid();
  string gridString = createString();
  initializeGrid(gridString);




}
//Overloaded Constructor
Grid::Grid(int row, int column, int gm, string lg)
{
  rows = row;
  columns = column;
  gameMode = gm;
  makeGrid();
  initializeGrid(lg);
  numPopulation(lg);
}

//Text File constructor
Grid::Grid(string userFileName, int gameMode)
{
  this->gameMode = gameMode;
  string fileStr = readFile(userFileName);
  makeGrid();
  initializeGrid(fileStr);
  numPopulation(fileStr);
}

//Deconstructor
Grid::~Grid()
{
  for (int i = 0; i < rows; ++i)
  {
    delete[] theGrid[i];
  }
  delete[] theGrid;
}

//Making the game board array
void Grid::makeGrid()
{
  theGrid = new char*[rows];
  for (int i = 0; i < rows; ++i)
  {
    theGrid[i] = new char[columns];
    for (int x = 0; x < columns; ++x)
    {
      theGrid[i][x] = '-';
    }
  }
}

//Method to put values of grid into a string
string Grid::getString()
{
  //Iterate through the rows
  string gridOutput = "";

  for (int i = 0; i < rows; ++i)
  {
    for (int x = 0; x < columns; ++x)
    {
      gridOutput += theGrid[i][x];
    }
    gridOutput += "\n";
  }
  return gridOutput;
}


//Make a string method for the grid
string Grid::createString()
{
  //Returns a string
  int totalCells = rows * columns;
  population = density * totalCells;
  population = (int)population;
  int otherCells = totalCells - population;

  //Going through population and otherCells to put in Xs and -s
  for (int i = 0; i < population; ++i)
  {
    popString += 'X';
  }
  for (int i = 0; i < otherCells; ++i)
  {
    popString += "-";
  }
  //Shuffling the string in order to initialize the grid
  random_shuffle(popString.begin(), popString.end());
  return popString;
}

void Grid::initializeGrid(string popString)
{
  //Initializes the grid
  for (int i = 0; i < rows; ++i)
  {

    for (int x = 0; x < columns; ++x)
    {
      theGrid[i][x] = popString[0];
      popString.erase(0,1);
    }
  }
}
//Get the next generation's grid
string Grid::getNextGen()
{
  string gridString = "";
  for (int i = 0; i < rows; ++i)
  {
    for (int x = 0; x < columns; ++x)
    {
      //If the cell is an 'X' already and has 2 or 3 neighbors,
        //make it an 'X'. Otherwise, make it a dash
      if (theGrid[i][x] == 'X' && numNeighbors(i,x) == 2)
      {
        gridString += "X";
      }
      else if (theGrid[i][x] == 'X' && numNeighbors(i,x) == 3)
      {
        gridString += "X";
      }
      else if (theGrid[i][x] == '-' && numNeighbors(i,x) == 3)
      {
        gridString+= "X";
      }
      else
      {
        gridString += "-";
      }
    }
    }
  return gridString;
  }

//Count the neighbors of each cell
int Grid::numNeighbors(int coordX, int coordY)
{
  int neighbors = 0;
  //Counting total neighbors
  if (isNeighbor(coordX+1,coordY, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX-1,coordY, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX,coordY+1, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX,coordY-1, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX-1,coordY-1, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX-1,coordY+1, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX+1,coordY-1, coordX, coordY))
    neighbors++;
  if (isNeighbor(coordX+1,coordY+1, coordX, coordY))
    neighbors++;
  return neighbors;
}
//Checking the x and y coordinates based on the modes
bool Grid::isNeighbor(int coordX, int coordY, int origX, int origY)
{
  //CLASSIC MODE
  if (gameMode == 1)
  {
    //Check if neighbors less than 0 (above) out of bounds
    if (coordX < 0)
      return false;
    //Check if neighbors less than 0 (to the left) are out of bounds
    if (coordY < 0)
      return false;
    //Check if neighbors below our rows are out of bounds
    if (coordX >= rows)
      return false;
    //Check if neighbors to the right of columns are out of bounds
    if (coordY >= columns)
      return false;
    //Check if the neighbor is a dash or X
    if (theGrid[coordX][coordY] == '-')
      return false;
    if (theGrid[coordX][coordY] == 'X')
      return true;
  }

  //DOUGHNUT MODE
  else if (gameMode == 2)
  {
    if (isOnBorder(origX, origY))
    {
      if (coordX < 0)
        coordX = (rows - 1);
      else if (coordX >= rows)
        coordX = 0;

      if (coordY < 0)
        coordY = (columns - 1);
      else if (coordY >= columns)
        coordY = 0;
    }
    if(theGrid[coordX][coordY] == 'X')
      return true;
    else
      return false;
  }

  //MIRROR MODE
  else if (gameMode == 3)
  {
    if (isOnBorder(origX, origY))
    {
      if (coordX < 0)
        coordX = 0;
      else if (coordX >= rows)
        coordX = (rows - 1);

      if (coordY < 0)
        coordY = 0;
      else if (coordY >= columns)
        coordY = (columns - 1);
    }
    if (theGrid[coordX][coordY] == 'X')
      return true;
    else
      return false;
  }
}

//Method to count the population
void Grid::numPopulation(string inString)
{
  countPop = 0;

  for (int i = 0; i < inString.length(); ++i)
  {
    if (inString[i] == 'X')
    {
      countPop++;
    }
  }
}

//Method to check if the coordinates are on the edge of the grid
bool Grid::isOnBorder(int coordX, int coordY)
{
  if (coordX == 0)
    return true;
  else if (coordX == rows - 1)
    return true;

  if (coordY == 0)
    return true;
  else if (coordY == (columns - 1))
    return true;

  return false;
}

//Takes the user's text file and get the string from it
string Grid::readFile(string userFile)
{
  ifstream inputFile;
  inputFile.open(userFile);

  if (!inputFile)
  {
    throw runtime_error("Cannot open file: " + userFile);
  }

  //Counting how many lines are in the file
  string line;
  int countLine = 0;
  while (getline(inputFile, line))
  {
    countLine++;
  }

  //Goes back to beginning of the file
  inputFile.clear();
  inputFile.seekg(0);
  string* fileLines = new string[countLine];

  //Create an array that holds all the lines from the file
  int counter = 0;
  while (getline(inputFile, line))
  {
    fileLines[counter] = line;
    counter++;
  }
  inputFile.clear();
  inputFile.seekg(0);

  //Have to close the file after
  inputFile.close();

  //Converting the string to an integer
  this->rows = stoi((string)fileLines[0]);
  this->columns = stoi((string)fileLines[1]);

  //Checks if the userFile is the correct layout we can use
  if (this->rows != countLine - 2 || correctColumns(fileLines, countLine, columns)==false)
  {
    throw runtime_error("The file you gave isn't the right layout!");
  }
  string gridString = "";
  for (int i = 2; i < countLine; ++i)
  {
    gridString += fileLines[i];
  }
  return gridString;

}

//Method to write to the output file
void Grid::writeFile(string userFile, string writeString)
{
  ofstream outputFile;

  outputFile.open(userFile, ofstream::app);

  if (!outputFile)
    throw runtime_error("Cannot open output file.");

  outputFile << writeString << endl;

  outputFile.close();

}

//Method to check if the grid input matches the dimensions given by user
bool Grid::correctColumns(string* &fileLines, int countLine, int columns)
{
  for (int i = 2; i < countLine; ++i)
  {
    if (fileLines[i].length() != columns)
      return false;
  }

  return true;
}
