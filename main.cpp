#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

int main(){

    fstream newFile;
    string inputFile;
    string rows;
    float theNumbers;
    vector<vector<float> > dataTable;

    cout << "Please type in the name of the file you would like to test." << endl;

    cin >> inputFile;

    newFile.open(inputFile);

    if(!newFile.is_open()){

        cout << "Error opening file." << endl;
    }

    while(getline(newFile, rows)){

        dataTable.push_back(vector<float>());
        istringstream ss(rows);

        while(ss >> theNumbers){

            dataTable.back().push_back(theNumbers);
        }

    }

    newFile.close();

    // for(unsigned int i = 0; i < dataTable.size(); i++){

    //     for(unsigned int j = 0; j < dataTable.at(i).size(); j++){

    //         cout << dataTable.at(i).at(j) << " ";
    //     }

    //     cout << endl;
    // }

    cout << "This data set has " << dataTable.at(0).size() - 1 << " features (not including the class attribute), with " << dataTable.size() << " instances." << endl;

    return 0;
}