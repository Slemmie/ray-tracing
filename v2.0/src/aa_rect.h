// axis aligned rectangle surface

#include "util/util.h"

#include "hittable.h"

#include <memory>

class XY_rect : public Hittable {
	
public:
	
	XY_rect() { }
	
	XY_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr <Material> _material) :
	m_material(_material),
	m_x0(_x0),
	m_x1(_x1),
	m_y0(_y0),
	m_y1(_y1),
	m_k(_k)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& output_box) const override;
	
private:
	
	std::shared_ptr <Material> m_material;
	
	double m_x0, m_x1;
	double m_y0, m_y1;
	
	double m_k;
	
};

class XZ_rect : public Hittable {
	
public:
	
	XZ_rect() { }
	
	XZ_rect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr <Material> _material) :
	m_material(_material),
	m_x0(_x0),
	m_x1(_x1),
	m_z0(_z0),
	m_z1(_z1),
	m_k(_k)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& output_box) const override;
	
private:
	
	std::shared_ptr <Material> m_material;
	
	double m_x0, m_x1;
	double m_z0, m_z1;
	
	double m_k;
	
};

class YZ_rect : public Hittable {
	
public:
	
	YZ_rect() { }
	
	YZ_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr <Material> _material) :
	m_material(_material),
	m_y0(_y0),
	m_y1(_y1),
	m_z0(_z0),
	m_z1(_z1),
	m_k(_k)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& output_box) const override;
	
private:
	
	std::shared_ptr <Material> m_material;
	
	double m_y0, m_y1;
	double m_z0, m_z1;
	
	double m_k;
	
};
