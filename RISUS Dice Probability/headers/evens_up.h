#pragma once
#include <iostream>
#include <list>
#include <vector>

#include "probability_calculator.h"

class Evens_Up : public Probability_Calculator {
public:
    double success_probability(int num_dice, int target_successes);

    void print_failure_combinations(int num_dice, int target_successes,
        std::ostream& os, int precision = 3);

private:
    void possible_rolls(int num_dice, int target_success, double& failure_prob);

    void possible_rolls(int num_dice, int target_success, std::ostream& os, 
        int precision);
    
    double roll_probability(const std::list<short>& container);
    
    void rec_possible_rolls(Node* root, std::list<short>& path, const int num_dice, 
        const int target_success, std::function<void(const std::list<short>&)> callback_fn,
        bool is_root = false);

    int count_successes(const std::list<short>& roll);
};