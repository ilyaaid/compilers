import { Compiler } from './compiler';
import { ActivePosition, Fragment, Position } from './fragment';
import { STRToken, Token, DomainTag, NUMToken } from './token';
import fs from 'fs';
import { code, isNumber } from './utils';
import { Automat } from './automat';

export type ReadFileT = {
    bytesRead: number
    ch: string
}

export class Scanner {
    private fd: number;
    private p: ActivePosition;
    private automat: Automat;


    constructor(public compiler: Compiler) {
        this.p = new ActivePosition(0, -1, -1, this);
        this.fd = 0;
        this.automat = new Automat();
    }

    async init(path: string): Promise<void> {
        this.fd = fs.openSync(path, 'r');
        await this.p.next();
    }

    async nextToken(): Promise<Token | undefined> {
        await this.p.skipWhiteSpace();

        let state = 0;
        let lastFinalState = -1;
        this.automat.setState(state);

        let start = this.p.getNewPosition();
        let end: Position;

        let tmpAttr = this.p.ch;
        let finalAttr = '';

        const isValid = {
            found: false,
            attempt: 0,
        }
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
            if (this.automat.stateToDomainTag[state] != DomainTag.NOT_FINAL) {
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
            if (tag == DomainTag.N_STR) {
                this.compiler.addMessage(false, end!, 'expected symbol: \'');
            }
            const token = Token.createFromDomainTag(finalAttr, tag, new Fragment(start, end!));
            // if (!token) {
            //     this.compiler.addMessage(true, start, 'unexpected token');
            // }
            return token;
        } else {
            await this.p.next();
        }

        return undefined;
    }

    async nextChar(): Promise<string> {
        try {
            const ch = await new Promise<ReadFileT>((resolve, reject) => {
                fs.read(this.fd, Buffer.alloc(1), 0, 1, -1, (err, bytesRead, buffer) => {
                    // console.log(err, bytesRead, buffer.toString());
                    if (err) {
                        reject(err);
                    }
                    resolve({
                        bytesRead,
                        ch: buffer.toString()
                    })
                });
            })
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
