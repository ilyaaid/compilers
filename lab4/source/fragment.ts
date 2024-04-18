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

    async nextElement(movePos: boolean = true): Promise<string> {
        if (movePos) {
            this.nextPos();
        }
        if (this.ch_queue.length > 0) {
            return this.ch_queue.shift()!;
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

    async next(): Promise<void> {
        if (this.isNewLine()) {
            this.nextLine();
        }
        this.ch = await this.nextElement();
        if (this.ch == '\r') {
            const tmpCh = await this.nextElement(false);
            if (tmpCh == '\n') {
                this.nextPos();
                this.ch = tmpCh;
            } else {
                this.nextLine();
                this.ch_queue.push(tmpCh);
            }
        }
    }

    isNewLine(): boolean {
        return this.ch == '\n' || this.ch == '\r';
    }

    isWhiteSpace(): boolean {
        return this.ch.trim() == '';
    }
}


export class Fragment {
    constructor(public starting: Position, public following: Position) { }

    getString(): string {
        return `${this.starting.getString()}-${this.following.getString()}`;
    }
}
