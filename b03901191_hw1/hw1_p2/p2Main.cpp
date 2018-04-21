#include <iostream>
#include <string>
#include <iomanip>
#include "p2Table.h"

using namespace std;

int main()
{
   Table table;
   // TODO: read in the csv file
   ///////////////////////////ifstream file("tset_hw1.csv");//me path??
   string csvFile;
   //getline(table,csvFile,',','^M');//???
   cout << "Please enter the file name: ";
   cin >> csvFile;
   if (table.read(csvFile))
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
   else exit(-1); // csvFile does not exist.

   table.print();     
   cout << "Please enter the commands: " << endl;

   // TODO read and execute commands
   while (true) {//me
      //table.print();

   //cin command
      string commands;
      int x;    
      //string ain;//?
   
      cin >> commands;//????C = dijige col: sum avg max min
      if(commands == "SUM"){ 
         cin >> x; 
         //table.sum(x);
         cout << "the summantion of data in columnn #" << x << " is " << table.sum(x) << "." << endl;}//table.???
      else if (commands == "MAX"){ 
         cin >> x;
         //table.max(x);
         cout << "the maximum of data in columnn #" << x << " is " << table.max(x) <<"." << endl;}
         
      else if(commands == "MIN"){
         cin >> x;
         //table.min(x);
         cout << "the minimum of data in columnn #" << x << " is " << table.min(x) << "." << endl;}
      else if(commands == "COUNT"){
         cin >> x;
         //table.cout(x);
         cout << "the distinct count of data in columnn #" << x << " is " << table.count(x) << "." << endl;}
         
      else if(commands == "AVE"){
         cin >> x; 
         //table.avg(x);
         cout << "the summantion of data in columnn #" << x << " is " << fixed << setprecision(1) << table.avg(x) << "." << endl;}
         //table.add(x);
      else if(commands == "ADD") table.add();
      else if(commands == "PRINT") table.print();
         //EXIT
      else if(commands == "EXIT") exit(-1);

   }

//table.clear();//me put where??
   return 0;//me??
}
