#include "Shader.h"

using std::cout; 
using std::endl;

Shader::Shader()
{
}


Shader::~Shader()
{
}

bool Shader::load()
{
	if (!compile(GL_VERTEX_SHADER)) {
		return false; 
	}
	if (!compile(GL_FRAGMENT_SHADER)) {
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vprogram);
	glAttachShader(program, fprogram);
	glLinkProgram(program);

	GLint test;
	glGetShaderiv(program, GL_LINK_STATUS, &test);

	if (test != 0) {
		return true;
	}
	else {
		GLint loglength;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &loglength);

		GLchar* log = new GLchar[loglength];

		glGetShaderInfoLog(fprogram, loglength, 0, log);

		for (int i = 0; i < loglength; i++)
		{
			cout << log[i];
		}

		delete[] log;
	}

	return false; 
}

bool Shader::compile(GLenum shaderType)
{

	if (shaderType == GL_VERTEX_SHADER) {
		std::ifstream instream(filenamev, std::ios::binary); // opens in binary mode 
		if (!instream.is_open()) {
			cout << "Error loading file"; 
			return false;
		}
		// get length of file 
		instream.seekg(0, std::ios::end); // go to the end of file 
		int length = instream.tellg();  // file length 
		instream.seekg(0, std::ios::beg); // go to beginning 							

		char* bytes = new char[length + 1];
		bytes[length] = '\0';
		instream.read(bytes, length); // read to wear, # bytes 

		instream.close();
		cout << "Read file sucessfully" << endl;

		vprogram = glCreateShader(shaderType);
		glShaderSource(vprogram, 1, &bytes, 0);
		glCompileShader(vprogram);

		delete[] bytes; 

		GLint test;
		glGetShaderiv(vprogram, GL_COMPILE_STATUS, &test); 

		if (test != 0) {
			cout << "Shader made sucessfully" << endl;
			return true;
		}
		else {
			GLint loglength; 
			glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &loglength);

			GLchar* log = new GLchar[loglength];

			glGetShaderInfoLog(vprogram, loglength, 0, log);

			for (int i = 0; i < loglength; i++)
			{
				cout << log[i]; 
			}

			delete[] log; 
			glDeleteShader(vprogram);
		}
	}
	else if (shaderType == GL_FRAGMENT_SHADER) {
		std::ifstream instream(filenamef, std::ios::binary); // opens in binary mode 
		if (!instream.is_open()) {
			cout << "Error loading file"; 
			return false;
		}

		// get length of file 
		instream.seekg(0, std::ios::end); // go to the end of file 
		int length = instream.tellg();  // file length 
		instream.seekg(0, std::ios::beg); // go to beginning 							

		char* bytes = new char[length + 1];
		bytes[length] = '\0';
		instream.read(bytes, length); // read to wear, # bytes 

		instream.close();
		cout << "Read file sucessfully" << endl;

		fprogram = glCreateShader(shaderType);
		glShaderSource(fprogram, 1, &bytes, 0);
		glCompileShader(fprogram);

		delete[] bytes; 

		GLint test;
		glGetShaderiv(fprogram, GL_COMPILE_STATUS, &test);
		if (test != 0) {
			cout << "Shader made sucessfully" << endl;
			return true;
		}
		else {
			GLint loglength;
			glGetShaderiv(fprogram, GL_INFO_LOG_LENGTH, &loglength);

			GLchar* log = new GLchar[loglength];

			glGetShaderInfoLog(fprogram, loglength, 0, log);

			for (int i = 0; i < loglength; i++)
			{
				cout << log[i];
			}
			
			delete[] log;
			glDeleteShader(fprogram);
		}
	}
	else {
		return false; 
	}

	return true;
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram); 
}
