#include "common.h"
#include "GLRenderer.h"

void GLRenderer::Initialize(HWND hWnd) {

	// -------------------------------------------
	// Initialize OpenGL 
	// -------------------------------------------
	hDC = GetDC(hWnd);
	setPixelFormat(hDC);

	HDC hOldDC = wglGetCurrentDC();
	HGLRC hOldRC = wglGetCurrentContext();

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glewInit();    

	if (glewIsSupported("GL_VERSION_3_1")) {       
		TRACE_RENDERER("Ready for OpenGL 3.1\n"); 
		TRACE_RENDERER("Context created: " << glGetString(GL_VERSION) << std::endl)
	}
	else {  
		TRACE_RENDERER("OpenGL 3.1 not supported\n");        
		exit(1);    
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);   

	// Vsync
	wglSwapIntervalEXT(1);

	debugRenderer.Initialize();

	TRACE_RENDERER("Initialized OpenGL!" << std::endl);

	// Prepare some test scene
	camera.position = glm::vec3(1000, 1000, 1000);
	camera.lookAtVector = glm::vec3(0.0f);
	camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.nearPlane = 10.0f;
	camera.farPlane = 50000.0f;
	camera.aspectRatio = ((float)WIDTH) / ((float)HEIGHT);
	camera.cameraAngle = glm::radians(40.0f);

	camera.calculateDirection();
	camera.calculateRightVector();
	camera.calculateUpVector();
	camera.calculatePlaneDistance();

	camera.calculatePerspectiveMatrix();
	camera.calculateViewMatrices();

}

void GLRenderer::Deinitialize() {

	debugRenderer.Deinitialize();

	// Delete GL Context
	wglMakeCurrent(NULL,NULL);  
	wglDeleteContext(hRC); 

	hRC = NULL;
}

float angle = 0.0f;
float height = 30.0f;

void GLRenderer::Render() {
	camera.calculateViewMatrices();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Scene goes here

	debugRenderer.Render(camera);

	SwapBuffers(hDC);
}

void GLRenderer::setPixelFormat(HDC hDC) {
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iFormat = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, iFormat, &pfd );
}


void GLRenderer::cameraForward(float amount) {
	camera.position += amount * camera.direction;
}

void GLRenderer::cameraRight(float amount) {
	camera.position += amount * camera.rightVector;
}

void GLRenderer::cameraUp(float amount) {
	camera.position += amount * camera.upVector;
}

void GLRenderer::cameraYaw(float amount) {
	camera.direction = glm::rotate(camera.direction, amount, camera.upVector);
	camera.calculateLookAtVector();
	camera.calculateRightVector();
}

void GLRenderer::cameraPitch(float amount) {
	camera.direction = glm::rotate(camera.direction, amount, camera.rightVector);
	camera.calculateLookAtVector();
	camera.calculateUpVector();
}

void GLRenderer::cameraRoll(float amount) {
	camera.upVector = glm::rotate(camera.upVector, amount, camera.direction);
	camera.calculateRightVector();
}

void GLRenderer::ProcessInput(UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'W':
			cameraForward(10.0f);
			break;
		case 'A':
			cameraRight(-1.0f);
			break;
		case 'S':
			cameraForward(-10.0f);
			break;
		case 'D':
			cameraRight(1.0f);
			break;
		case 33:
			cameraUp(1.0f);
			break;
		case 34:
			cameraUp(-1.0f);
			break;
		case VK_NUMPAD4:
			cameraYaw(0.02f);
			break;
		case VK_NUMPAD6:
			cameraYaw(-0.02f);
			break;
		case VK_NUMPAD2:
			cameraPitch(0.02f);
			break;
		case VK_NUMPAD8:
			cameraPitch(-0.02f);
			break;
		case VK_NUMPAD7:
			cameraRoll(-0.02f);
			break;
		case VK_NUMPAD9:
			cameraRoll(0.02f);
			break;	
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_TAB:
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEWHEEL:
		short wheelMovement = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}
