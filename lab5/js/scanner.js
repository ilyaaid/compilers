"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Scanner = void 0;
const fragment_1 = require("./fragment");
const token_1 = require("./token");
const fs_1 = __importDefault(require("fs"));
const automat_1 = require("./automat");
class Scanner {
    constructor(compiler) {
        this.compiler = compiler;
        this.p = new fragment_1.ActivePosition(0, -1, -1, this);
        this.fd = 0;
        this.automat = new automat_1.Automat();
    }
    async init(path) {
        this.fd = fs_1.default.openSync(path, 'r');
        await this.p.next();
    }
    async nextToken() {
        await this.p.skipWhiteSpace();
        let state = 0;
        let lastFinalState = -1;
        this.automat.setState(state);
        let start = this.p.getNewPosition();
        let end;
        let tmpAttr = this.p.ch;
        let finalAttr = '';
        const isValid = {
            found: false,
            attempt: 0,
        };
        while (!this.p.isEnd()) {
            state = this.automat.nextState(this.p.ch);
            if (state == -1) {
                if (!isValid.found) {
                    // пока не найден ни один токен и при этом ловушка
                    if (isValid.attempt == 0) {
                        this.compiler.addMessage(true, this.p.getNewPosition(), 'unexpected token');
                        isValid.attempt++;
                    }
                    // переходим на следующий элемент и пропускаем пробелы, пока не встретим правильный или ошибочный элемент
                    await this.p.next();
                    await this.p.skipWhiteSpace();
                    start = this.p.getNewPosition();
                    tmpAttr = this.p.ch;
                    state = 0;
                    this.automat.setState(state);
                    continue;
                }
                break;
            }
            if (this.automat.stateToDomainTag[state] != token_1.DomainTag.NOT_FINAL) {
                isValid.found = true;
                lastFinalState = state;
                finalAttr = tmpAttr;
                this.p.clearChQueue();
                end = this.p.getNewPosition();
            }
            await this.p.next(false);
            tmpAttr += this.p.ch;
        }
        if (lastFinalState >= 0) {
            this.p.clearChQueue();
            const tag = this.automat.stateToDomainTag[lastFinalState];
            if (tag == token_1.DomainTag.N_STR) {
                this.compiler.addMessage(false, end, 'expected symbol: \'');
            }
            const token = token_1.Token.createFromDomainTag(finalAttr, tag, new fragment_1.Fragment(start, end));
            // if (!token) {
            //     this.compiler.addMessage(true, start, 'unexpected token');
            // }
            return token;
        }
        else {
            await this.p.next();
        }
        return undefined;
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
//# sourceMappingURL=scanner.js.map