class TrivialComponent extends Component {
    multiply(numA, numB) {
        return numA * numB;
    }
    sumArray(numbers) {
        let result = 0.0;
        for (const i in numbers) {
            result += numbers[i];
        }
        return result;
    }
    getHelloWorld() {
        return "Hello, world!"
    }
    splitStr(str) {
        return str.split(' ');
    }
    printStr(str) {
        log(str);
    }
}
