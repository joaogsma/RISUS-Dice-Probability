#include <iomanip>
#include <sstream>
#include <vector>

#include "probability_calculator.h"

Node::~Node()
{
    typedef std::vector<Node*>::const_iterator child_cit;

    for (child_cit it = children.begin(); it != children.end(); ++it)
        delete *it;
}

void Node::create_children()
{
    for (short i = 1; i <= 6; ++i)
        children.push_back(new Node(i));
}

void Probability_Calculator::print_table(std::ostream& os, int max_dice, int max_successes, bool updates)
{
    const int float_precision = 2;

    // Create the table
    std::vector<std::vector<double> > table(max_successes + 1,
        std::vector<double>(max_dice + 1, 0));

    int computed_cells = 0;
    if (updates)
        os << "Computed cells: " << std::endl;

    // Fill the table
    for (int dice = 1; dice <= max_dice; ++dice)
    {
        for (int successes = 1; successes <= max_successes; ++successes)
        {
            table[successes][dice] = success_probability(dice, successes);

            if (updates)
                os << (++computed_cells) << '/' <<
                max_dice * max_successes << std::endl;
        }
    }

    const std::string::size_type cell_lenght = 6 + float_precision;
    const std::string::size_type line_length = (max_dice + 1) * cell_lenght +
        max_dice + 2;

    const std::string divide(line_length, '-');

    std::stringstream ss;

    ss << std::fixed << std::setprecision(float_precision);

    const std::string declaration_padding_fst(std::floor((line_length - 7) / 2.f), '=');
    const std::string declaration_padding_snd(std::ceil((line_length - 7) / 2.f), '=');

    ss << declaration_padding_fst << " TABLE " << declaration_padding_snd << std::endl;

    ss << divide << std::endl;

    // ========== First line ==========
    {
        std::string padding((cell_lenght - 6) / 2, ' ');

        ss << '|' << padding << "# Suc." << padding << '|';

        for (int dice = 1; dice <= max_dice; ++dice)
            ss << padding << dice << " dice" << padding << '|';

        ss << std::endl;
    }
    // ================================

    ss << divide << std::endl;

    // ========== General line ==========
    {

        for (int target_success = 1; target_success <= max_successes; ++target_success)
        {
            int target_success_length = 0;
            double exponent = 0;

            while (target_success / std::pow(10, exponent++) >= 1)
                target_success_length++;

            double half_padding = (cell_lenght - target_success_length) / 2.f;

            const std::string padding_snd(std::floor(half_padding), ' ');
            const std::string padding_fst(std::ceil(half_padding), ' ');

            ss << '|' << padding_fst << target_success << padding_snd << '|';

            for (int num_dice = 1; num_dice <= max_dice; ++num_dice)
            {
                double value = table[target_success][num_dice];
                std::string zero_padding = value < 0.1 ? "0" : "";

                ss << ' ' << zero_padding << 100 * value << "% |";
            }

            ss << std::endl << divide << std::endl;
        }
    }
    // ==================================

    os << ss.str();
}

bool Probability_Calculator::decide_run_again()
{
    char run_again;

    std::cout << "Run again (y/n)?" << std::endl;

    while (std::cin >> run_again && run_again != 'y' && run_again != 'n')
        std::cout << "Please type either \'y\' or \'n\'" << std::endl;

    return run_again == 'y';
}

void Probability_Calculator::run()
{
    do
    {
        int max_num_dice, max_target_success;
        std::cout << "Type the maximum number of dice being thrown... ";
        std::cin >> max_num_dice;

        std::cout << "Type the maximum desired number of sucesses... ";
        std::cin >> max_target_success;

        char updates_choice;
        std::cout << "Show updates during computation (y/n)?" << std::endl;
        while (std::cin >> updates_choice && updates_choice != 'y' && 
            updates_choice != 'n')
        {
            std::cout << "Please type either \'y\' or \'n\'" << std::endl;
        }

        print_table(std::cout, max_num_dice, max_target_success, updates_choice == 'y');

    } while ( decide_run_again() && std::cout << std::string(20, '=') << std::endl );
}