#pragma once

#include "Bricks.hpp"
#include "Vector2D.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Level data structure using a matrix
struct Level {
    int levelNumber;
    string name;
    string difficulty;
    vector<vector<int>> brickMatrix;  // Matrix representing brick layout
    int rows;
    int cols;

    Level() : levelNumber(0), rows(0), cols(0) {}

    Level(int num, string n, string diff)
        : levelNumber(num), name(n), difficulty(diff), rows(0), cols(0) {
    }
};

class LevelManager {
private:
    vector<Level> levels;
    int currentLevelIndex;

public:
	LevelManager() :currentLevelIndex(0) 
    {
		loadAllLevels();
    }

    // Load a level from a file
    bool loadLevelFromFile(const string& filename)
    {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error: Could not open level file: " << filename << endl;
            return false;
        }

        Level level;
        string line;

        // Read level metadata
        if (getline(file, line)) {
            // Format: LEVEL_NUMBER
            level.levelNumber = stoi(line);
        }

        if (getline(file, line)) {
            // Format: LEVEL_NAME
            level.name = line;
        }

        if (getline(file, line)) {
            // Format: DIFFICULTY
            level.difficulty = line;
        }

        if (getline(file, line)) {
            // Format: ROWS COLS
            istringstream iss(line);
            iss >> level.rows >> level.cols;
        }

        // Read the brick matrix
        level.brickMatrix.clear();
        for (int row = 0; row < level.rows; row++) {
            if (!getline(file, line)) {
                cerr << "Error: Incomplete level data in file: " << filename << endl;
                return false;
            }

            vector<int> rowData;
            istringstream iss(line);
            int value;

            while (iss >> value) {
                rowData.push_back(value);
            }

            if (rowData.size() != level.cols) {
                cerr << "Error: Row " << row << " has incorrect number of columns in file: "
                    << filename << endl;
                return false;
            }

            level.brickMatrix.push_back(rowData);
        }

        file.close();

        levels.push_back(level);
        cout << "Loaded level: " << level.name << " (Level " << level.levelNumber << ")" << endl;

        return true;
    }

    // Load all levels from a directory
    void loadAllLevels()
    {
        vector<string> levelFiles = {
            "Levels/level1.txt",
            "Levels/level2.txt",
            "Levels/level3.txt",
            "Levels/level4.txt",
            "Levels/level5.txt"
        };

        for (const string& filename : levelFiles) {
            loadLevelFromFile(filename);
        }

        if (levels.empty()) {
            cerr << "Warning: No levels loaded! Creating default level." << endl;
            // Create a default level if no files are found
            Level defaultLevel(1, "Default Level", "Easy");
            defaultLevel.rows = 8;
            defaultLevel.cols = 10;

            // Simple default pattern
            for (int row = 0; row < 8; row++) {
                vector<int> rowData;
                for (int col = 0; col < 10; col++) {
                    int hp = (row < 2) ? 3 : (row < 5) ? 2 : 1;
                    rowData.push_back(hp);
                }
                defaultLevel.brickMatrix.push_back(rowData);
            }

            levels.push_back(defaultLevel);
        }
    }

    // Get current level
    Level* getCurrentLevel()
    {
        if (currentLevelIndex >= 0 && currentLevelIndex < levels.size()) {
            return &levels[currentLevelIndex];
        }
        return nullptr;
    }

    // Get specific level by index
    Level* getLevel(int index)
    {
        if (index >= 0 && index < levels.size()) {
            return &levels[index];
        }
        return nullptr;
    }

    // Level navigation
    bool nextLevel()
    {
        if (currentLevelIndex < levels.size() - 1) {
            currentLevelIndex++;
            return true;
        }
        return false;
    }
    bool previousLevel()
    {
        if (currentLevelIndex > 0) {
            currentLevelIndex--;
            return true;
        }
        return false;
    }

    void setLevel(int levelIndex)
    {
        if (levelIndex >= 0 && levelIndex < levels.size()) {
            currentLevelIndex = levelIndex;
        }
    }

    // Get level info
    int getCurrentLevelNumber() const { return currentLevelIndex + 1; }
    int getTotalLevels() const { return levels.size(); }
    bool hasMoreLevels() const { return currentLevelIndex < levels.size() - 1; }

    // Create bricks from current level matrix
    void createBricksFromLevel(vector<Brick*>& bricks, int brickWidth, int brickHeight,
        int padding, int xOffset, int yOffset)
    {
        // Clear existing bricks
        for (Brick* brick : bricks) {
            delete brick;
        }
        bricks.clear();

        Level* level = getCurrentLevel();
        if (!level) {
            cerr << "Error: No current level!" << endl;
            return;
        }

        // Create bricks based on the matrix
        for (int row = 0; row < level->rows; row++) {
            for (int col = 0; col < level->cols; col++) {
                int hp = level->brickMatrix[row][col];

                // 0 means no brick at this position
                if (hp > 0) {
                    float x = xOffset + col * (brickWidth + padding);
                    float y = yOffset + row * (brickHeight + padding);

                    Brick* brick = new Brick(x, y, brickWidth, brickHeight, hp);
                    bricks.push_back(brick);
                }
            }
        }

        cout << "Created " << bricks.size() << " bricks for level: " << level->name << endl;
    }

    // Utility function to save a level to file
    static bool saveLevelToFile(const Level& level, const string& filename)
    {
        ofstream file(filename);

        if (!file.is_open()) {
            cerr << "Error: Could not create level file: " << filename << endl;
            return false;
        }

        // Write level metadata
        file << level.levelNumber << endl;
        file << level.name << endl;
        file << level.difficulty << endl;
        file << level.rows << " " << level.cols << endl;

        // Write brick matrix
        for (const auto& row : level.brickMatrix) {
            for (size_t i = 0; i < row.size(); i++) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << " ";
                }
            }
            file << endl;
        }

        file.close();
        cout << "Saved level to: " << filename << endl;

        return true;
    }

    ~LevelManager()
    {
        levels.clear();
    }
};
