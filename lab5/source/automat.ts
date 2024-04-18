import { runInThisContext } from 'vm';
import { code, replace } from './utils';
import { DomainTag } from './token';

// массив состояний, где каждый элемент это массив объектов
type tableT = Array<Array<number>>

export class Automat {
    private table: tableT;
    private state: number;

    public readonly stateToDomainTag: Array<DomainTag> = [
        DomainTag.NOT_FINAL,
        DomainTag.ID,
        DomainTag.ID,
        DomainTag.DO,
        DomainTag.ID,
        DomainTag.ID,
        DomainTag.ID,
        DomainTag.ID,
        DomainTag.WHILE,
        DomainTag.NUM,
        DomainTag.L_BR,
        DomainTag.BRS,
        DomainTag.R_BR,
        DomainTag.NOT_FINAL,
        DomainTag.N_STR,
        DomainTag.N_STR,
        DomainTag.STR,
    ]

    constructor() {
        this.table = Array(17);
        this.fillTable();
        this.state = 0;
    }

    addLetersAndDigits(arr: Array<any>, state: number) {
        replace(arr, code('a'), code('z'), state);
        replace(arr, code('A'), code('Z'), state);
        replace(arr, code('0'), code('9'), state);
    }

    setState(state: number) {
        this.state = state;
    }

    nextState(char: string): number {
        this.state = this.table[this.state][code(char)];
        return this.state;
    }

    fillTable() {
        for (let i = 0; i < this.table.length; ++i) {
            const arr = new Array(128).fill(-1); // заполняем ловушками
            switch (i) {
                case 0: {
                    replace(arr, code('a'), code('z'), 1);
                    replace(arr, code('A'), code('Z'), 1);
                    arr[code('d')] = 2;
                    arr[code('w')] = 4;
                    replace(arr, code('0'), code('9'), 9);
                    arr[code('<')] = 10;
                    arr[code('>')] = 12;
                    arr[code('\'')] = 13;
                    break;
                }
                case 1: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 2: {
                    this.addLetersAndDigits(arr, 1);
                    arr[code('o')] = 3;
                    break;
                }
                case 3: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 4: {
                    this.addLetersAndDigits(arr, 1);
                    arr[code('h')] = 5;
                    break;
                }
                case 5: {
                    this.addLetersAndDigits(arr, 1);
                    arr[code('i')] = 6;
                    break;
                }
                case 6: {
                    this.addLetersAndDigits(arr, 1);
                    arr[code('l')] = 7;
                    break;
                }
                case 7: {
                    this.addLetersAndDigits(arr, 1);
                    arr[code('e')] = 8;
                    break;
                }
                case 8: {
                    this.addLetersAndDigits(arr, 1);
                    break;
                }
                case 9: {
                    replace(arr, code('0'), code('9'), 9);
                    break;
                }
                case 10: {
                    arr[code('>')] = 11;
                    break;
                }
                case 12: {
                    break;
                }
                case 13: {
                    arr.fill(13);
                    arr[code('\n')] = 14;
                    arr[code('\r')] = 15;
                    arr[code('\'')] = 16;
                }
                case 14: {
                    break;
                }
                case 15: {
                    arr[code('\n')] = 14;
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
