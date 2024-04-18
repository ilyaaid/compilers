"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.STRToken = exports.BRSToken = exports.R_BRToken = exports.L_BRToken = exports.NUMToken = exports.WHILEToken = exports.DOToken = exports.IDToken = exports.Token = exports.DomainTag = void 0;
var DomainTag;
(function (DomainTag) {
    DomainTag[DomainTag["ID"] = 0] = "ID";
    DomainTag[DomainTag["DO"] = 1] = "DO";
    DomainTag[DomainTag["WHILE"] = 2] = "WHILE";
    DomainTag[DomainTag["NUM"] = 3] = "NUM";
    DomainTag[DomainTag["L_BR"] = 4] = "L_BR";
    DomainTag[DomainTag["R_BR"] = 5] = "R_BR";
    DomainTag[DomainTag["BRS"] = 6] = "BRS";
    DomainTag[DomainTag["N_STR"] = 7] = "N_STR";
    DomainTag[DomainTag["STR"] = 8] = "STR";
    DomainTag[DomainTag["NOT_FINAL"] = 9] = "NOT_FINAL";
})(DomainTag || (exports.DomainTag = DomainTag = {}));
class Token {
    static createFromDomainTag(attr, tag, coords) {
        switch (tag) {
            case DomainTag.ID: {
                return new IDToken(attr, tag, coords);
            }
            case DomainTag.NUM: {
                return new NUMToken(Number(attr), tag, coords);
            }
            case DomainTag.DO: {
                return new DOToken(tag, coords);
            }
            case DomainTag.WHILE: {
                return new WHILEToken(tag, coords);
            }
            case DomainTag.NUM: {
                return new NUMToken(Number(attr), tag, coords);
            }
            case DomainTag.L_BR: {
                return new L_BRToken(tag, coords);
            }
            case DomainTag.R_BR: {
                return new R_BRToken(tag, coords);
            }
            case DomainTag.BRS: {
                return new BRSToken(tag, coords);
            }
            case DomainTag.N_STR: {
                return new STRToken(attr.split(/\r?\n/)[0], tag, coords);
            }
            case DomainTag.STR: {
                return new STRToken(attr, tag, coords);
            }
            default: {
                return undefined;
            }
        }
    }
    getString() {
        return `${DomainTag[this.tag]} ${this.coords.getString()}`;
    }
    ;
}
exports.Token = Token;
class IDToken extends Token {
    constructor(attr, tag, coords) {
        super();
        this.attr = attr;
        this.tag = tag;
        this.coords = coords;
    }
    getString() {
        return super.getString() + `: ${this.attr}`;
    }
}
exports.IDToken = IDToken;
class DOToken extends Token {
    constructor(tag, coords) {
        super();
        this.tag = tag;
        this.coords = coords;
    }
}
exports.DOToken = DOToken;
class WHILEToken extends Token {
    constructor(tag, coords) {
        super();
        this.tag = tag;
        this.coords = coords;
    }
}
exports.WHILEToken = WHILEToken;
class NUMToken extends Token {
    constructor(attr, tag, coords) {
        super();
        this.attr = attr;
        this.tag = tag;
        this.coords = coords;
    }
    getString() {
        return super.getString() + `: ${this.attr}`;
    }
}
exports.NUMToken = NUMToken;
class L_BRToken extends Token {
    constructor(tag, coords) {
        super();
        this.tag = tag;
        this.coords = coords;
    }
}
exports.L_BRToken = L_BRToken;
class R_BRToken extends Token {
    constructor(tag, coords) {
        super();
        this.tag = tag;
        this.coords = coords;
    }
}
exports.R_BRToken = R_BRToken;
class BRSToken extends Token {
    constructor(tag, coords) {
        super();
        this.tag = tag;
        this.coords = coords;
    }
}
exports.BRSToken = BRSToken;
// export class N_STRToken extends Token {
//     constructor(public attr: string, public tag: DomainTag, public coords: Fragment) {
//         super();
//     }
//     getString(): string {
//         return super.getString() + `: ${this.attr}`;
//     }
// }
class STRToken extends Token {
    constructor(attr, tag, coords) {
        super();
        this.attr = attr;
        this.tag = tag;
        this.coords = coords;
    }
    getString() {
        return super.getString() + `: ${this.attr}`;
    }
}
exports.STRToken = STRToken;
//# sourceMappingURL=token.js.map