/*
 * File Name: main.cpp
 * Program Name: Down Sampler
 * Name:  Andy Nguyen
 * NetID: adn200004
 * Date: 8/31/2023
 */

// typical c++ libraries used to implement program
#include <iostream> // used to take in inputs and outputs with cin and cout
#include <fstream>  // used to read in inputs from file
#include <string>   // used to store long array of characters
#include <vector> // used to help store values from CSV
#include <unordered_set> // essentially a hashset

using namespace std;

// struct used to help represent 3D point and help extract coordinates
struct Point {

  float x, y, z;

}; 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

vector<Point> parseCSV(const string& fileName) {

  vector<Point> inputPointCloud;

  ifstream inputCSV(fileName);
  string fileLine;

  getline(inputCSV, fileLine); // skips the first line
  while (getline(inputCSV, fileLine)) { // loop through each line of CSV

    Point cPoint;

    // parse x, y, and z and store into coordinate struct
    int removalIndex = fileLine.find(',');
    cPoint.x = stof(fileLine.substr(0, removalIndex));
    fileLine =  fileLine.substr(removalIndex + 1);

    removalIndex = fileLine.find(',');
    cPoint.y = stof(fileLine.substr(0, removalIndex));
    fileLine =  fileLine.substr(removalIndex + 1);

    cPoint.z = stof(fileLine);

    // push the coordinate struct into the vector
    inputPointCloud.push_back(cPoint);

    if (inputCSV.eof()) { break; } // reached end of file

  }

  inputCSV.close();

  return inputPointCloud;

} // parseCSV

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

vector<Point> uniformDownSampler(const vector<Point>& inputPointCloud, float voxelSize) {

  vector<Point> dsPointCloud;
  unordered_set<int> voxelSet;

  for (const Point& cPoint : inputPointCloud) {

    int voxelX = static_cast<int>(cPoint.x / voxelSize);
    int voxelY = static_cast<int>(cPoint.y / voxelSize);
    int voxelZ = static_cast<int>(cPoint.z / voxelSize);

    int voxelKey = voxelX + voxelY * 1000  + voxelZ * 10000; // attempts to generate unique key from point values using scaling factor

    if (voxelSet.find(voxelKey) == voxelSet.end()) { // checks set if key is unique
      
      // if so, insert into set and add point to new pointcloud
      voxelSet.insert(voxelKey);
      dsPointCloud.push_back(cPoint);

    }

  }

  return dsPointCloud;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

void outputToCSV(const vector<Point>& dsPointCloud, string fileName) {

  ofstream outputCSV(fileName);

  if (!outputCSV.is_open()) { // checks if the file name provided can be opened
        
    cerr << "Error: Unable to open the CSV file." << std::endl;
    return;

  }

  outputCSV <<  "x,y,z" << endl;
  for (const Point& cPoint : dsPointCloud) { // loop through pointcloud and output points to CSV file
        
    outputCSV << cPoint.x << "," << cPoint.y << "," << cPoint.z << endl;
  
  }

  outputCSV.close();

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

int main() {
  
  // file name that the program will read to create the report
  const string CSV_READ_FILE = "input.csv";
  const string CSV_OUTPUT_FILE = "output.csv";
  const float VOXEL_SIZE = .050f;

  // extract and parse CSV
  vector<Point> inputPointCloud = parseCSV(CSV_READ_FILE);

  // downsampled
  vector<Point> dsPointCloud = uniformDownSampler(inputPointCloud, VOXEL_SIZE);

  // write to output CSV
  outputToCSV(dsPointCloud, CSV_OUTPUT_FILE);
  cout << "Downsampled pointcloud has been written to output CSV!\n" << endl;

  cout << "# of points in orginal pointcloud: " << inputPointCloud.size() << endl;
  cout << "# of points in downsampled pointcloud: " << dsPointCloud.size() << endl;

  return -1;

} // main

