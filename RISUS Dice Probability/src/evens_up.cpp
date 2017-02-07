#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>

#include "evens_up.h"

// ============================================================================
// =========================== INTERFACE FUNCTIONS ============================
// ============================================================================

double Evens_Up::success_probability(int cliche_level, int target_number)
{
    double failure_prob = 0;

    possible_rolls(cliche_level, target_number, failure_prob);

    return 1 - failure_prob;
}

void Evens_Up::print_failure_combinations(int cliche_level, int target_number, 
    std::ostream& os, int precision)
{
    possible_rolls(cliche_level, target_number, os, precision);
}

// ============================================================================



// ============================================================================
// ========================= IMPLEMENTATION FUNCTIONS =========================
// ============================================================================

int Evens_Up::count_successes(const std::list<short>& roll)
{
    int successes = 0;

    for (std::list<short>::const_iterator it = roll.begin(); it != roll.end(); ++it)
        successes += ( *it % 2 == 0 );

    return successes;
}

double Evens_Up::roll_probability(const std::list<short>& container)
{
    return 1 / std::pow(6, container.size());
}

void Evens_Up::rec_possible_rolls(Node* root, std::list<short>& path, 
    const int cliche_level, const int target_number, 
    std::function<void(const std::list<short>&)> callback_fn, bool is_root)
{
    typedef std::vector<Node*>::const_iterator child_cit;

    if (!root) return;

    // Add node's value to the path
    if (!is_root)
        path.push_back(root->value);

    // If enough successes, stop recursion
    if ( count_successes(path) < target_number )
    {
        // If no more dice (and not enough successes), add path to rolls
        if (cliche_level == 0)
            callback_fn(path);

        if (cliche_level > 0)
        {
            // Create children if needed
            if (root->children.empty())
                root->create_children();

            // Continue recursion. If 1-5, decrease the number of dice
            for (child_cit it = root->children.begin(); it != root->children.end(); ++it)
            {
                int new_cliche_level = cliche_level - ((*it)->value != 6);
                rec_possible_rolls(*it, path, new_cliche_level, target_number, callback_fn);
            }
        }
    }

    // ========== Clean-up ==========
    // Remove node's value from the path
    if (!is_root)
        path.pop_back();

    // Delete children
    for (child_cit it = root->children.begin(); it != root->children.end(); ++it)
        delete *it;

    // Empty children vector
    root->children.clear();
    // ==============================
}

void Evens_Up::possible_rolls(int cliche_level, int target_number, double& failure_prob)
{
    failure_prob = 0;

    Node* root = new Node(0);
    std::list<short> path;

    auto increase_lambda_fn = [&](const std::list<short>& path) -> void
    {
        failure_prob += roll_probability(path);
    };

    rec_possible_rolls(root, path, cliche_level, target_number,
        increase_lambda_fn, true);

    delete root;
}

void Evens_Up::possible_rolls(int cliche_level, int target_number, std::ostream& os,
    int precision)
{
    Node* root = new Node(0);
    std::list<short> path;

    std::streamsize original_precision = os.precision();

    os << std::setprecision(precision) << std::fixed;
    
    // Find longest roll sequence
    const int longest_roll_sz = 2 * target_number - 1;
    int padding_limit = longest_roll_sz + (longest_roll_sz - 1) + 5;

    // Compute the line length. 28 is the minimum value
    const int line_length = std::max( 28, padding_limit + 4 + precision );

    // Adjust the padding limit
    padding_limit = line_length - 4 - precision;
    // Line of equal signs for the title
    const std::string equal_sign_line( line_length, '=' );

    std::function<void(const std::list<short>&)> lambda = 
        [&](const std::list<short>& roll) -> void
    {
        double roll_prob = roll_probability(roll);
        
        std::stringstream ss;
        int line_size = 1;

        std::list<short>::const_iterator roll_it = roll.begin();
        ss << *roll_it;

        while (++roll_it != roll.end())
        {
            ss << '-' << *roll_it;
            line_size += 2;
        }

        // Create the padding
        std::string point_padding(padding_limit - line_size, '.');
        point_padding.front() = point_padding.back() = ' ';

        ss << point_padding;

        if (roll_prob < 0.1) ss << '0';

        os << ss.str() << 100 * roll_prob << '%' << std::endl;
    };

    std::string title(" Failure Combinations ");
    
    double title_half_padding = (line_length - title.size()) / 2.f;
    std::string title_padding_fst( std::floor(title_half_padding), '=' );
    std::string title_padding_snd( std::ceil(title_half_padding), '=' );

    os << std::endl << equal_sign_line << std::endl <<
        title_padding_fst << title << title_padding_snd << std::endl <<
        equal_sign_line << std::endl;

    rec_possible_rolls(root, path, cliche_level, target_number,
        lambda, true);

    os << equal_sign_line << std::endl;
    os << std::setprecision(original_precision) << std::defaultfloat;

    delete root;
}

// ============================================================================