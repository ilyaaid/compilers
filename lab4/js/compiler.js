"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Compiler = exports.Message = void 0;
const scanner_1 = require("./scanner");
class Message {
    constructor(isErr, pos, text) {
        this.isErr = isErr;
        this.pos = pos;
        this.text = text;
    }
    getString() {
        return `${this.isErr ? 'error' : 'warning'} ${this.pos.getString()}: ${this.text}`;
    }
}
exports.Message = Message;
class Compiler {
    constructor() {
        this.messages = [];
    }
    getScanner() {
        return new scanner_1.Scanner(this);
    }
    addMessage(isErr, pos, text) {
        this.messages.push(new Message(isErr, pos, text));
    }
    getMessages() {
        return this.messages;
    }
}
exports.Compiler = Compiler;
