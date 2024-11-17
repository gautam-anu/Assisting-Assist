#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // For std::find

using namespace std;

// Function to read file data into a vector of pairs
vector<pair<string, string>> readFile(const string &fileName) {
    ifstream inputFile(fileName);
    vector<pair<string, string>> data;

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return data;
    }

    string line;
    bool isArticulate = true; // Toggle between articulate and class
    string articulate, courseClass;
    while (getline(inputFile, line)) {
        if (isArticulate) {
            articulate = line;
        } else {
            courseClass = line;
            data.emplace_back(articulate, courseClass);
        }
        isArticulate = !isArticulate; // Switch between articulate and class
    }

    inputFile.close();
    return data;
}

// Function to compare data between two colleges
vector<pair<string, pair<string, string>>> compareColleges(
    const vector<pair<string, string>> &data1,
    const vector<pair<string, string>> &data2) {

    vector<pair<string, pair<string, string>>> comparison;

    // Collect all articulation keys from both datasets
    vector<string> allKeys;
    for (const auto &entry : data1) {
        allKeys.push_back(entry.first);
    }
    for (const auto &entry : data2) {
        if (find(allKeys.begin(), allKeys.end(), entry.first) == allKeys.end()) {
            allKeys.push_back(entry.first);
        }
    }

    // Create a comparison vector
    for (const auto &key : allKeys) {
        string articulation1 = "No articulation in selected College 1";
        string articulation2 = "No articulation in selected College 2";

        for (const auto &entry : data1) {
            if (entry.first == key) {
                articulation1 = entry.second;
                break;
            }
        }
        for (const auto &entry : data2) {
            if (entry.first == key) {
                articulation2 = entry.second;
                break;
            }
        }

        comparison.push_back({key, {articulation1, articulation2}});
    }

    return comparison;
}

int main() {
    // Define universities, majors, and associated file names
    map<string, map<string, map<string, string>>> universities = {
        {"UCLA", {
            {"Biology", {
                {"City College", "biology_classes1.txt"},
                {"East LA College", "biology_classes2.txt"},
                {"Harbor College", "biology_classes3.txt"},
                {"Mission College", "biology_classes4.txt"},
                {"Pierce College", "biology_classes5.txt"},
                {"Southwest College", "biology_classes6.txt"},
                {"Trade Technical College", "biology_classes7.txt"},
                {"Valley College", "biology_classes8.txt"},
                {"West LA College", "biology_classes9.txt"}
            }},
            {"Computer Science", {
                {"City College", "articulate_classes1.txt"},
                {"East LA College", "articulate_classes2.txt"},
                {"Harbor College", "articulate_classes3.txt"},
                {"Mission College", "articulate_classes4.txt"},
                {"Pierce College", "articulate_classes5.txt"},
                {"Southwest College", "articulate_classes6.txt"},
                {"Trade Technical College", "articulate_classes7.txt"},
                {"Valley College", "articulate_classes8.txt"},
                {"West LA College", "articulate_classes9.txt"}
            }}
        }},
        {"UC Irvine", {
            {"Computer Science", {
                {"City College", "uci_comp_sci_city.txt"},
                {"East LA College", "uci_comp_sci_east.txt"},
                {"Harbor College", "uci_comp_sci_harbor.txt"},
                {"Mission College", "uci_comp_sci_mission.txt"},
                {"Pierce College", "uci_comp_sci_pierce.txt"},
                {"Southwest College", "uci_comp_sci_southwest.txt"},
                {"Trade Technical College", "uci_comp_sci_trade.txt"},
                {"Valley College", "uci_comp_sci_valley.txt"},
                {"West LA College", "uci_comp_sci_westla.txt"}
            }}
        }}
    };

    // Prompt user to select a university
    cout << "Select a university to compare majors:\n";
    int universityChoice = 1;
    vector<string> universityNames;
    for (const auto &university : universities) {
        cout << universityChoice++ << ". " << university.first << endl;
        universityNames.push_back(university.first);
    }

    cout << "Enter the number for the university: ";
    cin >> universityChoice;
    if (universityChoice < 1 || universityChoice > universityNames.size()) {
        cerr << "Invalid choice. Exiting program." << endl;
        return 1;
    }

    string selectedUniversity = universityNames[universityChoice - 1];
    auto &majors = universities[selectedUniversity];

    // Prompt user to select a major
    cout << "\nSelect a major to compare:\n";
    int majorChoice = 1;
    vector<string> majorNames;
    for (const auto &major : majors) {
        cout << majorChoice++ << ". " << major.first << endl;
        majorNames.push_back(major.first);
    }

    cout << "Enter the number for the major: ";
    cin >> majorChoice;
    if (majorChoice < 1 || majorChoice > majorNames.size()) {
        cerr << "Invalid choice. Exiting program." << endl;
        return 1;
    }

    string selectedMajor = majorNames[majorChoice - 1];
    auto &colleges = majors[selectedMajor];

    // Prompt user to select two colleges for comparison
    cout << "\nSelect two colleges to compare for the " << selectedMajor << " major at " << selectedUniversity << ":\n";
    vector<string> collegeNames;
    int collegeIndex = 1;
    for (const auto &college : colleges) {
        cout << collegeIndex++ << ". " << college.first << endl;
        collegeNames.push_back(college.first);
    }

    int college1, college2;
    cout << "Enter the number for the first college: ";
    cin >> college1;
    cout << "Enter the number for the second college: ";
    cin >> college2;

    if (college1 < 1 || college1 > collegeNames.size() ||
        college2 < 1 || college2 > collegeNames.size() || college1 == college2) {
        cerr << "Invalid college selections. Exiting program." << endl;
        return 1;
    }

    string college1Name = collegeNames[college1 - 1];
    string college2Name = collegeNames[college2 - 1];

    // Read data for the selected colleges
    vector<pair<string, string>> data1 = readFile(colleges[college1Name]);
    vector<pair<string, string>> data2 = readFile(colleges[college2Name]);

    if (data1.empty() || data2.empty()) {
        cerr << "Error: Could not read data for one or both colleges." << endl;
        return 1;
    }

    // Compare the two colleges
    vector<pair<string, pair<string, string>>> comparison = compareColleges(data1, data2);

    // Output the comparison
    cout << "\nComparison between " << college1Name << " and " << college2Name
         << " for the " << selectedMajor << " major at " << selectedUniversity << ":\n";

    for (const auto &entry : comparison) {
        cout << entry.first << ":\n";
        cout << "  " << college1Name << ": " << entry.second.first << "\n";
        cout << "  " << college2Name << ": " << entry.second.second << "\n";
    }

    return 0;
}
