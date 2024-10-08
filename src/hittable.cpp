

#include "hittable.hpp"
#include "utils.hpp"
#include <cmath> 


using std::vector; 

/*----------------Sphere----------------*/

Sphere::Sphere() : radius(1), origin(Point(0, 0, 0)), material(Material()), transformation(identity()) {}

vector<Intersection> Sphere::intersect(const Ray& ray) const { 
    //transform the ray instead of the sphere, with the type of transformation matrix being held in the 
    //hittable object. This means we have to take the inverse to give the correct transformation to the ray  
    Ray transformed_ray = transform_ray(ray, inverse(transformation)); 
    //calculates whether a ray hits a sphere by calculating the discriminant 
    Vec sphere_to_ray = transformed_ray.origin - origin; 

    double a = dot(transformed_ray.direction, transformed_ray.direction);
    double b = 2 * dot(transformed_ray.direction, sphere_to_ray); 
    double c = dot(sphere_to_ray, sphere_to_ray) - 1; 

    double discriminant = b*b - 4 * a * c; 

    //If the discriminant is negative, the ray has missed
    if (discriminant < 0) return vector<Intersection>(); 

    double intersect_1 = (-b - sqrt(discriminant)) / (2 * a); 
    double intersect_2 = (-b + sqrt(discriminant)) / (2 * a); 

    Intersection intersection_1(intersect_1, this); 
    Intersection intersection_2(intersect_2, this); 

    if (compare_doubles(intersect_1, intersect_2)) return vector<Intersection>{intersection_1}; 
    
    return vector<Intersection>{intersection_1, intersection_2}; 
}

void Sphere::transform(const Matrix& new_transformation) { 
    transformation = transformation * new_transformation;
}

Vec Sphere::normal_at(const Point& world_point) { 
    //Returns the normal vector at a point. 
    //Because the sphere is transformed, we must make calculations to compute the normal at the 
    //transformed state. 
    Point object_point = inverse(transformation) * world_point; 
    Vec object_normal = object_point - Point(0, 0, 0);
    Vec world_normal = transpose(inverse(transformation)) * object_normal; 
    return unit_vector(world_normal);
}
