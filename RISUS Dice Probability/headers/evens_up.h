#pragma once
#include <iostream>
#include <list>
#include <vector>

#include "probability_calculator.h"

class Evens_Up : public Probability_Calculator {
public:
    double success_probability(int cliche_level, int target_numbers);

    void print_failure_combinations(int cliche_level, int target_numbers,
        std::ostream& os, int precision = 3);

private:
    void possible_rolls(int cliche_level, int target_number, double& failure_prob);

    void possible_rolls(int cliche_level, int target_number, std::ostream& os, 
        int precision);
    
    double roll_probability(const std::list<short>& container);
    
    void rec_possible_rolls(Node* root, std::list<short>& path, const int cliche_level, 
        const int target_number, std::function<void(const std::list<short>&)> callback_fn,
        bool is_root = false);

    int count_successes(const std::list<short>& roll);
};