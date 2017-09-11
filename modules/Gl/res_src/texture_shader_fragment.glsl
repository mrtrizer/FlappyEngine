#if defined(GL_ES)
precision mediump float;
#endif
uniform sampler2D uTex;
uniform vec4 uColor;
varying vec2 vTexCoord;
void main() {
#if defined(IOS)
   gl_FragColor = texture2D(uTex,vTexCoord).rgba * uColor;
#else
   gl_FragColor = texture2D(uTex,vTexCoord).bgra * uColor;
#endif
}
