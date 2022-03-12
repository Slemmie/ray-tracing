// derive scene classes from base class

#pragma once

#include "util/util.h"

class Hittable_list;

namespace scene {
	
	class Scene {
		
	public:
		
		virtual Hittable_list get_world() = 0;
		
	};
	
	class Two_spheres : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture);
		
	};
	
	class Random_demo : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture);
		
	};
	
} /// namespace scene
