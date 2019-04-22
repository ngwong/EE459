// Provide output capability.
#include <iostream>

using std::cout;
using std::endl;

int main() {

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
