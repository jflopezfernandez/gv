
#include "gv.hpp"

#ifndef DEFAULT_SIMULATION_ITERATIONS
#define DEFAULT_SIMULATION_ITERATIONS 10
#endif // DEFAULT_SIMULATION_ITERATIONS

namespace Options = boost::program_options;

class Point
{
    int x;
    int y;

public:
    Point()
        : x { }, y { }
    {
        //
    }

    Point(int n)
        : x { n }, y { n }
    {
        //
    }

    Point(int a, int b)
        : x { a }, y { b }
    {
        //
    }

    void move(int x_offset, int y_offset)
    {
        x += x_offset;
        y += y_offset;
    }

    std::string str() const noexcept
    {
        std::ostringstream oss;

        oss << "(" << x << "," << y << ")";

        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& outputStream, const Point& p)
    {
        return outputStream << p.str();
    }

    void print() const noexcept
    {
        std::cout << *this << std::endl;
    }
};

int main(int argc, const char *argv[])
{
    // Simulation configuration options
    size_t iterations = DEFAULT_SIMULATION_ITERATIONS;

    Options::options_description generic("Generic Options");
    generic.add_options()
        ("help", "Display this help menu and exit.")
        ("version", "Display program version information and exit.")
    ;

    Options::options_description config("Configuration");
    config.add_options()
        ("iterations,i", Options::value<size_t>(&iterations)->default_value(DEFAULT_SIMULATION_ITERATIONS), "Number of iterations in the simulation")
    ;

    Options::positional_options_description pos;
    pos.add("iterations", 1);

    Options::options_description opts("Program Options");
    opts
        .add(generic)
        .add(config);

    Options::variables_map vm;
    Options::store(Options::command_line_parser(argc, argv)
        .options(opts)
        .positional(pos)
        .run(),
    vm);
    Options::notify(vm);

    if (vm.count("help")) {
        std::cout << opts << std::endl;

        return EXIT_SUCCESS;
    }

    if (vm.count("version")) {
        std::cout << "<Program Version Info>" << std::endl;

        return EXIT_SUCCESS;
    }

    // Initialize random number generation mechanism
    std::random_device entropy;
    std::mt19937_64 gen(entropy());
    std::uniform_int_distribution dist(-1, 1);

    // Initialize location
    Point p;
    
    // Display initial location
    p.print();

    // Run the simulation
    for (size_t i = 0; i < iterations; ++i) {
        // Simulate movement
        p.move(dist(gen), dist(gen));

        // Print the new point value.
        p.print();
    }

    return EXIT_SUCCESS;
}

