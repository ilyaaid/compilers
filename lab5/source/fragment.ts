import { Scanner } from './scanner';

export class Position {
    constructor(public line: number, public pos: number, public index: number) { }

    getString(): string {
        return `(${this.line}, ${this.pos})`;
    }
}

export class ActivePosition extends Position {
    public ch: string;
    private ch_queue: string[];

    constructor(public line: number, public pos: number, public index: number, public sc: Scanner) {
        super(line, pos, index);
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

    isEnd(): boolean {
        return this.ch == '';
    }

    async clearChQueue() {
        while (this.ch_queue.length > 0) {
            await this.nextElement();
        }
    }

    async nextElement(movePos: boolean = true): Promise<string> {
        if (!movePos) {
            const ch = await this.sc.nextChar();
            this.ch_queue.push(ch);
            return ch;
        }
        this.nextPos();
        if (this.ch_queue.length > 0) {
            return this.ch_queue.shift()!;
        }
        return await this.sc.nextChar();
    }

    async next(movePos: boolean = true): Promise<void> {
        if (this.isNewLine()) {
            this.nextLine();
        }
        this.ch = await this.nextElement(movePos);
    }

    async skipWhiteSpace(): Promise<void> {
        while (this.isWhiteSpace()) {
            await this.next();
        }
    }

    isNewLine(): boolean {
        return this.ch == '\n';
    }

    isWhiteSpace(): boolean {
        return this.ch != '' && this.ch.trim() == '';
    }
}


export class Fragment {
    constructor(public starting: Position, public following: Position) { }

    getString(): string {
        return `${this.starting.getString()}-${this.following.getString()}`;
    }
}
