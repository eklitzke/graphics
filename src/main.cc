#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint program, vbo_triangle;
GLint attribute_coord2d;

std::string ReadFile(const std::string &filename) {
  std::ifstream f(filename);
  return std::string(std::istreambuf_iterator<char>(f),
                     std::istreambuf_iterator<char>());
}

void PrintLog(GLuint object) {
  GLint log_length = 0;
  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else if (glIsProgram(object)) {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else {
    std::cerr << "PrintLog: not a shader or a program\n";
    return;
  }

  std::unique_ptr<char[]> log(new char[log_length]);
  if (glIsShader(object)) {
    glGetShaderInfoLog(object, log_length, NULL, log.get());
  } else if (glIsProgram(object)) {
    glGetProgramInfoLog(object, log_length, NULL, log.get());
  }
  std::cerr << log.get();
}

GLuint CreateShader(const std::string &filename, GLenum type) {
  const std::string source = ReadFile(filename);
  if (!source.length()) {
    std::cerr << "failed to read " << filename << "\n";
    return 0;
  }

  GLuint res = glCreateShader(type);
  const GLchar* sources[2] = {
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
    "#define GLES2\n",
#else
    "#version 120\n",
#endif
    source.c_str()
  };
  glShaderSource(res, 2, sources, NULL);

  glCompileShader(res);

  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE) {
    std::cerr << "%s: " << filename;
    PrintLog(res);
    glDeleteShader(res);
    return 0;
  }
  return res;
}

int init_resources(void) {
  GLuint vs, fs;
  if ((vs = CreateShader("src/triangle.v.glsl", GL_VERTEX_SHADER)) == 0) {
    return 0;
  }
  if ((fs = CreateShader("src/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) {
    return 0;
  }

  GLint link_ok = GL_FALSE;
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (link_ok == GL_FALSE) {
    std::cerr << "glLinkProgram: ";
    PrintLog(program);
    return 0;
  }

  const char *attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    std::cerr << "could not bind attribute: " << attribute_name << "\n";
    return 0;
  }

  GLfloat triangle_vertices[] = {
    0.0,  0.8,
    -0.8, -0.8,
    0.8, -0.8,
  };
  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

  return 1;
}

void onDisplay() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glEnableVertexAttribArray(attribute_coord2d);
  glVertexAttribPointer(
    attribute_coord2d,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    0);

  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(attribute_coord2d);

  glutSwapBuffers();
}

void free_resources() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(2, 0);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutCreateWindow("hello world");

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    std::cerr << "Error: " << glewGetErrorString(glew_status) << "\n";
    return 1;
  }

  if (!GLEW_VERSION_2_0) {
    std::cerr << "Error: your graphic card does not support OpenGL 2.0\n";
    return 1;
  }

  if (init_resources()) {
    glutDisplayFunc(onDisplay);
    glutMainLoop();
  }

  free_resources();
  return 0;
}
