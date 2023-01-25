#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2_camera.h"
#include <stack>
#include <utility>
#include <iostream>
#include <algorithm>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        int ValidNeighbour(int index_i, int index_j, int maze_lenght);
        std::pair<int,int> RandomNeighbour(int index_i, int index_j, int maze_lenght);
        int StopPoint(int index_i, int index_j, int maze_lneght, std::vector< std::pair<int, int> > buffer);
        bool isPointInsideAABB(glm::vec3 position1, glm::vec3 position2, glm::vec3 proprietes1, glm::vec3 proprietes2);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        std::string projectionType;
        float fov, fovSpeed;
        float width, height;
        float zNear, zFar;
        //maze variables
        int value;
        int index_i, index_j;
        int next_i, next_j;
        int current_i, current_j;
        int maze_l;
        std::vector< std::pair<int, int> >::iterator in_visited;
        std::pair<int, int> stack_buffer;
        glm::vec3 finishPos;
        std::pair<int, int> spawnPoint;
        std::stack< std::pair<int,int> > history;
        std::vector< std::pair<int, int> > visited;
        std::vector<glm::vec3> finish;
        std::vector< std::pair<int, int> > spawnCoord;
        std::vector<glm::vec3> wallCoordVec;
        glm::vec3 wallCoord;
        bool wallHit;
        bool exitPoint;
        float collisionHeadFactor;
        float collisionWallFactor;
        //player variables
        float head_x, head_y, head_z;
        float rotation;

        //projectile
        bool ready;
        glm::vec3 projectilePos;
        glm::vec3 projectileDir;
        bool pullProjectile;
        int cooldown;

        //enemy
        glm::vec3 enemyPos;
        glm::vec3 enemyDir;
        float varE, enemyX, enemyZ;
        bool enemyHit;
        float enemyColisionFactor;

        //life
        glm::vec3 lifePos;
        int time;

        

    };
}   // namespace m1
