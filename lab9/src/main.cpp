#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "scanner.hpp"
#include "token.hpp"
#include "position.hpp"
#include "parser.hpp"

using namespace std;

int main(const int argc, const char **argv)
{

   ifstream cin("/home/ilya/projects/bmstu/compilers/lab9/input.txt");
   auto sc = std::make_shared<Scanner>(&cin, &cout);

   Parser::setParameters(sc);
   Parser& p = Parser::getInstance();
   shared_ptr<Node> root = p.parse();

   // root->print(0);

   ofstream out("/home/ilya/projects/bmstu/compilers/lab9/tree.dot");
   root->printDOT(out);
   cout << endl;
   return 0;
}
