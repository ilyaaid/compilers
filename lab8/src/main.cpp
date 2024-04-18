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

   ifstream cin("/home/ilya/projects/bmstu/compilers/lab8/input.txt");
   Scanner scanner(&cin, &cout);

   shared_ptr<Token> lval = make_shared<Token>();
   shared_ptr<location_type> frag = make_shared<location_type>();

   DOMAIN_TAG tag;
   vector<Token> tokens;
   do
   {
      tag = scanner.lex(lval, frag);
      tokens.push_back(*lval);
      cout << lval->tag << "=" << lval->attr << ":" << lval->frag << endl;
   } while (tag != DOMAIN_TAG::TAG_END);

   Parser p(tokens);
   shared_ptr<Node> root = p.parse();

   root->print(0);

   ofstream out("/home/ilya/projects/bmstu/compilers/lab8/tree.dot");
   root->printDOT(out);

   cout << endl;
   return 0;
}
