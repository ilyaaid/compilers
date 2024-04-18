import { Fragment } from './fragment';

export enum DomainTag {
    ID,
    DO,
    WHILE,
    NUM,
    L_BR,
    R_BR,
    BRS,
    N_STR,
    STR,
    NOT_FINAL,
}

export abstract class Token {
    abstract tag: DomainTag;
    abstract coords: Fragment;

    static createFromDomainTag(attr: string, tag: DomainTag, coords: Fragment): Token | undefined {
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

    getString(): string {
        return `${DomainTag[this.tag]} ${this.coords.getString()}`;
    };
}

export class IDToken extends Token {
    constructor(public attr: string, public tag: DomainTag, public coords: Fragment) {
        super();
    }

    getString(): string {
        return super.getString() + `: ${this.attr}`;
    }
}

export class DOToken extends Token {
    constructor(public tag: DomainTag, public coords: Fragment) {
        super();
    }

}

export class WHILEToken extends Token {
    constructor(public tag: DomainTag, public coords: Fragment) {
        super();
    }

}

export class NUMToken extends Token {
    constructor(public attr: number, public tag: DomainTag, public coords: Fragment) {
        super();
    }

    getString(): string {
        return super.getString() + `: ${this.attr}`;
    }
}

export class L_BRToken extends Token {
    constructor(public tag: DomainTag, public coords: Fragment) {
        super();
    }
}

export class R_BRToken extends Token {
    constructor(public tag: DomainTag, public coords: Fragment) {
        super();
    }
}

export class BRSToken extends Token {
    constructor(public tag: DomainTag, public coords: Fragment) {
        super();
    }
}


// export class N_STRToken extends Token {
//     constructor(public attr: string, public tag: DomainTag, public coords: Fragment) {
//         super();
//     }

//     getString(): string {
//         return super.getString() + `: ${this.attr}`;
//     }
// }

export class STRToken extends Token {

    constructor(public attr: string, public tag: DomainTag, public coords: Fragment) {
        super();
    }

    getString(): string {
        return super.getString() + `: ${this.attr}`;
    }
}
