#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

void featureSearch(vector<vector<float> > data){

    vector<int> currentSetOfFeatures;
    float bestAccuracySoFar;
    float accuracy;

    for(int i = 0; i < data.size(); i++){

        cout << "On the " << i + 1 << "th level of the search tree" << endl;

        vector<int> featureToAddAtThisLevel;
        bestAccuracySoFar = 0;


        for(int j = 0; j < data.at(i).size() - 1; j++){

            cout << "--Considering adding the " << j + 1 << " feature" << endl;
            accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j + 1);
        }
    }
}

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

    //cout << fixed << setprecision(7) << dataTable.at(0).at(1) << " times " << dataTable.at(0).at(2) << " equals " << dataTable.at(0).at(1) * dataTable.at(0).at(2) << endl; 

    cout << "This data set has " << dataTable.at(0).size() - 1 << " features (not including the class attribute), with " << dataTable.size() << " instances." << endl;

    featureSearch(dataTable);
    return 0;
}