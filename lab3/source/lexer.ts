
import fs from 'fs';
//@ts-ignore
import readline from 'linebyline';
// const readline = require('linebyline')

class Token {
    private tag: string;
    private coords: number[];
    private value: string;
    // private regRes: any[];

    constructor(tag:string, coords:number[], value:string = '') {
        this.tag = tag;
        this.coords = coords;
        this.value = value;
        // this.regRes = regRes;
    }

    toString() {
        return `${this.tag} (${this.coords[0]}, ${this.coords[1]})` + (this.value ? `:${this.value}` : '');
    }
}

export class Lexer {
    private tokens: Token[];
    private regexp: RegExp;

    constructor(pattern:string) {
        this.tokens = [];
        this.regexp = new RegExp(pattern, 'gu');
    }

    allTokens() {
        return this.tokens;
    }

    async parse(filename:string) {
        const data  = (await fs.promises.readFile(filename)).toString();
        const lines = data.split(/\r?\n/);

        lines.forEach((line, ind) => {
            this.readLine(line, ind);
        })
    }

    readLine(line: string, lineNumber:number) {

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
                
            } else if (!is_error_period){
                is_error_period = true;
                this.tokens.push(new Token('syntax error', [lineNumber, i]));
            }

            this.regexp.lastIndex = 0;
        }
    }
}
