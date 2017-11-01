class TestComponent2 extends Component {
    constructor() {
        super();
        log('Constructor 2');
        log(this.initialized.toString());
    }
    init() {
        log('Init');
    }
    deinit() {
        log('Deinit');
    }
    update(dt) {
        this.entity.jsComponent('TestComponent1').hello();
        let transform = this.entity.component('TransformComponent');
        let newPos = {'x':1,'y':2,'z':3};
        transform.setPos(newPos);
        log(transform.pos().z);
        log(this.entity.jsComponent('TestComponent1').name);
        this.entity.component('TransformComponent').setAngle2DRad(1.5);
        log('Angle: ' + this.entity.component('TransformComponent').angle2DRad());
    }
}
