class TestComponent1 extends Component {
    constructor() {
        super();
        log('Constructor 1');
        log(this.initialized.toString());
        this.name = 'Vasia';
    }
    init() {
        log('Init');
    }
    deinit() {
        log('Deinit');
    }
    hello() {
        log('Hello, Im ' + this.name);
    }
    update(dt) {
        log('TestComponent1 update');
    }
}
