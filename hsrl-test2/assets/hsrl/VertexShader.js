attribute vec4 aPosition;
attribute vec2 aTextureCoord;
varying vec2 vTextureCoord;

uniform mat4 projectionView;
uniform mat4 model;

void main() {
 gl_Position = projectionView * model * aPosition;
 vTextureCoord = aTextureCoord;
}
