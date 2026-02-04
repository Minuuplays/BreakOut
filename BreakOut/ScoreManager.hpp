#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// ScoreManager - demonstrates File I/O (OOP requirement!)
class ScoreManager {
private:
    string filename;
    int highScore;

public:
    ScoreManager(const std::string& file)
        : filename(file), highScore(0) {
        loadHighScore();
    }

    // File I/O operations
    void loadHighScore() {
        ifstream inFile(filename);  // File input stream

        if (inFile.is_open()) {
            if (inFile >> highScore) {
                cout << "High score loaded from file: " << highScore << endl;
            }
            else {
                cout << "No valid high score in file, starting at 0" << endl;
                highScore = 0;
            }
            inFile.close();
        }
        else {
            // File doesn't exist yet - first time running
            cout << "High score file not found, creating new one" << endl;
            highScore = 0;
            saveHighScore(0);  // Create the file
        }
    }
    void saveHighScore(int score) {
        ofstream outFile(filename);  // File output stream

        if (outFile.is_open()) {
            outFile << score;
            outFile.close();
            cout << "High score saved to file: " << score << endl;
        }
        else {
            cerr << "ERROR: Could not save high score to file!" << endl;
        }
    }

    int getHighScore() const { return highScore; }

    // Checking
    bool isNewHighScore(int score) const {
        return score > highScore;
    }

    // Updating highScore
    void updateHighScore(int score) {
        if (isNewHighScore(score)) {
            cout << "\n*** NEW HIGH SCORE! ***" << endl;
            cout << "Previous: " << highScore << " -> New: " << score << endl;
            highScore = score;
            saveHighScore(highScore);
        }
    }

    ~ScoreManager() {
        cout << "ScoreManager destroyed" << endl;
    }
};