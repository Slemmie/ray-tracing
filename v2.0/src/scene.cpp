// implements scene.h

#include "scene.h"

#include "hittable_list.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"
#include "movable_sphere.h"
#include "aa_rect.h"
#include "box.h"

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
	
	void Two_spheres::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov,
	double& _aperture, vec3d& _background) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.0;
		_background = vec3(0.7, 0.8, 1.0);
	}
	
	Hittable_list Two_perlin_spheres::get_world() {
		Hittable_list result;
		
		auto perlin_texture = std::make_shared <tex::Noise> (4.0);
		
		result.push(std::make_shared <Sphere> (vec3(0.0, -1000.0, 0.0), 1000.0,
		std::make_shared <Lambertian> (perlin_texture)));
		
		result.push(std::make_shared <Sphere> (vec3(0.0, 2.0, 0.0), 2.0,
		std::make_shared <Lambertian> (perlin_texture)));
		
		return result;
	}
	
	void Two_perlin_spheres::set_params(vec3d& _look_from, vec3d& _look_at,
	double& _vfov, double& _aperture, vec3d& _background) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.0;
		_background = vec3(0.7, 0.8, 1.0);
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
	
	void Random_demo::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov,
	double& _aperture, vec3d& _background) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.1;
		_background = vec3(0.7, 0.8, 1.0);
	}
	
	Hittable_list Earth::get_world() {
		auto earth_texture = std::make_shared <tex::Image> ("img/earth_map.jpg");
		
		auto earth_surface = std::make_shared <Lambertian> (earth_texture);
		
		auto globe_sphere = std::make_shared <Sphere> (vec3(0.0, 0.0, 0.0), 2.0, earth_surface);
		
		return Hittable_list(globe_sphere);
	}
	
	void Earth::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov,
	double& _aperture, vec3d& _background) {
		_look_from = vec3(13.0, 2.0, 3.0);
		_look_at = vec3(0.0, 0.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.0;
		_background = vec3(0.7, 0.8, 1.0);
	}
	
	Hittable_list Simple_light::get_world() {
		Hittable_list result;
		
		auto perlin_texture = std::make_shared <tex::Noise> (4.0);
		
		result.push(std::make_shared <Sphere> (vec3(0.0, -1000.0, 0.0), 1000.0,
		std::make_shared <Lambertian> (perlin_texture)));
		
		result.push(std::make_shared <Sphere> (vec3(0.0, 2.0, 0.0), 2.0,
		std::make_shared <Lambertian> (perlin_texture)));
		
		auto diffuse_light = std::make_shared <Diffuse_light> (vec3(4.0, 4.0, 4.0));
		
		result.push(std::make_shared <XY_rect> (3.0, 5.0, 1.0, 3.0, -2.0, diffuse_light));
		
		return result;
	}
	
	void Simple_light::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov,
	double& _aperture, vec3d& _background) {
		_look_from = vec3(26.0, 3.0, 6.0);
		_look_at = vec3(0.0, 2.0, 0.0);
		_vfov = 20.0;
		_aperture = 0.0;
		_background = vec3(0.0, 0.0, 0.0);
	}
	
	Hittable_list Cornell_box::get_world() {
		Hittable_list result;
		
		auto red = std::make_shared <Lambertian> (vec3(0.65, 0.05, 0.05));
		auto white = std::make_shared <Lambertian> (vec3(0.73, 0.73, 0.73));
		auto green = std::make_shared <Lambertian> (vec3(0.12, 0.45, 0.15));
		auto light = std::make_shared <Diffuse_light> (vec3(15.0, 15.0, 15.0));
		
		result.push(std::make_shared <YZ_rect> (0.0, 555.0, 0.0, 555.0, 555.0, green));
		result.push(std::make_shared <YZ_rect> (0.0, 555.0, 0.0, 555.0, 0.0, red));
		result.push(std::make_shared <XZ_rect> (213.0, 343.0, 227.0, 332.0, 554.0, light));
		result.push(std::make_shared <XZ_rect> (0.0, 555.0, 0.0, 555.0, 0.0, white));
		result.push(std::make_shared <XZ_rect> (0.0, 555.0, 0.0, 555.0, 555.0, white));
		result.push(std::make_shared <XY_rect> (0.0, 555.0, 0.0, 555.0, 555.0, white));
		
		std::shared_ptr <Hittable> box[2];
		
		box[0] = std::make_shared <Box> (vec3(0.0, 0.0, 0.0), vec3(165.0, 330.0, 165.0), white);
		box[0] = std::make_shared <Rotate_y> (box[0], 15.0);
		box[0] = std::make_shared <Translate> (box[0], vec3(265.0, 0.0, 295.0));
		
		box[1] = std::make_shared <Box> (vec3(0.0, 0.0, 0.0), vec3(165.0, 165.0, 165.0), white);
		box[1] = std::make_shared <Rotate_y> (box[1], -18.0);
		box[1] = std::make_shared <Translate> (box[1], vec3(130.0, 0.0, 65.0));
		
		result.push(box[0]);
		result.push(box[1]);
		
		return result;
	}
	
	void Cornell_box::set_params(vec3d& _look_from, vec3d& _look_at, double& _vfov,
	double& _aperture, vec3d& _background) {
		_look_from = vec3(278.0, 278.0, -800.0);
		_look_at = vec3(278.0, 278.0, 0.0);
		_vfov = 40.0;
		_aperture = 0.0;
		_background = vec3(0.0, 0.0, 0.0);
	}
	
} /// namespace scene
