#include "shadows-common.h" /* pch */

#include "Shader.h"
#include "shaderutils.h"

ShaderProgram::ShaderProgram()
{
	shaders[0] = -1;
	shaders[1] = -1;
	program = -1;
}

void ShaderProgram::start()
{	
	program = glCreateProgram();
}

int ShaderProgram::attachFileAs(const GLenum type, const string shaderpath)
{
	if ((type == GL_VERTEX_SHADER && shaders[0] != -1)
		|| (type == GL_FRAGMENT_SHADER && shaders[1] != -1))
	{
		SHADOW_LOG_ERROR("Duplicate shader attachments");
		return 0; /* fail */
	}

	switch(type)
	{
		case GL_VERTEX_SHADER:
			createShader(&shaders[0], GL_VERTEX_SHADER, shaderpath.c_str());
			break;;
		case GL_FRAGMENT_SHADER:
			createShader(&shaders[1], GL_FRAGMENT_SHADER, shaderpath.c_str());
			break;
		default:
			SHADOW_LOG_ERROR("Invalid shader attachment type");
			return 0; /* fail */
	}

	return 1; /* succuess */
}

int ShaderProgram::link()
{


	/*glBindAttribLocation(program, SHADER_POS, "positionzz");
	cout << " positionzz error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_COLOR, "colorzz");
	cout << " colorzz error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_AMB, "amb_in");
	cout << " amb_in error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_DIFF, "diff_in");
	cout << " diff_in error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_SPEC, "spec_in");
	cout << " spec_in error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_SHINE, "shine_in");
	cout << " shine_in error " << glGetError() << endl;
	glBindAttribLocation(program, SHADER_NORMAL, "normalzz");
	cout << " normalzz error " << glGetError() << endl;*/
	linkProgram(&program, shaders, 2);
	return 0;
}

int ShaderProgram::activate()
{
	if (program == -1)
	{
		SHADOW_LOG_ERROR("Activating shader without linking");
		return 0; /* fail */
	}


    glUseProgram(program);
    return 1;
}

int ShaderProgram::getAttributeIndex(const char *name)
{
	if (program == -1)
	{
		SHADOW_LOG_ERROR("Activating shader without linking");
		return -1; /* fail */
	}

	return glGetAttribLocation(program, name);
}

void ShaderProgram::setUniform4f(const char* name, float v1, float v2, float v3, float v4)
{
    activate();
	int index = glGetUniformLocation(program , name);
	glUniform4f(index, v1, v2, v3, v4);
}

void ShaderProgram::setUniform1i(const char* name, int v1)
{
    activate();
	int index = glGetUniformLocation(program , name);
	glUniform1i(index, v1);
	cout << "uniform1i " << v1 << " " << glGetError() << endl;
}