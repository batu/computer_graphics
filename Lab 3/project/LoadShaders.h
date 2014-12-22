#pragma once
#include "includes.h"

struct ShaderInfo {
	GLenum vTarget;
	const char *vShaderFile;
	GLenum fTarget;
	const char *fShaderFile;
};

GLuint LoadShaders( ShaderInfo shaderInfo );
const char* getShaderProgram( const char *filePath, string &shaderProgramText );