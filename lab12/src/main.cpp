#include <iostream>
#include <fstream>

#include "scanner.hpp"
#include "driver.hpp"
#include "parser_generated.hpp"

using namespace std;

int main(const int argc, const char** argv)
{
    const string input_file = "/home/ilya/projects/bmstu/compilers/lab12/src/input.txt";
    ifstream in(input_file);
    Scanner scanner(&in, &cout);

    Driver driver;
    ps::Parser parser(scanner, driver);
    parser.parse();

    driver.program->check();
    cout << "The program is correct!";
    cout << endl;
    return 0;
}
