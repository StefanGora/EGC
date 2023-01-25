#pragma once

#include "components/simple_scene.h"
#include <cmath>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;

        //player variables
        float squareSide;
        float translateX,translateY;
        float lilCircleSize;
        float translateLcX, translateLcY;
        float translateRcX, translateRcY;
        float angle;

        //map variables
        float mapSize;

        //enemy variables
        float enemySize;
        float enemyCount;
        float enemyAngle;

        //healthBar variables
        float healthBarStrokeX;
        float healthBarX;
        float healthBarY;
        float healthBarSize;
        float healthBarStrokeSize;
        int maxHitPoints;

        //projectile variables
        float projectileX, projectileY;
        float projectileAngle;
        float projectileSize;
        bool pullProjectile;
        float cooldown;

        // drop variables
        float dropCooldown;
        float dropX;
        float dropY;
        float dropSize;

        //shuriken variables
        float shurikenX, shurikenY;
        float angularStep;
        float tinyShurikenX;
        float tinyShurikenY;
        float shurikenRotation;
        bool pullShuriken;
        float rasenShuriken;
        float shurikenCooldown;
        float shurikenAngle;

        //heal variables
        float healthCircleSize;
        float healthRectangularSize;

        //wall varibles
        float gateX;
        float gateY;
        float gateCooldown;
        float wallLengthSquare;
        float wallLength;
        float squareWallX;
        float squareWallY;
  

        //other variables
        float colisionFactor;
        int i;
        int score;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
