"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.replace = exports.code = exports.isNumber = void 0;
function isNumber(value) {
    return /^[0-9]+$/.test(value);
}
exports.isNumber = isNumber;
function code(char) {
    return char.charCodeAt(0);
}
exports.code = code;
function replace(arr, from, to, val) {
    arr.forEach((cval, ind) => {
        if (ind >= from && ind <= to) {
            arr[ind] = val;
        }
    });
}
exports.replace = replace;
//# sourceMappingURL=utils.js.map