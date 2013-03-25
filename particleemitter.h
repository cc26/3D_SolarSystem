#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "common.h"
#include <QtOpenGL>
#include "camera.h"

class ParticleEmitter
{
    struct __attribute__ ((aligned (16))) Particle
    {
        /**
          * Determines whether or not this particle is active. If it is not active,
          * it should not be drawn or modified by updateParticles().
          */
        bool active;
        /**
          * The amount of life remaining. When the particle is first born, it should
          * have a life of 1.0. Each step this should decrease by decay (i.e.
          * life -= decay). When this value reaches zero, the particle should reset.
          */
        float life;
        /**
          * The amount the life of this particle decreases per step. See the documentation
          * for Particle.life.
          */
        float decay;
        /**
          * The particle's color. Currently the same for all particles; however, it would be
          * possible to have randomly generated colors or colors based on time. Used when
          * drawing the particle.
          */
        float3 color;
        /**
          * The particle's current position in 3D space. Updated every step based on
          * the particle's velocity.
          */
        float3 pos;
        /**
          * The direction this particle is currently moving. The velocity of any particle
          * in the system is: velocity = ParticleEmitter.m_speed * Particle.dir
          */
        float3 dir;
        /**
          * The force acting on this particle (e.g. from gravity). At each update step,
          * Particle.dir += Particle.force.
          */
        float3 force;

        /**
          * For Cube
          *
          */
        float3 Texcord;
        float3 size;
        float angle;
    };


public:
    ParticleEmitter(GLuint textureId = 0,
                    unsigned maxParticles = 2000,
                    float scale = .5f,
                    float3 color = float3(1.0f, 0.4f, 0.15f),
                    float3 velocity = float3(0.0f, 0.0001f, 0.0f),
                    float3 force = float3(0.0001f, 0.0001f, 0.0001f),
                    float fuzziness = 50.0f,
                    float speed = 50.0f / 10000.0f
                    );

    ~ParticleEmitter();


    void drawParticles();
    void drawFragmentCube();

    void resetParticle(unsigned i);
    void resetParticle_comet(unsigned i,Vector3 cur,Vector3 previous);
    void resetParticle_meteor(unsigned i);
    void resetParticle_Cube(unsigned i, Vector3 cur);
    void resetParticles_explosion(Vector3 cur);

    void resetParticles();
    void resetParticles_comet(Vector3 cur,Vector3 previous);
    void resetParticles_cube(Vector3 cur);
    void resetParticle_explosion(unsigned i, Vector3 cur);

    void updateParticles();
    void updateParticles_comet(Vector3 cur,Vector3 previous);
    void updateParticles_meteor();
    void updateParticles_explosion(Vector3 cur);
    void updateParticles_Cube(Vector3 cur);

    void drawCube(int i);

    inline void setRadius(float radius) { this->m_radius = radius;}

protected:
    /** Contains all particles in the scene, active or inactive */
    Particle *m_particles;
    /** The maximum number of particles in the scene (same as the size of the m_particles array) */
    unsigned m_maxParticles;
    /** The ID of the OpenGL texture that is used to draw particles */
    GLuint m_textureID;
    /** The speed that particles move in the simulation */
    float m_speed;
    /** The randomness of particle original direction */
    float m_fuzziness;
    /** The size of each particle (i.e. the length of each particle's square) */
    float m_scale;
    /** The color of all the particles in the scene */
    float3 m_color;
    /** The initial velocity of every particle emitted into the scene */
    float3 m_velocity;
    /** The force vector applied to every particle emitted into the scene */
    float3 m_force;

    float m_radius;
    float3 m_center;
};


#endif // PARTICLEEMITTER_H
