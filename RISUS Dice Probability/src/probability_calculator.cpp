#include <iomanip>
#include <sstream>
#include <stdexcept>
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

void Probability_Calculator::print_table(std::ostream& os, int max_cliche_level, 
    int max_target_number, bool updates)
{
    if (max_cliche_level < 1 || max_cliche_level > 6) 
        throw std::domain_error("Invalid maximum cliche level");

    if (max_target_number < 1)
        throw std::domain_error("Invalid maximum target number");

    const int float_precision = 2;

    // Create the table
    std::vector<std::vector<double> > table(max_target_number + 1,
        std::vector<double>(max_cliche_level + 1, 0));

    int computed_cells = 0;
    if (updates)
        os << "Computed cells: " << std::endl;

    // Fill the table
    for (int cliche_level = 1; cliche_level <= max_cliche_level; ++cliche_level)
    {
        for (int target_number = 1; target_number <= max_target_number; ++target_number)
        {
            table[target_number][cliche_level] = success_probability(cliche_level, target_number);

            if (updates)
                os << (++computed_cells) << '/' <<
                max_cliche_level * max_target_number << std::endl;
        }
    }

    std::string::size_type cell_lenght = 6 + float_precision;
    std::string::size_type initial_line_length = (max_cliche_level + 1) * cell_lenght +
        max_cliche_level + 2;

    std::string divide(initial_line_length, '-');

    std::stringstream ss;

    ss << std::fixed << std::setprecision(float_precision);

    const std::string declaration_padding_fst( std::ceil((initial_line_length - 7) / 2.f), '=' );
    const std::string declaration_padding_snd( std::floor((initial_line_length - 7) / 2.f), '=' );

    ss << declaration_padding_fst << " TABLE " << declaration_padding_snd << std::endl;
    
    
    // ========== First row ==========
    {
        const std::string cell_text = " CLICHE LEVELS ";

        if ( cell_text.size() > initial_line_length - cell_lenght - 1 )
        {
            double extra_line_length = cell_text.size() + cell_lenght + 3 - initial_line_length;
            int cell_extra_padding = std::ceil( extra_line_length / max_cliche_level );
            
            cell_lenght += cell_extra_padding;

            initial_line_length = (max_cliche_level + 1) * cell_lenght + max_cliche_level + 2;
            divide = std::string(initial_line_length, '-');
        }

        double half_padding = ( max_cliche_level * cell_lenght + max_cliche_level -
            1 - cell_text.size() ) / 2.f;

        const std::string padding_fst( std::ceil(half_padding), ' ' );
        const std::string padding_snd( std::floor(half_padding), ' ' );

        const std::string empty_cell(cell_lenght + 1, ' ');
        
        // Upper division
        ss << empty_cell << std::string(initial_line_length - cell_lenght - 1, '-') << std::endl;

        // First line
        ss << empty_cell << '|' << padding_fst << cell_text << padding_snd << '|' << std::endl;
    }
    // ================================

    ss << divide << std::endl;

    // ========== Second row ==========
    {
        double half_padding = (cell_lenght - 6) / 2.f;
        std::string padding_fst(std::ceil(half_padding), ' ');
        std::string padding_snd(std::floor(half_padding), ' ');

        ss << '|' << padding_fst << "# Suc." << padding_snd << '|';

        half_padding = (cell_lenght - 1) / 2.f;
        padding_fst = std::string(std::ceil(half_padding), ' ');
        padding_snd = std::string(std::floor(half_padding), ' ');

        for (int cliche_level = 1; cliche_level <= max_cliche_level; ++cliche_level)
            ss << padding_fst << cliche_level << padding_snd << '|';

        ss << std::endl;
    }
    // ================================

    ss << divide << std::endl;

    // ========== General line ==========
    {
        for (int target_number = 1; target_number <= max_target_number; ++target_number)
        {
            int target_number_length = 0;
            double exponent = 0;

            while (target_number / std::pow(10, exponent++) >= 1)
                target_number_length++;

            double half_padding = (cell_lenght - target_number_length) / 2.f;

            std::string padding_fst(std::ceil(half_padding), ' ');
            std::string padding_snd(std::floor(half_padding), ' ');

            ss << '|' << padding_fst << target_number << padding_snd << '|';

            for (int cliche_level = 1; cliche_level <= max_cliche_level; ++cliche_level)
            {
                double value = table[target_number][cliche_level];
                std::string zero_padding = value < 0.1 ? "0" : "";

                half_padding = (cell_lenght - 6) / 2.f;

                std::string padding_fst(std::ceil(half_padding), ' ');
                std::string padding_snd(std::floor(half_padding), ' ');

                ss << padding_fst << zero_padding << 100 * value << '%' << padding_snd << '|';
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
        int max_cliche_level, max_target_number;
        std::cout << "Type the maximum number of cliche level being thrown... ";
        std::cin >> max_cliche_level;

        std::cout << "Type the maximum desired target number... ";
        std::cin >> max_target_number;

        char updates_choice;
        std::cout << "Show updates during computation (y/n)?" << std::endl;
        while (std::cin >> updates_choice && updates_choice != 'y' && 
            updates_choice != 'n')
        {
            std::cout << "Please type either \'y\' or \'n\'" << std::endl;
        }

        print_table(std::cout, max_cliche_level, max_target_number, updates_choice == 'y');

    } while ( decide_run_again() && std::cout << std::string(20, '=') << std::endl );
}