#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <gl/GL.h>
#include <fstream>

#include "math.h"

using namespace std;
/////////////
// DEFINES //
/////////////
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE0                       0x84C0
#define GL_BGRA                           0x80E1
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

//////////////
// TYPEDEFS //
//////////////
typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
	int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef void (APIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint(APIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint(APIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef GLint(APIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const char *name);
typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void (APIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void (APIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char* *string, const GLint *length);
typedef void (APIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
	const GLvoid *pointer);
typedef void (APIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const char *name);
typedef GLint(APIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const char *name);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

typedef struct VertexType {
	VectorType position;
	VectorType color;
}VertexType;

HDC g_deviceContext = 0;
HGLRC g_renderingContext = 0;
char g_videoCardDescription[128];

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

int g_vertexCount, g_indexCount;
unsigned int g_vertexArrayId, g_vertexBufferId, g_indexBufferId;

unsigned int g_vertexShader;
unsigned int g_fragmentShader;
unsigned int g_shaderProgram;

const char VS_SHADER_SOURCE_FILE[] = "color.vs";
const char PS_SHADER_SOURCE_FILE[] = "color.ps";

float g_positionX = 0, g_positionY = 0, g_positionZ = -10;
float g_rotationX = 0, g_rotationY = 0, g_rotationZ = 0;
float g_worldMatrix[16];
float g_viewMatrix[16];
float g_projectionMatrix[16];

bool InitializeOpenGL(HWND hWnd, int screenWidth, int screenHeight, float screenDepth, float screenNear, bool vsync)
{
	int attributeListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int attributeList[5];
	float fieldOfView, screenAspect;
	char *vendorString, *rendererString;

	g_deviceContext = GetDC(hWnd);
	if (!g_deviceContext) return false;

	attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = 24;

	attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = 8;

	attributeListInt[18] = 0;

	result = wglChoosePixelFormatARB(g_deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);
	if (result != 1) return false;

	result = SetPixelFormat(g_deviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if (result != 1) return false;

	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	attributeList[4] = 0;

	g_renderingContext = wglCreateContextAttribsARB(g_deviceContext, 0, attributeList);
	if (g_renderingContext == NULL) return false;

	result = wglMakeCurrent(g_deviceContext, g_renderingContext);
	if (result != 1) return false;

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	BuildIdentityMatrix(g_worldMatrix);

	fieldOfView = PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	BuildPerspectiveFovLHMatrix(g_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	vendorString = (char*)glGetString(GL_VENDOR);
	rendererString = (char*)glGetString(GL_RENDERER);

	strcpy_s(g_videoCardDescription, vendorString);
	strcpy_s(g_videoCardDescription, " - ");
	strcpy_s(g_videoCardDescription, rendererString);

	if (vsync)
	{
		result = wglSwapIntervalEXT(1);
	}
	else
	{
		result = wglSwapIntervalEXT(0);
	}
	if (result != 1) return false;

	return true;
}

bool LoadExtensionList()
{
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB) return false;

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) return false;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!wglSwapIntervalEXT) return false;

	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	if (!glAttachShader) return false;

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if (!glBindBuffer) return false;
	
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	if (!glBindVertexArray) return false;

	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	if (!glBufferData) return false;
	
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	if (!glCompileShader)return false;
	
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	if (!glCreateProgram) return false;
	
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	if (!glCreateShader) return false;
	
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	if (!glDeleteBuffers) return false;
	
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	if (!glDeleteProgram) return false;
	
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	if (!glDeleteShader) return false;
	
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	if (!glDeleteVertexArrays) return false;
	
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	if (!glDetachShader) return false;
	
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if (!glEnableVertexAttribArray) return false;
	
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if (!glGenBuffers) return false;
	
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	if (!glGenVertexArrays) return false;
	
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	if (!glGetAttribLocation) return false;
	
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	if (!glGetProgramInfoLog) return false;
	
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	if (!glGetProgramiv) return false;
	
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	if (!glGetShaderInfoLog) return false;
	
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	if (!glGetShaderiv) return false;
	
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	if (!glLinkProgram) return false;
	
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	if (!glShaderSource) return false;
	
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	if (!glUseProgram) return false;

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if (!glVertexAttribPointer) return false;

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	if (!glBindAttribLocation) return false;

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	if (!glGetUniformLocation) return false;

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	if (!glUniformMatrix4fv) return false;
	
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	if (!glActiveTexture) return false;

	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	if (!glUniform1i) return false;

	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	if (!glGenerateMipmap) return false;

	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	if (!glDisableVertexAttribArray) return false;

	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	if (!glUniform3fv) return false;

	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	if (!glUniform4fv) return false;
	
	return true;
}

void FinalizeOpenGL(HWND hWnd)
{
	if (g_renderingContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(g_renderingContext);
		g_renderingContext = 0;
	}

	if (g_deviceContext)
	{
		ReleaseDC(hWnd, g_deviceContext);
		g_deviceContext = 0;
	}
}

void GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, g_videoCardDescription);
	return;
}

bool InitializeExtensions(HWND hWnd)
{
	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;

	deviceContext = GetDC(hWnd);
	if (!deviceContext) return false;

	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if (error != 1) return false;

	renderContext = wglCreateContext(deviceContext);
	if (!renderContext) return false;

	error = wglMakeCurrent(deviceContext, renderContext);
	if (error != 1) return false;

	result = LoadExtensionList();
	if (!result) return false;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = NULL;

	ReleaseDC(hWnd, deviceContext);
	deviceContext = 0;

	return true;
}

void OutPutShaderErrorMessage(HWND hWnd, unsigned int shaderId, const char* shaderFilename)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;
	wchar_t newString[128];
	unsigned int error;
	size_t convertedChars;

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
	logSize++;

	infoLog = new char[logSize];
	if (!infoLog) return;

	glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

	fout.open("shder-error.txt");

	for (i = 0; i < logSize; i++)
	{
		fout << infoLog[i];
	}
	fout.close();

	error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
	if (error != 0) return;

	MessageBoxW(hWnd, L"Error compiling shader. Check shader-err0r.txt for message.", newString, MB_OK);

	return;
}

void OutputLinkerErrorMessage(HWND hWnd, unsigned int programId)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
	logSize++;

	infoLog = new char[logSize];
	if (!infoLog) return;

	glGetProgramInfoLog(programId, logSize, NULL, infoLog);

	fout.open("shder-error.txt");

	for (i = 0; i < logSize; i++)
	{
		fout << infoLog[i];
	}
	fout.close();

	MessageBox(hWnd, _T("Error compiling linker. Check linker-error.txt for message."), _T("Linker Errror"), MB_OK);

}

char* LoadShaderSourceFile(const char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	fin.open(filename);
	if (fin.fail()) return 0;

	fileSize = 0;

	fin.get(input);
	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}
	fin.close();

	buffer = new char[fileSize + 1];
	if (!buffer) return 0;

	fin.open(filename);
	fin.read(buffer, fileSize);
	fin.close();
	buffer[fileSize] = '\0';

	return buffer;
}

bool InitializeShader(HWND hWnd, const char* vsFilename, const char* fsFilename)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;

	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
	if (!vertexShaderBuffer) return false;

	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if (!fragmentShaderBuffer) return false;

	g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(g_vertexShader, 1, &vertexShaderBuffer, NULL);
	glShaderSource(g_fragmentShader, 1, &fragmentShaderBuffer, NULL);

	delete[] vertexShaderBuffer;
	vertexShaderBuffer = 0;

	delete[] fragmentShaderBuffer;
	fragmentShaderBuffer = 0;

	glCompileShader(g_vertexShader);
	glCompileShader(g_fragmentShader);

	glGetShaderiv(g_vertexShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutPutShaderErrorMessage(hWnd, g_vertexShader, vsFilename);
		return false;
	}

	glGetShaderiv(g_fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutPutShaderErrorMessage(hWnd, g_fragmentShader, fsFilename);
		return false;
	}

	g_shaderProgram = glCreateProgram();

	glAttachShader(g_shaderProgram, g_vertexShader);
	glAttachShader(g_shaderProgram, g_fragmentShader);

	glLinkProgram(g_shaderProgram);

	glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(hWnd, g_shaderProgram);
		return false;
	}

	return true;
}

void ShutdownShader()
{
	glDetachShader(g_shaderProgram, g_vertexShader);
	glDetachShader(g_shaderProgram, g_fragmentShader);

	glDeleteShader(g_vertexShader);
	glDeleteShader(g_fragmentShader);

	glDeleteProgram(g_shaderProgram);

}
bool SetShaderParameters(float* worldMatrix, float* viewMatrix, float* projectionMatrix)
{
	unsigned int location;

	location = glGetUniformLocation(g_shaderProgram, "worldMatrix");
	if (location == -1) return false;
	glUniformMatrix4fv(location, 1, false, worldMatrix);

	location = glGetUniformLocation(g_shaderProgram, "viewMatrix");
	if (location == -1) return false;
	glUniformMatrix4fv(location, 1, false, viewMatrix);

	location = glGetUniformLocation(g_shaderProgram, "ProjectionMatrix");
	if (location == -1) return false;
	glUniformMatrix4fv(location, 1, false, projectionMatrix);

	return false;
}

bool InitializeBuffers()
{
	VertexType vertices[] = {
		{{ 1.0f,  1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 0.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f, 1.0f } },
		{ { 1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f },{ 0.5f, 1.0f, 0.5f } },
		{ { -1.0f, -1.0f,  1.0f },{ 1.0f, 0.5f, 1.0f } },
	};
	uint16_t indices[] = { 1, 2, 3, 3, 2, 6, 6, 7, 3, 3, 0, 1, 0, 3, 7, 7, 6, 4, 4, 6, 5, 0, 7, 4, 1, 0, 4, 1, 4, 5, 2, 1, 5, 2, 5, 6 };

	g_vertexCount = sizeof(vertices) / sizeof(VertexType);

	g_indexCount = sizeof(indices) / sizeof(uint16_t);

	glGenVertexArrays(1, &g_vertexArrayId);
	glBindVertexArray(g_vertexArrayId);

	glGenBuffers(1, &g_vertexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, g_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (char*)NULL+ (3*sizeof(float)));

	glGenBuffers(1, &g_indexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_indexCount*sizeof(uint16_t), indices, GL_STATIC_DRAW);

	return true;
}

void ShutdownBuffers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &g_vertexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &g_indexBufferId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &g_vertexArrayId);

	return;
}

void RenderBuffers()
{
	glBindVertexArray(g_vertexArrayId);

	glDrawElements(GL_TRIANGLES, g_indexCount, GL_UNSIGNED_SHORT, 0);

	return;
}

void CalculateCameraPosition()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[9];

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = g_positionX;
	position.y = g_positionY;
	position.z = g_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = g_rotationX * 0.0174532925f;
	yaw = g_rotationY * 0.0174532925f;
	roll = g_rotationZ * 0.0174532925f;

	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);

	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	BuildViewMatrix(position, lookAt, up, g_viewMatrix);
}

void Draw()
{
	static float rotateAngle = 0.0f;

	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rotateAngle += PI / 120;
	float rotationMatrixY[16];
	float rotationMatrixZ[16];
	MatrixRotationY(rotationMatrixY, rotateAngle);
	MatrixRotationZ(rotationMatrixZ, rotateAngle);
	MatrixMultiply(g_worldMatrix, rotationMatrixZ, rotationMatrixY);

	CalculateCameraPosition();

	glUseProgram(g_shaderProgram);
	SetShaderParameters(g_worldMatrix, g_viewMatrix, g_projectionMatrix);

	RenderBuffers();

	SwapBuffers(g_deviceContext);
}

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lPrarm);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("Temporary");

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
						_T("Temporary"),
						_T("Temporary"),
						WS_OVERLAPPEDWINDOW,
						0, 0, 640, 480,
						NULL,
						NULL,
						hInstance,
						NULL);

	ShowWindow(hWnd, SW_HIDE);

	InitializeExtensions(hWnd);
	DestroyWindow(hWnd);
	hWnd = NULL;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("Hello, Engine!");

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		_T("Hello, Engine!"),
		_T("Hello, Engine!"),
		WS_OVERLAPPEDWINDOW,
		300, 300, 960, 400,
		NULL,
		NULL,
		hInstance,
		NULL);

	InitializeOpenGL(hWnd, 960, 540, SCREEN_DEPTH, SCREEN_NEAR, true);
	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);

	InitializeShader(hWnd, VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
	InitializeBuffers();


	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	ShutdownBuffers();
	ShutdownShader();
	FinalizeOpenGL(hWnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			Draw();
			return 0;
		}break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}