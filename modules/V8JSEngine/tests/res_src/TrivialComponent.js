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
        this.objHolder = obj;
        return obj.pos().z;
    }
    extractZPosFromSavedObject() {
        return this.objHolder.pos().z;
    }
    getSavedObject() {
        return this.objHolder;
    }

    createCppObject() {
        let cppObject = new TransformComponent();
        cppObject.setPos({x: 100, y:200, z:300});
        log("x:" + cppObject.pos().x);
        return cppObject;
    }
}
