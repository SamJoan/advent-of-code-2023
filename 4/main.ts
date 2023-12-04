import { promises as fs } from "fs"

class Card {
    winningNumbers: Object = {};
    numbers: Array<string> = [];
    score: number = 0;
}

let cards:Array<Card> = [];

async function main() {
    //let data = await fs.readFile('input_test.txt');
    let data = await fs.readFile('input.txt');
    let lines = data.toString().split(/(?:\r\n|\r|\n)/g);   
    let total = 0;
    for(let line of lines) {
        if(line == '') break;

        let card = new Card();

        let splat = line.split(':');
        let cardStr = splat[0];
        let numbersStr = splat[1];

        splat = numbersStr.split("|");
        let winningNumbersStr = splat[0];
        let winningNumbers = winningNumbersStr.split(" ");
        let cardNumbersStr = splat[1];
        let cardNumbers = cardNumbersStr.split(" ");

        for(let number of winningNumbers) {
            if(number == '') continue;
            card.winningNumbers[number] = true;
        }

        for(let number of cardNumbers) {
            if(number == '') continue;
            card.numbers.push(number);
            if(number in card.winningNumbers) {
                if(card.score == 0) {
                    card.score = 1;
                } else {
                    card.score = card.score * 2;
                }
            }
        }

        total += card.score;

        console.log(`card.score ${card.score}, total ${total}`);
    }
}

main();
