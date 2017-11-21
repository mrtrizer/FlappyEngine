class JsAccessComponent extends Component {

    callCalcSum(val1, val2) {
        return this.entity().jsComponent('TestComponent1').calcSum(val1, val2);
    }

}
