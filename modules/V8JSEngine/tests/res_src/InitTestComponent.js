class InitTestComponent extends Component {

    constructor() {
        super();
        this.constructorCounter = 1;
        this.initCounter = 0;
        this.deinitCounter = 0;
        this.updateCounter = 0;
    }

    init() {
        this.initCounter++;
    }

    deinit() {
        this.deinitCounter++;
    }

    update(dt) {
        this.updateCounter++;
    }
}
