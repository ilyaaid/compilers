"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.isNumber = void 0;
function isNumber(value) {
    return /^[0-9]+$/.test(value);
}
exports.isNumber = isNumber;
