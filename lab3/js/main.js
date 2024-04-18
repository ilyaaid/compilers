"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lexer_1 = require("./lexer");
let ident = '\\p{Lu}[\\p{L}\\p{N}-]*';
let dir = '\\p{Sc}(\\p{Lu})+';
let op = '[()<>]';
let pattern = '(?<ident>^' + ident + ')' + '|' + '(?<dir>^' + dir + ')' + '|' + '(?<op>^' + op + ')';
const lexer = new lexer_1.Lexer(pattern);
lexer.parse('input.txt').then(data => {
    lexer.allTokens().forEach(token => {
        console.log(token.toString());
    });
});
