"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Fragment = exports.ActivePosition = exports.Position = void 0;
class Position {
    constructor(line, pos, index) {
        this.line = line;
        this.pos = pos;
        this.index = index;
    }
    getString() {
        return `(${this.line}, ${this.pos})`;
    }
}
exports.Position = Position;
class ActivePosition extends Position {
    constructor(line, pos, index, sc) {
        super(line, pos, index);
        this.line = line;
        this.pos = pos;
        this.index = index;
        this.sc = sc;
        this.ch = '';
        this.ch_queue = [];
    }
    getNewPosition() {
        return new Position(this.line, this.pos, this.index);
    }
    async nextElement(movePos = true) {
        if (movePos) {
            this.nextPos();
        }
        if (this.ch_queue.length > 0) {
            return this.ch_queue.shift();
        }
        return await this.sc.nextChar();
    }
    nextLine() {
        this.line++;
        this.pos = -1;
    }
    nextPos() {
        this.pos++;
        this.index++;
    }
    async next() {
        if (this.ch == '\n') {
            this.nextLine();
        }
        this.ch = await this.nextElement();
        if (this.ch == '\r') {
            const tmpCh = await this.nextElement(false);
            if (tmpCh == '\n') {
                this.nextPos();
                this.ch = tmpCh;
            }
            else {
                this.nextLine();
                this.ch_queue.push(tmpCh);
            }
        }
    }
    isNewLine() {
        return this.ch == '\n' || this.ch == '\r';
    }
    isWhiteSpace() {
        return this.ch.trim() == '';
    }
}
exports.ActivePosition = ActivePosition;
class Fragment {
    constructor(starting, following) {
        this.starting = starting;
        this.following = following;
    }
    getString() {
        return `${this.starting.getString()}-${this.following.getString()}`;
    }
}
exports.Fragment = Fragment;
