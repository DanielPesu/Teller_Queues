#include "simulation.h"
#include <iostream>
using namespace std;

int main()
{
  char file_name[255];

  cout << "Enter the file name: ";
  cin.getline(file_name, 255);

  Simulation sim1(SINGLE_QUEUE), sim2(INDEPENDENT_QUEUES);

  if (sim1.Initialise(file_name) && sim2.Initialise(file_name))
  {
    cout << "Initialisation Successful!" << std::endl;
  sim1.Run();
  sim1.Analyse(cout);

  sim2.Run();
  sim2.Analyse(cout);
  }
  else
    cout << "Unable to open \'" << file_name << "\'." << std::endl;



  return 0;
}
