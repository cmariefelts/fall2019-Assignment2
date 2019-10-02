#include "Grid.h"
#include <iostream>
#include <thread>
#include <chrono>

//For the sleep function, used a resource (in README.txt)
using namespace std::this_thread;
using namespace std::chrono;

using namespace std;
//Call keepRunning method
bool keepRunning(Grid*& currentGen, Grid*& nextGen);
//Counts how many generations have the same population
int genStabilized = 0;

//See if it has stabilized/keep last 3 generations
string prevGenStrings[3];

int main()
{

srand(time(NULL));

//User input
int userPause;
string userAnswer;
string userFileName;
string outputFileName;
int width;
int height;
float density;
Grid* currentGen;
int gameMode;

//Printing to the user's interface
cout << "Welcome to The Game Of Life" << endl;
cout << "What game mode would you like to run in? 1 = Classic Mode, 2 = Doughnut Mode, 3 = Mirror Mode." << endl;
cin >> gameMode;
//Error checking if gameMode is a correct input
if (gameMode > 3 || gameMode < 1)
{
  cout << "You're wrong. Goodbye." << endl;
  return 1;
}
cout << "Would you like to: \n 1. Have a short time delay \n 2. Press enter to get each next generation \n 3. Write everything to an output file" << endl;
cin >> userPause;
if (userPause == 3)
{
    cout << "Enter output file name" << endl;
    cin >> outputFileName;
}
//Error checking if userPause is a correct input
if (userPause > 3 || userPause < 1)
{
  cout << "You're wrong. Goodbye." << endl;
  return 1;
}
//Ask if the user wants to provide a map file of the world
cout << "Would you like to provide a map file of the world? (yes/no)" << endl;
cin >> userAnswer;
if (userAnswer == "yes")
{
  cout << "Enter text file name: " << endl;
  cin >> userFileName;
  //Error checking if userFile is a valid input file
  try
  {
    currentGen = new Grid(userFileName, gameMode);
  }
  catch (runtime_error e)
  {
    cout << "There's a problem with your file. \n" << e.what() << endl;
    return 1;
  }
}
else if (userAnswer == "no")
{
  //Ask user for the width of their world
  cout << "What is the width of your world?" << endl;
  cin >> width;
  if (width < 1)
  {
    cout << "You're wrong. Goodbye." << endl;
    return 1;
  }
  //Ask user for the height of their world
  cout << "What is the height of your world?" << endl;
  cin >> height;
  if (height < 1)
  {
    cout << "You're wrong. Goodbye." << endl;
    return 1;
  }
  //Ask user for decimal value for population density
  cout << "What is the decimal value > than 0 and <= 1 for the initial population density." << endl;
  cin >> density;
  //Error checking if density is a valid value
  if (density > 1 || density < 0)
  {
    cout << "You're wrong. Goodbye." << endl;
    return 1;
  }
  currentGen = new Grid(height, width, density, gameMode);
}


  //Print statements for each generation
  int genCounter = 0;
  string currentGenOutString = currentGen->getString();
  cout << "First Generation: " << "\n" << currentGenOutString << endl;
  if (userPause == 3)
  {
    string outputStr = "Generation Count: " + to_string(genCounter) + "\n";
    currentGen->writeFile(outputFileName, outputStr);
    currentGen->writeFile(outputFileName, currentGenOutString);
  }

  genCounter++;

  bool stillRunning = true;

  cin.get();

  //Keep doing this while the program is still running
  while (stillRunning)
  {
    string nextGenStr = currentGen->getNextGen();

    Grid* nextGen = new Grid(currentGen->rows, currentGen->columns, gameMode, nextGenStr);

    string outString = nextGen->getString();
    cout << "Next Generation Population: " << nextGen->countPop << endl;
    cout << "Generation: " << genCounter << "\n" << outString << endl;


    //Putting last 3 generation strings into array
    prevGenStrings[genCounter % 3] = currentGen->getString();
    prevGenStrings[(genCounter + 1) % 3] = nextGen->getString();

    //sleep_for() function
    if(userPause == 1)
    {
      sleep_for(seconds(2));
    }
    //Otherwise, the user will press enter for each generation they want to see
    else if (userPause == 2)
    {
      cout << "Press enter for next generation!" << endl;
      cin.get();
    }
    else
    {
    //ELSE: They output their grid into an output file they give
      string outputStr = "Generation Count: " + to_string(genCounter) + "\n";
      currentGen->writeFile(outputFileName, outputStr);
      currentGen->writeFile(outputFileName, outString);
    }


    stillRunning = keepRunning(currentGen, nextGen);

    cout << "Still Running: " << stillRunning << endl;
    currentGen->~Grid();
    currentGen = nextGen;

    genCounter++;

  }
}

//Checks if program is stabilized
bool keepRunning(Grid*& currentGen, Grid*& nextGen)
{
  //Checks if the population is 0, then if the generations are the same
  if (nextGen->countPop == 0)
    return false;

  string currentGenString = currentGen->getString();
  string nextGenStr = nextGen->getString();
  bool equalGrids = true;
  for (int i = 0; i < currentGenString.length(); ++i)
  {
    if (currentGenString[i] != nextGenStr[i])
    {
      equalGrids = false;
      break;
    }
  }
  if (equalGrids == true)
  {
    cout << "Grids are Equal.\n";
    return false;
  }

  if (currentGen-> countPop == nextGen-> countPop)
  {
    genStabilized++;
    if (genStabilized > 4)
      return false;
  }
  else
    genStabilized = 0;

  //Iterating through the previous generation strings to see if we have equal generations
  //If they are equal generations, the program is stabilized.
  for (int i = 0; i < 3; ++i)
  {
    for (int x = 0; x < 3; ++x)
    {
      //Make sure we don't compare same exact generation
      if (i == x)
        continue;

      //If it finds 2 generation strings that are the same, then we have repeating simulation
      //& it has stabilized
      if (prevGenStrings[i] == prevGenStrings[x])
      {
        return false;
      }
    }
  }

  return true;
}
