import { promises as fs } from "fs"

class Card {
    winningNumbers: Object = {};
    numbers: Array<string> = [];
    numbersWon: number = 0; 
    nbCopies: number = 1;
    id: number;
}


async function main() {
    let cards:Array<Card> = [];
    //let data = await fs.readFile('input_test.txt');
    let data = await fs.readFile('input.txt');
    let lines = data.toString().split(/(?:\r\n|\r|\n)/g);   
    let nb = 0;
    for(let line of lines) {
        if(line == '') break;
        nb++;

        let card = new Card();
        card.id = nb;

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
                card.numbersWon += 1;
            }
        }

        cards.push(card);
    }


    let totalCopies = 0;
    for(let i in cards) {
        let card = cards[i];
        console.log(`Processing card ${card.id} (won ${card.numbersWon})`);
        totalCopies += card.nbCopies;

        if(card.numbersWon > 0) {
            for(let x = 0; x < card.nbCopies; x++) {
                for(let j = 1; j <= card.numbersWon; j++) {
                    let index = parseInt(i) + j;
                    if(index in cards) {
                        let copyCard = cards[index]
                        copyCard.nbCopies += 1;
                    }
                }
            }
        }
    }

    console.log(totalCopies);
}

main();
