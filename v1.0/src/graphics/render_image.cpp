// implements gp::Single_texture_static_renderer

#include "renderer_image.h"

namespace gp {
	
	void Single_texture_static_renderer::update_texture(
	unsigned char* texture_buffer, int texture_width, int texture_height) {
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
	
} /// namespace gp
