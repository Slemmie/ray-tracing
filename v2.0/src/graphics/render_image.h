// dense code to display a single 2d texture on the entire screen
// will be abstracted later

#pragma once

#include "../util/vec3.h"

#include <glew/include/glew.h>

#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <atomic>
#include <mutex>

namespace gp {
	
	class Single_texture_static_renderer {
		
	public:
		
		Single_texture_static_renderer();
		
		~Single_texture_static_renderer();
		
		// on every frame
		void on_update();
		
		void update_texture(const unsigned char* texture_buffer, int texture_width, int texture_height);
		
		template <typename A> inline void update_texture(const vec3 <A>* texture_buffer,
		int texture_width, int texture_height) {
			unsigned char* buffer = new unsigned char[texture_width * texture_height * 4];
			assert(buffer);
			for (int i = 0; i < texture_width * texture_height * 4; i += 4) {
				buffer[i + 0] = texture_buffer[i / 4].r();
				buffer[i + 1] = texture_buffer[i / 4].g();
				buffer[i + 2] = texture_buffer[i / 4].b();
				buffer[i + 3] = 255;
			}
			update_texture(buffer, texture_width, texture_height);
			delete [] (buffer);
		}
		
		// change a pixel in the previously bound texture buffer
		// the procedure is buffered
		// as long as update_pixel() is not called on the same pixel at the same time;
		// this is multithread safe
		template <typename A> inline void update_pixel(const vec3 <A>& v, int x, int y) {
			int index = (y * m_texture_width + x) * 4;
			m_texture_buffer[index + 0] = v.r();
			m_texture_buffer[index + 1] = v.g();
			m_texture_buffer[index + 2] = v.b();
			m_texture_buffer[index + 3] = 255;
			
			m_texture_flush_limit_counter++;
			
			// the main rendering thread checks each frame if it should flush the buffer
		}
		
		// force flusing
		inline void flush_pixel_buffer() {
			m_wants_flush = true;
		}
		
	private: // texture section
		
		// flag can be set at any time for flushing
		std::atomic <bool> m_wants_flush = false;
		
		// rewrite the texture when update_pixel has been called this many times
		int m_texture_flush_limit;
		// the counter
		std::atomic <int> m_texture_flush_limit_counter;
		// mutex for the above described purpose
		std::mutex m_texture_flush_mutex;
		
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
