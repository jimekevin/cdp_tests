uniform mat4 view;
uniform mat4 projection;

attribute vec3 vertex;
attribute vec3 color;

varying vec4 vertColor;

void main() {
  gl_Position = projection * view * vec4(vertex, 1.0);
  //gl_Position = vec4(vertex, 1.0);
  gl_PointSize = 2.0;
  //vertColor = vec4(1.0, 0.5, 0.0, 1.0);
  vertColor = vec4(color, 1.0);
}