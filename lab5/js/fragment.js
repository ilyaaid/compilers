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
    nextLine() {
        this.line++;
        this.pos = -1;
    }
    nextPos() {
        this.pos++;
        this.index++;
    }
    isEnd() {
        return this.ch == '';
    }
    async clearChQueue() {
        while (this.ch_queue.length > 0) {
            await this.nextElement();
        }
    }
    async nextElement(movePos = true) {
        if (!movePos) {
            const ch = await this.sc.nextChar();
            this.ch_queue.push(ch);
            return ch;
        }
        this.nextPos();
        if (this.ch_queue.length > 0) {
            return this.ch_queue.shift();
        }
        return await this.sc.nextChar();
    }
    async next(movePos = true) {
        if (this.isNewLine()) {
            this.nextLine();
        }
        this.ch = await this.nextElement(movePos);
    }
    async skipWhiteSpace() {
        while (this.isWhiteSpace()) {
            await this.next();
        }
    }
    isNewLine() {
        return this.ch == '\n';
    }
    isWhiteSpace() {
        return this.ch != '' && this.ch.trim() == '';
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
//# sourceMappingURL=fragment.js.map