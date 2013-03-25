#ifndef VIEW_H
#define VIEW_H

//#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <QHash>
#include <QGLWidget>
#include <QString>

#include "camera.h"
#include "vector.h"
#include "resourceloader.h"
#include "particleemitter.h"

using namespace std;

class QGLShaderProgram;

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();
    void updateCamera();
    void updateCamera2(Vector3 eye);
    QHash<QString, QGLShaderProgram *> getShaderProgram() {return m_shaderPrograms;}

protected:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void renderPlanet();
    void renderMeteor();
    void renderComet();
    void renderScene();
    bool Collide_Dectection();
    void paintText();
    void drawLine();

    // Initialization
    void InitResources();
    void createShaderPrograms();
    void loadCubeMap();
    GLuint loadTexture(const QString& path);

    // particles
    ParticleEmitter *m_emitter_sun;
    ParticleEmitter *m_emitter_comet;
    ParticleEmitter *m_emitter_meteor;
    ParticleEmitter *m_emitter_cube;
    ParticleEmitter *m_emitter_explosion;

private:

    OrbitCamera m_camera;
    Vector2 m_prevMousePos;
    int m_prevTime;

    GLUquadric *m_quadric_sun;
    GLUquadric *m_quadric_earth;
    GLUquadric *m_quadric_moon;
    GLUquadric *m_quadric_saturn;
    GLUquadric *m_quadric_saturn_disk;
    float m_increment;
    GLuint m_texID_earth, m_texID_earth_normal, m_texID_earth_cloud,
           m_texID_moon, m_texID_moon_normal, m_texID_sun,
           m_texID_particle1 , m_texID_comet ,m_texID_comet_normal,
           m_texID_saturn, m_texID_andy;

    Model m_comet;


    //resources
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    GLuint m_skybox;
    GLuint m_cubeMap;
    GLuint m_cubeMap_Andy;

    // particles
    int position_counter;
    int position_counter_camera;
    int track_counter;


    float m_fps;
    vector<Vector3> m_points;
    vector<Vector3> camera_points;
    vector<Vector3> m_random_track;

    // Position
    Vector3 CometPos;
    Vector3 EarthPos;
    Vector3 SunPos;

    // flag
    bool drawComet;
    bool m_camera_random_track;
    bool m_follow_comet;
    bool stop;
    bool bumpmapping;
    bool collision;
    bool drawline;
    bool flag_resetParticleCube;

    void generate_random_track(vector<Vector3>&random_track, int points, int control_points);

    void setCameraDefaultValue();


    QFont m_font;

    int globalcounter;
private slots:
    void tick();



};

#endif // VIEW_H

