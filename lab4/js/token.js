"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.NumberToken = exports.StringToken = exports.Token = exports.DomainTag = void 0;
var DomainTag;
(function (DomainTag) {
    DomainTag[DomainTag["STRING"] = 0] = "STRING";
    DomainTag[DomainTag["NUMBER"] = 1] = "NUMBER";
})(DomainTag || (exports.DomainTag = DomainTag = {}));
class Token {
}
exports.Token = Token;
class StringToken extends Token {
    constructor(attr, tag, coords) {
        super();
        this.attr = attr;
        this.tag = tag;
        this.coords = coords;
    }
    getString() {
        return `${DomainTag[DomainTag.STRING]} ${this.coords.getString()}: ${this.attr}`;
    }
}
exports.StringToken = StringToken;
class NumberToken extends Token {
    constructor(attr, tag, coords) {
        super();
        this.attr = attr;
        this.tag = tag;
        this.coords = coords;
    }
    getString() {
        return `${DomainTag[DomainTag.NUMBER]} ${this.coords.getString()}: ${this.attr}`;
    }
}
exports.NumberToken = NumberToken;
