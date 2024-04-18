"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const compiler_1 = require("./compiler");
async function mainFunc() {
    let ch = await sc.nextToken();
    while (ch) {
        console.log(ch.getString());
        ch = await sc.nextToken();
    }
}
const cmp = new compiler_1.Compiler();
const sc = cmp.getScanner();
sc.init('input.txt').then(async () => {
    console.log('---tokens:---');
    await mainFunc();
    console.log('---messages:---');
    cmp.getMessages().forEach(mes => {
        console.log(mes.getString());
    });
});
