#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
// Structure to represent a single data point/sample in the dataset
struct DataPoint {
    vector<double> features; // Stores numerical feature values (e.g., sepal length, width)
    int label;                   // Stores the target class label (encoded as an integer)
};

// Function to load a CSV dataset into a vector of DataPoint structures
vector<DataPoint> loadCSV(const string& filename) {
    vector<DataPoint> dataset;
    ifstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open the file " << filename << endl;
        return dataset;
    }

    string line;

    // Optional: Read and discard the header line if your CSV contains column names
    // Remove or comment out the line below if your CSV has no header line
    getline(file, line); 

    // Read file line by line
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string value;
        DataPoint point;

        // Vector to temporarily store all values on the current line
        vector<double> rowValues;

        // Parse comma-separated tokens in the line
        while (getline(ss, value, ',')) {
            try {
                rowValues.push_back(stod(value)); // Convert string token to double
            } catch (const invalid_argument& e) {
                cout << "Warning: Could not convert value '" << value << "' to double. Skipping value." << endl;
            }
        }

        // Standard assumption: The last column is the class label/target, 
        // while all preceding columns are numerical features.
        if (!rowValues.empty()) {
            point.label = static_cast<int>(rowValues.back()); // Extract target label
            rowValues.pop_back();                             // Remove label from feature list
            point.features = rowValues;                      // Assign remaining values as features

            dataset.push_back(point);
        }
    }

    file.close();
    return dataset;
}

int main() {
    string filename = "dataset.csv"; // Replace with your actual file path
    
    cout << "Loading dataset from " << filename << "..." << endl;
    vector<DataPoint> dataset = loadCSV(filename);

    if (dataset.empty()) {
        cout << "No data loaded. Please verify file path and format." << endl;
        return 1;
    }

    // Display summary and preview first few rows
    cout << "\nSuccessfully loaded " << dataset.size() << " samples." << endl;
    cout << "Features per sample: " << dataset[0].features.size() << "\n" << endl;

    cout << "--- First 3 Loaded Samples ---" << endl;
    size_t previewCount = min<size_t>(3, dataset.size());
    for (size_t i = 0; i < previewCount; ++i) {
        cout << "Sample " << i + 1 << " Features: [ ";
        for (double f : dataset[i].features) {
            cout << f << " ";
        }
        cout << "] | Label: " << dataset[i].label << endl;
    }

    return 0;
}