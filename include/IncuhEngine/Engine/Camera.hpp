#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <Shader.hpp>
#include <Object.hpp>
#include <Window.hpp>
#include <stdScale.h>
#ifndef NOEXERNLIBS

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <Physics/PhysicsController.hpp>
#endif

#define GRAVITY -9.81f * 1.0f
#define FALLSPEED 5.0f
#define FALLTIME 100
#define JUMPSLOWER 1.4
#define CAM_TURN_SPEED 50

class Camera: public Object{
  public:
    Camera(glm::vec3 newCameraPos, float speed, Shader *shader, glm::mat4 *view_mat, Window *win, PhysicsController *controller);
    virtual ~Camera();

    void setSpeed(float speed);
    float getSpeed() { return base_speed; }
    void run()
    {
        isRun = 1;
        if (cam_speed != base_speed*2){
            cam_speed = base_speed * 2;
        }
    }

    void moveForward();
    void moveBackward();

    void moveUp();
    void moveDown();

    void strafeLeft();
    void strafeRight();

    void turnLeft();
    void turnRight();

    void jump();

    void updateView(glm::mat4 *_view);

    void updateMouse(float x, float y);

    void setDeltaTime(float newTime);

    void update();

    glm::vec3 getPos();
    glm::vec3 getForward();
    void setPos(glm::vec3 newPos);

    float getPitch();
    float getYaw();

    void getTarget(float *target);


  private:
    float cam_speed;
    float base_speed;

    //Basic Camera values
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;

    // Some Camera values
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    // Const values
    glm::vec3 up;
    glm::vec3 right;

    float __deltaTime;

    uint32_t __campos_loc;

    Shader *__shader;

    glm::mat4 *__view;

    float yaw_t;	// -90
    float pitch_t;

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    float lastX = 0.0f;
    float lastY = 0.0f;
    short firstmouse = 1;

    Window *__win;

    //float gravity;
    float *targetForward = nullptr;

    PhysicsController *__controller;

    float stepCount = GRAVITY;

    int isRun = 0;
};

#endif
