// dense code to display a single 2d texture on the entire screen
// will be abstracted later

#pragma once

namespace gp {
	
	class Single_texture_static_renderer {
		
	public:
		
		
		
	private: // texture section
		
		unsigned int m_texture_id;
		int m_texture_width;
		int m_texture_height;
		
		unsigned char* m_texture_buffer;
		
		void m_bind_texture();
		void m_unbind_texture();
		
	};
	
} /// namespace gp
