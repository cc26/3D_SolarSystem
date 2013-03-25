#include "particleemitter.h"
#include "stdio.h"
#include "stdlib.h"
#include "vector.h"
#include "view.h"
#include "QtCore"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

ParticleEmitter::ParticleEmitter(GLuint textureId,unsigned maxParticles,float scale, float3 color,
                                 float3 velocity,float3 force, float fuzziness, float speed
                                 ) :
                    m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)
{
    m_particles = new Particle[maxParticles];
    this->m_radius=3.0f;
    this->m_center.x=0; this->m_center.y=0; this->m_center.z=0;

    resetParticles();

    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;

    glClear(GL_ACCUM_BUFFER_BIT);
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }

    glDeleteTextures(1,&m_textureID);
}


void ParticleEmitter::resetParticle(unsigned i)
{
    m_particles[i].pos.zero();
    float theta = 2*M_PI*m_radius/(float)this->m_maxParticles*i;
    float phi = M_PI*(i%100)/100.f;

    float z = m_radius*cos(phi);
    float x = m_radius*sin(phi) * cos(theta);
    float y = m_radius*sin(phi) * sin(theta);

    m_particles[i].pos.x= x;
    m_particles[i].pos.y= y;
    m_particles[i].pos.z= z;

    m_particles[i].life=0.5f;
    m_particles[i].decay= urand(0.0025f,0.15f);
    m_particles[i].color=m_color;

    m_particles[i].force.x= -1.0f/powf(m_particles[i].pos.x,2);
    m_particles[i].force.y= -1.0f/powf(m_particles[i].pos.y,2);
    m_particles[i].force.z= -1.0f/powf(m_particles[i].pos.z,2);

    m_particles[i].dir.x= urand(-1*m_fuzziness,m_fuzziness+m_velocity.x);
    m_particles[i].dir.y= urand(-1*m_fuzziness,m_fuzziness+m_velocity.y);
    m_particles[i].dir.z= urand(-1*m_fuzziness,m_fuzziness+m_velocity.z);


}

void ParticleEmitter::resetParticle_comet(unsigned i,Vector3 cur,Vector3 previous)
{
    m_particles[i].pos.zero();

    float z = cur.z;
    float x = cur.x;
    float y = cur.y;
    m_particles[i].pos.x= x;
    m_particles[i].pos.y= y;
    m_particles[i].pos.z= z;

    m_particles[i].life=1.0f;
    m_particles[i].decay= urand(0.0025f,1.f);
    m_particles[i].color=m_color;

    m_particles[i].dir.x= urand(-1*m_fuzziness,m_fuzziness+m_velocity.x);
    m_particles[i].dir.y= urand(-1*m_fuzziness,m_fuzziness+m_velocity.y);
    m_particles[i].dir.z= urand(-1*m_fuzziness,m_fuzziness+m_velocity.z);

}

void ParticleEmitter::resetParticle_meteor(unsigned i)
{
    float x = urand(-200.f,200.f);
    float y = urand(-200.f,200.f);
    float z = 1.0;

    m_particles[i].pos.x= x;
    m_particles[i].pos.y= y;
    m_particles[i].pos.z= z;

    m_particles[i].life=50.0f;
    m_particles[i].decay= 0.5f;
    m_particles[i].color=m_color;

    m_particles[i].dir.x= urand(-1*m_fuzziness,m_fuzziness+m_velocity.x);
    m_particles[i].dir.y= urand(-1*m_fuzziness,m_fuzziness+m_velocity.y);
    m_particles[i].dir.z= urand(-1*m_fuzziness,0.0);
}

void ParticleEmitter::resetParticle_Cube(unsigned i, Vector3 cur)
{
    float z = cur.z;
    float x = cur.x;
    float y = cur.y;

    m_particles[i].pos.x= x;
    m_particles[i].pos.y= y;
    m_particles[i].pos.z= z;

    m_particles[i].life=1.0f;
    m_particles[i].decay= 0.0025f;
    m_particles[i].color=m_color;

    m_particles[i].dir.x= urand(-1*m_fuzziness,m_fuzziness+m_velocity.x);
    m_particles[i].dir.y= urand(-1*m_fuzziness,m_fuzziness+m_velocity.y);
    m_particles[i].dir.z= urand(-1*m_fuzziness,m_fuzziness+m_velocity.z);

    m_particles[i].Texcord.x = urand(-1.0,1.0);
    m_particles[i].Texcord.y = urand(-1.0,1.0);

    m_particles[i].size.x = urand(0,0.3);
    m_particles[i].size.y = urand(0,0.3);
    m_particles[i].size.z = urand(0,0.07);

    m_particles[i].angle = urand(0,360);

}

void ParticleEmitter::resetParticles()
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

void ParticleEmitter::resetParticles_comet(Vector3 cur, Vector3 previous)
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle_comet(i,cur,previous);
}

void ParticleEmitter::resetParticles_explosion(Vector3 cur){
    for (unsigned i=0; i< m_maxParticles; i++){
        resetParticle_explosion(i,cur);
    }
}

void ParticleEmitter::resetParticles_cube(Vector3 cur)
{
    for (unsigned i = 0; i < m_maxParticles; i++)
    {
        m_particles[i].active=true;
        resetParticle_Cube(i,cur);

    }
}



void ParticleEmitter::resetParticle_explosion(unsigned i, Vector3 cur)
{

    float theta = 2*M_PI*m_radius/(float)this->m_maxParticles*i;

    float x = m_radius * cos(theta)+cur.x;
    float z = m_radius * sin(theta)+cur.z;

    m_particles[i].pos.x= x;
    m_particles[i].pos.y= 0;
    m_particles[i].pos.z= z;

    m_particles[i].life=0.5f;
    m_particles[i].decay= urand(0.0025f,0.15f);
    m_particles[i].color=m_color;

    m_particles[i].force.x= -1.0f/powf(m_particles[i].pos.x,2);
    m_particles[i].force.z= -1.0f/powf(m_particles[i].pos.z,2);

    m_particles[i].dir.x= urand(-1*m_fuzziness,m_fuzziness+m_velocity.x);
    m_particles[i].dir.y= 0;
    m_particles[i].dir.z= urand(-1*m_fuzziness,m_fuzziness+m_velocity.z);

}


void ParticleEmitter::updateParticles()
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if(!m_particles[i].active){
            m_particles[i].active=true;
            resetParticle(i);
        }
        m_particles[i].pos+=m_speed*m_particles[i].dir;
        if(fabs(m_particles[i].pos.dot(m_particles[i].pos))>m_radius*m_radius*20.0*urand(0,1)){
            m_particles[i].active=false;
        }
        if(m_particles[i].dir.x<0){
            m_particles[i].dir.x+=-m_particles[i].force.x;
        }

        else if(m_particles[i].dir.y<0){
            m_particles[i].dir.y+=-m_particles[i].force.y;
        }

        else if(m_particles[i].dir.z<0){
            m_particles[i].dir.z+=-m_particles[i].force.z;
        }
        else{
            m_particles[i].dir+=m_particles[i].force;
        }
        m_particles[i].life-=m_particles[i].decay;
        if(m_particles[i].life<0){
            m_particles[i].active=false;
        }
    }
}

void ParticleEmitter::updateParticles_comet(Vector3 cur,Vector3 previous)
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if(!m_particles[i].active){
            m_particles[i].active=true;
            resetParticle_comet(i,cur,previous);
        }
        m_particles[i].pos+=m_speed*m_particles[i].dir;


        m_particles[i].life-=m_particles[i].decay;
        if(m_particles[i].life<0){
            m_particles[i].active=false;
        }
    }
}

void ParticleEmitter::updateParticles_meteor()
{
    for(unsigned i = 0 ; i < m_maxParticles; i++)
    {
        if(!m_particles[i].active){
            m_particles[i].active=true;
            resetParticle_meteor(i);
        }
        m_particles[i].pos+=m_speed*m_particles[i].dir;

        m_particles[i].life-=m_particles[i].decay;
        if(m_particles[i].life<0){
            m_particles[i].active=false;
        }
    }
}

void ParticleEmitter::updateParticles_Cube(Vector3 cur)
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {

        m_particles[i].pos+=m_speed*m_particles[i].dir;

        m_particles[i].life-=m_particles[i].decay;
        if(m_particles[i].life<0){
            m_particles[i].active=false;
        }
    }
}


void ParticleEmitter::updateParticles_explosion(Vector3 cur)
{
    m_radius+=0.1;
    float3 current;
    current.x = cur.x;
    current.y = cur.y;
    current.z = cur.z;

    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if(!m_particles[i].active && m_radius < 40){
            m_particles[i].active=true;
            resetParticle_explosion(i,cur);
        }

        m_particles[i].pos+=m_speed*m_particles[i].dir;
        m_particles[i].dir+=m_particles[i].force;
        m_particles[i].dir.y = 0;

        float dist = m_particles[i].pos.getDistance(current);

        if(m_radius >  40 || dist < m_radius-1
           || dist > m_radius+0.2){
            m_particles[i].active =0;
        }

        m_particles[i].life-=m_particles[i].decay;

        if(m_particles[i].life<0){
            m_particles[i].active=false;
        }
    }
}

void ParticleEmitter::drawParticles()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    //glBlendFunc(GL_ONE,GL_ONE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_textureID);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    for (unsigned i = 0; i < m_maxParticles; i++){
        if(m_particles[i].active){

            Particle p= m_particles[i];
            float x= p.pos.x;
            float y= p.pos.y;
            float z= p.pos.z;

            glColor4f(p.color.r,p.color.g,p.color.b,1);
            
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(x-m_scale/2.0f, y+m_scale/2.0f, z);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(x+m_scale/2.0f, y+m_scale/2.0f, z);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(x+m_scale/2.0f, y-m_scale/2.0f, z);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(x-m_scale/2.0f, y-m_scale/2.0f, z);
        }
    }
    glEnd();

    //retore color back to original
    glColor4f(1.0,1.0,1.0,0.0);

    //reset texture
    glBindTexture(GL_TEXTURE_2D,0);

    glAccum(GL_MULT,0.9f);
    glAccum(GL_ACCUM,0.1f);
    glAccum(GL_RETURN,1.0f);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

}

void ParticleEmitter::drawFragmentCube()
{

    for (unsigned i = 0; i < m_maxParticles; i++){
        if(m_particles[i].active){

            Particle p= m_particles[i];
            float x= p.pos.x;
            float y= p.pos.y;
            float z= p.pos.z;

            glPushMatrix();

            glLoadIdentity();
            glTranslatef(x,y,z);
            double angle = m_particles[i].angle += 5;
            glRotatef(angle,0,1,0);
            glScalef(m_particles[i].size.x,m_particles[i].size.y,m_particles[i].size.z);
            drawCube(i);

            glPopMatrix();
        }
    }

    glAccum(GL_MULT,0.6f);
    glAccum(GL_ACCUM,0.4f);
    glAccum(GL_RETURN,1.0f);

    glColor4f(1.0,1.0,1.0,0.0);

}

void ParticleEmitter::drawCube(int i)
{
    double length = 1/5.0;
    // FRONT
    glBegin(GL_POLYGON);
    glNormal3f(0,0,-1);
    glTexCoord2f(min(m_particles[i].Texcord.x+length,1.0),m_particles[i].Texcord.y);
    glVertex3f(  0.5, -0.5, -0.5 );
    glTexCoord2f(m_particles[i].Texcord.x,min(m_particles[i].Texcord.y+length,1.0));
    glVertex3f(  0.5,  0.5, -0.5 );
    glTexCoord2f(min(m_particles[i].Texcord.x+length,1.0),min(m_particles[i].Texcord.y+length,1.0));
    glVertex3f( -0.5,  0.5, -0.5 );
    glTexCoord2f(m_particles[i].Texcord.x,m_particles[i].Texcord.y);
    glVertex3f( -0.5, -0.5, -0.5 );

    glEnd();

    // BACK
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
    //glColor3f(   1.0,  1.0, 1.0 );
    glTexCoord2f(min(m_particles[i].Texcord.x+length,1.0),m_particles[i].Texcord.y);
    glVertex3f(  0.5, -0.5, 0.5 );
    glTexCoord2f(m_particles[i].Texcord.x,min(m_particles[i].Texcord.y+length,1.0));
    glVertex3f(  0.5,  0.5, 0.5 );
    glTexCoord2f(min(m_particles[i].Texcord.x+length,1.0),min(m_particles[i].Texcord.y+length,1.0));
    glVertex3f( -0.5,  0.5, 0.5 );
    glTexCoord2f(m_particles[i].Texcord.x,m_particles[i].Texcord.y);
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
    glNormal3f(1,0,0);
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
    glNormal3f(-1,0,0);
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
    glNormal3f(0,1,0);
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();

    // BOTTOM
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

}
