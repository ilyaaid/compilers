
export function isNumber(value: string): boolean {
    return /^[0-9]+$/.test(value);
}

export function code(char: string): number {
    return char.charCodeAt(0);
}

export function replace(arr: Array<any>, from: number, to: number, val: number) {
    arr.forEach((cval, ind) => {
        if (ind >= from && ind <= to) {
            arr[ind] = val;
        }
    })
}
