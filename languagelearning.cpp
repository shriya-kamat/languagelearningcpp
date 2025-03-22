#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>

using namespace std;

class Word {
private:
    string english;
    string german;

public:
    Word(const string& eng, const string& ger) : english(eng), german(ger) {}
    
    const string& getEnglish() const {
        return english;
    }
    
    const string& getGerman() const {
        return german;
    }
};

class LanguageLearningSystem {
private:
    vector<Word> vocabulary;
    int level;
    int score;

public:
    LanguageLearningSystem() : level(2), score(0) {
        loadDataset();
    }

    void loadDataset() {
        vocabulary.clear(); // Clear the vocabulary for the previous level.
        ifstream file("dataset.txt");
        if (!file.is_open()) {
            cerr << "Error: Unable to open the dataset file." << endl;
            exit(EXIT_FAILURE);
        }
        
        string line;
        int currentLevel = 0;
        
        while (getline(file, line)) {
            if (line.empty() || all_of(line.begin(), line.end(), ::isspace)) {
                continue;
            }
            
            if (line.find("Level") != string::npos) {
                string levelStr = line.substr(6);
                levelStr.erase(0, levelStr.find_first_not_of(' '));
                levelStr.erase(levelStr.find_last_not_of(' ') + 1);
                currentLevel = stoi(levelStr);
            } else if (currentLevel == level) {
                string english = line.substr(0, line.find(" "));
                string german = line.substr(line.find(" ") + 1);
                vocabulary.emplace_back(english, german);
            }
        }
        file.close();
    }

    void shuffleVocabulary() {
        random_shuffle(vocabulary.begin(), vocabulary.end());
    }

    void learn() {
        cout << "=== Learning Mode - Level " << level << " ===" << endl;
        for (const auto& word : vocabulary) {
            cout << "English: " << word.getEnglish() << "\tGerman: " << word.getGerman() << endl;
            string userTranslation;
            cout << "Your translation in German: ";
            cin >> userTranslation;
            if (userTranslation == word.getGerman()) {
                cout << "Correct!" << endl;
            } else {
                cout << "Incorrect. The correct translation is: " << word.getGerman() << endl;
            }
            cout << "--------------------------" << endl;
        }
        cout << "\n\n";
    }

    void quiz() {
        shuffleVocabulary();
        cout << "=== Quiz Mode - Level " << level << " ===" << endl;
        
        for (const auto& word : vocabulary) {
            cout << "English: " << word.getEnglish() << endl;
            string userTranslation;
            cout << "Your translation in German: ";
            cin >> userTranslation;
            if (userTranslation == word.getGerman()) {
                cout << "Correct!" << endl;
                score++; // Increment the score for correct answers.
            } else {
                cout << "Incorrect. The correct translation is: " << word.getGerman() << endl;
            }
            cout << "--------------------------" << endl;
        }
        cout << "\n\n";
        cout << "Quiz completed. Your score: " << score << "/" << vocabulary.size() << ".\n";
    }

    void startLearningSystem() {
        int choice;
        do {
            cout << "English-German Language Learning System\n";
            cout << "Choose Level (1 to 5)\n";
            cout << "Enter the level you want to learn: ";
            if (!(cin >> level) || level < 1 || level > 5) {
                cout << "Invalid level. Please enter a level between 1 and 5." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            loadDataset();
            
            cout << "1. Learn\n2. Quiz\n0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    learn();
                    break;
                case 2:
                    score = 0; // Reset score for the quiz.
                    quiz();
                    break;
                case 0:
                    cout << "Exiting Language Learning System. Auf Wiedersehen!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 0);
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr))); // Seed for shuffling
    LanguageLearningSystem languageSystem;
    languageSystem.startLearningSystem();
    return 0;
}
