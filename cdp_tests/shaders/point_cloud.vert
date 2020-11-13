//uniform mat4 transform;

attribute vec3 vertex;
attribute vec3 color;

varying vec4 vertColor;

void main() {
  //gl_Position = transform * vertex;
  gl_Position = vec4(vertex, 1.0);
  gl_PointSize = 5.0;
  vertColor = vec4(color, 1.0);
}