attribute vec3 coor32d;
varying vec3 f_color;

void main(void) {
  gl_Position = vec4(coord2d, 0.0, 1.0);
  f_color = v_color;
}