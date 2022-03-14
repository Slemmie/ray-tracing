// derive scene classes from base class

#pragma once

#include "util/util.h"

#include <string>

class Hittable_list;

namespace scene {
	
	class Scene {
		
	public:
		
		virtual Hittable_list get_world() = 0;
		
		virtual std::string to_string() const = 0;
		
	};
	
	class Two_spheres : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "two_spheres";
		}
		
	};
	
	class Two_perlin_spheres : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "two_perlin_spheres";
		}
		
	};
	
	class Random_demo : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "random_demo";
		}
		
	};
	
	class Earth : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "earth";
		}
		
	};
	
	class Simple_light : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "simple_light";
		}
		
	};
	
	class Cornell_box : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "cornell_box";
		}
		
	};
	
	class Cornell_smoke : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "cornell_smoke";
		}
		
	};
	
	class Random_demo_2 : public Scene {
		
	public:
		
		virtual Hittable_list get_world() override;
		
		void set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture, vec3d& _background);
		
		virtual std::string to_string() const override {
			return "random_demo_2";
		}
		
	};
	
} /// namespace scene
