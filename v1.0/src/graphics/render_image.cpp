// implements gp::Single_texture_static_renderer

#include "renderer_image.h"

#include <iostream>

namespace gp {
	
	Single_texture_static_renderer::Single_texture_static_renderer() :
	m_texture_id(0),
	m_texture_width(0),
	m_texture_height(0),
	m_texture_buffer(nullptr),
	m_shader_program_id(0),
	m_shader_source_count(2),
	m_shader_sources({ "./glsl/STST_vertex.glsl", "glsl/STST_fragment.glsl" }),
	m_shader_source_types({ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER })
	{
		m_create_shader_program();
	}
	
	Single_texture_static_renderer::~Single_texture_static_renderer() {
		m_destruct_texture();
		m_destruct_shader_program();
	}
	
	void Single_texture_static_renderer::on_update() {
		
	}
	
	void Single_texture_static_renderer::update_texture(
	const unsigned char* texture_buffer, int texture_width, int texture_height) {
		m_texture_width = texture_width;
		m_texture_height = texture_height;
		m_texture_buffer = texture_buffer;
		
		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, GL_RGBA8, texture_width, texture_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture_buffer);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Single_texture_static_renderer::m_bind_texture() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
	}
	
	void Single_texture_static_renderer::m_unbind_texture() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Single_texture_static_renderer::m_destruct_texture() {
		if (m_texture_id) {
			glDeleteTextures(1, &m_texture_id);
		}
	}
	
	void Single_texture_static_renderer::m_create_shader_program() {
		unsigned int program = glCreateProgram();
		
		unsigned int* ids = (unsigned int*) malloc(sizeof(unsigned int) * m_shader_source_count);
		if (!ids) {
			std::cerr << "[error]: malloc failed" << std::endl;
			std::cerr << "[error]: ...failed to create shader program" << std::endl;
			return;
		}
		
		for (size_t i = 0; i < m_shader_source_count; i++) {
			char* source = m_read_shader_source(m_shader_sources[i]);
			ids[i] = m_compile_shader(m_shader_source_types[i], source);
			free(source);
		}
		
		for (size_t i = 0; i < m_shader_source_count; i++) {
			glAttachShader(program, ids[i]);
		}
		
		glLinkProgram(program);
		
		glValidateProgram(program);
		
		for (size_t i = 0; i < m_shader_source_count; i++) {
			glDeleteShader(ids[i]);
		}
		
		m_shader_program_id = program;
	}
	
	int Single_texture_static_renderer::m_get_shader_uniform_location(const char* uniform_name) {
		auto it = m_shader_uniform_location_map.find(uniform_name);
		if (it != m_shader_uniform_location_map.end()) {
			return it->second;
		}
		return m_shader_uniform_location_map[name] = glGetUniformLocation(m_shader_program_id, uniform_name);
	}
	
	unsigned int Single_texture_static_renderer::m_compile_shader(
	unsigned int shader_type, const char* shader_source) {
		auto get_shader_type = [] (unsigned int type) -> std::string {
			switch (shader_type) {
				case GL_VERTEX_SHADER          : return "vertex";
				case GL_TESS_CONTROL_SHADER    : return "tessellation control";
				case GL_TESS_EVALUATION_SHADER : return "tessellation evaluation";
				case GL_GEOMETRY_SHADER        : return "geometry";
				case GL_FRAGMENT_SHADER        : return "fragment";
				case GL_COMPUTE_SHADER         : return "compute";
				default                        : break;
			}
			return "[unknown type]";
		};
		
		unsigned int id = glCreateShader(shader_type);
		glShaderSource(id, 1, &shader_source, nullptr);
		
		glCompile_shader(id);
		
		int compilation_result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_result);
		if (compilation_result == GL_FALSE) {
			int err_len;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &err_len);
			
			char* err_msg = (char*) malloc(sizeof(char) * err_len);
			if (!err_msg) {
				std::cerr << "[error]: malloc failed" << std::endl;
				std::cerr << "[error]: ...shader compilation failed" << std::endl;
				return 0;
			}
			
			glGetShaderInfoLog(id, err_len, &err_len, err_msg);
			
			std::cerr << "[error]: failed to compile " << get_shader_type(shader_type)
			<< " shader: " << err_msg << std::endl;
			
			free(err_msg);
			
			glDeleteShader(id);
			
			return 0;
		}
		
		return id;
	}
	
	char* Single_texture_static_renderer::m_read_shader_source(const char* filepath) {
		FILE* file = fopen(filepath, "rt");
		if (!file) {
			std::cerr << "[error]: failed to open file '" << filepath << "'" << std::endl;
			return nullptr;
		}
		
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		rewind(file);
		
		char* buffer = (char*) malloc(sizeof(char) * (size + 1));
		
		if (!buffer) {
			std::cerr << "[error]: malloc failed" << std::endl;
			return nullptr;
		}
		
		auto end = fread(buffer, sizeof(char), size, file);
		
		if ((size_t) size != (size_t) end) {
			std::cerr << "[warning]: ftell size and fread size mismatch (" <<
			size << ":" << end << ") file: '" << filepath << "'" << std::endl;
		}
		
		buffer[end] = '\0';
		
		fclose(file);
		
		return buffer;
	}
	
	void m_destruct_shader_program() {
		if (m_shader_program_id) {
			glDeleteProgram(m_shader_program_id);
		}
	}
	
} /// namespace gp
