import { Compiler } from './compiler';
import { ActivePosition, Fragment, Position } from './fragment';
import { StringToken, Token, DomainTag, NumberToken } from './token';
import fs from 'fs';
import { isNumber } from './utils';

export type ReadFileT = {
    bytesRead: number
    ch: string
}

export class Scanner {
    private fd: number;
    private p: ActivePosition;


    constructor(public compiler: Compiler) {
        this.p = new ActivePosition(0, -1, -1, this);
        this.fd = 0;
    }

    async init(path: string): Promise<void> {
        this.fd = fs.openSync(path, 'r');
        await this.p.next();
    }

    async nextToken(): Promise<Token | undefined> {
        let isValid = {
            ok: false,
            attempt: 0,
        };

        while (this.p.ch && isValid.ok == false) {
            if (this.p.ch == '"') {
                isValid.ok = true;
                const start = this.p.getNewPosition();
                let attr: string = '';
                await this.p.next();

                while (this.p.ch != '\"') {
                    if (this.p.ch == '\\') {
                        await this.p.next();
                        switch (this.p.ch) {
                            case 'n': { attr += '\n'; break; }
                            case 't': { attr += '\t'; break; }
                            case '\\': { attr += '\\'; break; }
                            case '"': { attr += '"'; break; }
                            default: {
                                this.compiler.addMessage(true,
                                    this.p.getNewPosition(),
                                    'unrecognized escape sequence');
                                break;
                            }
                        }
                    } else if (this.p.isNewLine()) {
                        this.compiler.addMessage(true, this.p.getNewPosition(), 'unexpected newline in string');
                        break;
                    } else {
                        attr += this.p.ch;
                    }
                    await this.p.next();
                }
                const newToken = new StringToken(attr, DomainTag.STRING, new Fragment(start, this.p.getNewPosition()));
                await this.p.next();
                return newToken;
            } else if (isNumber(this.p.ch)) {
                isValid.ok = true;
                const start = this.p.getNewPosition();
                let attr: number = 0;
                attr += Number(this.p.ch);
                attr *= 10;
                let end : Position = this.p.getNewPosition();
                await this.p.next();
                while (isNumber(this.p.ch) || this.p.ch == '_') {
                    if (isNumber(this.p.ch)) {
                        attr += Number(this.p.ch);
                        attr *= 10;
                    }
                    end = this.p.getNewPosition();
                    await this.p.next();
                }
                attr /= 10;
                return new NumberToken(attr, DomainTag.NUMBER, new Fragment(start, end));
            } else {
                if (!this.p.isWhiteSpace() && !this.p.isNewLine() && isValid.attempt == 0) {
                    this.compiler.addMessage(true, this.p.getNewPosition(), 'unexpected token');
                    isValid.attempt++;
                }
                await this.p.next();
            }
        }
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
