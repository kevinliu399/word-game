#include <Python.h>

class WordSimilitudeCalculator {
public:
    double get_score() {
        return m_score;
    }

    void set_score(double new_score) {
        m_score = new_score;
    }
private:
    double m_score = 0;
};