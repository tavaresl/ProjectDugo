#pragma once

class Score
{
private:
    unsigned int highScore;
    unsigned int currentScore;

public:
    Score() {
        highScore = 0;
        currentScore = 0;
    }

    void IncrementCurrentScore() {
        currentScore += 1;
    }

    void UpdateHighScore() {
        if (currentScore > highScore)
            highScore = currentScore;
    }

    void ResetCurrentScore() {
        currentScore = 0;
    }

    unsigned int Highscore() {
        return highScore;
    }

    unsigned int CurrentScore() {
        return currentScore;
    }
};
