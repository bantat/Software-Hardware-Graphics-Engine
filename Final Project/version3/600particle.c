#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

/* Feel free to read from this struct's members, but don't write to them,
except through accessor functions. */
typedef struct particleGLMesh particleGLMesh;
struct particleGLMesh {
  GLuint vertNum, attrDim, vaoNum, attrNum;
  GLuint *attrDims;
  GLuint *vaos;
  GLuint buffers[1];
};

typedef struct partParticle partParticle;
struct partParticle {
  particleGLMesh *meshGL;
  GLuint vertNum;
  GLuint attrNum;
  GLuint attrDim;
  GLuint *attrDims;
  GLdouble *vertices;
  GLdouble *velocities;
};

/* Feel free to read from this struct's members, but don't write to them except
through the accessor functions. */
typedef struct particleNode particleNode;
struct particleNode {
  GLdouble rotation[3][3];
  GLdouble translation[3];
  GLuint unifDim;
  GLdouble *unif;
  particleGLMesh *particleGL;
  texTexture **tex;
  GLuint texNum;
};


typedef struct particleProgram particleProgram;
struct particleProgram {
	GLuint program;
	GLint *attrLocs;
  GLint colorLoc;
  GLint textureLoc;
  GLint viewingLoc, modelingLoc;
  GLint lightPosLoc, lightColLoc, lightAttLoc, lightDirLoc, lightCosLoc, camPosLoc;
};

/* attrLocs is meshGL->attrNum locations in the active shader program. index is
an integer between 0 and meshGL->voaNum - 1, inclusive. This function
initializes the VAO at that index in the meshGL's array of VAOs, so that the
VAO can render using those locations. */
void particleGLVAOInitialize(particleGLMesh *particleGL, GLuint index, GLint attrLocs[]) {
  glBindVertexArray(particleGL->vaos[index]);
  /* Make sure the intended shader program is active. (In a serious
  application, we might switch among several shaders rapidly.) Connect our
  attribute array to the attributes in the vertex shader. */

  GLint offset_num = 0;

  for (GLuint i = 0; i < particleGL->attrNum; i++) {
    glEnableVertexAttribArray(attrLocs[i]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, particleGL->buffers[0]);

  for (GLuint i = 0; i < particleGL->attrNum; i++) {
    GLuint attrDim = particleGL->attrDims[i];
    glVertexAttribPointer(attrLocs[i], attrDim, GL_DOUBLE, GL_FALSE,
                          particleGL->attrDim * sizeof(GLdouble),
                          BUFFER_OFFSET(offset_num * sizeof(GLdouble)));

    offset_num += attrDim;
  }

  glBindVertexArray(0);
}

int particleCPUInitialize(particleGLMesh *particleGL, partParticle *particleCPU,
    meshMesh *mesh, GLuint attrNum, GLuint attrDims[]) {
  particleCPU->meshGL = particleGL;
  particleCPU->attrDims = (GLuint *)malloc((attrNum) * sizeof(GLuint));
  if (particleCPU->attrDims == NULL) {
      return 1;
  }
  for (int i = 0; i < attrNum; i++) {
      particleCPU->attrDims[i] = attrDims[i];
  }
  particleCPU->vertices = (GLdouble *)malloc(mesh->vertNum * mesh->attrDim * sizeof(GLdouble));
  for (int i = 0; i < (mesh->vertNum * mesh->attrDim); i++) {
    particleCPU->vertices[i] = mesh->vert[i];
  }
  particleCPU->velocities = (GLdouble *)malloc(mesh->vertNum * 3 * sizeof(GLdouble));
  particleCPU->vertNum = mesh->vertNum;
  particleCPU->attrNum = attrNum;
  particleCPU->attrDim = mesh->attrDim;
  return 0;
}

void particleCPUDestroy(partParticle *particleCPU) {
  free(particleCPU->attrDims);
  free(particleCPU->vertices);
  free(particleCPU->velocities);
}

void particleUpdate(partParticle *particleCPU) {
  GLdouble velo[3];
  int attrIndex = 0;
  for (int i = 0; i < particleCPU->vertNum; i++) {
    velo[0] = particleCPU->velocities[i * 3];
    velo[1] = particleCPU->velocities[(i * 3) + 1];
    velo[2] = particleCPU->velocities[(i * 3) + 2];
    particleCPU->vertices[attrIndex] = particleCPU->vertices[attrIndex] + velo[0];
    particleCPU->vertices[attrIndex + 1] = particleCPU->vertices[attrIndex + 1] + velo[1];
    particleCPU->vertices[attrIndex + 2] = particleCPU->vertices[attrIndex + 2] + velo[2];
    attrIndex += particleCPU->attrDim;
  }
  glBindBuffer(GL_ARRAY_BUFFER, particleCPU->meshGL->buffers[0]);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
  particleCPU->vertNum * particleCPU->meshGL->attrDim * sizeof(GLdouble), (GLvoid *)particleCPU->vertices);
}

/* Copies the unifDim-dimensional vector from unif into the node. */
void particleSetVelocities(partParticle *particleCPU, GLdouble velo[]) {
  for (int i = 0; i < (3 * particleCPU->vertNum); i++) {
    particleCPU->velocities[i] = velo[i];
  }
}

/* Sets one uniform in the node, based on its index in the unif array. */
void particleSetOneVelocity(partParticle *particleCPU, int index, GLdouble velo) {
  particleCPU->velocities[index] = velo;
}

/* Initializes an OpenGL mesh from a non-OpenGL mesh. vaoNum is the number of
vertex array objects attached to this mesh storage. Typically vaoNum equals the
number of distinct shader programs that will need to draw the mesh. Returns 0
on success, non-zero on failure. */
int particleGLInitialize(particleGLMesh *particleGL, meshMesh *mesh, GLuint attrNum,
        GLuint attrDims[], GLuint vaoNum) {
    particleGL->attrDims = (GLuint *)malloc((attrNum + vaoNum) * sizeof(GLuint));
    if (particleGL->attrDims == NULL)
        return 1;
    for (int i = 0; i < attrNum; i += 1)
        particleGL->attrDims[i] = attrDims[i];
    particleGL->vaos = &particleGL->attrDims[attrNum];
    glGenVertexArrays(vaoNum, particleGL->vaos);
    particleGL->vaoNum = vaoNum;
    particleGL->attrNum = attrNum;
    particleGL->vertNum = mesh->vertNum;
    particleGL->attrDim = mesh->attrDim;
    glGenBuffers(1, particleGL->buffers);
    glBindBuffer(GL_ARRAY_BUFFER, particleGL->buffers[0]);
    glBufferData(GL_ARRAY_BUFFER,
        particleGL->vertNum * particleGL->attrDim * sizeof(GLdouble),
        (GLvoid *)(mesh->vert), GL_DYNAMIC_DRAW);
    return 0;
}

/* Renders the already-initialized OpenGL mesh. attrDims is an array of length
attrNum. For each i, its ith entry is the dimension of the ith attribute
vector. Similarly, attrLocs is an array of length attrNum, giving the location
of the ith attribute in the active OpenGL shader program. */
void particleGLRender(particleGLMesh *particleGL, GLuint vaoIndex) {
  /* Make sure the intended shader program is active. (In a serious
  application, we might switch among several shaders rapidly.) Connect our
  attribute array to the attributes in the vertex shader. */
  glBindVertexArray(particleGL->vaos[vaoIndex]);
  glDrawArrays(GL_POINTS, 0, particleGL->vertNum);
  glBindVertexArray(0);
}

/* Deallocates the resources backing the initialized OpenGL mesh. */
void particleGLDestroy(particleGLMesh *particleGL) {
  glDeleteBuffers(2, particleGL->buffers);
  glDeleteVertexArrays(particleGL->vaoNum, particleGL->vaos);
  free(particleGL->attrDims);
}

/* Initializes a sceneNode struct. The translation and rotation are initialized
to trivial values. The user must remember to call sceneDestroy or
sceneDestroyRecursively when finished. Returns 0 if no error occurred. */
int particleInitialize(particleNode *node, GLuint unifDim, GLuint texNum,
                    particleGLMesh *mesh) {
  node->unif = (GLdouble *)malloc(unifDim * sizeof(GLdouble) +
                                  texNum * sizeof(texTexture *));
  if (node->unif == NULL) return 1;
  node->tex = (texTexture **)&(node->unif[unifDim]);
  mat33Identity(node->rotation);
  vecSet(3, node->translation, 0.0, 0.0, 0.0);
  node->unifDim = unifDim;
  node->particleGL = mesh;
  node->texNum = texNum;
  return 0;
}

/* Deallocates the resources backing this scene node. Does not destroy the
resources backing the mesh, etc. */
void particleDestroy(particleNode *node) {
  if (node->unif != NULL) free(node->unif);
  node->unif = NULL;
}

/*** Accessors ***/

/* Copies the unifDim-dimensional vector from unif into the node. */
void particleSetUniform(particleNode *node, double unif[]) {
  vecCopy(node->unifDim, unif, node->unif);
}

/* Sets one uniform in the node, based on its index in the unif array. */
void particleSetOneUniform(particleNode *node, int index, double unif) {
  node->unif[index] = unif;
}

/* Copies the unifDim-dimensional vector from unif into the node. */
void particleSetTexture(particleNode *node, texTexture *texList[]) {
  for (GLuint i = 0; i < node->texNum; i++) {
    node->tex[i] = texList[i];
  }
}

/* Sets one uniform in the node, based on its index in the unif array. */
void particleSetOneTexture(particleNode *node, int index, texTexture *tex) {
  node->tex[index] = tex;
}

/* Sets the node's rotation. */
void particleSetRotation(particleNode *node, GLdouble rot[3][3]) {
  vecCopy(9, (GLdouble *)rot, (GLdouble *)(node->rotation));
}

/* Sets the node's translation. */
void particleSetTranslation(particleNode *node, GLdouble transl[3]) {
  vecCopy(3, transl, node->translation);
}

/* Sets the particle's mesh. */
void particleSetMesh(particleNode *node, particleGLMesh *mesh) { node->particleGL = mesh; }

/* Renders the node, its younger siblings, and their descendants. parent is the
modeling matrix at the parent of the node. If the node has no parent, then this
matrix is the 4x4 identity matrix. Loads the modeling transformation into
modelingLoc. The attribute information exists to be passed to particleGLRender. The
uniform information is analogous, but sceneRender loads it, not particleGLRender. */
void particleRender(particleNode *node, GLint modelingLoc,
                 GLuint unifNum, GLuint unifDims[], GLint unifLocs[],
                 GLuint vaoIndex,
                 GLint textureLocs[]) {
  /* Set the uniform modeling matrix. */

  // printf("node->tex: %f,%f\n", node->tex[0]->openGL, node->tex[1]->openGL);

  GLdouble model[4][4];
  mat44Isometry(node->rotation, node->translation, model);
  GLfloat unif_mat[4][4];
  mat44OpenGL(model, unif_mat);
  glUniformMatrix4fv(modelingLoc, 1, GL_FALSE, (GLfloat *)unif_mat);
  /* !! */
  GLuint offset_num = 0;
  /* Set the other uniforms. The casting from double to float is annoying. */
  for (GLuint i = 0; i < unifNum; i++) {
    GLuint unifDim = unifDims[i];
    if (unifDim == 1) {
      GLfloat values[1];
      vecOpenGL(unifDim, &node->unif[offset_num], values);
      glUniform1fv(unifLocs[i], 1, values);
    } else if (unifDim == 2) {
      GLfloat values[2];
      vecOpenGL(unifDim, &node->unif[offset_num], values);
      glUniform2fv(unifLocs[i], 1, values);
    } else if (unifDim == 3) {
      GLfloat values[3];
      vecOpenGL(unifDim, &node->unif[offset_num], values);
      glUniform3fv(unifLocs[i], 1, values);
    } else if (unifDim == 4) {
      GLfloat values[4];
      vecOpenGL(unifDim, &node->unif[offset_num], values);
      glUniform4fv(unifLocs[i], 1, values);
    }
    offset_num = offset_num + unifDim;
  }
  /* !! */
  /* Render the mesh, the children, and the younger siblings. */

  for (GLuint i = 0; i < node->texNum; i++) {
    if (i == 0) {
      texRender(node->tex[i], GL_TEXTURE0, i, textureLocs[i]);
    } else if (i == 1) {
      texRender(node->tex[i], GL_TEXTURE1, i, textureLocs[i]);
    } else if (i == 2) {
      texRender(node->tex[i], GL_TEXTURE2, i, textureLocs[i]);
    } else if (i == 3) {
      texRender(node->tex[i], GL_TEXTURE3, i, textureLocs[i]);
    } else if (i == 4) {
      texRender(node->tex[i], GL_TEXTURE4, i, textureLocs[i]);
    } else if (i == 5) {
      texRender(node->tex[i], GL_TEXTURE5, i, textureLocs[i]);
    } else if (i == 6) {
      texRender(node->tex[i], GL_TEXTURE6, i, textureLocs[i]);
    } else if (i == 7) {
      texRender(node->tex[i], GL_TEXTURE7, i, textureLocs[i]);
    }
  }
  particleGLRender(node->particleGL, vaoIndex);
  for (GLuint i = 0; i < node->texNum; i++) {
    if (i == 0) {
      texUnrender(node->tex[i], GL_TEXTURE0);
    } else if (i == 1) {
      texUnrender(node->tex[i], GL_TEXTURE1);
    } else if (i == 2) {
      texUnrender(node->tex[i], GL_TEXTURE2);
    } else if (i == 3) {
      texUnrender(node->tex[i], GL_TEXTURE3);
    } else if (i == 4) {
      texUnrender(node->tex[i], GL_TEXTURE4);
    } else if (i == 5) {
      texUnrender(node->tex[i], GL_TEXTURE5);
    } else if (i == 6) {
      texUnrender(node->tex[i], GL_TEXTURE6);
    } else if (i == 7) {
      texUnrender(node->tex[i], GL_TEXTURE7);
    }
  }
}


int particleProgramInitialize(particleProgram *prog, GLuint attrNum) {
	prog->attrLocs = (GLint *)malloc(attrNum * sizeof(GLint));
	if (prog->attrLocs == NULL) {
		fprintf(stderr, "particleProgramInitialize: malloc failed.\n");
		return 1;
	}
	/* The vertex shader produces just geometry --- no texture coordinates,
	lighting information, or any of that. */
  GLchar vertexCode[] = "\
		#version 140\n\
		uniform mat4 viewing;\
		uniform mat4 modeling;\
		in vec3 position;\
		in vec2 texCoords;\
		in vec3 normal;\
		out vec3 fragPos;\
		out vec3 normalDir;\
		out vec2 st;\
		void main(void) {\
			mat4 scaleBias = mat4(\
				0.5, 0.0, 0.0, 0.0, \
				0.0, 0.5, 0.0, 0.0, \
				0.0, 0.0, 0.5, 0.0, \
				0.5, 0.5, 0.5, 1.0);\
			vec4 worldPos = modeling * vec4(position, 1.0);\
			gl_Position = viewing * worldPos;\
      gl_PointSize = 4;\
			fragPos = vec3(worldPos);\
			normalDir = vec3(modeling * vec4(normal, 0.0));\
			st = st;\
		}";
	GLchar fragmentCode[] = "\
		#version 140\n\
    uniform sampler2D texture0;\
		uniform vec3 color;\
		uniform vec3 camPos;\
		uniform vec3 lightPos;\
		uniform vec3 lightCol;\
		uniform vec3 lightAtt;\
		uniform vec3 lightAim;\
		uniform float lightCos;\
		in vec3 fragPos;\
		in vec3 normalDir;\
		in vec2 st;\
		out vec4 fragColor;\
		void main(void) {\
			vec3 diffuse = vec3(color);\
			vec3 litDir = normalize(lightPos - fragPos);\
			float diffInt, specInt = 0.0;\
			if (dot(lightAim, -litDir) < lightCos)\
				diffInt = 0.0;\
			else\
				diffInt = 1.0;\
			vec3 diffRefl = max(0.2, diffInt) * lightCol * diffuse;\
			fragColor = vec4(diffuse, 1.0);\
		}";
	prog->program = makeProgram(vertexCode, fragmentCode);
	if (prog->program == 0) {
		free(prog->attrLocs);
		return 2;
	}
	/* Set up the locations. Only one of the attribute locations is meaningful.
	The others are harmless dummy locations. */
	glUseProgram(prog->program);
	prog->attrLocs[0] = glGetAttribLocation(prog->program, "position");
	for (int i = 1; i < attrNum; i += 1)
		prog->attrLocs[i] = -1;
	prog->viewingLoc = glGetUniformLocation(prog->program, "viewing");
	prog->modelingLoc = glGetUniformLocation(prog->program, "modeling");
  prog->textureLoc = glGetUniformLocation(prog->program, "texture0");
  prog->colorLoc = glGetUniformLocation(prog->program, "color");
  prog->camPosLoc = glGetUniformLocation(prog->program, "camPos");
  prog->lightPosLoc = glGetUniformLocation(prog->program, "lightPos");
  prog->lightColLoc = glGetUniformLocation(prog->program, "lightCol");
  prog->lightAttLoc = glGetUniformLocation(prog->program, "lightAtt");
  prog->lightDirLoc = glGetUniformLocation(prog->program, "lightAim");
  prog->lightCosLoc = glGetUniformLocation(prog->program, "lightCos");
	return 0;
}

/* Deallocates the resources backing the shadow program. */
void particleProgramDestroy(particleProgram *prog) {
	free(prog->attrLocs);
	glDeleteProgram(prog->program);
}
