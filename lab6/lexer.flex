%option noyywrap bison-bridge bison-locations

%{
    #include <iostream>
    #include <vector>
    #include <string>
    #include <map>

    

    using namespace std;

    // Объявление тегов

    enum Tags
    {
        TAG_IDENT = 1,
        TAG_NUMBER
    };

    vector<string> tag2name = {
        "EOP", "IDENT", "NUMBER"};

    // Объявление фрагмента и типа YYLTYPE

    struct Position
    {
        int line, pos, index;
    };

    std::ostream &operator<<(std::ostream &os, const Position &pos)
    {
        return os << "(" << pos.line << "," << pos.pos << ")";
    }

    struct Fragment
    {
        Position starting, following;
    };

    std::ostream &operator<<(std::ostream &os, const Fragment &frag)
    {
        return os << frag.starting << "-" << frag.following;
    }

    using YYLTYPE = Fragment;

    union Token
    {
        int code;
        int num;
        long long lnum;
    };

    using YYSTYPE = Token;

    int continued = 0;
    Position cur{0, 0, 0};

#define YY_USER_ACTION \
    {                  \
        int i;  \
    if (!continued)     \
        yylloc->starting = cur;  \
    continued = 0;\
    \
    for (int i = 0; i < yyleng; i++)    \
    {   \
        if (yytext[i] == '\n')  \
        {   \
            cur.line++; \
            cur.pos = 0;    \
        }   \
        else    \
        {   \
            cur.pos++;  \
        }   \
        cur.index++;    \
    }\
\
    yylloc->following = cur;\
}

    map<char, int> romans =
    {
        {'M', 1000}, {'D', 500}, {'C', 100},
        {'L',   50}, {'X',  10}, {'V',   5},
        {'I',    1}
    };

    int romstr2num(const string& str) {
        int ans = 0;
        int i = 0;
        for (; i < str.size() - 1; ++i) {
            if (romans[str[i]] >= romans[str[i + 1]]) {
                ans += romans[str[i]];
            } else {
                ans += romans[str[i + 1]] - romans[str[i]];
                i++;
            }
        }
        if (i == str.size() - 1) {
            ans += romans[str[i]];
        }
        return ans;
    }
%}

IDENT [0-9]*
NUMBER M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})

%%
{IDENT} {
    yylval->code = TAG_IDENT;
    string str = yytext;
    yylval->lnum = stoll(str);
    return TAG_IDENT;
}
NIHIL {
    yylval->code = TAG_NUMBER;
    string str = yytext;
    yylval->num = 0;
    return TAG_NUMBER;
}
{NUMBER} {
    yylval->code = TAG_NUMBER;
    string str = yytext;
    yylval->num = romstr2num(str);
    return TAG_NUMBER;
}
.
\n
%%

int main(int args, char **argv)
{
    FILE *src = fopen(argv[1], "r");
    yyin = src;


    int tag;
    YYSTYPE value;
    YYLTYPE coords;
    tag = yylex(&value, &coords);
    while (tag != 0) {
        cout << tag2name[tag] << " " << coords;
        if (tag == TAG_IDENT) {
            cout << ":" << value.lnum;
        } else {
            cout << ":" << value.num;
        }
        cout << endl;
        tag = yylex(&value, &coords);
    }
    fclose(src);
}
