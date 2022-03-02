// dense code to display a single 2d texture on the entire screen
// will be abstracted later

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace gp {
	
	class Single_texture_static_renderer {
		
	public:
		
		Single_texture_static_renderer();
		
		~Single_texture_static_renderer();
		
		// on every frame
		void on_update();
		
		void update_texture(const unsigned char* texture_buffer, int texture_width, int texture_height);
		
	private: // texture section
		
		unsigned int m_texture_id;
		int m_texture_width;
		int m_texture_height;
		
		std::vector <unsigned char> m_texture_buffer;
		
		void m_bind_texture() const;
		void m_unbind_texture() const;
		
		void m_destruct_texture();
		
	private: // shader section
		
		unsigned int m_shader_program_id;
		
		std::unordered_map <std::string, int> m_shader_uniform_location_map;
		
		const size_t m_shader_source_count;
		std::vector <std::string> m_shader_sources;
		std::vector <unsigned int> m_shader_source_types;
		
		void m_create_shader_program();
		
		int m_get_shader_uniform_location(const char* uniform_name);
		unsigned int m_compile_shader(unsigned int shader_type, const char* shader_source);
		char* m_read_shader_source(const char* filepath);
		
		void m_bind_shader() const;
		
		void m_unbind_shader() const;
		
		void m_destruct_shader_program();
		
	private: // general graphics section
		
		unsigned int m_vao, m_vbo, m_ebo;
		
	};
	
} /// namespace gp
