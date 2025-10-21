#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>
#include <random>
#include <Eigen/Dense>
#ifdef _OPENMP
#include <omp.h>
#endif

using Eigen::Vector3d;
static thread_local std::mt19937 rng{std::random_device{}()};
inline double rand01() { return std::uniform_real_distribution<double>(0.0, 1.0)(rng); }

// ----------------- Ray structure -----------------
struct Ray {
    Vector3d origin;
    Vector3d direction;
    Ray() {}
    Ray(const Vector3d& o, const Vector3d& d) : origin(o), direction(d) {}
    Vector3d at(double t) const { return origin + t * direction; }
};

// ----------------- Sphere -----------------
struct Sphere {
    Vector3d center;
    double radius;
    Vector3d color;
    double reflectivity;
    double refr_index;
    bool is_metal;
    bool is_glass;

    Sphere(const Vector3d& c, double r, const Vector3d& col,
           double refl=0.0, double ri=1.0, bool metal=false, bool glass=false)
        : center(c), radius(r), color(col), reflectivity(refl),
          refr_index(ri), is_metal(metal), is_glass(glass) {}
};

// ----------------- Scene intersection -----------------
bool hit_sphere(const Sphere& s, const Ray& r, double t_min, double t_max,
                double& t_hit, Vector3d& normal_out) {
    Vector3d oc = r.origin - s.center;
    double a = r.direction.dot(r.direction);
    double b = 2.0 * oc.dot(r.direction);
    double c = oc.dot(oc) - s.radius * s.radius;
    double disc = b*b - 4*a*c;

    if (disc < 0) return false;
    double sqrtd = std::sqrt(disc);

    double root = (-b - sqrtd) / (2.0 * a);
    if (root < t_min || root > t_max) {
        root = (-b + sqrtd) / (2.0 * a);
        if (root < t_min || root > t_max) return false;
    }

    t_hit = root;
    Vector3d hit_point = r.at(root);
    normal_out = (hit_point - s.center).normalized();
    return true;
}

// ----------------- Reflection & Refraction -----------------
Vector3d reflect(const Vector3d& v, const Vector3d& n) {
    return v - 2 * v.dot(n) * n;
}

bool refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat, Vector3d& refr_dir) {
    double cos_theta = std::fmin((-uv).dot(n), 1.0);
    Vector3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
    double k = 1.0 - r_out_perp.squaredNorm();
    if (k < 0) return false;
    Vector3d r_out_parallel = -std::sqrt(k) * n;
    refr_dir = r_out_perp + r_out_parallel;
    return true;
}

// ----------------- Ray color (recursive) -----------------
Vector3d ray_color(const Ray& r, const std::vector<Sphere>& world, int depth) {
    if (depth <= 0) return Vector3d(0, 0, 0);

    double closest_t = std::numeric_limits<double>::infinity();
    const Sphere* hit_obj = nullptr;
    Vector3d hit_normal;

    for (const auto& s : world) {
        double t_hit; Vector3d normal;
        if (hit_sphere(s, r, 1e-3, closest_t, t_hit, normal)) {
            closest_t = t_hit;
            hit_obj = &s;
            hit_normal = normal;
        }
    }

    if (hit_obj) {
        Vector3d hit_point = r.at(closest_t);
        if (hit_obj->is_metal) {
            Vector3d reflected = reflect(r.direction.normalized(), hit_normal);
            Ray scattered(hit_point, reflected + 0.05 * Vector3d(rand01(), rand01(), rand01()));
            return hit_obj->color.cwiseProduct(ray_color(scattered, world, depth - 1));
        } else if (hit_obj->is_glass) {
            double ref_ratio = (r.direction.dot(hit_normal) > 0) ? hit_obj->refr_index : 1.0 / hit_obj->refr_index;
            Vector3d refr_dir;
            if (refract(r.direction.normalized(), hit_normal, ref_ratio, refr_dir)) {
                Ray refracted(hit_point, refr_dir);
                return ray_color(refracted, world, depth - 1);
            } else {
                Vector3d reflected = reflect(r.direction.normalized(), hit_normal);
                Ray reflected_ray(hit_point, reflected);
                return ray_color(reflected_ray, world, depth - 1);
            }
        } else {
            Vector3d target = hit_point + hit_normal + Vector3d(rand01(), rand01(), rand01());
            Ray scattered(hit_point, target - hit_point);
            return 0.5 * hit_obj->color.cwiseProduct(ray_color(scattered, world, depth - 1));
        }
    }

    // Background gradient
    Vector3d unit_dir = r.direction.normalized();
    double t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
}

// ----------------- Main -----------------
int main() {
    // Image settings
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 25;

    // Scene setup
    std::vector<Sphere> world = {
        Sphere(Vector3d(0, -100.5, -1), 100, Vector3d(0.8, 0.8, 0.0)), // ground
        Sphere(Vector3d(0, 0, -1), 0.5, Vector3d(0.1, 0.2, 0.5)),       // diffuse
        Sphere(Vector3d(-1, 0, -1), 0.5, Vector3d(0.8, 0.8, 0.8), 0.0, 1.5, false, true), // glass
        Sphere(Vector3d(1, 0, -1), 0.5, Vector3d(0.8, 0.6, 0.2), 0.8, 1.0, true, false)   // metal
    };

    // Camera setup
    Vector3d origin(0, 0, 0);
    Vector3d horizontal(4.0, 0, 0);
    Vector3d vertical(0, 2.25, 0);
    Vector3d lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3d(0, 0, 1);

    std::vector<Vector3d> framebuffer(image_width * image_height);

    std::cerr << "Rendering...\n";
    #pragma omp parallel for schedule(dynamic, 1)
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Vector3d pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + rand01()) / (image_width - 1);
                double v = (j + rand01()) / (image_height - 1);
                Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
                pixel_color += ray_color(r, world, max_depth);
            }
            framebuffer[j * image_width + i] = pixel_color / samples_per_pixel;
        }
    }

    // Output to PPM
    std::ofstream out("images/output.ppm");
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Vector3d c = framebuffer[j * image_width + i];
            c = c.array().sqrt(); // gamma correction
            int ir = static_cast<int>(255.999 * std::clamp(c.x(), 0.0, 0.999));
            int ig = static_cast<int>(255.999 * std::clamp(c.y(), 0.0, 0.999));
            int ib = static_cast<int>(255.999 * std::clamp(c.z(), 0.0, 0.999));
            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    out.close();
    std::cerr << "✅ Done! Saved to images/output.ppm\n";
    return 0;
}
