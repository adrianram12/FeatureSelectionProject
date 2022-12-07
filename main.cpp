#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

// float leaveOneOutCrossValidation(vector<vector<float> > data, vector<int> currentFeatures, int featureToAdd){

//     float accuracy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

//     return accuracy;
// }

float calculateDistance(vector<vector<float> > data){

    float sum = 0;
    
    int j = 1;

    for(int i = 0; i < data.size(); i++){

        while(j <= data.at(i).size()){

            float sum = sum + pow((data.at(i).at(j) - data.at(i+1).at(j)), 2);
        }
    }

    return sum;

}

float leaveOneOutCrossValidation(vector<vector<float> > data, vector<int> currentFeatures, int featureToAdd){

    int numberCorrectlyClassified = 0;
    float accuracy;

    float nearestNeighborDistance;
    int nearestNeighborLocation;
    int nearestNeighborLabel;

    for(int i = 0; i < data.size(); i++){

        //cout << "data.size() " << data.size() << endl;
        //cout << "data.at(i).size() " << data.at(i).size() << endl;

       // vector<vector<float> > objectsToClassify(&data[1], &data[data.at(i).size() - 1]);
        //vector<vector<float> > labelObjectToClassify(&data[0], &data[1]);
        //float sum = 0;
        //int k = 1;

        //cout << "Looping over i, at the " << i + 1 << " location" << endl;
        //cout << "The " << i + 1 << "th object is in class " << data.at(i).at(0) << endl;
        //cout << endl;

        nearestNeighborDistance = numeric_limits<float>::max();
        nearestNeighborLocation = numeric_limits<int>::max();

        for(int j = 0; j < data.size(); j++){

            
            int k = 1;
            float sum = 0;

            if(j != i){

                //cout << "Ask if " << i + 1 << " is nearest neighbor with " << j + 1 << endl;

                while(k <= data.at(i).size() - 1){

                    //cout << "k " << k << endl;
                   //cout << "X1 " << data.at(i).at(k) << " X2 " << data.at(j).at(k) << endl;
                  // cout << "i " << i << " j " << j << endl;
                    sum = sum + pow((data.at(i).at(k) - data.at(j).at(k)), 2);
                    //cout << "SUMM!!!!!!!!!!! " << sum << endl;
                    k++;

                }

                //k = 1;

                float distance = sqrt(sum);
                //cout << "DISTANCE!!!!!!!!!!!!!!! " << distance << endl;
               // cout << "NEARESTNEIGHBORDISTANCE!!!!!!! " << nearestNeighborDistance << endl;

                if(distance < nearestNeighborDistance){

                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = j;
                    //cout << "nearestNeighborlocation " << nearestNeighborLocation << endl;
                    nearestNeighborLabel = data.at(nearestNeighborLocation).at(0);
                }
            
            }
        
        }

        //cout << "Object " << i + 1 << " is class " << data.at(i).at(0) << endl;
        //cout << "Its nearest neighbor is " << nearestNeighborLocation << " which is in class " << nearestNeighborLabel << endl;

        if(data.at(i).at(0) == nearestNeighborLabel){

            numberCorrectlyClassified = numberCorrectlyClassified + 1;
        }
    }

    accuracy = static_cast<double>(numberCorrectlyClassified) / static_cast<double>(data.size());

   cout << "ACCURACY!!!!!!!! " << accuracy << endl;

    return accuracy;
}


bool alreadyConsidered(vector<int> &currentSetOfFeatures, int featureInQuestion){

    for(unsigned int i = 0; i < currentSetOfFeatures.size(); i++){

        if(currentSetOfFeatures.at(i) == featureInQuestion){

            return true;
        }
    }

    return false;
}

void featureSearch(vector<vector<float> > data){

    vector<int> currentSetOfFeatures;
    float bestAccuracySoFar;
    float accuracy;
    int featureToAddAtThisLevel;

    for(int i = 0; i < data.at(0).size() - 1; i++){

       cout << "On the " << i + 1 << "th level of the search tree" << endl;

        featureToAddAtThisLevel = 0;
        bestAccuracySoFar = 0;


        for(int j = 0; j < data.at(0).size() - 1; j++){

            bool check = alreadyConsidered(currentSetOfFeatures, j + 1);

            if(!check){

                cout << "--Considering adding the " << j + 1 << " feature" << endl;
                //accuracy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j + 1);
                //cout << "ACCURACY!!!!!!!!!! " << accuracy << endl;
                //cout << "BESTACCURACY!!!!!! " << bestAccuracySoFar << endl;
            }

            if(accuracy > bestAccuracySoFar){

                bestAccuracySoFar = accuracy;
                featureToAddAtThisLevel = j + 1;
            }
        }

        currentSetOfFeatures.push_back(featureToAddAtThisLevel);

        for(int l = 0; l < currentSetOfFeatures.size(); l++){

            cout << currentSetOfFeatures.at(l) << " ";
        }
        cout << "Accuracy of this level: " << bestAccuracySoFar << endl;
        cout << "On level " << i + 1 << " I added feature " << featureToAddAtThisLevel << " to the current set" << endl;
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

    //demoAccuracy(dataTable);
    return 0;
}