"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Scanner = void 0;
const fragment_1 = require("./fragment");
const token_1 = require("./token");
const fs_1 = __importDefault(require("fs"));
const utils_1 = require("./utils");
class Scanner {
    constructor(compiler) {
        this.compiler = compiler;
        this.p = new fragment_1.ActivePosition(0, -1, -1, this);
        this.fd = 0;
    }
    async init(path) {
        this.fd = fs_1.default.openSync(path, 'r');
        await this.p.next();
    }
    async nextToken() {
        let isValid = {
            ok: false,
            attempt: 0,
        };
        while (this.p.ch && isValid.ok == false) {
            if (this.p.ch == '"') {
                isValid.ok = true;
                const start = this.p.getNewPosition();
                let attr = '';
                await this.p.next();
                while (this.p.ch != '\"') {
                    if (this.p.ch == '\\') {
                        await this.p.next();
                        switch (this.p.ch) {
                            case 'n': {
                                attr += '\n';
                                break;
                            }
                            case 't': {
                                attr += '\t';
                                break;
                            }
                            case '\\': {
                                attr += '\\';
                                break;
                            }
                            case '"': {
                                attr += '"';
                                break;
                            }
                            default: {
                                this.compiler.addMessage(true, this.p.getNewPosition(), 'unrecognized escape sequence');
                                break;
                            }
                        }
                    }
                    else if (this.p.isNewLine()) {
                        this.compiler.addMessage(true, this.p.getNewPosition(), 'unexpected newline in string');
                        break;
                    }
                    else {
                        attr += this.p.ch;
                    }
                    await this.p.next();
                }
                const newToken = new token_1.StringToken(attr, token_1.DomainTag.STRING, new fragment_1.Fragment(start, this.p.getNewPosition()));
                await this.p.next();
                return newToken;
            }
            else if ((0, utils_1.isNumber)(this.p.ch)) {
                isValid.ok = true;
                const start = this.p.getNewPosition();
                let attr = 0;
                attr += Number(this.p.ch);
                attr *= 10;
                let end = this.p.getNewPosition();
                await this.p.next();
                while ((0, utils_1.isNumber)(this.p.ch) || this.p.ch == '_') {
                    if ((0, utils_1.isNumber)(this.p.ch)) {
                        attr += Number(this.p.ch);
                        attr *= 10;
                    }
                    end = this.p.getNewPosition();
                    await this.p.next();
                }
                attr /= 10;
                return new token_1.NumberToken(attr, token_1.DomainTag.NUMBER, new fragment_1.Fragment(start, end));
            }
            else {
                if (!this.p.isWhiteSpace() && !this.p.isNewLine() && isValid.attempt == 0) {
                    this.compiler.addMessage(true, this.p.getNewPosition(), 'unexpected token');
                    isValid.attempt++;
                }
                await this.p.next();
            }
        }
    }
    async nextChar() {
        try {
            const ch = await new Promise((resolve, reject) => {
                fs_1.default.read(this.fd, Buffer.alloc(1), 0, 1, -1, (err, bytesRead, buffer) => {
                    // console.log(err, bytesRead, buffer.toString());
                    if (err) {
                        reject(err);
                    }
                    resolve({
                        bytesRead,
                        ch: buffer.toString()
                    });
                });
            });
            if (ch.bytesRead) {
                return ch.ch;
            }
        }
        catch (err) {
            console.log(err);
        }
        return '';
    }
}
exports.Scanner = Scanner;
