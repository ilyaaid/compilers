import { Position } from './fragment';
import { Scanner } from './scanner';

export class Message {
    constructor(private isErr: boolean, private pos:Position, private text: string) { }

    getString() : string {
        return `${this.isErr ? 'error' : 'warning'} ${this.pos.getString()}: ${this.text}`
    }
}

export class Compiler {
    private messages: Message[];

    constructor() {
        this.messages = [];
    }

    getScanner(): Scanner {
        return new Scanner(this);
    }

    addMessage(isErr: boolean, pos:Position, text: string) {
        this.messages.push(new Message(isErr, pos, text));
    }

    getMessages(): Message[] {
        return this.messages;
    }
}
