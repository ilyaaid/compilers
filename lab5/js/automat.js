"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Automat = void 0;
const utils_1 = require("./utils");
const token_1 = require("./token");
class Automat {
    constructor() {
        this.stateToDomainTag = [
            token_1.DomainTag.NOT_FINAL,
            token_1.DomainTag.ID,
            token_1.DomainTag.ID,
            token_1.DomainTag.DO,
            token_1.DomainTag.ID,
            token_1.DomainTag.ID,
            token_1.DomainTag.ID,
            token_1.DomainTag.ID,
            token_1.DomainTag.WHILE,
            token_1.DomainTag.NUM,
            token_1.DomainTag.L_BR,
            token_1.DomainTag.BRS,
            token_1.DomainTag.R_BR,
            token_1.DomainTag.NOT_FINAL,
            token_1.DomainTag.N_STR,
            token_1.DomainTag.N_STR,
            token_1.DomainTag.STR,
        ];
        this.table = Array(17);
        this.fillTable();
        this.state = 0;
    }
    addLetersAndDigits(arr, state) {
        (0, utils_1.replace)(arr, (0, utils_1.code)('a'), (0, utils_1.code)('z'), state);
        (0, utils_1.replace)(arr, (0, utils_1.code)('A'), (0, utils_1.code)('Z'), state);
        (0, utils_1.replace)(arr, (0, utils_1.code)('0'), (0, utils_1.code)('9'), state);
    }
    setState(state) {
        this.state = state;
    }
    nextState(char) {
        this.state = this.table[this.state][(0, utils_1.code)(char)];
        return this.state;
    }
    fillTable() {
        for (let i = 0; i < this.table.length; ++i) {
            const arr = new Array(128).fill(-1); // заполняем ловушками
            switch (i) {
                case 0: {
                    (0, utils_1.replace)(arr, (0, utils_1.code)('a'), (0, utils_1.code)('z'), 1);
                    (0, utils_1.replace)(arr, (0, utils_1.code)('A'), (0, utils_1.code)('Z'), 1);
                    arr[(0, utils_1.code)('d')] = 2;
                    arr[(0, utils_1.code)('w')] = 4;
                    (0, utils_1.replace)(arr, (0, utils_1.code)('0'), (0, utils_1.code)('9'), 9);
                    arr[(0, utils_1.code)('<')] = 10;
                    arr[(0, utils_1.code)('>')] = 12;
                    arr[(0, utils_1.code)('\'')] = 13;
                    break;
                }
                case 1: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 2: {
                    this.addLetersAndDigits(arr, 1);
                    arr[(0, utils_1.code)('o')] = 3;
                    break;
                }
                case 3: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 4: {
                    this.addLetersAndDigits(arr, 1);
                    arr[(0, utils_1.code)('h')] = 5;
                    break;
                }
                case 5: {
                    this.addLetersAndDigits(arr, 1);
                    arr[(0, utils_1.code)('i')] = 6;
                    break;
                }
                case 6: {
                    this.addLetersAndDigits(arr, 1);
                    arr[(0, utils_1.code)('l')] = 7;
                    break;
                }
                case 7: {
                    this.addLetersAndDigits(arr, 1);
                    arr[(0, utils_1.code)('e')] = 8;
                    break;
                }
                case 8: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 9: {
                    (0, utils_1.replace)(arr, (0, utils_1.code)('0'), (0, utils_1.code)('9'), 9);
                    break;
                }
                case 10: {
                    arr[(0, utils_1.code)('>')] = 11;
                    break;
                }
                case 12: {
                    break;
                }
                case 13: {
                    arr.fill(13);
                    arr[(0, utils_1.code)('\n')] = 14;
                    arr[(0, utils_1.code)('\r')] = 15;
                    arr[(0, utils_1.code)('\'')] = 16;
                }
                case 14: {
                    break;
                }
                case 15: {
                    arr[(0, utils_1.code)('\n')] = 14;
                    break;
                }
                case 16: {
                    break;
                }
            }
            this.table[i] = arr;
        }
    }
}
exports.Automat = Automat;
//# sourceMappingURL=automat.js.map