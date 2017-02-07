#pragma once

#include <functional>
#include <iostream>
#include <list>

class Node {
public:
    short value;
    std::vector<Node*> children;

    Node(short value) : value(value) {}

    ~Node();

    void create_children();
};

class Probability_Calculator {
public:
    // Pure abstract functions
    virtual double success_probability(int cliche_level, int target_number) = 0;

    // ========== Implemented functions ==========
    virtual void print_table(std::ostream& os, int max_cliche_level, int max_target_number,
        bool updates = false);

    void run();
    // ===========================================

private:
    // ========== Pure virtual functions ==========
    virtual double roll_probability(const std::list<short>& container) = 0;

    virtual void possible_rolls(int cliche_level, int target_number, 
        double& failure_prob) = 0;

    virtual void rec_possible_rolls(Node* root, std::list<short>& path, 
        const int cliche_level, const int target_number, 
        std::function<void(const std::list<short>&)> callback_fn, 
        bool is_root = false) = 0;
    // ============================================

    // Implemented function
    bool decide_run_again();
};

