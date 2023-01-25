#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <cmath>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


vector<float> horizontalWallX = { 0,1100,900,0 };
vector<float> horizontalWallY = { 400,750, 750, 1000 };
vector<float> verticalWallX = { 0, 900, 400 };
vector<float> verticalWallY = { 0, 1100, 850 };
vector<float> enemy_X;
vector<float> enemy_Y;
vector<float> enemyEyes_X;
vector<float> enemyEyes_Y;
vector<float> hitPointsX;


 


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    //other variables
    i = 0;
    score = 0;

    //map variables
    mapSize = 1500;

    //player variables
    squareSide = 50;
    translateX = 750;
    translateY = 750;
    lilCircleSize = 15;
    translateLcX = translateX - squareSide / 2;
    translateLcY = translateY + squareSide;
    translateRcX = translateX + squareSide / 2;
    translateRcY = translateY + squareSide;
    angle = 0;
    
   //wall varibles
    gateX = 900;
    gateY = 750;
    gateCooldown = 0;
    wallLengthSquare = 300;
    wallLength = 100;
    squareWallX = mapSize - wallLengthSquare;
    squareWallY = 0;



    //enemy variables
    enemySize = 90;
    enemyCount = 5;
    //fill enemy vectors with random positions
    for (i = 0; i < enemyCount; i++) {
        if (i % 2 == 0) {
            enemy_X.push_back(std::rand() % (int)(mapSize - 0.75 * mapSize + 1));
        }
        else {
            enemy_X.push_back((0.75 * mapSize) + (std::rand() % (int)(mapSize - 0.75 * mapSize + 1)));
        }
        enemy_Y.push_back(std::rand() % (int)(mapSize - enemySize + 1));
        enemyEyes_X.push_back(enemy_X[i]);
        enemyEyes_Y.push_back(enemy_Y[i]);
    }

    //healthBar variables
    healthBarStrokeX = translateX + 360;
    healthBarX = translateX + 360;
    healthBarY = translateY + 290;
    healthBarSize = 17.5;
    maxHitPoints = 16;
    healthBarStrokeSize = 70;
    //set X for each hit point
    for (i = 0; i < maxHitPoints; i++) {
        hitPointsX.push_back(healthBarX);
        healthBarX += healthBarSize;
    }

    //projectile variables
    projectileAngle = 0;
    projectileSize = 20;
    pullProjectile = false;
    cooldown = 0;

    // drop variables
    dropCooldown = 0;
    dropX = 0;
    dropY = 0;
    dropSize = 30;


    //shuriken variables
    angularStep = 0;
    tinyShurikenX = healthBarStrokeX + lilCircleSize;
    tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
    shurikenRotation = 0;
    pullShuriken = false;
    rasenShuriken = 3;
    shurikenCooldown = 0;

    //heal variables
    healthCircleSize = 30;
    healthRectangularSize = 7;

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(translateX - resolution.x/2, translateY - resolution.y / 2, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    glm::vec3 corner = glm::vec3(0, 0, 0);


    //player meshes
    Mesh* lilCircle = object2D::CreateCircle("lilCircle", corner, lilCircleSize, glm::vec3(1, 1, 0), true);
    AddMeshToList(lilCircle);

    Mesh* emptyLilCircle = object2D::CreateCircle("emptyLilCircle", corner, lilCircleSize, glm::vec3(0, 0, 0), false);
    AddMeshToList(emptyLilCircle);

    Mesh* circle = object2D::CreateCircle("circle", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(circle);

    Mesh* emptyCircle = object2D::CreateCircle("emptyCircle", corner, squareSide, glm::vec3(0, 0, 0), false);
    AddMeshToList(emptyCircle);
    //player meshes

    //wall meshes
    Mesh* horizontalWall = object2D::CreateHorizontalRectangle("horizontalWall", corner, wallLength, glm::vec3(0, 1, 0), true);
    AddMeshToList(horizontalWall);
    Mesh* verticalWall = object2D::CreateVerticalRectangle("verticalWall", corner, wallLength, glm::vec3(0, 1, 0), true);
    AddMeshToList(verticalWall);
    Mesh* wallSquare = object2D::CreateSquare("wallSquare", corner, wallLengthSquare, glm::vec3(0, 1, 0), true);
    AddMeshToList(wallSquare);
    //wall meshes

    //map meshes
    Mesh* map = object2D::CreateSquare("map", corner, mapSize, glm::vec3(1, 0, 0), false);
    AddMeshToList(map);
    //map meshes

    //enemy meshes
    Mesh* enemy = object2D::CreateSquare("enemy", corner, enemySize, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemy);

    Mesh* enemyStroke = object2D::CreateSquare("enemyStroke", corner, enemySize, glm::vec3(0, 0, 0), false);
    AddMeshToList(enemyStroke);

    Mesh* enemyEyeStroke = object2D::CreateSquare("enemyEyeStroke", corner, projectileSize, glm::vec3(0, 0, 0), false);
    AddMeshToList(enemyEyeStroke);

    Mesh* enemyEye = object2D::CreateSquare("enemyEye", corner, projectileSize, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemyEye);
    //enemy meshes

    //projectile meshes
    Mesh* square = object2D::CreateSquare("square", corner, projectileSize, glm::vec3(0, 0, 0), true);
    AddMeshToList(square);
    //projectile meshes

    //healthBar meshes
    Mesh* healthBar = object2D::CreateVerticalRectangle("healthBar", corner, healthBarSize, glm::vec3(1, 0, 0), true);
    AddMeshToList(healthBar);
    Mesh* healthBarStroke = object2D::CreateHorizontalRectangle("healthBarStroke", corner, healthBarStrokeSize, glm::vec3(0, 0, 0), false);
    AddMeshToList(healthBarStroke);
    //healthBar meshes

    //shuriken meshes
    Mesh* shuriken = object2D::CreateShuriken("shuriken", corner, squareSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(shuriken);

    Mesh* lilShuriken = object2D::CreateShuriken("lilShuriken", corner, dropSize, glm::vec3(0, 0, 0), true);
    AddMeshToList(lilShuriken);

    Mesh* tinyShuriken = object2D::CreateShuriken("tinyShuriken", corner, lilCircleSize, glm::vec3(0, 0, 0), true);
    AddMeshToList(tinyShuriken);
    //shuriken meshes

    //heal meshes
    Mesh* healthCircle = object2D::CreateCircle("healthCircle", corner, healthCircleSize, glm::vec3(0, 1, 0), true);
    AddMeshToList(healthCircle);
    Mesh* horizontalHealthB = object2D::CreateHorizontalRectangle("horizontalHealthB", corner, healthRectangularSize, glm::vec3(1, 1, 1), true);
    AddMeshToList(horizontalHealthB);
    Mesh* verticalHealthB = object2D::CreateVerticalRectangle("verticalHealthB", corner, healthRectangularSize, glm::vec3(1, 1, 1), true);
    AddMeshToList(verticalHealthB);
    //heal meshes

  
    

    
    

}

bool collisionSqareWall( float PlayerX, float PlayerY, float WallX, float WallY, float PlayerSize, float WallSize) // AABB - AABB collision
{

    // collision x-axis?
    bool collisionX = (PlayerX + PlayerSize >= WallX) && (WallX + WallSize >= PlayerX);
    // collision y-axis?
    bool collisionY = (PlayerY + PlayerSize >= WallY) && (WallY + WallSize >= PlayerY);
    // collision only if on both axes
    return collisionX && collisionY;
};

bool collisionHorizontalWall(float PlayerX, float PlayerY, float WallX, float WallY, float PlayerSize, float WallSize) // AABB - AABB collision
{

    // collision x-axis?
    bool collisionX = (PlayerX + PlayerSize >= WallX) && (WallX + 4 * WallSize >= PlayerX);
    // collision y-axis?
    bool collisionY = (PlayerY + PlayerSize >= WallY) && (WallY + WallSize >= PlayerY);
    // collision only if on both axes
    return collisionX && collisionY;
};

bool collisionVerticalWall(float PlayerX, float PlayerY, float WallX, float WallY, float PlayerSize, float WallSize) // AABB - AABB collision
{

    // collision x-axis?
    bool collisionX = (PlayerX + PlayerSize >= WallX) && (WallX + WallSize >= PlayerX);
    // collision y-axis?
    bool collisionY = (PlayerY + PlayerSize >= WallY) && (WallY + 4 * WallSize >= PlayerY);
    // collision only if on both axes
    return collisionX && collisionY;
};



void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.752941, 0.752941, 0.752941, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    

    //rander tiny rectangulars as hit points
    for (i = 0; i < maxHitPoints; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(hitPointsX[i], healthBarY);
        RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);
    }

    //rander max health stroke
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(healthBarStrokeX, healthBarY);
    RenderMesh2D(meshes["healthBarStroke"], shaders["VertexColor"], modelMatrix);
    //rander HealthBar


    // rander player
    
    //left eye
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(-angle - M_PI / 2);
    modelMatrix *= transform2D::Translate(translateLcX, translateLcY);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);
    RenderMesh2D(meshes["emptyLilCircle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(-angle - M_PI / 2);
    modelMatrix *= transform2D::Translate(translateLcX, translateLcY);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);
    RenderMesh2D(meshes["lilCircle"], shaders["VertexColor"], modelMatrix);
    //left eye

    //right eye
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(-angle - M_PI / 2);
    modelMatrix *= transform2D::Translate(translateRcX, translateRcY);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);
    RenderMesh2D(meshes["emptyLilCircle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(-angle - M_PI / 2);
    modelMatrix *= transform2D::Translate(translateRcX, translateRcY);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);
    RenderMesh2D(meshes["lilCircle"], shaders["VertexColor"], modelMatrix);
    //right eye

    //player body
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    RenderMesh2D(meshes["emptyCircle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    //player body

    // rander player

    //rander drop
    dropCooldown++;

    //init drop X and Y after dropCooldown
    if (dropCooldown == 150 || dropCooldown == 400) {
        dropX = std::rand() % (int)(mapSize - enemySize + 1);
        dropY = std::rand() % (int)(mapSize - enemySize + 1);
    }

    //shuriken spawn
    if (dropCooldown > 150 && dropCooldown < 400) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(dropX, dropY);
        RenderMesh2D(meshes["lilShuriken"], shaders["VertexColor"], modelMatrix);
    }
    // heal spawn
    if (dropCooldown > 400 && dropCooldown < 750) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(dropX - 2 * healthRectangularSize, dropY);
        RenderMesh2D(meshes["horizontalHealthB"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(dropX - healthRectangularSize / 2, dropY - 1.5 * healthRectangularSize);
        RenderMesh2D(meshes["verticalHealthB"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(dropX, dropY);
        RenderMesh2D(meshes["healthCircle"], shaders["VertexColor"], modelMatrix);

    }
    //check if player takes shuriken drop
    if (collisionSqareWall(translateX, translateY, dropX, dropY, squareSide, healthCircleSize) == true && dropCooldown < 400)
    {
        dropCooldown = 0;
        dropX = std::rand() % (int)(mapSize - enemySize + 1);
        dropY = std::rand() % (int)(mapSize - enemySize + 1);
        rasenShuriken++;
        if (rasenShuriken > 3) {
            rasenShuriken = 3;
        }

    }
    //check if player takes heal drop
    if (collisionSqareWall(translateX, translateY, dropX, dropY, squareSide, squareSide) == true && dropCooldown > 400) {

        dropCooldown = 0;
        dropX = std::rand() % (int)(mapSize - enemySize + 1);
        dropY = std::rand() % (int)(mapSize - enemySize + 1);
        if (maxHitPoints < 16) {
            maxHitPoints++;
        }
        ;
    }

    //if drop spanws in a well cange drop X and Y
    if (collisionSqareWall(dropX, dropY, squareWallX, squareWallY, dropSize, squareSide) == true) {
        dropX = std::rand() % (int)(mapSize - enemySize + 1);
        dropY = std::rand() % (int)(mapSize - enemySize + 1);
    }

    if (collisionSqareWall(dropX, dropY, squareWallX, squareWallY, dropSize, wallLengthSquare) == true) {
        dropX = std::rand() % (int)(mapSize - enemySize + 1);
        dropY = std::rand() % (int)(mapSize - enemySize + 1);
    }
    for (i = 0; i < 4; i++) {
        if (collisionHorizontalWall(dropX, dropY, horizontalWallX[i], horizontalWallY[i], dropSize, wallLength) == true) {
            dropX = std::rand() % (int)(mapSize - enemySize + 1);
            dropY = std::rand() % (int)(mapSize - enemySize + 1);
        }
    }
    for (i = 0; i < 3; i++) {
        if (collisionVerticalWall(dropX, dropY, verticalWallX[i], verticalWallY[i], dropSize, wallLength) == true) {
            dropX = std::rand() % (int)(mapSize - enemySize + 1);
            dropY = std::rand() % (int)(mapSize - enemySize + 1);
        }
    }
    if (collisionVerticalWall(dropX, dropY, gateX, gateY, dropSize, wallLength) == true && gateCooldown > 100) {
            dropX = std::rand() % (int)(mapSize - enemySize + 1);
            dropY = std::rand() % (int)(mapSize - enemySize + 1);
    }
    //if drop spanws in a well cange drop X and Y

    //reset drop cooldown
    if (dropCooldown > 750) {
        dropCooldown = 0;
    }

    //rander how many shurikens has the player as amunition
    if (rasenShuriken == 1) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
    }
    else if (rasenShuriken == 2) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX + 50, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
    }
    else if (rasenShuriken == 2) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX + 50, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX + 100, tinyShurikenY);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
    }
    else if (rasenShuriken == 3) {
        angularStep += deltaTimeSeconds;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX, tinyShurikenY);
        modelMatrix *= transform2D::Rotate(angularStep * 5);

        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX + 50, tinyShurikenY);
        modelMatrix *= transform2D::Rotate(angularStep * 5);

        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tinyShurikenX + 100, tinyShurikenY);
        modelMatrix *= transform2D::Rotate(angularStep * 5);
        RenderMesh2D(meshes["tinyShuriken"], shaders["VertexColor"], modelMatrix);
    }
    //rander how many shurikens has the player as amunition
    
    //rander drop

    //rander HealthBar



    //rander enemy
    //enemy get the angle betwin them and player
    for (i = 0; i < enemyCount; i++) {
        enemyAngle = atan2(translateY - enemy_Y[i], translateX - enemy_X[i]);
        enemy_X[i] -= sin(enemyAngle - M_PI / 2) * deltaTimeSeconds * (50 + (std::rand() % (90 - 50 + 1)));
        enemy_Y[i] += cos(enemyAngle - M_PI / 2) * deltaTimeSeconds * (50 + (std::rand() % (90 - 50 + 1)));

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i] + enemySize - 1.3 * lilCircleSize, enemy_Y[i] - lilCircleSize);
        RenderMesh2D(meshes["enemyEyeStroke"], shaders["VertexColor"], modelMatrix);
        
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i] + enemySize - 1.3* lilCircleSize, enemy_Y[i]-lilCircleSize);
        RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], modelMatrix);
        
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i], enemy_Y[i] - lilCircleSize);
        RenderMesh2D(meshes["enemyEyeStroke"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i], enemy_Y[i]-lilCircleSize);
        RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i], enemy_Y[i]);
        RenderMesh2D(meshes["enemyStroke"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_X[i], enemy_Y[i]);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);

    }
    //rander enemy
    
    // rander projectile
    if (pullProjectile == true) {
        cooldown++;
        modelMatrix = glm::mat3(1);
        projectileX -= sin(-projectileAngle-M_PI/2)*deltaTimeSeconds*200;
        projectileY += cos(-projectileAngle-M_PI/2)*deltaTimeSeconds*200;
        modelMatrix *= transform2D::Translate(projectileX, projectileY);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    //check colision wall-projectile
        if ((projectileX < 0 || projectileX > mapSize) || (projectileY < 0 || projectileY > mapSize)) {
            pullProjectile = false;
            cooldown = 0;
        }
    //check colision enemy-projectile
        for (i = 0; i < enemyCount; i++) {
            if (collisionSqareWall(projectileX, projectileY, enemy_X[i], enemy_Y[i], projectileSize, enemySize) == true) {
                enemy_X[i] = std::rand() % (int)(mapSize - enemySize + 1);
                enemy_Y[i] = std::rand() % (int)(mapSize - enemySize + 1);
                pullProjectile = false;
                score++;
                cooldown = 0;

            }
        }

    } 
    // rander projectile

    // rander Shuriken
 
    //check amuntion for rasenShuriken ( big shuriken)
    if (pullShuriken == true && rasenShuriken == 3) {

        shurikenCooldown++;
        modelMatrix = glm::mat3(1);
        shurikenX -= sin(-shurikenAngle - M_PI / 2) * deltaTimeSeconds * 200;
        shurikenY += cos(-shurikenAngle - M_PI / 2) * deltaTimeSeconds * 200;
        modelMatrix *= transform2D::Translate(shurikenX, shurikenY);
        shurikenRotation += deltaTimeSeconds;
        modelMatrix *= transform2D::Rotate(shurikenRotation * 10);
        RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
     
     //check colision wall-shuriken
        if ((shurikenX < 0 || shurikenX > mapSize) || (shurikenY < 0 || shurikenY > mapSize)) {
            pullShuriken = false;
            rasenShuriken = 0;
            shurikenCooldown = 0;

        }
     //check colision wall-enemy
        for (i = 0; i < 5; i++) {
            if (collisionSqareWall(shurikenX, shurikenY, enemy_X[i], enemy_Y[i], squareSide, enemySize) == true) {
                enemy_X[i] = std::rand() % (int)(mapSize - enemySize + 1);
                enemy_Y[i] = std::rand() % (int)(mapSize - enemySize + 1);
                score++;
            }
        }

    }
    //check amuntion for rasenShuriken ( big shuriken)
    else if (pullShuriken == true && rasenShuriken < 3) {
        shurikenCooldown++;
        modelMatrix = glm::mat3(1);
        shurikenX -= sin(-shurikenAngle - M_PI / 2) * deltaTimeSeconds * 200;
        shurikenY += cos(-shurikenAngle - M_PI / 2) * deltaTimeSeconds * 200;
        modelMatrix *= transform2D::Translate(shurikenX, shurikenY);
        angularStep += deltaTimeSeconds;
        modelMatrix *= transform2D::Rotate(angularStep * 10);
        RenderMesh2D(meshes["lilShuriken"], shaders["VertexColor"], modelMatrix);
    //check colision wall-shuriken
        if ((shurikenX < 0 || shurikenX > mapSize) || (shurikenY < 0 || shurikenY > mapSize)) {
            pullShuriken = false;
            shurikenCooldown = 0;

        }
    //check colision wall-enemy
        for (i = 0; i < 5; i++) {
            if (collisionSqareWall(shurikenX, shurikenY, enemy_X[i], enemy_Y[i], squareSide, enemySize) == true) {
                enemy_X[i] = std::rand() % (int)(mapSize - enemySize + 1);
                enemy_Y[i] = std::rand() % (int)(mapSize - enemySize + 1);
                score++;

            }
        }

    }
    // rander Shuriken
  
    //rander map
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
    // rander map

    // rander walls
    for (i = 0; i < 4; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(horizontalWallX[i], horizontalWallY[i]);
        RenderMesh2D(meshes["horizontalWall"], shaders["VertexColor"], modelMatrix);
    }


    for (i = 0; i < 3; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(verticalWallX[i], verticalWallY[i]);
        RenderMesh2D(meshes["verticalWall"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(squareWallX, squareWallY);
    RenderMesh2D(meshes["wallSquare"], shaders["VertexColor"], modelMatrix);
    //rander wall

    // rander gate
    gateCooldown++;
    if (gateCooldown > 100 && gateCooldown <200) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gateX, gateY);
        RenderMesh2D(meshes["verticalWall"], shaders["VertexColor"], modelMatrix);

    }
    //reset gate cooldwon
    if (gateCooldown > 200) {
        gateCooldown = 0;
    }
    // rander gate

    // healthBar decrementation
    for (i = 0; i < 5; i++) {
        if (collisionSqareWall(translateX, translateY, enemy_X[i], enemy_Y[i], squareSide, enemySize) == true) {
            enemy_X[i] = std::rand() % (int)(mapSize - enemySize + 1);
            enemy_Y[i] = std::rand() % (int)(mapSize - enemySize + 1);
            maxHitPoints--;
            if (maxHitPoints <= 0) {
                cout << "Score:" << " " << score << endl;
                cout << "Game Over!" << endl;
                exit(0);
            }

        }
    }
    // healthBar decrementation


}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

/*
*     tinyShurikenX = healthBarStrokeX + lilCircleSize;
    tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
*/
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    auto camera = GetSceneCamera();
    glm::ivec2 resolution = window->GetResolution();
    

    if (window->KeyHold(GLFW_KEY_W)) {
        translateY += (deltaTime * 100);
        translateLcY += (deltaTime * 100);
        translateRcY += (deltaTime * 100);
        healthBarY += (deltaTime * 100);
        tinyShurikenY += (deltaTime * 100);
        
        //colsion with map
        if (translateY > (mapSize - squareSide)) {
            translateY = mapSize - squareSide - lilCircleSize;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
        }

        //colsion with walls
        if (collisionSqareWall(translateX, translateY, squareWallX, squareWallY, squareSide, wallLengthSquare) == true) {
            translateY = translateY - squareSide;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;

        }
        for (i = 0; i < 4; i++) {
            if (collisionHorizontalWall(translateX, translateY, horizontalWallX[i], horizontalWallY[i], squareSide, wallLength) == true) {
                translateY = translateY - squareSide;
                translateLcY= translateY + squareSide;
                translateRcY = translateY + squareSide;
                healthBarY = translateY + 290;
                tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
            }
        }
        for (i = 0; i < 3; i++) {
            if (collisionVerticalWall(translateX, translateY, verticalWallX[i], verticalWallY[i], squareSide, wallLength) == true) {
                translateY = translateY - squareSide;
                translateLcY = translateY + squareSide;
                translateRcY = translateY + squareSide;
                healthBarY = translateY + 290;
                tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
            }
        }
        if (collisionVerticalWall(translateX, translateY, gateX, gateY, squareSide, wallLength) == true && gateCooldown > 100) {
            translateY = translateY - squareSide;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
        }
        //colsion with walls

        //moving camera
        camera->SetPosition(glm::vec3(translateX - resolution.x / 2, translateY - resolution.y / 2, 50));
        camera->Update();


    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateY -= (deltaTime * 100);
        translateLcY -= (deltaTime * 100);
        translateRcY -= (deltaTime * 100);
        healthBarY -= (deltaTime * 100);
        tinyShurikenY -= (deltaTime * 100);

        //colision with map
        if (translateY < squareSide) {
            translateY = squareSide;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
        }

        //colsion with walls
        if (collisionSqareWall(translateX, translateY, squareWallX, squareWallY, squareSide, wallLengthSquare) == true) {
            translateY = translateY + squareSide;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
        }

        for (i = 0; i < 4; i++) {
            if (collisionHorizontalWall(translateX, translateY, horizontalWallX[i], horizontalWallY[i], squareSide, wallLength) == true) {
                translateY = translateY + squareSide;
                translateLcY = translateY + squareSide;
                translateRcY = translateY + squareSide;
                healthBarY = translateY + 290;
                tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
            }
        }
        for (i = 0; i < 3; i++) {
            if (collisionVerticalWall(translateX, translateY, verticalWallX[i], verticalWallY[i], squareSide, wallLength) == true) {
                translateY = translateY + squareSide;
                translateLcY = translateY + squareSide;
                translateRcY = translateY + squareSide;
                healthBarY = translateY + 290;
                tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
            }
        }
        if (collisionVerticalWall(translateX, translateY, gateX, gateY, squareSide, wallLength) == true && gateCooldown > 100) {
            translateY = translateY + squareSide;
            translateLcY = translateY + squareSide;
            translateRcY = translateY + squareSide;
            healthBarY = translateY + 290;
            tinyShurikenY = healthBarY - 1.5 * lilCircleSize;
        }
        //colsion with walls

        //moving camera
        camera->SetPosition(glm::vec3(translateX - resolution.x / 2, translateY - resolution.y / 2, 50));
        camera->Update();
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= ( deltaTime * 100);
        translateLcX -= (deltaTime * 100);
        translateRcX -= (deltaTime * 100);
        healthBarStrokeX -= (deltaTime * 100);
        tinyShurikenX -= (deltaTime * 100);
        for (i = 0; i < maxHitPoints; i++) {
            hitPointsX[i] -= (deltaTime * 100);
        }

        //colsions with map
        if (translateLcX < squareSide) {
            translateX = squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            colisionFactor = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) {
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
            healthBarStrokeX = translateX + 360;
            tinyShurikenX = translateX + 360 + lilCircleSize;
        }
        if (collisionSqareWall(translateX, translateY, squareWallX, squareWallY, squareSide, wallLengthSquare) == true) {
            translateX = translateX + squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            healthBarStrokeX = translateX + 360;
            tinyShurikenX = translateX + 360 + lilCircleSize;
            colisionFactor = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) { 
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (collisionHorizontalWall(translateX, translateY, horizontalWallX[i], horizontalWallY[i], squareSide, wallLength) == true) {
                translateX = translateX + squareSide;
                translateLcX = translateX - squareSide / 2;
                translateRcX = translateX + squareSide / 2;
                healthBarStrokeX = translateX + 360;
                tinyShurikenX = translateX + 360 + lilCircleSize;
                colisionFactor = translateX + 360;
                for (i = 0; i < maxHitPoints; i++) {
                    hitPointsX[i] = colisionFactor;
                    colisionFactor += healthBarSize;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            if (collisionVerticalWall(translateX, translateY, verticalWallX[i], verticalWallY[i], squareSide, wallLength) == true) {
                translateX = translateX + squareSide;
                translateLcX = translateX - squareSide / 2;
                translateRcX = translateX + squareSide / 2;
                healthBarStrokeX = translateX + 360;
                tinyShurikenX = translateX + 360 + lilCircleSize;
                colisionFactor = translateX + 360;
                for (i = 0; i < maxHitPoints; i++) {
                    hitPointsX[i] = colisionFactor;
                    colisionFactor += healthBarSize;
                }
            }
        }
        if (collisionVerticalWall(translateX, translateY, gateX, gateY, squareSide, wallLength) == true && gateCooldown > 100) {
            translateX = translateX + squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            healthBarStrokeX = translateX + 360;
            tinyShurikenX = translateX + 360 + lilCircleSize;
            colisionFactor = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) {
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
;
        }
        //colsions with map

        //moving camera
        camera->SetPosition(glm::vec3(translateX - resolution.x / 2, translateY - resolution.y / 2, 50));
        camera->Update();
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += (deltaTime * 100);
        translateLcX += (deltaTime * 100);
        translateRcX += (deltaTime * 100);
        healthBarStrokeX += (deltaTime * 100);
        tinyShurikenX += (deltaTime * 100);
        for (i = 0; i < maxHitPoints; i++) {
            hitPointsX[i] += (deltaTime * 100);
        }

        //colision with map
        if (translateX > (mapSize - squareSide)) {
            translateX = mapSize - squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            healthBarStrokeX = translateX + 360;
            colisionFactor = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) {
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
            tinyShurikenX = healthBarStrokeX + lilCircleSize;
        }
        //colision with wall
        if (collisionSqareWall(translateX, translateY, squareWallX, squareWallY, squareSide, wallLengthSquare) == true) {
            translateX = translateX - squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            colisionFactor = translateX + 360;
            healthBarStrokeX = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) {
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
            tinyShurikenX = healthBarStrokeX + lilCircleSize;
        }
        for (int i = 0; i < 4; i++) {
            if (collisionHorizontalWall(translateX, translateY, horizontalWallX[i], horizontalWallY[i], squareSide, wallLength) == true) {
                translateX = translateX - squareSide;
                translateLcX = translateX - squareSide / 2;
                translateRcX = translateX + squareSide / 2;
                healthBarStrokeX = translateX + 360;
                colisionFactor = translateX + 360;
                for (i = 0; i < maxHitPoints; i++) {
                    hitPointsX[i] = colisionFactor;
                    colisionFactor += healthBarSize;
                }
                tinyShurikenX = healthBarStrokeX + lilCircleSize;
            }

        }
        for (i = 0; i < 3; i++) {
            if (collisionVerticalWall(translateX, translateY, verticalWallX[i], verticalWallY[i], squareSide, wallLength) == true) {
                translateX = translateX - squareSide;
                translateLcX = translateX - squareSide / 2;
                translateRcX = translateX + squareSide / 2;
                healthBarStrokeX = translateX + 360;
                colisionFactor = translateX + 360;
                for (i = 0; i < maxHitPoints; i++) {
                    hitPointsX[i] = colisionFactor;
                    colisionFactor += healthBarSize;
                }
                tinyShurikenX = healthBarStrokeX + lilCircleSize;
            }

        }
        if ( (collisionVerticalWall(translateX, translateY, gateX, gateY, squareSide, wallLength) == true) && (gateCooldown > 100) ) {
            translateX = translateX - squareSide;
            translateLcX = translateX - squareSide / 2;
            translateRcX = translateX + squareSide / 2;
            healthBarStrokeX = translateX + 360;
            colisionFactor = translateX + 360;
            for (i = 0; i < maxHitPoints; i++) {
                hitPointsX[i] = colisionFactor;
                colisionFactor += healthBarSize;
            }
            tinyShurikenX = healthBarStrokeX + lilCircleSize;
        }
        //colision with wall

        //moving camera
        camera->SetPosition(glm::vec3(translateX - resolution.x / 2, translateY - resolution.y / 2, 50));
        camera->Update();

    }




}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    auto camera = GetSceneCamera();
    glm::ivec2 resolution = window->GetResolution();
    //get mouse angle
    angle = atan2(mouseY - (resolution.y/2), mouseX - (resolution.x/2));
   
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
        if (cooldown <= 0) {
            projectileX = translateX;
            projectileY = translateY;
            projectileAngle = angle;
            pullProjectile = true;
            cooldown++;
        }

    }


    
    if (window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
        if (shurikenCooldown <= 0 && rasenShuriken != 0) {
            shurikenX = translateX;
            shurikenY = translateY;
            shurikenAngle = angle;
            pullShuriken = true;
            if (rasenShuriken < 3) {
                rasenShuriken--;
            }
            shurikenCooldown++;
        }
    }
    



}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event



}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
