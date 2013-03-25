#include "view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include <QWheelEvent>
#include <QMouseEvent>
#include "glm.h"
#include "bcurve.h"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#define radian(degree) (degree*M_PI/180.0)

GLfloat ctrlpoints[4][3] = {
        { 10,0,0}, {0,10,-40 },
        {-40,-10,0}, {0,0,40}};

extern "C"
{
    extern void APIENTRY glActiveTexture(GLenum);
}

View::View(QWidget *parent) : QGLWidget(parent)
{

    setCameraDefaultValue();

    m_increment = 0.0;

    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    setCursor(Qt::BlankCursor);

    // Particles init
    setAutoBufferSwap(false);

    // The tracks related variable setting up
    position_counter=0;
    position_counter_camera = 0;
    track_counter =0;
    globalcounter = 0;


    for(int i=0; i<200; i++){
        Bcurve::computeBezier4points(camera_points,Vector3(0,0,-10),Vector3(-10,0,0),Vector3(-10,0,0),Vector3(0,0,m_camera.zoom),(float)i/200.0);
        //Bcurve::computeBezier4points(m_points,Vector3(30,0,0),Vector3(-10,20,0),Vector3(-10,-40,0),Vector3(30,0,0),(float)i/200.0);
        //cout<<points.size()<<endl;
        //Bcurve::computeBezier4points(camera_points,Vector3(0,0,0),Vector3(-10,10,0),Vector3(-10,10,0),Vector3(0,0,20),(float)i/200.0);
    }


    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    // Comet Init
    drawComet = false;
    CometPos.x = 40; CometPos.y = 30; CometPos.z = -0;

    // flag for key
    bumpmapping = true;
    stop = false;
    m_camera_random_track = false;
    m_follow_comet = false;
    collision = false;
    drawline = false;
    flag_resetParticleCube = false;

    m_font=QFont();


}

View::~View()
{
    gluDeleteQuadric(m_quadric_sun);
    gluDeleteQuadric(m_quadric_earth);
    gluDeleteQuadric(m_quadric_moon);
    gluDeleteQuadric(m_quadric_saturn);
    gluDeleteQuadric(m_quadric_saturn_disk);

    foreach (QGLShaderProgram *sp, m_shaderPrograms)
        delete sp;

    glDeleteTextures(1,&m_texID_earth);
    glDeleteTextures(1,&m_texID_earth_normal);
    glDeleteTextures(1,&m_texID_earth_cloud);
    glDeleteTextures(1,&m_texID_moon);
    glDeleteTextures(1,&m_texID_moon_normal);
    glDeleteTextures(1,&m_texID_sun);
    glDeleteTextures(1,&m_texID_comet);
    glDeleteTextures(1,&m_texID_comet_normal);
    glDeleteTextures(1,&m_texID_saturn);
    glDeleteTextures(1,&m_texID_andy);

    glDeleteLists(m_skybox,1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap_Andy);

    SAFE_DELETE(m_emitter_sun);
    SAFE_DELETE(m_emitter_comet);
    SAFE_DELETE(m_emitter_meteor);
    SAFE_DELETE(m_emitter_explosion);
    SAFE_DELETE(m_emitter_cube);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glmDelete(m_comet.model);

    makeCurrent();
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / 60);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    //QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));


    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //initialize parameters
    m_quadric_sun = gluNewQuadric();
    m_quadric_earth = gluNewQuadric();
    m_quadric_moon = gluNewQuadric();
    m_quadric_saturn = gluNewQuadric();
    m_quadric_saturn_disk = gluNewQuadric();

    //Set up Global (ambient lighting)
    GLfloat global_ambient[] = {0.2f,0.2f,0.2f,1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

    //Set up GL_LIGHT0 with position and lighting properties
    GLfloat ambientLight[] = {0.3f,0.3f,0.3f,1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION,position);

    // Enable spot light
    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable Texturing
    glEnable(GL_TEXTURE_2D);


    //Particles
    m_emitter_sun = new ParticleEmitter(loadTexture(":/textures/particle3.jpg"),3000);
    m_emitter_comet = new ParticleEmitter(loadTexture(":/textures/particle1.bmp"),5000,1.f,float3(0.7,0.5,0.3),0.01f,5000);
    m_emitter_meteor = new ParticleEmitter(loadTexture(":/textures/particle1.bmp"),1500,.5f,float3(1.0,1.0,1.0));
    m_emitter_cube = new ParticleEmitter(loadTexture(":/textures/earthmap1k.jpg"),1000,.5f,float3(1.0f, 0.4f, 0.15f),float3(0.0f, 0.0001f, 0.0f),float3(0.0001f, 0.0001f, 0.0001f),50.0f
                                         ,50.0f/25000.0f);
    m_emitter_explosion = new ParticleEmitter(loadTexture(":/textures/particle3.jpg"),500,1.2f,float3(1,1,1));
    m_emitter_explosion->setRadius(0.1);

    updateCamera();


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Load resources
    InitResources();


}

void View::InitResources()
{
    createShaderPrograms();
    m_skybox = ResourceLoader::loadSkybox();
    //m_comet  = ResourceLoader::loadObjModel("../final/model/planet.OBJ");
    //m_comet  = ResourceLoader::loadObjModel("/course/cs123/bin/models/xyzrgb_dragon.obj");

    loadCubeMap();

    m_texID_sun = loadTexture(":/textures/sun.png");
    m_texID_earth = loadTexture(":/textures/earthmap1k.jpg");
    m_texID_earth_normal = loadTexture(":/textures/earthbump1k_enhance.png");
    m_texID_earth_cloud = loadTexture(":/textures/earthcloudmap.jpg");
    m_texID_moon = loadTexture(":/textures/moonmap2k.jpg");
    m_texID_moon_normal = loadTexture(":/textures/moonmap2k_enhance2.jpg");
    m_texID_comet = loadTexture(":/textures/mercurymap.jpg");
    m_texID_comet_normal = loadTexture(":/textures/mercurybump_normal.jpg");
    m_texID_saturn = loadTexture(":/textures/saturnmap.jpg");
    m_texID_andy = loadTexture(":/textures/Andy.jpg");


}

void View::createShaderPrograms()
{
    const QGLContext *ctx = context();
    m_shaderPrograms["bumpmapping"] = ResourceLoader::newShaderProgram(ctx, "../final/Shader/BumpMapping.vert", "../final/Shader/BumpMapping.frag");
    m_shaderPrograms["bumpmapping2"] = ResourceLoader::newShaderProgram(ctx, "../final/Shader/BumpMapping2.vert", "../final/Shader/BumpMapping2.frag");
    m_shaderPrograms["texture"] = ResourceLoader::newShaderProgram(ctx,"../final/Shader/texture.vert", "../final/Shader/texture.frag");
    m_shaderPrograms["phong"] = ResourceLoader::newShaderProgram(ctx,"../final/Shader/phong.vert", "../final/Shader/phong.frag");

}

void View::loadCubeMap()
{
    QList<QFile *> fileList;
    fileList.append(new QFile(":/textures/stranight1_1024.jpg"));
    fileList.append(new QFile(":/textures/stranight1_1024.jpg"));
    fileList.append(new QFile(":/textures/stranight1_1024.jpg"));
    fileList.append(new QFile(":/textures/stranight4_1024.jpg"));
    fileList.append(new QFile(":/textures/stranight4_1024.jpg"));
    fileList.append(new QFile(":/textures/stranight1_1024.jpg"));

    QList<QFile *> fileList2;
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    fileList2.append(new QFile("../final/textures/andybox1.jpg"));
    m_cubeMap = ResourceLoader::loadCubeMap(fileList);
    m_cubeMap_Andy = ResourceLoader::loadCubeMap(fileList2);
}


void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glDisable(GL_LIGHTING);

    renderScene();

    swapBuffers();

    glEnable(GL_LIGHTING);

}


void View::renderScene()
{
    if(!stop){
      m_increment++;
    }

    /******Draw Cube Box *******/

    glActiveTexture(GL_TEXTURE0);
    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);

    if(collision)
    {
        globalcounter++;
        if(globalcounter > 240)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap_Andy);
        }
        else
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        }
    }
    else
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    }

    glCallList(m_skybox);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    glDisable(GL_TEXTURE_CUBE_MAP);


    /****** Collide detection *******/

    Collide_Dectection();


    /******* Fire arround the sun *******/

    if(globalcounter < 265)
    {
        m_emitter_sun->updateParticles();
        m_emitter_sun->drawParticles();
    }

    /******* render meteor ********/

    m_emitter_meteor->updateParticles_meteor();
    m_emitter_meteor->drawParticles();


    /****** render comet & render comet trail *****/

    renderComet();

    /******* render Text ********/

    paintText();
    drawLine();

    /****** render Earth explosion ******/

    if(collision)
    {
        if(!flag_resetParticleCube)
        {
            cout << "came in here" << endl;
            m_emitter_cube->resetParticles_cube(EarthPos);
            m_emitter_explosion->resetParticles_explosion(EarthPos);
            flag_resetParticleCube = true;
        }
        m_emitter_cube->updateParticles_Cube(EarthPos);
        m_emitter_explosion->updateParticles_explosion(EarthPos);
        m_emitter_explosion->drawParticles();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_texID_earth);

        m_shaderPrograms["phong"]->bind();
        m_shaderPrograms["phong"]->setUniformValue("tex",1);

        m_emitter_cube->drawFragmentCube();

        m_shaderPrograms["phong"]->release();
    }

    /******* render all the planet in scene ****/

    renderPlanet();

    /****** Pre-Motion **********/

    if(position_counter_camera < this->camera_points.size())
    {
        Vector3 cur = this->camera_points[position_counter_camera++];
        this->updateCamera2(cur);
    }

    if(this->m_camera_random_track){

        int size = m_random_track.size();

        Vector3 cur = m_random_track[(track_counter++)%size];
        this->updateCamera2(cur);

    }


    //glFlush();
}

void View::renderPlanet()
{
    //Get the time in second
    float time = m_increment / (float) 60.0;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    /*****render earth ****************/

    if(!collision){

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_texID_earth);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_texID_earth_normal);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,m_texID_earth_cloud);


        if(bumpmapping)
        {
            m_shaderPrograms["bumpmapping"]->bind();
            m_shaderPrograms["bumpmapping"]->setUniformValue("tex",1);
            m_shaderPrograms["bumpmapping"]->setUniformValue("tex_normal",2);
            m_shaderPrograms["bumpmapping"]->setUniformValue("tex_cloud",3);
            m_shaderPrograms["bumpmapping"]->setUniformValue("envMap",0);
        }
        else
        {
            m_shaderPrograms["texture"]->bind();
            m_shaderPrograms["texture"]->setUniformValue("tex",1);
        }

        glPushMatrix();

        EarthPos.x = 10*cos(radian(time*20));
        EarthPos.z = -10*sin(radian(time*20));
        EarthPos.y = 0;

        //cout << "earth pos x  : " <<  EarthPos.x << endl;
        //cout << "earth pos z  : " <<  EarthPos.z << endl;

        // rotate around the sun
        glRotatef(time*20.0,0.0,1.0,0.0);
        glTranslatef(10,0.0,0.0);
        glPushMatrix();
        glRotatef(90,-1.0,0,0.0);
        //self rotate
        glRotatef(time*50,0,0.0,-1.0);
        gluQuadricNormals(m_quadric_earth,GLU_SMOOTH);
        gluQuadricTexture(m_quadric_earth,GL_TRUE);
        gluSphere(m_quadric_earth,1.5,25.0,25.0);
        glPopMatrix();

        m_shaderPrograms["bumpmapping"]->release();
        m_shaderPrograms["texture"]->release();

        glBindTexture(GL_TEXTURE_2D,0);

        /*** render earth finised ***/

        /*** Draw the orbit of the moon ***/
        if(drawline){
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            glPushMatrix();
            glRotatef(90,1,0,0);
            GLint circle_points = 100;
            glBegin(GL_LINE_LOOP);
            glColor3f(0.0f, 0.2f, 0.2f);
            for (int i = 0; i < circle_points; i++) {
                float angle = 2*PI*i/circle_points;
                glVertex2f(2.5*(cos(angle)),2.5*(sin(angle)));
            }
            glEnd();
            glPopMatrix();
            glEnable(GL_TEXTURE_2D);

            glColor4f(1.0f,1.0f,1.0f,0.0f);
        }
        /*** finish the orbit drawing *******/

        /*** render moon ****/

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_texID_moon);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_texID_moon_normal);

        if(bumpmapping)
        {
            m_shaderPrograms["bumpmapping2"]->bind();
            m_shaderPrograms["bumpmapping2"]->setUniformValue("tex",1);
            m_shaderPrograms["bumpmapping2"]->setUniformValue("tex_normal",2);
        }
        else
        {
            m_shaderPrograms["texture"]->bind();
            m_shaderPrograms["texture"]->setUniformValue("tex",1);
        }

        glPushMatrix();
        glRotatef(time*50,0,1.0,0);
        glTranslatef(-2.5,0.0,0.0);
        gluQuadricNormals(m_quadric_moon,GLU_SMOOTH);
        gluQuadricTexture(m_quadric_moon,GL_TRUE);
        gluSphere(m_quadric_moon,0.5,50.0,50.0);
        glPopMatrix();

        m_shaderPrograms["bumpmapping2"]->release();
        m_shaderPrograms["texture"]->release();
        glBindTexture(GL_TEXTURE_2D,0);
        /*** render moon finished ***/

        glPopMatrix();
    }
    /**** render Satrun ******/

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,m_texID_saturn);

    m_shaderPrograms["phong"]->bind();
    m_shaderPrograms["phong"]->setUniformValue("tex",1);


    glPushMatrix();
    glRotatef(-45*time,0,1.0,0);
    glTranslatef(-25,0,0);
    //glRotatef(time*50,0,0,1.0);
    gluQuadricNormals(m_quadric_saturn,GLU_SMOOTH);
    gluQuadricTexture(m_quadric_saturn,GL_TRUE);
    gluSphere(m_quadric_saturn,1.0,50.0,50.0);

    m_shaderPrograms["phong"]->release();

    m_shaderPrograms["texture"]->bind();
    m_shaderPrograms["texture"]->setUniformValue("tex",1);

    glPushMatrix();
    glRotatef(45,1,0,0);
    //glTranslatef(cur.x,cur.y,cur.z);
    gluQuadricNormals(this->m_quadric_saturn_disk,GLU_SMOOTH);
    gluQuadricTexture(this->m_quadric_saturn_disk,GL_TRUE);
    //glBindTexture(GL_TEXTURE_2D,this->id_sun);
    gluDisk(this->m_quadric_saturn_disk,1.2,3,50,1);
    glPopMatrix();
    glPopMatrix();

    m_shaderPrograms["texture"]->release();

    glBindTexture(GL_TEXTURE_2D,0);

    /********** render sun (every planet must render before the sun **************/

    if(globalcounter < 265)
    {
        glEnable(GL_DEPTH_TEST);

        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,m_texID_sun);
        glEnable(GL_BLEND);


        m_shaderPrograms["texture"]->bind();
        m_shaderPrograms["texture"]->setUniformValue("tex",4);

        SunPos.x = 0.0;
        SunPos.y = 0.0;
        SunPos.z = 0.0;

        gluQuadricNormals(m_quadric_sun,GLU_SMOOTH);
        gluQuadricTexture(m_quadric_sun,GL_TRUE);
        gluSphere(this->m_quadric_sun,3.0f,50,50);

        m_shaderPrograms["texture"]->release();
        glBindTexture(GL_TEXTURE_2D,0);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    /********************************/

//        glBegin(GL_QUADS);
//            //glMultiTexCoord2f(GL_TEXTURE1,1,1);
//            glNormal3f(0.0,0.0,1.0);
//            glTexCoord2f(1,1);
//            glVertex3f( 1.5,1.5,0);
//            //glMultiTexCoord2f(GL_TEXTURE1,0,1);
//            glTexCoord2f(0,1);
//            glVertex3f(-1.5,1.5,0);
//            //glMultiTexCoord2f(GL_TEXTURE1,0,0);
//            glTexCoord2f(0,0);
//            glVertex3f(-1.5,-1.5,0);
//            //glMultiTexCoord2f(GL_TEXTURE1,1,0);
//            glTexCoord2f(1,0);
//            glVertex3f( 1.5,-1.5,0);
//        glEnd();


    // Enable light again
    glEnable(GL_LIGHTING);
}


void View::renderComet()
{
    if(drawComet)
    {
        Vector3 cur , pre;

        if(position_counter < this->m_points.size())
        {
            if(position_counter==0)
                m_emitter_comet->resetParticles_comet(m_points[0],pre);

           pre = this->m_points[position_counter];
           cur = this->m_points[position_counter++];

           m_emitter_comet->updateParticles_comet(cur,pre);
           m_emitter_comet->drawParticles();
        }
        else
        {
           drawComet = false;
           position_counter = 1;
           m_emitter_comet->resetParticles_comet(m_points[0],pre);
           //pre = this->m_points[position_counter];
           //cur = this->m_points[position_counter++];
        }



        //glFlush();

        CometPos.x = cur.x;
        CometPos.y = cur.y;
        CometPos.z = cur.z;

        if(this->m_follow_comet){
                Vector3 comet_pos;
                comet_pos.x = pre.x - 2.1;
                comet_pos.z = pre.z + 2.1;
                comet_pos.y = pre.y + 2.1;

                updateCamera2(comet_pos);

        }

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_texID_comet);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_texID_comet_normal);

        if(bumpmapping)
        {
            m_shaderPrograms["bumpmapping2"]->bind();
            m_shaderPrograms["bumpmapping2"]->setUniformValue("tex",1);
            m_shaderPrograms["bumpmapping2"]->setUniformValue("tex_normal",2);
        }
        else
        {
            m_shaderPrograms["texture"]->bind();
            m_shaderPrograms["texture"]->setUniformValue("tex",1);
        }


        glPushMatrix();
        //glTranslatef(-6.0f,0.f,0.f);
        glTranslatef(cur.x,cur.y,cur.z);

        gluQuadricNormals(m_quadric_moon,GLU_SMOOTH);
        gluQuadricTexture(m_quadric_moon,GL_TRUE);
        gluSphere(m_quadric_moon,1.0,50.0,50.0);
        glPopMatrix();

        m_shaderPrograms["bumpmapping2"]->release();
        m_shaderPrograms["texture"]->release();
        glBindTexture(GL_TEXTURE_2D,0);
    }
}

void View::drawLine(){

    if(drawline)
    {
        glPushMatrix();
        glLoadIdentity();
        glRotatef(90,1,0,0);
        GLint circle_points = 100;
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3f, 0.0f, 0.5f);
        for (int i = 0; i < circle_points; i++) {
            float angle = 2*PI*i/circle_points;
            glVertex2f(10*cos(angle),10*sin(angle));
        }
        glEnd();
        glPopMatrix();
        glColor4f(1.0f,1.0f,1.0f,0.0f);


        glPushMatrix();
        glLoadIdentity();
        glRotatef(90,1,0,0);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3f, 0.0f, 0.5f);
        for (int i = 0; i < circle_points; i++) {
            float angle = 2*PI*i/circle_points;
            glVertex2f(25*cos(angle),25*sin(angle));
        }
        glEnd();
        glPopMatrix();
        glColor4f(1.0f,1.0f,1.0f,0.0f);


        glColor3f(0.2, 0.2, 0.2);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < this->m_points.size(); i++){
            Vector3 p = m_points[i];
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
        glColor4f(1.0,1.0,1.0,0.0);


        glColor3f(0.2, 0.2, 0.2);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < this->m_random_track.size(); i++){
            Vector3 p = m_random_track[i];
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
        glColor4f(1.0,1.0,1.0,0.0);
    }


}


void View::renderMeteor()
{
    //Get the time in second
    float time = m_increment / (float) 60.0;

    glPushMatrix();
    glTranslatef(-20+time*10,-20+time*10,-20);
    // golden color
    glColor4f(1.0,0.83,0.0,1.0);
    gluSphere(gluNewQuadric(),0.2,15,15);
    glPopMatrix();

    //retore color back to original
    glColor4f(1.0,1.0,1.0,0.0);

    glAccum(GL_MULT,0.5f);
    glAccum(GL_ACCUM,0.5f);
    glAccum(GL_RETURN,1.0f);

}

bool View::Collide_Dectection()
{

    if( (EarthPos-CometPos).length() <= 1.5 + 0.5 )
    {
        cout << "collision occurred !! " << endl;
        drawComet = false;
        collision = true;
        //stop = true;
        CometPos.x = 50; CometPos.y = 50; CometPos.z = -10;
    }
}


void View::resizeGL(int w, int h)
{
    glClear(GL_ACCUM_BUFFER_BIT);
    glViewport(0, 0, w, h);
    updateCamera();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;
    if (!deltaX && !deltaY) return;
    //QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    Vector2 pos(event->x(), event->y());
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        m_camera.mouseMove(pos - m_prevMousePos);
        updateCamera();
    }
    m_prevMousePos = pos;

}

void View::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        m_camera.mouseWheel(event->delta());
        updateCamera();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
    else if (event->key() == Qt::Key_A)
    {
        m_camera.center.x -= 0.5;
        updateCamera();
    }
    else if (event->key() == Qt::Key_D)
    {
        m_camera.center.x += 0.5;
        updateCamera();
    }
    else if (event->key() == Qt::Key_C && drawComet == false)
    {
        cout << "reset !" << endl;

        drawComet = true;
        position_counter = 0;
        CometPos.x = 50; CometPos.y = 50; CometPos.z = -10;

        Vector3 collide_Pos;
        double deg = -120.0;
        collide_Pos.x = EarthPos.x*cos(radian(deg)) - EarthPos.z*sin(radian(deg));
        collide_Pos.z = EarthPos.x*sin(radian(deg)) + EarthPos.z*cos(radian(deg));
        collide_Pos.y = 0;

        cout << "collide pos x : " << collide_Pos.x << endl;
        cout << "collide pos y : " << collide_Pos.y << endl;
        cout << "collide pos z : " << collide_Pos.z << endl;

        m_points.clear();

        for(int i=0 ; i < 360 ; i++)
        {
            Bcurve::computeBezier5points(m_points,CometPos,
                                                  Vector3(-50,0,0),Vector3(-150,20,-20),Vector3(-80,10,-10),
                                                  collide_Pos,
                                                  (float)i/360.0);
        }
    }
    else if (event->key() == Qt::Key_R)
    {
        // Generate the random track
        generate_random_track(this->m_random_track,400,5);
        m_camera.center = EarthPos;
        m_camera_random_track = true;
    }
    // Set camera back to the original position
    else if (event->key() == Qt::Key_T)
    {

        setCameraDefaultValue();
        updateCamera();
        stop = false;
        m_camera_random_track = false;
        m_follow_comet = false;
        flag_resetParticleCube = false;
        collision = false;
        m_emitter_explosion->setRadius(0.01);
        globalcounter = 0;

    }
    // Let the camera follow the comet
    else if (event->key() == Qt::Key_F)
    {
        this->m_follow_comet = true;
    }
    // Pause the movement of the moon and the earth
    else if (event->key() == Qt::Key_P)
    {
         if(stop == false){
             stop =true;
         }
         else{
             stop = false;
         }
    }
    // Switch the camera with different position
    else if (event->key() == Qt::Key_3)
    {
        m_camera.center = EarthPos;
        m_camera.zoom = 12;
        stop = true;
        updateCamera();
    }
    // Enable/Disable Bump Mapping
    else if (event->key() == Qt::Key_B)
    {
        if(bumpmapping == false)
            bumpmapping = true;
        else
            bumpmapping = false;
    }
    else if (event->key() == Qt::Key_L){
        if( drawline == true)
            drawline = false;
        else
            drawline = true;
    }
    else if (event->key() == Qt::Key_1){
        stop = false;
        m_camera_random_track = false;
        m_follow_comet = false;
        setCameraDefaultValue();
        updateCamera();

    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
void View::updateCamera()
{


    float ratio = ((float) width()) / height();
    Vector3 dir(-Vector3::fromAngles(m_camera.theta, m_camera.phi));
    Vector3 eye(m_camera.center - dir * m_camera.zoom);

    //std::cout<<"eye"<<eye<<endl;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective - set up a perspective projection matrix
    //fovy: the angle
    gluPerspective(m_camera.fovy, ratio, 0.1f, 1000.f);
    gluLookAt(eye.x, eye.y, eye.z, m_camera.center.x, m_camera.center.y, m_camera.center.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void View::updateCamera2(Vector3 eye){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective - set up a perspective projection matrix
    //fovy: the angle
    float ratio = ((float) width()) / height();

    gluPerspective(m_camera.fovy, ratio, 0.1f, 1000.f);


    if(m_follow_comet)
    {
        gluLookAt(eye.x, eye.y, eye.z, EarthPos.x, EarthPos.y, EarthPos.z,
                  m_camera.up.x, m_camera.up.y, m_camera.up.z);
    }
    else
    {
        gluLookAt(eye.x, eye.y, eye.z, m_camera.center.x, m_camera.center.y, m_camera.center.z,
                  m_camera.up.x, m_camera.up.y, m_camera.up.z);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void View::generate_random_track(vector<Vector3>& random_track, int points, int control_points){

    Vector3 start_end = EarthPos+15;

    Vector3 ctrl1 = Vector3(rand()%50-49,rand()%50-49,rand()%100-99);
    Vector3 ctrl2 = Vector3(rand()%100-99,rand()%100-99,rand()%100-99);
    Vector3 ctrl3 = Vector3(rand()%50-49,rand()%100-99,rand()%50-49);

    for(int i=0; i<points; i++){
        if(control_points == 4){
            Bcurve::computeBezier4points(random_track,start_end,ctrl1,ctrl2,start_end,(float)i/points);
        }else if(control_points == 5){
            Bcurve::computeBezier5points(random_track,start_end,ctrl1,ctrl2,ctrl3,start_end,(float)i/points);
        }
    }


}

void View::setCameraDefaultValue(){

    m_camera.center = Vector3(0.0f, 0.0f, 0.0f);
    m_camera.up = Vector3(0.0f, 1.f, 0.0f);
    m_camera.zoom = 24.0f;
    m_camera.theta = 0;
    m_camera.phi =0;
    //m_camera.theta = M_PI * 1.5f, m_camera.phi = (1.0f/3);
    m_camera.fovy = 60.0f;

}


void View::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "A: Left shift",this->font());
    renderText(10, 35, "D: Right shift", this->font());
    renderText(10, 50, "R: Random Track", this->font());
    renderText(10, 65, "T: Reset Camera", this->font());
    renderText(10, 80, "F: Follow the Comet", this->font());
    renderText(10, 95, "3: Earth", this->font());
    renderText(10, 110,"B: Enable/Disable Bump Mapping", this->font());
    renderText(10, 125,"L: Show Lines", this->font());
    renderText(10, 140,"Space: quit", this->font());

    if(globalcounter > 265)
    {
        glPushMatrix();
        glLoadIdentity();
        QFont font = QFont("Times New Roman", 30, 4);
        font.bold();
        font.setPointSize(50);
        renderText(width()/2.f-50*3.5, height()/2.f,"Thank you", font);

        glPopMatrix();
    }

}


GLuint View::loadTexture(const QString &path)
{
    QFile file(path);

    QImage image, texture;
    if(!file.exists()) return -1;
    image.load(file.fileName());
    texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D,id);

    glTexImage2D(GL_TEXTURE_2D,0,3,texture.width(),texture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,texture.bits());
    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());


    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return id;
}
