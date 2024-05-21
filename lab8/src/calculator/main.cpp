#include <iostream>
#include <fstream>

#include "analyzer/scanner.hpp"
#include "calculator.hpp"

using namespace std;

int main(const int argc, const char** argv)
{
    const string grammar_file = "/home/ilya/projects/bmstu/compilers/lab8/arithexpr_test.txt";
    const string table_dest_filename = "/home/ilya/projects/bmstu/compilers/lab8/grammar_language_table.hpp";

    ifstream cin(grammar_file);
    Scanner scanner(&cin, &cout);

    shared_ptr<semantic_type> lval = make_shared<semantic_type>();
    shared_ptr<location_type> frag = make_shared<location_type>();

    vector<Token> tokens;
    DOMAIN_TAG tag;
    do
    {
        tag = scanner.lex(lval, frag);
        Token new_token = Token(tag, *lval, *frag);
        tokens.push_back(new_token);
        cout << new_token << endl;
    } while (tag != 0);

    Parser p(tokens);
    shared_ptr<Node> root = p.parse();
    ofstream out("/home/ilya/projects/bmstu/compilers/lab8/tree.dot");
    root->printDOT(out);

    Calculator calc(root);

    cout << endl;
    cout << calc.calculate();

    cout << endl;
    return 0;
}
