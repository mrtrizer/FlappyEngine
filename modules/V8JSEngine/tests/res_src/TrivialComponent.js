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
    extractZPosFromObject(obj) {
        log(typeof obj);
        log("x: " + obj.pos().x);
        this.objHolder = obj;
        return obj.pos().z;
    }
    extractZPosFromSavedObject() {
        log(typeof this.objHolder);
        log("x: " + this.objHolder.pos().x);
        return this.objHolder.pos().z;
    }
}
