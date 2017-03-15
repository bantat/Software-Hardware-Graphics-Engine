Particle System Implementation by Sabastian Mugazambi and Tore Banta
*__________________________________________________________________*

We explored how our engine can be customized to handle particle systems in addition to its current capabilities. 


1.0 -  Goal and Demo Overview
*___________________________*

The goal was to have a demo executable that would demonstrate how our customized system can initialize particle clouds,
scene nodes and add effects at a particle by particle level. As we worked on the project we developed 5 versions of our
system and each version marks a milestone of the project. The source code in the root folder includes each version of the
project in its respective folder. 

The C file '600mainParticles.c' in each version folder poses as our demo for each milestone stage of the project. Below we
outline how to run the executable files and in 4.0 we give a change log for what changed between versions. 


2.0 - How to compile and run our executables
*__________________________________________*

The final version of our project is in the folder 'Final Version'. Please run this one for a complete demo. 

Within each version folder;

On macOS, compile with...
    clang 600mainParticles.c /usr/local/gl3w/src/gl3w.o -lglfw -framework OpenGL -framework CoreFoundation

Run with...
	./a.out


3.0 - Files & Functions Added/Changed
*___________________________________*

At every version stage of the project we edited or added new functions within the following files; 

'600mainParticles.c'
	void handleKey(GLFWwindow *window, int key, int scancode, int action,int mods)	//Edit;
	int initializeScene(void)	//Edit;
	void destroyScene(void)	//Edit; 
	int particlesInitialize(void)	//New;
	void render(void)	//Edit;
	int main(void)	//Edit
	
'600particle.c'
	struct particleGLMesh	//New;
	struct partParticle	//New;
	struct particleNode	//New;
	struct particleProgram	//New;

	void particleGLVAOInitialize(particleGLMesh *particleGL, GLuint index, GLint attrLocs[])	//New;  
	int particleCPUInitialize(particleGLMesh *particleGL, partParticle *particleCPU,meshMesh *mesh, GLuint attrNum, GLuint attrDims[])	//New;
	void particleCPUDestroy(partParticle *particleCPU)	//New;
	void particleUpdate(partParticle *particleCPU)	//New;
	void particleSetVelocities(partParticle *particleCPU, GLdouble velo[])	//New;
	void particleSetOneVelocity(partParticle *particleCPU, int index, GLdouble velo)	//New;
	int particleGLInitialize(particleGLMesh *particleGL, meshMesh *mesh, GLuint attrNum, GLuint attrDims[], GLuint vaoNum)	//New;
	void particleGLRender(particleGLMesh *particleGL, GLuint vaoIndex)	//New;
	void particleGLDestroy(particleGLMesh *particleGL)	//New;
	int particleInitialize(particleNode *node, GLuint unifDim, GLuint texNum, particleGLMesh *mesh)	//New;
	void particleDestroy(particleNode *node)	//New;
	void particleSetUniform(particleNode *node, double unif[])	//New;
	void particleSetOneUniform(particleNode *node, int index, double unif)	//New;
	void particleSetTexture(particleNode *node, texTexture *texList[])	//New;
	void particleSetOneTexture(particleNode *node, int index, texTexture *tex)	//New;
	void particleSetRotation(particleNode *node, GLdouble rot[3][3])	//New;
	void particleSetTranslation(particleNode *node, GLdouble transl[3])	//New;
	void particleSetMesh(particleNode *node, particleGLMesh *mesh)	//New;
	void particleRender(particleNode *node, GLint modelingLoc, GLuint unifNum, GLuint unifDims[], GLint unifLocs[], GLuint vaoIndex, GLint textureLocs[]) //New;
	int particleProgramInitialize(particleProgram *prog, GLuint attrNum)	//New;
	void particleProgramDestroy(particleProgram *prog)	//New;

'580mesh.c'
	int range_rand(int min_num, int max_num)	//New;
	int meshInitializeRainCloud(meshMesh *mesh, GLuint length, GLuint width, GLuint height, GLuint density, GLuint attrDim)	//New;


4.0 - Change log for different versions
*_____________________________________*

'version1'
	render existing meshes as points rather than triangles
	and a simple gradient texture
	same shader program

'version2'
	Add velocity and transformations to particles, update buffer while rendering
	Remove texture and use color uniform for particles
	Add separate shader program for particle system

'version3'
	Create a cloud like mesh of particles at some given height	
	Add particles to CPU
	Add velocities and particle properties to CPU

'version4'
	Add a texture to the particles, alpha blending/transparency
	Make particles with a downward velocity
	Add particle size attenuation to emulate depth within the scene
	Add particle mesh modding (%) so that the particles are recycled

'Final Version'
	Randomize velocities at a particle by particle level
	Scale the particles to a more realistic particle / snow flake size
	Code clean up and testing


