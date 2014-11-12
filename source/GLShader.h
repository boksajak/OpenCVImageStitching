//----------------------------------------------------------------------------------------
/**
* \file       GLShader.h
* \author     Jakub Bokšanský
* \date       2012/28/03
* \brief      Shader class
*
*  Parent class for holding uniform and attribute locations of different shaders.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

class GLShader
{
public:
	GLShader(GLuint programId) : programId(programId) {}

	virtual ~GLShader() {
		glDeleteProgram(programId);
	}

	GLuint programId;
};

