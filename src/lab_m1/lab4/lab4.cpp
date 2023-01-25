#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;


    angularStepHead = 0;
    angularStepBody1 = 0;
    angularStepBody2 = 0;
    angularStepHand1 = 0;
    angularStepHand2 = 0;
    angularStepHand3 = 0;
    angularStepLeft = 0;
    angularStepRight = 0;
  
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    //ma deranja OCD-ul ca nu era ca celelalte :)
    RenderMesh(meshes["box"], shaders["Normal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
   
    //body
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6.0f, 2.2f, -1.5f);
    modelMatrix *= transform3D::Scale(1.0f, 1.6f, 1.0f);
    modelMatrix *= transform3D::RotateOY(angularStepBody1);

    modelMatrix *= transform3D::Translate(0, -0.5, 0);
    modelMatrix *= transform3D::RotateOX(angularStepBody2);
    modelMatrix *= transform3D::Translate(0, 0.5, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepBody2);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);


    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
  
    //head
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6.0f, 3.2f, -1.5f);
    modelMatrix *= transform3D::Translate(0, -0.7, 0);
    modelMatrix *= transform3D::RotateOX(angularStepHead);
    modelMatrix *= transform3D::Translate(0, 0.7, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepHead);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    
    //left leg
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(5.7f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(0.5, 0, 0);
    modelMatrix *= transform3D::RotateOX(angularStepLeft);
    modelMatrix *= transform3D::Translate(-0.5, 0, 0);
    modelMatrix *= transform3D::Scale(0.3f, 0.6f, 0.3f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(5.7f, 1.1f, -1.5f);

    modelMatrix *= transform3D::Translate(0, -0.5, 0);
    modelMatrix *= transform3D::RotateOX(angularStepLeft);
    modelMatrix *= transform3D::Translate(0, 0.5, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepLeft);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);

    modelMatrix *= transform3D::Scale(0.3f, 0.6f, 0.3f);

    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);




    //right leg
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6.3f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(0.5, 0, 0);
    modelMatrix *= transform3D::RotateOX(angularStepRight);
    modelMatrix *= transform3D::Translate(-0.5, 0, 0);
    modelMatrix *= transform3D::Scale(0.3f, 0.6f, 0.3f);

    RenderMesh(meshes["box"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6.3f, 1.1f, -1.5f);
    modelMatrix *= transform3D::Translate(0, -0.5, 0);
    modelMatrix *= transform3D::RotateOX(angularStepRight);
    modelMatrix *= transform3D::Translate(0, 0.5, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepRight);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);
    
    modelMatrix *= transform3D::Scale(0.3f, 0.6f, 0.3f);

    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);







    //left hand
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(5.35f, 2.3f, -1.5f);
    modelMatrix *= transform3D::Translate(0.65, 0, 0);
    modelMatrix *= transform3D::RotateOY(angularStepHand1);
    modelMatrix *= transform3D::Translate(-0.65, 0, 0);

    modelMatrix *= transform3D::Translate(0, -0.7, 0);
    modelMatrix *= transform3D::RotateOX(angularStepHand2);
    modelMatrix *= transform3D::Translate(0, 0.7, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepHand2);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);


    modelMatrix *= transform3D::Translate(-0.2, 0, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepHand3);
    modelMatrix *= transform3D::Translate(0.2, 0, 0);

    modelMatrix *= transform3D::Scale(0.3f, 1.2f, 0.3f);


    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    


    //right hand
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6.65f, 2.3f, -1.5f);
    modelMatrix *= transform3D::Translate(-0.65, 0, 0);
    modelMatrix *= transform3D::RotateOY(angularStepHand1);
    modelMatrix *= transform3D::Translate(0.65, 0, 0);

    modelMatrix *= transform3D::Translate(0, -0.7, 0);
    modelMatrix *= transform3D::RotateOX(angularStepHand2);
    modelMatrix *= transform3D::Translate(0, 0.7, 0);

    modelMatrix *= transform3D::Translate(0, -0.2, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepHand2);
    modelMatrix *= transform3D::Translate(0, 0.2, 0);


    modelMatrix *= transform3D::Translate(-0.2, 0, 0);
    modelMatrix *= transform3D::RotateOX(-angularStepHand3);
    modelMatrix *= transform3D::Translate(0.2, 0, 0);

    modelMatrix *= transform3D::Scale(0.3f, 1.2f, 0.3f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);










}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    // CUB 1  WS - Y, AD - X, RF - Z
    if (window->KeyHold(GLFW_KEY_W)) {
        translateY += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_F)) {
        translateZ += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        translateZ -= deltaTime;
    }

    //CUB 2 1 - big, 2 - smal
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }


    //CUB 3 3,4 - OX, 5,6 - OY, 7,8 - OZ back/face

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        angularStepBody1 += deltaTime;
        angularStepHand1 += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_I)) {
        angularStepLeft += deltaTime;

    }

    if (window->KeyHold(GLFW_KEY_O)) {
        angularStepLeft-= deltaTime;

    }

    if (window->KeyHold(GLFW_KEY_P)) {
        angularStepLeft += deltaTime;
        angularStepRight += deltaTime;
        angularStepBody2 += deltaTime;
        angularStepHead += deltaTime;
        angularStepHand2 += deltaTime;
        angularStepHand3 +=deltaTime;

    }

}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
