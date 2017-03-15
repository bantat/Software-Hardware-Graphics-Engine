GLchar vertexCode[] = "\
  #version 140\n\
  uniform mat4 viewing;\
  uniform mat4 modeling;\
  uniform vec3 camPos;\
  in vec3 position;\
  in vec2 texCoords;\
  in vec3 normal;\
  out vec3 fragPos;\
  out vec3 normalDir;\
  out vec2 st;\
  const float minPointScale = 0.1;\
  const float maxPointScale = 0.7;\
  void main(void) {\
    mat4 scaleBias = mat4(\
      0.5, 0.0, 0.0, 0.0, \
      0.0, 0.5, 0.0, 0.0, \
      0.0, 0.0, 0.5, 0.0, \
      0.5, 0.5, 0.5, 1.0);\
    float point_scale;\
    float size = 2.0;\
    point_scale = (distance(position, camPos)/8000.0);\
    point_scale = max(point_scale, minPointScale);\
    point_scale = min(point_scale, maxPointScale);\
    vec4 worldPos = modeling * vec4(position, 1.0);\
    vec4 viewPos = viewing * worldPos;\
    gl_Position = viewPos;\
    gl_PointSize = viewPos[3] * point_scale;\
    fragPos = vec3(worldPos);\
    normalDir = vec3(modeling * vec4(normal, 0.0));\
    st = texCoords;\
  }";
