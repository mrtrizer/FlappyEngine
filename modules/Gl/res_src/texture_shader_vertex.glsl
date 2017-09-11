attribute vec2 aPosition;
attribute vec2 aTexCoord;
uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
varying vec2 vTexCoord;
void main() {
   vTexCoord = aTexCoord;
   gl_Position = uPMatrix * uMVMatrix * vec4(aPosition,0,1);
}
