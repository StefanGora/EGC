#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <algorithm>
#include "lab_m1/tema2/transform3D.h"

using namespace std;
using namespace m1;


int maze[30][30];


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    projectionType = "perspective";
    fov = RADIANS(60.0f);
    fovSpeed = 5;

    width = 5;
    height = 5;

    zNear = 0.01f;
    zFar = 200.0f;
}


Tema2::~Tema2()
{
}

int Tema2::ValidNeighbour(int index_i, int index_j, int maze_lenght) {
    if (index_i < 0) {
        return -1;
    }
    else if (index_i >= maze_lenght) {
        return -1;
    }
    else if (index_j < 0) {
        return -1;
    }
    else if (index_j >= maze_lenght) {
        return -1;
    }
    else {
        return 0;
    }
}




std::pair<int, int> Tema2::RandomNeighbour(int index_i, int index_j, int maze_lenght) {
    std::vector< std::pair<int, int> > neighbours_vector;
    std::pair<int, int> coordonates;
    std::pair<int, int> neighbour;
    int length = 0;
    int index = 0;
    if (ValidNeighbour(index_i, index_j - 2, maze_lenght) == 0) {
        coordonates.first = index_i;
        coordonates.second = index_j - 2;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i - 2, index_j, maze_lenght) == 0) {
        coordonates.first = index_i - 2;
        coordonates.second = index_j;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i + 2, index_j, maze_lenght) == 0) {
        coordonates.first = index_i + 2;
        coordonates.second = index_j;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i, index_j + 2, maze_lenght) == 0) {
        coordonates.first = index_i;
        coordonates.second = index_j + 2;
        neighbours_vector.push_back(coordonates);
    }
    length = neighbours_vector.size();
    index = rand() % length;
    neighbour = neighbours_vector[index];

    return neighbour;
}

int Tema2::StopPoint(int index_i, int index_j, int maze_lenght, std::vector< std::pair<int, int> > buffer) {
    std::vector< std::pair<int, int> > neighbours_vector;
    std::vector< std::pair<int, int> >::iterator found;
    std::pair<int, int> coordonates;
    int index = 0;
    int length = 0;
    if (ValidNeighbour(index_i, index_j - 2, maze_lenght) == 0) {
        coordonates.first = index_i;
        coordonates.second = index_j - 2;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i - 2, index_j, maze_lenght) == 0) {
        coordonates.first = index_i - 2;
        coordonates.second = index_j;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i + 2, index_j, maze_lenght) == 0) {
        coordonates.first = index_i + 2;
        coordonates.second = index_j;
        neighbours_vector.push_back(coordonates);
    }
    if (ValidNeighbour(index_i, index_j + 2, maze_lenght) == 0) {
        coordonates.first = index_i;
        coordonates.second = index_j + 2;
        neighbours_vector.push_back(coordonates);
    }


    for (index = 0; index < neighbours_vector.size(); index++) {
        //found = std::find(buffer.begin(), buffer.end(), neighbours_vector[index] );
        if (!std::count( buffer.begin(), buffer.end(), neighbours_vector[index] ) ){
            return -1;
        }
    }
    return 0;


}

bool Tema2::isPointInsideAABB(glm::vec3 position1, glm::vec3 position2, glm::vec3 proprietes1, glm::vec3 proprietes2) {
    return (position1.x - proprietes1.x/2 <= position2.x + proprietes2.x/2 && position1.x + proprietes1.x / 2 >= position2.x - proprietes2.x/2) && 
        (position1.z - proprietes1.z / 2 <= position2.z + proprietes2.z / 2 && position1.z + proprietes1.z / 2 >= position2.z - proprietes2.x / 2);
}



void Tema2::Init()
{
    time = 0;


    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);


    next_i = 1;
    next_j = 1;
    maze_l = 30;

    for (int i = 0; i < maze_l; i++) {
        for (int j = 0; j < maze_l; j++) {
            maze[i][j] = 0;

        }
    }
    stack_buffer = make_pair(1, 1);
    history.push(stack_buffer);
    visited.push_back(stack_buffer);

    while (history.size() > 0)
    {
        if (StopPoint(next_i, next_j, maze_l, visited) == 0) {
            value = rand() % 4 == 0 ? 2 : 1;
            maze[next_i][next_j] = value;
            history.pop();
            if (history.size() != 0) {
                stack_buffer = history.top();
                next_i = stack_buffer.first;
                next_j = stack_buffer.second;
            }

        }
        else {
            current_i = next_i;
            current_j = next_j;
            value = rand() % 4 == 0 ? 2 : 1;
            maze[current_i][current_j] = value;
            stack_buffer = RandomNeighbour(next_i, next_j, maze_l);
            if (!std::count(visited.begin(), visited.end(), stack_buffer)) {
                visited.push_back(stack_buffer);
            }
            history.push(stack_buffer);
            next_i = stack_buffer.first;
            next_j = stack_buffer.second;
            if (next_i == current_i) {
                if ((next_j - current_j) > 0) {
                    maze[current_i][next_j - 1] = value;
                }
                else {
                    maze[current_i][next_j + 1] = value;
                }
            }
            if (next_j == current_j) {
                if ((next_i - current_i) > 0) {
                    maze[next_i-1][current_j] = value;
                }
                else {
                    maze[next_i+1][current_j] = value;
                }
            }
            //in_visited = std::find(visited.begin(), visited.end(), stack_buffer);


        }
    }

    index_i = 0;
    index_j = 0;
    while (maze[index_i][index_j] != 1) {
        index_i = rand() % maze_l;
        index_j = rand() % maze_l;
    }
    //Player
    head_x =  -index_j*50;
    head_y = 11.5;
    head_z =  - index_i * 50;

    renderCameraTarget = false;
    ready = false;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(head_x, head_y + 7, head_z + camera->distanceToTarget), glm::vec3(head_x, head_y, head_z), glm::vec3(0, 1, 0));


    //projectile
    pullProjectile = false;
    finishPos = glm::vec3(0,0,0);
    for (int i = 0; i < maze_l; i++) {
        for (int j = 0; j < maze_l; j++) {
            if (maze[i][j] == 1 && i == maze_l) {
                finishPos = glm::vec3(-j*50,0,-i*50);
                finish.push_back(finishPos);
            }
            else if (maze[i][j] == 1 && j == maze_l) {
                finishPos = glm::vec3(-j * 50, 0, -i * 50);
                finish.push_back(finishPos);
            }
        }


    }



    
    for (int i = 0; i < maze_l; i++) {
        for (int j = 0; j < maze_l; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");

    }
    

    collisionHeadFactor = 3;
    collisionWallFactor = 50;
    wallHit = false;
    for (int i = 0; i < maze_l; i++) {
        for (int j = 0; j < maze_l; j++) {
            if (maze[i][j] == 0) {
                wallCoord = glm::vec3(-j * 50, 0, -i * 50);
                wallCoordVec.push_back(wallCoord);
            }
        }
    }

    exitPoint = false;
    cooldown = 0;

    enemyColisionFactor = 10;
            
    
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    time++;
    if (time == 1000) {
        exit(0);
    }
    {


        varE -= deltaTimeSeconds / 2;
        if (varE < -6.6f) {
            varE = 6.6f;
        }
        enemyX = cos(2 * varE) * 18.0f;
        enemyZ = (sin(4 * varE) / 2) * 18.0f;

        glm::mat4 modelMatrix = glm::mat4(1);






        
        for (int i = 0; i < maze_l; i++) {
            for (int j = 0; j < maze_l; j++) {
                if (maze[i][j] == 0) {

                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-j * 50, 0, -i * 50));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 25, 0));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(50));
                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 0));
                    modelMatrix = glm::mat4(1);
                }
                else if (maze[i][j] == 2) {
                    enemyPos = glm::vec3(-j * 50 + enemyX, 0, -i * 50 + enemyZ);
                    modelMatrix = glm::translate(modelMatrix, enemyPos);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 0));
                    modelMatrix = glm::mat4(1);

                }
            }
        }




        glm::vec3 cam = camera->GetTargetPosition();
        cam.y = 0;
        head_x = cam.x;
        head_z = cam.z;

        lifePos = camera->position;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(lifePos.x ,lifePos.y,lifePos.z);
       modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(5, 3, 5);
        modelMatrix *= transform3D::Scale(6, 0.5, 0);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0.26));
        //head
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation);
        modelMatrix *= transform3D::Scale(3,3,3);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.973, 0.718, 0.49));
        //head

        //body
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(0, -4.5, 0);
        modelMatrix *= transform3D::Scale(3, 6, 1);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0., 0.518, 0.698));
        //body
        
        //right leg
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(-0.7, -9.5, 0);
        modelMatrix *= transform3D::Scale(1.5, 4, 1);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.141, 0.208, 0.49));
        //right leg

        //left leg
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(0.75, -9.5, 0);
        modelMatrix *= transform3D::Scale(1.5, 4, 1);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.141, 0.208, 0.49));
        //left leg

        //left hand
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(-2.3, -3.5, 0);
        modelMatrix *= transform3D::Scale(1.5, 4, 1);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.973, 0.718, 0.49));
        //left leg

        //right_hand
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(head_x, head_y, head_z);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(2.3, -3.5, 0);
        modelMatrix *= transform3D::Scale(1.5, 4, 1);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.973, 0.718, 0.49));
        //right_hand






        glm::mat4 modelMatrix1 = glm::mat4(1);
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(100));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(-20, 0, -20));
        RenderSimpleMesh(meshes["plane50"], shaders["LabShader"], modelMatrix1, glm::vec3(0, 0, 1));

        
        if (ready == true && pullProjectile == true) {
            cooldown++;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(projectilePos.x, projectilePos.y, projectilePos.z);
            projectilePos += projectileDir  * deltaTimeSeconds * glm::vec3(10);
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0));
        }

        if (cooldown == 200) {
            cooldown = 0;
            pullProjectile = false;
            projectileDir = camera->forward;
            projectilePos = camera->GetTargetPosition();
        }





    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.




    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}




/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */



void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        pullProjectile = true;
        projectileDir = camera->forward;
        projectilePos = camera->GetTargetPosition();

    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        float cameraSpeed = 15.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(deltaTime * cameraSpeed);
            for (int i = 0; i < wallCoordVec.size(); i++) {
                wallHit = isPointInsideAABB(camera->GetTargetPosition(), wallCoordVec[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (wallHit == true) {
                    camera->MoveForward(-2);
                }

            }

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
            for (int i = 0; i < wallCoordVec.size(); i++) {
                wallHit = isPointInsideAABB(camera->GetTargetPosition(), wallCoordVec[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (wallHit == true) {
                    camera->TranslateRight(2);
                }

            }
            for (int i = 0; i < finish.size(); i++) {
                exitPoint = isPointInsideAABB(camera->GetTargetPosition(), finish[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (exitPoint == true) {
                    exit(0);
                }

            }

        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward

            camera->MoveForward(-deltaTime * cameraSpeed);
            for (int i = 0; i < wallCoordVec.size(); i++) {
                wallHit = isPointInsideAABB(camera->GetTargetPosition(), wallCoordVec[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (wallHit == true) {
                    camera->MoveForward(10);
                }

            }
            for (int i = 0; i < finish.size(); i++) {
                exitPoint = isPointInsideAABB(camera->GetTargetPosition(), finish[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (exitPoint == true) {
                    exit(0);
                }

            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
            for (int i = 0; i < wallCoordVec.size(); i++) {
                wallHit = isPointInsideAABB(camera->GetTargetPosition(), wallCoordVec[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (wallHit == true) {
                    camera->TranslateRight(-2);
                }

            }
            for (int i = 0; i < finish.size(); i++) {
                exitPoint = isPointInsideAABB(camera->GetTargetPosition(), finish[i], glm::vec3(collisionHeadFactor), glm::vec3(collisionWallFactor));
                if (exitPoint == true) {
                    exit(0);
                }

            }
        }





  
    }
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, "Model");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "View");

    
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    location = glGetUniformLocation(shader->program, "Projection");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    location = glGetUniformLocation(shader->program, "color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnKeyPress(int key, int mods)
{

    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
        if (renderCameraTarget) {
            ready = true;
            projectileDir = camera->forward;
            projectilePos = camera->GetTargetPosition();
            //camera->Set(glm::vec3(head_x, head_y + 5, head_z), glm::vec3(head_x, head_y +5, head_z - 2), glm::vec3(0, 1, 0));
            camera->distanceToTarget = -10;
            camera->MoveForward(10.0f);
        }
        else {
            //camera->Set(glm::vec3(head_x, head_y + 4, head_z + 12), glm::vec3(head_x, head_y + 4, head_z), glm::vec3(0, 1, 0)); //bun
            camera->distanceToTarget = 10;
            camera->MoveForward(-10.0f);
            ready = false;
            pullProjectile = false;
        }
        
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    

    
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        rotation = atan2((camera->forward.x), camera->forward.z);
        if (renderCameraTarget) {


            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (!renderCameraTarget) {

        
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
