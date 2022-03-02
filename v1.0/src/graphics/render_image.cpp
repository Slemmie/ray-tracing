// implements gp::Single_texture_static_renderer

#include "render_image.h"

#include <glm/include/glm.hpp>
#include <glm/include/gtc/matrix_transform.hpp>
#include <glm/include/gtc/type_ptr.hpp>

#include <iostream>

namespace gp {
	
	struct Vertex {
		
		float x, y;
		float tx, ty;
		
		Vertex(float _x, float _y, float _tx, float _ty) :
		x(_x), y(_y), tx(_tx), ty(_ty)
		{ }
		
	};
	
	extern int window_width;
	extern int window_height;
	
	Single_texture_static_renderer::Single_texture_static_renderer() :
	m_texture_flush_limit(1024),
	m_texture_id(0),
	m_texture_width(0),
	m_texture_height(0),
	m_shader_program_id(0),
	m_shader_source_count(2),
	m_vao(0),
	m_vbo(0),
	m_ebo(0)
	{
		m_shader_sources = { "./src/graphics/glsl/STST_vertex.glsl", "./src/graphics/glsl/STST_fragment.glsl" };
		m_shader_source_types = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
		
		m_create_shader_program();
		
		std::vector <Vertex> vertices = {
			Vertex(-1.0f, -1.0f, 0.0f, 0.0f), // top left
			Vertex(+1.0f, -1.0f, 1.0f, 0.0f), // top right
			Vertex(+1.0f, +1.0f, 1.0f, 1.0f), // bottom left
			Vertex(-1.0f, +1.0f, 0.0f, 1.0f)  // bottom right
		};
		
		std::vector <uint32_t> indices = {
			0, 1, 2, // tl -> tr -> bl
			2, 3, 0  // tr -> br -> bl
		};
		
		m_bind_shader();
		
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		glUniform1i(m_get_shader_uniform_location("u_texture"), (int32_t) 0);
		
		m_unbind_shader();
	}
	
	Single_texture_static_renderer::~Single_texture_static_renderer() {
		m_destruct_texture();
		m_destruct_shader_program();
		
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}
	
	void Single_texture_static_renderer::on_update() {
		m_bind_shader();
		m_bind_texture();
		glBindVertexArray(m_vao);
		
		glm::mat4 proj = glm::ortho(
		-float(window_width) / 2.0f, float(window_width) / 2.0f,
		-float(window_height) / 2.0f, float(window_height) / 2.0f,
		-1.0f, 1.0f);
		glUniformMatrix4fv(m_get_shader_uniform_location("u_proj"), 1, GL_FALSE, glm::value_ptr(proj));
		
		// handle texture scaling
		// always preserve texture aspect ratio
		float win_asp = (float) window_width / (float) window_height;
		float tex_asp = (float) m_texture_width / (float) m_texture_height;
		glm::mat4 model(1.0f);
		if (win_asp > tex_asp) {
			// snap to height
			model = glm::scale(model,
			glm::vec3(tex_asp * float(window_height) / 2.0f, float(window_height) / 2.0f, 1.0f));
		} else {
			// snap to width
			model = glm::scale(model,
			glm::vec3(float(window_width) / 2.0f,  float(window_width) / 2.0f / tex_asp, 1.0f));
		}
		glUniformMatrix4fv(m_get_shader_uniform_location("u_model"), 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*) 0);
		
		glBindVertexArray(0);
		m_unbind_texture();
		m_unbind_shader();
	}
	
	void Single_texture_static_renderer::update_texture(
	const unsigned char* texture_buffer, int texture_width, int texture_height) {
		m_texture_width = texture_width;
		m_texture_height = texture_height;
		m_texture_buffer = std::vector <unsigned char>
		(&texture_buffer[0], &texture_buffer[texture_width * texture_height * 4]);
		
		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture_buffer.data());
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Single_texture_static_renderer::m_bind_texture() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
	}
	
	void Single_texture_static_renderer::m_unbind_texture() const {
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
			char* source = m_read_shader_source(m_shader_sources[i].c_str());
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
		
		free(ids);
		
		m_shader_program_id = program;
	}
	
	int Single_texture_static_renderer::m_get_shader_uniform_location(const char* uniform_name) {
		auto it = m_shader_uniform_location_map.find(uniform_name);
		if (it != m_shader_uniform_location_map.end()) {
			return it->second;
		}
		return m_shader_uniform_location_map[uniform_name] =
		glGetUniformLocation(m_shader_program_id, uniform_name);
	}
	
	unsigned int Single_texture_static_renderer::m_compile_shader(
	unsigned int shader_type, const char* shader_source) {
		auto get_shader_type = [] (unsigned int type) -> std::string {
			switch (type) {
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
		
		glCompileShader(id);
		
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
	
	void Single_texture_static_renderer::m_bind_shader() const {
		glUseProgram(m_shader_program_id);
	}
	
	void Single_texture_static_renderer::m_unbind_shader() const {
		glUseProgram(0);
	}
	
	void Single_texture_static_renderer::m_destruct_shader_program() {
		if (m_shader_program_id) {
			glDeleteProgram(m_shader_program_id);
		}
	}
	
} /// namespace gp
