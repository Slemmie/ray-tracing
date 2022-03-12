// implements scene.h

#include "scene.h"

#include "hittable_list.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"
#include "movable_sphere.h"

#include <memory>

namespace scene {
	
	Hittable_list Two_spheres::get_world() {
		Hittable_list result;
		
		auto checker_texture =
		std::make_shared <tex::Checkers> (vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));
		
		result.push(std::make_shared <Sphere> (vec3(0.0, -10.0, 0.0), 10.0,
		std::make_shared <Lambertian> (checker_texture)));
		
		result.push(std::make_shared <Sphere> (vec3(0.0, 10.0, 0.0), 10.0,
		std::make_shared <Lambertian> (checker_texture)));
		
		return result;
	}
	
	void Two_spheres::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.0;
	}
	
	Hittable_list Random_demo::get_world() {
		Hittable_list result;
		
		auto checker_ground =
		std::make_shared <tex::Checkers> (vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));
		result.push(std::make_shared <Sphere> (vec3(0.0, -1000.0, 0.0), 1000.0,
		std::make_shared <Lambertian> (checker_ground)));
		
		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = Random::real();
				vec3 center(a + 0.9 * Random::real(), 0.2, b + 0.9 * Random::real());
				if ((center - vec3(4.0, 0.2, 0.0)).length() > 0.9) {
					std::shared_ptr <Material> sphere_material;
					if (choose_mat < 0.8) {
						auto albedo = vec3d::random() * vec3d::random();
						sphere_material = std::make_shared <Lambertian> (albedo);
						auto center_end = center + vec3(0.0, Random::range(0.0, 0.5), 0.0);
						result.push(std::make_shared <Moving_sphere>
						(center, center_end, 0.0, 1.0, 0.2, sphere_material));
					} else if (choose_mat < 0.95) {
						auto albedo = vec3d::random(0.5, 1.0);
						auto fuzz = Random::range(0.0, 0.5);
						sphere_material = std::make_shared <Metal> (albedo, fuzz);
						result.push(std::make_shared <Sphere> (center, 0.2, sphere_material));
					} else {
						sphere_material = std::make_shared <Dielectric> (1.5);
						result.push(std::make_shared <Sphere> (center, 0.2, sphere_material));
					}
				}
			}
		}

		auto material1 = std::make_shared <Dielectric> (1.5);
		result.push(std::make_shared <Sphere> (vec3(0.0, 1.0, 0.0), 1.0, material1));

		auto material2 = std::make_shared <Lambertian> (vec3(0.4, 0.2, 0.1));
		result.push(std::make_shared <Sphere> (vec3(-4.0, 1.0, 0.0), 1.0, material2));

		auto material3 = std::make_shared <Metal> (vec3(0.7, 0.6, 0.5), 0.0);
		result.push(std::make_shared <Sphere> (vec3(4.0, 1.0, 0.0), 1.0, material3));
		
		return result;
	}
	
	void Random_demo::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov, double& _aperture) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.1;
	}
	
} /// namespace scene
