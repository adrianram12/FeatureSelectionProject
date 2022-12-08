#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
#include <ctime>

using namespace std;

vector<int> bestFeaturesTotal;
float bestAccuracyTotal = 0;
bool whichSearch;
int classifier1 = 0;
int classifier2 = 0;

float leaveOneOutCrossValidation(vector<vector<float> > data, vector<int> currentFeatures, int featureToAdd){

    int numberCorrectlyClassified = 0;
    float accuracy;

    float nearestNeighborDistance;
    int nearestNeighborLocation;
    int nearestNeighborLabel;

    vector<vector<float> > newData;

   if(whichSearch){

        newData.resize(data.size(), vector<float>(currentFeatures.size() + 2)); 

        for(int x = 0; x < data.size(); x++){
   
            if(currentFeatures.size() == 0){

                newData.at(x).at(0) = data.at(x).at(0);
                newData.at(x).at(newData.at(x).size() - 1) = data.at(x).at(featureToAdd);
                
            } 

            for(int y = 1; y <= currentFeatures.size(); y++){

                newData.at(x).at(0) = data.at(x).at(0);
                newData.at(x).at(newData.at(x).size() - 1) = data.at(x).at(featureToAdd); 

                newData.at(x).at(y) = data.at(x).at(currentFeatures.at(y-1));

            }
        }      
    }
    
    else if(!whichSearch){

        int k;
        newData = data;

        for (int i = 0; i < newData.size(); i++) { // rows

            for (int j = 1; j <= newData.at(i).size(); j++) { // columns

                if (j == featureToAdd) {

                    if(j == newData.at(i).size() - 1){

                        newData.at(i).resize(newData.at(i).size() - 1);
                    }
                    else{

                        k = j;

                        while ((k + 1) < newData.at(i).size()) {

                            newData.at(i).at(k) = newData.at(i).at(k + 1);
                            k++;
                        }

                       newData.at(i).resize(newData.at(i).size() - 1);
                    }
                }
            }  
        }
    }

    for(int i = 0; i < newData.size(); i++){   

        nearestNeighborDistance = numeric_limits<float>::max();
        nearestNeighborLocation = numeric_limits<int>::max();

        for(int j = 0; j < newData.size(); j++){  

            int k = 1;
            float sum = 0;

            if(j != i){

                while(k <= newData.at(i).size() - 1){  

                    sum = sum + pow((newData.at(i).at(k) - newData.at(j).at(k)), 2); 
                    
                    k++;
                }

                float distance = sqrt(sum);

                if(distance < nearestNeighborDistance){

                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = j;
                    nearestNeighborLabel = newData.at(nearestNeighborLocation).at(0);
                }
            
            }
        
        }

        if(newData.at(i).at(0) == nearestNeighborLabel){ 

            numberCorrectlyClassified = numberCorrectlyClassified + 1;
        }
    }

    accuracy = static_cast<double>(numberCorrectlyClassified) / static_cast<double>(data.size());

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

void forwardSelection(vector<vector<float> > data){ 

    vector<int> currentSetOfFeatures;
    vector<int> copyCurrentSetOfFeatures;
    float bestAccuracySoFar;
    float accuracy = 0;
    int featureToAddAtThisLevel;

    cout << "The accuracy for considering no features is " << (static_cast<float>(classifier1) / static_cast<float>(data.size()))*100 << "%" << endl;
    cout << endl;

    for(int i = 0; i < data.at(0).size() - 1; i++){ 

        featureToAddAtThisLevel = 0;
        bestAccuracySoFar = 0;


        for(int j = 0; j < data.at(0).size() - 1; j++){ 
            bool check = alreadyConsidered(currentSetOfFeatures, j + 1); 

            if(!check){

                cout << "--Considering adding feature " << j + 1 << endl;  
                accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j + 1); 
                currentSetOfFeatures.push_back(j + 1);
                cout << "   Using feature(s) { ";

                for(int s = 0; s < currentSetOfFeatures.size(); s++){

                    cout << currentSetOfFeatures.at(s) << " ";
                }
                cout << "} accuracy is " << accuracy * 100 << "%" << endl;
                
                currentSetOfFeatures.erase(remove(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), (j + 1)), currentSetOfFeatures.end());
                
                if(accuracy > bestAccuracySoFar){

                    if(accuracy > bestAccuracyTotal){

                        bestAccuracyTotal = accuracy;
                        copyCurrentSetOfFeatures = currentSetOfFeatures;
                        copyCurrentSetOfFeatures.push_back(j + 1);
                        bestFeaturesTotal = copyCurrentSetOfFeatures;
                    }

                    bestAccuracySoFar = accuracy;
                    featureToAddAtThisLevel = j + 1;
                }
            }
        }

        currentSetOfFeatures.push_back(featureToAddAtThisLevel);

        cout << endl;
        cout << "Best accuracy of this level: " << bestAccuracySoFar * 100 << "%" << endl;
        cout << "On level " << i + 1 << " I added feature " << featureToAddAtThisLevel << " to the current set" << endl; 
        cout << endl;
    }

    cout << "Finished Forward Selection!" << endl;
    cout << "The best feature subset is { ";
    for(int l = 0; l < bestFeaturesTotal.size(); l++){

            cout << bestFeaturesTotal.at(l) << " ";
    }
    cout << "}, which has an accuracy of " << bestAccuracyTotal * 100 << "%" << endl;
    cout << endl;
    
}

void backwardElimination(vector<vector<float> > data){

    vector<int> currentSetOfFeatures;

    for(int n = 0; n < data.at(0).size() - 1; n++){

        currentSetOfFeatures.push_back(n + 1);
    }

    vector<int> copyCurrentSetOfFeatures;
    float bestAccuracySoFar;
    float accuracy = 0;
    int featureToAddAtThisLevel;

    cout << "The accuracy for considering no features is " << (static_cast<float>(classifier2) / static_cast<float>(data.size()))*100 << "%" << endl;
    cout << endl;
    
    for(int i = 0; i < data.at(0).size() - 1; i++){ 
 
        featureToAddAtThisLevel = 0;
        bestAccuracySoFar = 0; 


        for(int j = 0; j < data.at(0).size() - 1; j++){ 
            bool check = alreadyConsidered(currentSetOfFeatures, j + 1); 

            if(check){

                cout << "--Considering removing the " << j + 1 << " feature" << endl;  
                accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j + 1); 
                currentSetOfFeatures.erase(remove(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), (j + 1)), currentSetOfFeatures.end());
                cout << "   Using feature(s) { ";

                for(int s = 0; s < currentSetOfFeatures.size(); s++){

                    cout << currentSetOfFeatures.at(s) << " ";
                }
                cout << "} accuracy is " << accuracy * 100 << "%" << endl;
                currentSetOfFeatures.push_back(j + 1);


                if(accuracy > bestAccuracySoFar){

                    if(accuracy > bestAccuracyTotal){

                        bestAccuracyTotal = accuracy;
                        copyCurrentSetOfFeatures = currentSetOfFeatures;
                        copyCurrentSetOfFeatures.erase(remove(copyCurrentSetOfFeatures.begin(), copyCurrentSetOfFeatures.end(), (j + 1)), copyCurrentSetOfFeatures.end());
                        bestFeaturesTotal = copyCurrentSetOfFeatures;
                    }

                    bestAccuracySoFar = accuracy;
                    featureToAddAtThisLevel = j + 1;
                }
            }
        }

        currentSetOfFeatures.erase(remove(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), featureToAddAtThisLevel), currentSetOfFeatures.end());
        
        cout << endl;
        cout << "Best accuracy of this level: " << bestAccuracySoFar * 100 << "%" << endl;
        cout << "On level " << i + 1 << " I removed feature " << featureToAddAtThisLevel << " from the current set" << endl; 
        cout << endl;
    }

    cout << "Finished Backward Elimination!" << endl;
    cout << "The best feature subset is { ";
    for(int l = 0; l < bestFeaturesTotal.size(); l++){

            cout << bestFeaturesTotal.at(l) << " ";
    }
    cout << "}, which has an accuracy of " << bestAccuracyTotal * 100 << "%" << endl;
    cout << endl;

}

int main(){

    fstream newFile;
    string inputFile;
    string rows;
    float theNumbers;
    int theAlgorithm;
    vector<vector<float> > dataTable;

    cout << "Welcome to my Feature Selection Algorithm." << endl;
    cout << endl;

    cout << "Please type in the name of the file you would like to test." << endl;

    cin >> inputFile;
    cout << endl;

    cout << "Please type the number of the algorithm you want to run." << endl;
    cout << "Type '1' for Forward Selection." << endl;
    cout << "Type '2' for Backward Elimination." << endl;

    cin >> theAlgorithm;

    clock_t startTime = clock();

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

    for(unsigned int i = 0; i < dataTable.size(); i++){

        if(dataTable.at(i).at(0) == 1){

            classifier1++;
        }
        else{
            classifier2++;
        }
    }

    cout << endl;
    cout << "This data set has " << dataTable.at(0).size() - 1 << " features (not including the class attribute), with " << dataTable.size() << " instances." << endl;
    cout << endl;

    if(theAlgorithm == 1){

        whichSearch = true;
        cout << "Now beginning Forward Selection." << endl;
        cout << endl;
        forwardSelection(dataTable);
        double time = ((clock() - startTime) / (double)CLOCKS_PER_SEC);
        cout << fixed << setprecision(1) << "It took " << time << " seconds to compute." << endl;
        
    }
    else if(theAlgorithm == 2){

        whichSearch = false;
        cout << "Now beginning Backward Elimination." << endl;
        backwardElimination(dataTable);
        double time = ((clock() - startTime) / (double)CLOCKS_PER_SEC);
        cout << fixed << setprecision(1) << "It took " << time << " seconds to compute." << endl;
       
    }
   
    return 0;
}