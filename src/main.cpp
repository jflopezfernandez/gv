
#include "gv.hpp"

#ifndef DEFAULT_SIMULATION_ITERATIONS
#define DEFAULT_SIMULATION_ITERATIONS 10
#endif // DEFAULT_SIMULATION_ITERATIONS

namespace IO
{
    // Don't flush the output buffer when printing a newline character
    template <typename T, typename CharT = std::char_traits<T>>
    std::basic_ostream<T, CharT>&
    EndLine(std::basic_ostream<T, CharT>& outputStream)
    {
        return outputStream << outputStream.widen('\n');
    }
} // namespace IO

namespace Options = boost::program_options;

enum class PrintFormat {
    Canonical, // (x,y)
    Bare       // x y
};

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

    std::string str(PrintFormat format = PrintFormat::Canonical) const noexcept
    {
        std::ostringstream oss;

        if (format == PrintFormat::Canonical) {
            oss << "(" << x << "," << y << ")";
        } else if (format == PrintFormat::Bare) {
            oss << x << " " << y;
        }

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

    // Initialize location at the origin
    Point p(0, 0);
    
    // Display initial location
    // p.print();
    std::cout << p.str(PrintFormat::Bare) << IO::EndLine;

    // Run the simulation
    for (size_t i = 0; i < iterations; ++i) {
        // Generate next random movement
        const int x1 = dist(gen);
        const int y1 = dist(gen);

        // Simulate movement
        p.move(x1, y1);

        // Print the new point value.
        // p.print();
        std::cout << p.str(PrintFormat::Bare) << IO::EndLine;
    }

    return EXIT_SUCCESS;
}

