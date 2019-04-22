// Provide output capability.
#include <iostream>
#include <list>

#define WINDOWSIZE 5.0

using std::cout;
using std::endl;
using std::list;

// find average while there's at least 5 distances in list.
void findAvg(list<double>& distances) 
{
   while (distances.size() >= WINDOWSIZE) 
   {
      double sum = 0.0;
      cout << "Last 5 distances: ";
      for (int i = 0; i < WINDOWSIZE; ++i) 
      {
         double num = distances.front();
         distances.pop_front();
         sum += num;
         cout << num << " ";
      }
      cout << endl << "Moving Average: " << sum / (double) WINDOWSIZE << endl << endl;
   }
}

// simple test
int main() {
  list<double> distances;
  distances.push_back(0.1);
  distances.push_back(0.2);
  distances.push_back(0.3);
  distances.push_back(0.4);
  distances.push_back(0.5);
  distances.push_back(0.6);
  
  findAvg(distances);

  distances.push_back(0.7);
  distances.push_back(0.8);
  distances.push_back(0.9);
  distances.push_back(0.1);
  distances.push_back(0.2);
  distances.push_back(0.2);
  distances.push_back(0.4);
  distances.push_back(0.5);
  distances.push_back(0.6);

  findAvg(distances);
}


/*
int main(int argc, char const *argv[])
{
   const int ARRAY_SIZE = 10;
   double distance[ARRAY_SIZE] = {1.0, 2.0, 3.0, 4.0, 5.0,
                                   6.0, 7.0, 8.0, 9.0, 10.0};
   int windowSize = 5;
   double sum = 0.0;
   double average = 0.0;

   for (int i = 0; i <= (ARRAY_SIZE - windowSize); i++) {

      sum = 0.0;
      cout << "Last 5 distances: ";

      for (int j = i; j < i + windowSize; j++) {
         sum += distance[j];
         cout << distance[j] << " ";
      }

      average = sum / windowSize;
      cout << endl << "Moving Average: " << average << endl << endl;

   }
   return 0;
}
*/
