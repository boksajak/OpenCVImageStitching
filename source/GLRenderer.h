
// Include GLEW
#include <GL/glew.h>
#include <GL/wglew.h>

// OpenGL include fiels
#include <gl/gl.h>
#include <gl/glu.h>

// Include GLM library
#include "glm.h"

#include "Camera.h"
#include "GLShaderManager.h"
#include "GLDebugRenderer.h"

class GLRenderer {
public:
	GLRenderer() { }
	~GLRenderer() { }

	void Initialize(HWND hWnd);
	void Deinitialize();

	void Render();

	void ProcessInput(UINT message, WPARAM wParam, LPARAM lParam);

private:

	GLDebugRenderer debugRenderer;

	HDC hDC;
	HGLRC hRC;

	Camera camera;

	void setPixelFormat(HDC hDC);

	void cameraForward(float amount);
	void cameraRight(float amount);
	void cameraUp(float amount);
	void cameraYaw(float amount);
	void cameraPitch(float amount);
	void cameraRoll(float amount);

};

