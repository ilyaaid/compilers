import { Compiler } from './compiler';


async function mainFunc() {
    let ch = await sc.nextToken()
    while (ch) {
        console.log(ch.getString());
        ch = await sc.nextToken();
    }
}

const cmp = new Compiler();
const sc = cmp.getScanner();
sc.init('test.txt').then(async () => {
    console.log('---tokens:---');
    await mainFunc();
    console.log('---messages:---');
    cmp.getMessages().forEach(mes => {
        console.log(mes.getString());
    })
})

