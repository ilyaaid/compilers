
import { Lexer } from './lexer';

let ident = '\\p{Lu}[\\p{L}\\p{N}-]*';
let dir = '\\p{Sc}(\\p{Lu})+';
let op = '[()<>]';
let pattern = '(?<ident>^' + ident + ')' + '|' + '(?<dir>^' + dir + ')' + '|' + '(?<op>^' + op + ')';

const lexer = new Lexer(pattern);
lexer.parse('input.txt').then(data => {
    lexer.allTokens().forEach(token => {
        console.log(token.toString());
    });
});

