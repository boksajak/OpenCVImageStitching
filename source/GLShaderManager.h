//----------------------------------------------------------------------------------------
/**
* \file       GLShaderManager.h
* \author     Jakub Bokšanský
* \date       2012/28/03
* \brief      GLShaderManager class
*
*  Class holding information about compiled shaders.
*
*/
//----------------------------------------------------------------------------------------

#pragma once
#include "common.h"

#include "GLShader.h"

#include "Utils\TextFile.h"
#include "Utils\StringBuilder.h"

#define MAX_SHADER_NAME_LENGTH 512

class GLShaderManager
{
public:
	/// Returns pointer to prepared shader.
	///	  \param name   filename of desired shader, without extension (pair of .fp and .vp is assumed)
	static GLShader* getShaderByName(const char* name) {

		StringBuilder sb(MAX_SHADER_NAME_LENGTH);
		sb.Append("shaders/", name);

		GLShader* newShader;
		GLuint progamId = createShader(sb.GetString());

	/*	if (!strcmp(name, "axes"))  {
			newShader = new GLDebugAxesShader(progamId);
		} else if (!strcmp(name, "boxes"))  {
			newShader = new GLDebugBoxesShader(progamId);
		} else if (!strcmp(name, "circle"))  {
			newShader = new GLDebugCircleShader(progamId);
		} else if (!strcmp(name, "grids"))  {
			newShader = new GLDebugGridsShader(progamId);
		} else if (!strcmp(name, "points"))  {
			newShader = new GLDebugPointsShader(progamId);
		} else if (!strcmp(name, "spotlight"))  {
			newShader = new GLDebugSpotlightShader(progamId);
		} else if (!strcmp(name, "vector"))  {
			newShader = new GLDebugVectorsShader(progamId);
		} else {
			assert(false);
		}*/

		return newShader;
	}

private:

	static GLuint createShader(const char* name) {
		char *vs = NULL,*fs = NULL, *gs = NULL,*gs_eval = NULL,*gs_control = NULL;  

		GLuint p,v,f,g,g_eval,g_control;    

		TRACE_LOADER("Compiling shader: " << name << std::endl);

		v = glCreateShader(GL_VERTEX_SHADER);  
		f = glCreateShader(GL_FRAGMENT_SHADER); 
		g = glCreateShader(GL_GEOMETRY_SHADER); 
		g_eval = glCreateShader(GL_TESS_EVALUATION_SHADER); 
		g_control = glCreateShader(GL_TESS_CONTROL_SHADER); 

		StringBuilder sb(MAX_SHADER_NAME_LENGTH);


		sb.Append(name, ".vp");
		TextFile vertexShaderFile(sb.GetString());
		vertexShaderFile.ReadFile(vs);

		sb.Clear(); 
		sb.Append(name, ".fp");
		TextFile fragmentShaderFile(sb.GetString());
		fragmentShaderFile.ReadFile(fs);

		sb.Clear(); 
		sb.Append(name, ".geom");
		TextFile geometryShaderFile(sb.GetString());
		geometryShaderFile.ReadFile(gs);

		sb.Clear(); 
		sb.Append(name, "_eval.geom");
		TextFile evalGeometryShaderFile(sb.GetString());
		evalGeometryShaderFile.ReadFile(gs_eval);

		sb.Clear(); 
		sb.Append(name, "_control.geom");
		TextFile controlGeometryShaderFile(sb.GetString());
		controlGeometryShaderFile.ReadFile(gs_control);

		if (!vs) {
			TRACE_ERROR("Could not load shader. Vertex shader file ("<<name<<") not found!" << std::endl);
			return -1;
		}

		const char * vv = vs;
		const char * ff = fs; 
		const char * gg = gs; 
		const char * gg_eval = gs_eval; 
		const char * gg_control = gs_control; 

		glShaderSource(v, 1, &vv, NULL);
		if (fs) glShaderSource(f, 1, &ff,NULL);
		if (gs) glShaderSource(g, 1, &gg,NULL);
		if (gs_eval) glShaderSource(g_eval, 1, &gg_eval,NULL);
		if (gs_control) glShaderSource(g_control, 1, &gg_control,NULL);

		glCompileShader(v);  
		if (fs) glCompileShader(f);  
		if (gs) glCompileShader(g);  
		if (gs_eval) glCompileShader(g_eval);  
		if (gs_control) glCompileShader(g_control);  

		printShaderInfoLog(v);  
		if (fs) printShaderInfoLog(f);  
		if (gs) printShaderInfoLog(g);  
		if (gs_eval) printShaderInfoLog(g_eval);  
		if (gs_control) printShaderInfoLog(g_control);  

		p = glCreateProgram();  

		glAttachShader(p,v);   
		if (fs) glAttachShader(p,f);    
		if (gs) glAttachShader(p,g);    
		if (gs_control) glAttachShader(p,g_eval);    
		if (gs_eval) glAttachShader(p,g_control);   

		glBindFragDataLocation(p, 0, "outputColor");    // TODO: toto asi netreba napisat natvrdo, ale co uz

		glLinkProgram(p);  
		printProgramInfoLog(p);  

		// Cleanup
		glDetachShader(p, v);
		if (fs) glDetachShader(p, f);
		if (gs) glDetachShader(p, g);
		if (gs_eval) glDetachShader(p, g_eval);
		if (gs_control) glDetachShader(p, g_control);

		glDeleteShader(v);
		if (fs) glDeleteShader(f);
		if (gs) glDeleteShader(g);
		if (gs_eval) glDeleteShader(g_eval);
		if (gs_control) glDeleteShader(g_control);

		return p;
	}
	static void printShaderInfoLog(GLuint obj){    
		int infologLength = 0;
		int charsWritten  = 0;    
		char *infoLog;     

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);     

		if (infologLength > 0)    {  
			infoLog = (char *)malloc(infologLength);    
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			if (charsWritten>0) TRACE(infoLog << std::endl);        
			free(infoLog);    
		}
	} 
	static void printProgramInfoLog(GLuint obj){   
		int infologLength = 0;    
		int charsWritten  = 0; 

		char *infoLog;   

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);  

		if (infologLength > 0)    {     
			infoLog = (char *)malloc(infologLength);  
			glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);    
			if (charsWritten>0) TRACE(infoLog << std::endl);    
			free(infoLog);   
		}
	} 
};
