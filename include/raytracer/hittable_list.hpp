#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <memory>
#include <vector>

#include <raytracer/hittable.hpp>

namespace rt
{

class HittableList : public Hittable
{
private:
    std::vector<std::shared_ptr<Hittable>> objects_;
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> obj)
    {
        objects_.push_back(obj);
    }

    void add (std::shared_ptr<Hittable> obj)
    {
        objects_.push_back(obj);
    }

    void clear ()
    {
        objects_.clear();
    }

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override
    {
        //checks all the obj in the scene and assign rec to the closest one hit
        double closest_so_far = ray_tmax;
        bool at_least_one_hit = false;
        HitRecord obj_rec;
        for(const auto& object : objects_)
        {
            if(object -> hit(r, ray_tmin, closest_so_far, obj_rec))
            {
                closest_so_far = obj_rec.t;
                rec = obj_rec;
                at_least_one_hit = true;
            }
        }

        return at_least_one_hit;
    }
};


}//namespace rt

#endif