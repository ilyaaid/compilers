"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Lexer = void 0;
const fs_1 = __importDefault(require("fs"));
// const readline = require('linebyline')
class Token {
    // private regRes: any[];
    constructor(tag, coords, value = '') {
        this.tag = tag;
        this.coords = coords;
        this.value = value;
        // this.regRes = regRes;
    }
    toString() {
        return `${this.tag} (${this.coords[0]}, ${this.coords[1]})` + (this.value ? `:${this.value}` : '');
    }
}
class Lexer {
    constructor(pattern) {
        this.tokens = [];
        this.regexp = new RegExp(pattern, 'gu');
    }
    allTokens() {
        return this.tokens;
    }
    async parse(filename) {
        const data = (await fs_1.default.promises.readFile(filename)).toString();
        const lines = data.split(/\r?\n/);
        lines.forEach((line, ind) => {
            this.readLine(line, ind);
        });
    }
    readLine(line, lineNumber) {
        let is_error_period = false;
        for (let i = 0; i < line.length; ++i) {
            if (/\s/.test(line[i])) {
                continue;
            }
            const res = this.regexp.exec(line.slice(i));
            if (res) {
                is_error_period = false;
                for (let gr in res.groups) {
                    if (res.groups[gr]) {
                        this.tokens.push(new Token(gr, [lineNumber, i], res.groups[gr]));
                    }
                }
                i += this.regexp.lastIndex - 1;
            }
            else if (!is_error_period) {
                is_error_period = true;
                this.tokens.push(new Token('syntax error', [lineNumber, i]));
            }
            this.regexp.lastIndex = 0;
        }
    }
}
exports.Lexer = Lexer;
