#include <iostream>
#include <typeinfo>

#include "../classes/linalg/linalg.h"

#include <amp.h>
#include <amp_math.h>

#include <chrono>
#include <Windows.h>

using namespace std;


#define GravityConstant 6.67E-11

#define ObjectTrackPoints 500
#define MaxCosineBtwTrajectoryPoints    0.99619469809174553229501040247389

class Object {
protected:

public:

    vec<double, 3> pos;
    vec<double, 3> vel;

    double mass;

    double rad;

    double time_delta;
    double time;

    vec<double, 3>  track_pos   [ObjectTrackPoints];
    double          track_time  [ObjectTrackPoints];
    uint32_t        track_end;



    Object() {
        pos = { 0., 0., 0. };
        vel = { 0., 0., 0. };

        mass = 1.;

        rad = 1.;

        time_delta = 0.1;
        time = 0.;

        for (uint32_t i = 0; i < ObjectTrackPoints; i++) {
            track_pos[i] = pos;
            track_time[i] = time;
        }

        track_end = 0;
    }
    Object(const Object& other) {
        pos = other.pos;
        vel = other.vel;

        mass = other.mass;

        rad = other.rad;

        time_delta = other.time_delta;
        time = other.time;

        for (uint32_t i = 0; i < ObjectTrackPoints; i++) {
            track_pos[i] = other.track_pos[i];
            track_time[i] = other.track_time[i];
        }

        track_end = other.track_end;
    }
    Object(const vec<double, 3> pos, const vec<double, 3> vel, const double mass, const double rad, const double time = 0.) {
        this->pos = pos;
        this->vel = vel;

        this->mass = mass;
        
        this->rad = rad;
        
        this->time = time;

        /*Позже может быть изменено, подгоночными кофициентами*/
        this->time_delta = 1. / vel.len() * vel.len();

        for (uint32_t i = 0; i < ObjectTrackPoints; i++) {
            track_pos[i] = pos;
            track_time[i] = time;
        }

        track_end = 0;
    }
    

    bool UpdateTrack() {

        vec<double, 3> pos0 = track_pos[(track_end + 1) % ObjectTrackPoints];
        vec<double, 3> pos1 = track_pos[(track_end + 0) % ObjectTrackPoints];
        vec<double, 3> pos2 = pos;

        vec<double, 3> dir0 = (pos1 - pos0).dir();
        vec<double, 3> dir1 = (pos2 - pos1).dir();

        double cosine = linalg::cosbtw(dir0, dir1);

        if (cosine <= MaxCosineBtwTrajectoryPoints) {
            track_end = (track_end + 1) % ObjectTrackPoints;

            track_pos   [track_end] = pos;
            track_time  [track_end] = time;

            return true;
        }

        return false;
    }
    void ForceUpdateTrack() {
        track_end = (track_end + 1) % ObjectTrackPoints;

        track_pos[track_end] = pos;
        track_time[track_end] = time;
    }

    bool Collision(const Object& other) const {
        if ((this->pos - other.pos).len() <= (this->rad + other.rad))
            return true;

        return false;
    }

    vec<double, 3> CalcGravityForce(const Object& other) const restrict(amp, cpu) {
        vec<double, 3> vector = other.pos - this->pos;
        double distance = vector.len();

        return (GravityConstant * this->mass * other.mass / (distance * distance)) * vector.dir();
    }

    vec<double, 3>  GetPosition()   const restrict(amp, cpu) {
        return pos;
    }
    vec<double, 3>  GetVelosity()   const restrict(amp, cpu) {
        return vel;
    }
    double          GetMass()       const restrict(amp, cpu) {
        return mass;
    }
    double          GetTime()       const restrict(amp, cpu) {
        return time;
    }
    double          GetTimeDelta()  const restrict(amp, cpu) {
        return time_delta;
    }

    void            SetTimeDelta(double time_delta)     restrict(amp, cpu) {
        this->time_delta = time_delta;
    }
};



class ObjectSystem {
protected:

    std::vector<Object> objects;

    double global_time;

public:

    ObjectSystem() {
        objects.resize(0);
        global_time = 0.;
    }

    void AddObject(const vec<double, 3> pos, const vec<double, 3> vel, const double mass, const double rad) {
        Object temp(pos, vel, mass, rad, global_time);
        objects.push_back(temp);
    }

    std::vector<vec<double, 3>> CalculateForces_type0() {
        std::vector<vec<double, 3>> forces(objects.size(), vec<double, 3>(0.));

        for (uint32_t n = 0; n < objects.size(); n++)
            for (uint32_t k = n + 1; k < objects.size(); k++) {
                vec<double, 3> force = objects[n].CalcGravityForce(objects[k]);
                forces[n] += force;
                forces[k] -= force;
            }

        return forces;
    }
    std::vector<vec<double, 3>> CalculateForces_type1() {
        std::vector<vec<double, 3>> forces(objects.size(), vec<double, 3>(0.));

        concurrency::array_view<Object, 1>          objs    (objects.size(),    objects);
        concurrency::array_view<vec<double, 3>, 1>  frcs    (forces.size(),     forces);

        frcs.discard_data();

        concurrency::extent<1> ex(objects.size());

        concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp, cpu) {
            frcs[i] = vec<double, 3>(0);
            for (uint32_t n = 0; n < i[0]; n++)
                frcs[i] += objs[i].CalcGravityForce(objs[n]);
            for (uint32_t n = i[0] + 1; n < ex.size(); n++)
                frcs[i] += objs[i].CalcGravityForce(objs[n]);
        });

        frcs.synchronize();

        return forces;
    }
    std::vector<vec<double, 3>> CalculateForces_type2(const uint32_t amount = 100) {
        if (amount >= objects.size())
            return CalculateForces_type1();
        
        std::vector<vec<double, 3>> forces(objects.size(), vec<double, 3>(0.));

        uint32_t amount_lth = objects.size() % amount;

        uint32_t tfone = (objects.size() / amount + uint32_t(objects.size() % amount != 0));


        std::vector<vec<double, 3>> forces_mat(tfone * objects.size());

        concurrency::array_view<Object, 1> objs(objects.size(), objects);

        concurrency::array_view<vec<double, 3>, 2> frcs_mat(objects.size(), tfone, forces_mat);
        frcs_mat.discard_data();

        concurrency::extent<2> ex(objects.size(), tfone);

        concurrency::parallel_for_each(ex, [=](concurrency::index<2> i) restrict(amp) {
            frcs_mat[i] = vec<double, 3>(0.);

            uint32_t begin = i[1] * amount;
            uint32_t end = i[1] * amount + amount * (i[1] != tfone - 1) + amount_lth * (i[1] == tfone - 1);

            if (begin <= i[0] && i[0] <= end) {
                for (uint32_t n = begin; n < i[0]; n++)
                    frcs_mat[i] += objs[i[0]].CalcGravityForce(objs[n]);
                for (uint32_t n = i[0] + 1; n < end; n++)
                    frcs_mat[i] += objs[i[0]].CalcGravityForce(objs[n]);
            }
            else {
                for (uint32_t n = begin; n < end; n++)
                    frcs_mat[i] += objs[i[0]].CalcGravityForce(objs[n]);
            }
        });

        //frcs_mat.synchronize();

        concurrency::array_view<vec<double, 3>, 1> frcs(objects.size(), forces);
        frcs.discard_data();

        concurrency::extent<1> ext(objects.size());

        concurrency::parallel_for_each(ext, [=](concurrency::index<1> i) restrict(amp) {
            frcs[i] = vec<double, 3>(0.);
            for (uint32_t n = 0; n < tfone; n++)
                frcs[i] += frcs_mat(concurrency::index<2>(i[0], n));
        });

        frcs.synchronize();

        return forces;
    }
    std::vector<vec<double, 3>> CalculateForces_type3() {
        
        concurrency::array_view<Object, 1>          objs(objects.size(), objects);
        

        std::vector<vec<double, 3>> forces_mat(objects.size() * objects.size());
        concurrency::array_view<vec<double, 3>, 2> frcs_mat(objects.size(), objects.size(), forces_mat);
        frcs_mat.discard_data();

        concurrency::extent<2> ext(objects.size(), objects.size());

        concurrency::parallel_for_each(ext, [=](concurrency::index<2> i) restrict(amp) {
            frcs_mat[i] = objs[i[0]].CalcGravityForce(objs[i[1]]);
        });


        std::vector<vec<double, 3>> forces(objects.size(), vec<double, 3>(0.));        
        concurrency::array_view<vec<double, 3>, 1>  frcs(forces.size(), forces);
        frcs.discard_data();

        concurrency::extent<1> ex(objects.size());

        concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp, cpu) {
            frcs[i] = vec<double, 3>(0);
            for (uint32_t n = 0; n < i[0]; n++)
                frcs[i] += frcs_mat(i[0], n);
            for (uint32_t n = i[0] + 1; n < ex.size(); n++)
                frcs[i] += frcs_mat(i[0], n);
        });

        frcs.synchronize();

        return forces;
    }
    std::vector<vec<double, 3>> CalculateForces_type4() {
        std::vector<vec<double, 3>> pos_s(objects.size());
        std::vector<double>         mas_s(objects.size());
        for (uint32_t i = 0; i < objects.size(); i++) {
            pos_s[i] = objects[i].GetPosition();
            mas_s[i] = objects[i].GetMass();
        }
        concurrency::array_view<vec<double, 3>, 1>  ps_s(objects.size(), pos_s);
        concurrency::array_view<double, 1>          ms_s(objects.size(), mas_s);

        std::vector<vec<double, 3>> forces(objects.size(), vec<double, 3>(0.));
        concurrency::array_view<vec<double, 3>, 1>  frcs(forces.size(), forces);
        frcs.discard_data();

        concurrency::extent<1> ex(objects.size());

        concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp, cpu) {
            frcs[i] = vec<double, 3>(0);
            for (uint32_t n = 0; n < i[0]; n++) {
                vec<double, 3> vector = ps_s[n] - ps_s[i];
                double distance = vector.len();
                frcs[i] += (GravityConstant * ms_s[i] * ms_s[n] / (distance * distance)) * vector.dir();
            }
            for (uint32_t n = i[0] + 1; n < ex.size(); n++) {
                vec<double, 3> vector = ps_s[n] - ps_s[i];
                double distance = vector.len();
                frcs[i] += (GravityConstant * ms_s[i] * ms_s[n] / (distance * distance)) * vector.dir();
            }
            });

        frcs.synchronize();

        return forces;
    }

    double CalculateEnergy() const {
        double energy = 0;
        for (uint32_t n = 0; n < objects.size(); n++) {
            energy += 0.5 * objects[n].mass * objects[n].vel.len() * objects[n].vel.len();
            for (uint32_t k = n + 1; k < objects.size(); k++)
                energy -= GravityConstant * objects[n].mass * objects[k].mass / (objects[n].pos - objects[k].pos).len();
        }
        return energy;
    }


};



int main() {
    
    ObjectSystem Sys;

    Sys.AddObject({ 0,+1,0 }, { 0,0,0 }, 1.0, 1.0);
    Sys.AddObject({ 0,-1,0 }, { 0,0,0 }, 1.0, 1.0);
    Sys.AddObject({ 0,-1,-1 }, { 0,0,0 }, 1.0, 1.0);
    Sys.AddObject({ 0,+1,+1 }, { 0,0,0 }, 1.0, 1.0);

    
    std::vector<vec<double, 3>> forces_type0 = Sys.CalculateForces_type0();
    std::vector<vec<double, 3>> forces_type1 = Sys.CalculateForces_type1();
    std::vector<vec<double, 3>> forces_type2 = Sys.CalculateForces_type2();

    for (uint32_t i = 0; i < forces_type0.size(); i++) {
        cout << forces_type0[i] << "\t" << forces_type1[i] << "\t" << forces_type2[i] << endl;
    }


    uint32_t object_amount = 1 << 6;
    uint32_t test_amount = 1;
    uint64_t Timer[2] = { 0, 0 };

    cout << endl << endl;
    cout << "Calculating time with " << object_amount << " objects and " << test_amount << " tests" << endl;


    cout << "\t" << "Creating Empty System      - ";
    ObjectSystem System;
    cout << "Complete!" << endl;


    cout << "\t" << "Generating Objects         - ";
    for (uint32_t i = 0; i < object_amount; i++)
        System.AddObject(linalg::random_vec3<double>(), { 0.,0.,0. }, linalg::random<double>(1., 10.), 1.0);
    cout << "Complete!" << endl;



    
    cout << "\t" << "Calculating froces (type0) - ";
    Timer[0] = chrono::high_resolution_clock::now().time_since_epoch().count();
    for (uint32_t i = 0; i < test_amount; i++)
        System.CalculateForces_type0();
    Timer[1] = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "Complete! - Average time: " << 1E-6 * double(Timer[1] - Timer[0]) / double(test_amount) << " milliseconds" << endl;



    cout << "\t" << "Calculating froces (type1) - ";
    Timer[0] = chrono::high_resolution_clock::now().time_since_epoch().count();
    for (uint32_t i = 0; i < test_amount; i++)
        System.CalculateForces_type1();
    Timer[1] = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "Complete! - Average time: " << 1E-6 * double(Timer[1] - Timer[0]) / double(test_amount) << " milliseconds" << endl;



    cout << "\t" << "Calculating froces (type2) - ";
    Timer[0] = chrono::high_resolution_clock::now().time_since_epoch().count();
    for (uint32_t i = 0; i < test_amount; i++)
        System.CalculateForces_type2();
    Timer[1] = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "Complete! - Average time: " << 1E-6 * double(Timer[1] - Timer[0]) / double(test_amount) << " milliseconds" << endl;



    cout << "\t" << "Calculating froces (type3) - ";
    Timer[0] = chrono::high_resolution_clock::now().time_since_epoch().count();
    for (uint32_t i = 0; i < test_amount; i++)
        System.CalculateForces_type3();
    Timer[1] = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "Complete! - Average time: " << 1E-6 * double(Timer[1] - Timer[0]) / double(test_amount) << " milliseconds" << endl;



    cout << "\t" << "Calculating froces (type4) - ";
    Timer[0] = chrono::high_resolution_clock::now().time_since_epoch().count();
    for (uint32_t i = 0; i < test_amount; i++)
        System.CalculateForces_type4();
    Timer[1] = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "Complete! - Average time: " << 1E-6 * double(Timer[1] - Timer[0]) / double(test_amount) << " milliseconds" << endl;





    cout << endl << endl << endl;
    return 0;
}
