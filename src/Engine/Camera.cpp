#include <Camera.hpp>

Camera::Camera(glm::vec3 newCameraPos, float speed, Shader *shader, glm::mat4 *view_mat, Window *win, PhysicsController *controller) :
cam_speed(speed),
cameraPos(newCameraPos),
cameraFront(glm::vec3(0.0f, 0.0f, 1.0f)),
cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
up(glm::vec3(0.0f, 1.0f, 0.0f)),
right(glm::vec3(1.0f, 0.0f, 0.0f)),
__deltaTime(1.0f),
__shader(shader),
__view(view_mat),
yaw_t(90.0f),
pitch_t(0.0f),
lastX(400),
lastY(300),
__win(win),
__controller(controller),
stepCount(0)
{
    __campos_loc = __shader->getUniformLocation("cameraPos");
    targetForward = (float*) malloc(sizeof(float)*3);
    targetForward[0] = 0.0f;
    targetForward[1] = FALLSPEED;
    targetForward[2] = 0.0f;

}
Camera::~Camera()
{

}

void Camera::setSpeed(float speed)
{
    cam_speed = speed;
}

void Camera::moveForward()
{
    targetForward[0] += cameraFront.x * (cam_speed) * __deltaTime;
    targetForward[2] += cameraFront.z * (cam_speed) * __deltaTime;
}
void Camera::moveBackward()
{
    targetForward[0] -= cameraFront.x * (cam_speed) * __deltaTime;
    targetForward[2] -= cameraFront.z * (cam_speed) * __deltaTime;
}

void Camera::moveUp()
{

    pitch_t += CAM_TURN_SPEED * __deltaTime;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch_t > 89.0f)
        pitch_t = 89.0f;
    if (pitch_t < -89.0f)
        pitch_t = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    front.y = sin(glm::radians(pitch_t));
    front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(front);
}

void Camera::moveDown()
{

    pitch_t -= CAM_TURN_SPEED * __deltaTime;

    if (pitch_t > 89.0f)
        pitch_t = 89.0f;
    if (pitch_t < -89.0f)
        pitch_t = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    front.y = sin(glm::radians(pitch_t));
    front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(front);
}

void Camera::strafeLeft()
{
    targetForward[0] -= (glm::normalize(glm::cross(cameraFront, cameraUp)).x * cam_speed) * __deltaTime;
    targetForward[2] -= (glm::normalize(glm::cross(cameraFront, cameraUp)).z * cam_speed) * __deltaTime;
}
void Camera::strafeRight()
{
    targetForward[0] += (glm::normalize(glm::cross(cameraFront, cameraUp)).x * cam_speed) * __deltaTime;
    targetForward[2] += (glm::normalize(glm::cross(cameraFront, cameraUp)).z * cam_speed) * __deltaTime;
}

void Camera::jump(){
	if (__controller->onGround()){
		stepCount = FALLTIME;
	}
}

void Camera::turnLeft()
{

    yaw_t -= CAM_TURN_SPEED * __deltaTime;
    //pitch_t += 1;

    if (pitch_t > 89.0f)
        pitch_t = 89.0f;
    if (pitch_t < -89.0f)
        pitch_t = -89.0f;

    if (yaw_t <= 0)
    {
        yaw_t = 360;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    front.y = sin(glm::radians(pitch_t));
    front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(front);

}
void Camera::turnRight()
{

    yaw_t += CAM_TURN_SPEED * __deltaTime;
    //pitch_t += 1;

    if (pitch_t > 89.0f)
        pitch_t = 89.0f;
    if (pitch_t < -89.0f)
        pitch_t = -89.0f;

    if (yaw_t >= 360)
    {
        yaw_t = 0;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    front.y = sin(glm::radians(pitch_t));
    front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(front);
}

void Camera::update()
{
    __shader->useMain();
    int x,y;
    SDL_GetMouseState(&x, &y);

    updateMouse((float) x, (float) y);
    glm::vec3 newPos;
    // New pos should be assigned to the value of the rigid body


    //*__view = glm::lookAt(newPos, newPos + cameraFront, cameraUp);
    *__view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    __shader->setVec3(__campos_loc, newPos);
    targetForward[0] = 0.0f;
    if (stepCount > 0){
    	targetForward[1] = -(FALLSPEED/JUMPSLOWER);
    	stepCount--;
    }
    else{
    	targetForward[1] = FALLSPEED;
    }
    targetForward[2] = 0.0f;
    cameraFront = glm::normalize(cameraFront);
}

void Camera::setDeltaTime(float newTime)
{
    __deltaTime = newTime;
}


glm::vec3 Camera::getPos()
{
    return cameraPos;
}

glm::vec3 Camera::getForward()
{
    return cameraFront;
}
void Camera::setPos(glm::vec3 newPos)
{
    cameraPos = newPos;
    float newControlPos[3];
    newControlPos[0] = newPos.x;
    newControlPos[1] = newPos.y;
    newControlPos[2] = newPos.z;
    __controller->translate(newControlPos);
}

float Camera::getPitch()
{
    return pitch_t;
}
float Camera::getYaw()
{
    return yaw_t;
}

void Camera::updateMouse(float x, float y)
{
    __win->resetCursor();

    float xoffset = x - (__win->getWidth()/2);
    float yoffset = (__win->getHeight()/2) - y;


    float sensitivity = 0.3f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw_t   += xoffset;
    pitch_t += yoffset;

    if(pitch_t > 89.0f)
        pitch_t = 89.0f;
    if(pitch_t < -89.0f)
        pitch_t = -89.0f;


    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    direction.y = sin(glm::radians(pitch_t));
    direction.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(direction);

}


void Camera::getTarget(float *target){
    memcpy(target, targetForward, sizeof(float) * 3);
}
