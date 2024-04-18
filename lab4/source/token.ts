import { Fragment } from './fragment';

export enum DomainTag {
    STRING,
    NUMBER
}


export abstract class Token {
    abstract tag: DomainTag;
    abstract coords: Fragment;

    abstract getString(): string;
}

export class StringToken extends Token {

    constructor(public attr: string, public tag: DomainTag, public coords: Fragment) {
        super();
    }

    getString(): string {
        return `${DomainTag[DomainTag.STRING]} ${this.coords.getString()}: ${this.attr}`;
    }
}

export class NumberToken extends Token {
    constructor(public attr: number, public tag: DomainTag, public coords: Fragment) {
        super();
    }

    getString(): string {
        return `${DomainTag[DomainTag.NUMBER]} ${this.coords.getString()}: ${this.attr}`;
    }

}
