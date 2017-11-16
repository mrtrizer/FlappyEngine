class CppAccessComponent extends Component {
    setAngleToTransform(angle) {
        this.entity.component('TransformComponent').setAngle2DRad(angle);
    }
    getAngleFromTransform() {
        return this.entity.component('TransformComponent').angle2DRad();
    }
    setPosToTransform(pos) {
        this.entity.component('TransformComponent').setPos(pos);
    }
    getPosFromTransform() {
        return this.entity.component('TransformComponent').pos();
    }
}
