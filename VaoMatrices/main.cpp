#pragma once

#include "GameModels.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<vector>

#include <iostream>
#include "display.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//usefull vec3 print command
#define printVec3(a,b,c) std::cout<<a<<","<<b<<","<<c<<std::endl;

Models::GameModels* gameModels;
GLuint program;
int ScreenWidth=800;int ScreenHeight=600;

//void roll( float angle, glm::vec3 upvector )
//{
//// roll the camera through angle degrees
//float cs = cos( M_PI/180 * angle );
//float sn = sin( M_PI/180 * angle );
//glm::vec3 t = upvector; // remember old u

//upvector = glm::vec3(cs*t.x -sn*v.x, cs*t.y -sn.v.y, cs*t.z -sn.v.z );
////v = ( sn*t.x+ cs*v.x,sn*t.y+ cs.v.y,sn*t.z+ cs.v.z );
////setModelViewMatrix( );
//}

glm::mat4 matrixFromAxisAngle(glm::vec3 axis, float angle) {

   double c = cos(angle);
   double s = sin(angle);
   double t = 1.0 - c;
   //  if axis is not already normalised then uncomment this
   // double magnitude = Math.sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
   // if (magnitude==0) throw error;
   // axis.x /= magnitude;
   // axis.y /= magnitude;
   // axis.z /= magnitude;


   glm::mat4 tmp;

   tmp[0][0] = c + axis.x*axis.x*t;
   tmp[1][1] = c + axis.y*axis.y*t;
   tmp[2][2] = c + axis.z*axis.z*t;


   double tmp1 = axis.x*axis.y*t;
   double tmp2 = axis.z*s;
   tmp[1][0] = tmp1 + tmp2;
   tmp[0][1] = tmp1 - tmp2;
   tmp1 = axis.x*axis.z*t;
   tmp2 = axis.y*s;
   tmp[2][0] = tmp1 - tmp2;
   tmp[0][2] = tmp1 + tmp2;    tmp1 = axis.y*axis.z*t;
   tmp2 = axis.x*s;
   tmp[2][1] = tmp1 + tmp2;
   tmp[1][2] = tmp1 - tmp2;

   return tmp;
}


Display display(ScreenWidth,ScreenHeight,"");

//static float angle;

enum rotationAxisFlag
{
    y,
    x,
    z
}
rotationAxisFlag=y;

glm::mat4 Model = glm::mat4(1);
glm::mat4 Model2 = glm::mat4(1);

glm::mat4 translationMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0,0));//glm::mat4(1);
glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1), glm::vec3(0,0,0));//glm::mat4(1);

glm::mat4 rotationMatrix = glm::mat4(1);
glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(0.5,0.5,0.5));

static bool translate=true;

glm::vec4 newUp(0,1,0,0);

static float z_rot;

void renderScene(void)
 {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.3, 0.3, 1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_DEPTH_CLAMP);
  glEnable( GL_MULTISAMPLE );

  glBindVertexArray(gameModels->GetModel("plane1"));

  glUseProgram(program);

  //Load Matrices to Shader START
  // Model matrix : an identity matrix (model will be at the origin)

//  printVec3(1,2,3);

  float angle = 0;

  if(display.m_flagLocalX)
  {
      angle = display.m_angleX;

      rotationMatrix = glm::rotate(/*glm::mat4(1) */ /*Model*/ rotationMatrix, angle, glm::vec3(1,0,0));//the rotation matrix is multiplied with itself producing a local coordinate rotation
      translationMatrix = glm::translate(glm::mat4(1)/*translationMatrix*/,glm::vec3(0,0,0));//this represents our global translation when reinitialized with glm::mat4(1) / or our local translation if multiplied with translation matrix itself from the previous iteration

      translationMatrix2 = glm::translate(glm::mat4(1), glm::vec3(0,0,-1));

  }

  if(display.m_flagLocalY)
  {
     angle = display.m_angleY;

     rotationMatrix = glm::rotate(/*glm::mat4(1) */ /*Model*/ rotationMatrix, angle, glm::vec3(0,1,0));//the rotation matrix is multiplied with itself producing a local coordinate rotation
     translationMatrix = glm::translate(glm::mat4(1)/*translationMatrix*/,glm::vec3(0,0,0));//this represents our global translation when reinitialized with glm::mat4(1) / or our local translation if multiplied with translation matrix itself from the previous iteration

     translationMatrix2 = glm::translate(glm::mat4(1), glm::vec3(0,0,-1));

  }

  if(display.m_flagLocalZ)
  {
    angle = display.m_angleZ;

    rotationMatrix = glm::rotate(/*glm::mat4(1) */ /*Model*/ rotationMatrix, angle, glm::vec3(0,0,1));//the rotation matrix is multiplied with itself producing a local coordinate rotation
    translationMatrix = glm::translate(glm::mat4(1)/*translationMatrix*/,glm::vec3(0,0,0));//this represents our global translation when reinitialized with glm::mat4(1) / or our local translation if multiplied with translation matrix itself from the previous iteration

    translationMatrix2 = glm::translate(glm::mat4(1), glm::vec3(0,0,-1));

    z_rot+=angle * (M_PI/180.0f);

  }

//  translationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.5,0,0));//glm::mat4(1);


//    Model =  translationMatrix*rotationMatrix;
//    Model =  rotationMatrix *translationMatrix*scaleMatrix;


// Right to left evaluation (because of OpenGL Column-Major matrices) ... so at first ROTATE .. and then TRANSLATE ()

    Model =  translationMatrix*rotationMatrix*scaleMatrix;//Often used order. Rotate and place wherever we want locally
//   Model =  rotationMatrix * translationMatrix * scaleMatrix;//Could be used to Rotate around the origin with the radius of translationMatrix


    Model2 = rotationMatrix*translationMatrix2*scaleMatrix;//Flipped rotationMatrix with translationMatrix to achieve rotation around a single point


//   Pre- or post-multiplication just defines the order of operations how the member of that matrix and vector are multiplied, its purely a notational convention.

//   The common operations in 3D graphics are post-multiplying a row-vector with column-major matrices (OpenGL) and pre-multiplying a column-vector with row-major matrices (DirectX).

//   Both are basically the same operations with the same results, just the notational convention is different.



  //    prevent from continues rotation
  display.m_flagLocalX=false;
  display.m_flagLocalY=false;
  display.m_flagLocalZ=false;


//  glm::mat4 Projection = gameModels->GetProjectionMatrix();//always the same probably
//  glm::mat4 View = gameModels->GetViewMatrix();//always the same probably

  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), ScreenWidth / (float)ScreenHeight, 0.5f, 500.f);


  // Camera matrix

  glm::vec3 newcameraposition= glm::vec3(Model2[3][0], Model2[3][1], Model2[3][2]);
  glm::vec3 target= glm::vec3(0,0,0);
  glm::vec3 cameraDirection = target-newcameraposition;
  glm::normalize(cameraDirection);
  glm::vec4 upVec = newUp;
  glm::vec3 rightVec = glm::cross(glm::vec3(upVec),cameraDirection);
  glm::normalize(rightVec);
  newUp= glm::vec4(glm::cross(cameraDirection,rightVec),0);

  glm::mat4 tmpmat=matrixFromAxisAngle(cameraDirection, z_rot);//to be used for roll only! so, if there's a rotation specified for z it will roll

  newUp =  tmpmat*upVec;

  printVec3(newcameraposition.x,newcameraposition.y,newcameraposition.z)

  glm::mat4 View = glm::lookAt(
                              glm::vec3(0,20,-450), // Camera is at (4,3,3), in World Space
                              glm::vec3(0,0,0), // and looks at the origin
                              glm::vec3(0,1,0 /*upVec*/)  // Head is up (set to 0,-1,0 to look upside-down)
                              );

// Our ModelViewProjection : multiplication of our 3 matrices
//  Model = glm::mat4(1);
//  Model =glm::translate(Model,glm::vec3(0,0,-10));
  glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication works the other way around (<----<----<)

  glm::mat4 MVP2       = Projection * View * Model2; // Remember, matrix multiplication works the other way around (<----<----<)

  //reset to origin
  //  glm::mat4 MVP(1);

  // Get a handle for our "MVP" uniform.
  // Only at initialisation time.
  GLuint MatrixID = glGetUniformLocation(program, "MVP");

  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  //Load Matrices to Shader END

//  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
////  glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
////  glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//  glm::mat4 View = glm::lookAt(
//                    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//                    glm::vec3(0,0,0), // and looks at the origin
//                    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//                );
//  glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//  glm::mat4 MVP = Projection * View * Model;
//  GLuint LocationMVP = glGetUniformLocation(program, "MVP");
//  glUniformMatrix4fv(LocationMVP, 1, GL_FALSE, glm::value_ptr(MVP));



//  for(int i =1;i<=6;i++)
//  {
//    /* Make our background black */
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 36);//plane made of 36 vertices (6 planes * 2 triangles * 3 vertices each = 36 vertices passed onto)
//    display.update();
//    SDL_Delay(200);
//  }



    // Now grab and draw the second Plane (Note this is switching to the new generated VAO)

    //Load Matrices to Shader END
    glBindVertexArray(gameModels->GetModel("plane2"));
    glUseProgram(program);
    glGetUniformLocation(program, "MVP");

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);//plane made of 6 vertices
}

void Init()
{
    //Trivial SDL initializations
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    //load and compile shaders
    Shader shader;
    shader.InitShaders("./shaders/shader.vs", "./shaders/shader.fs");
    program = shader.getProgramShaderId();

    //Enable Vaos & Load
    gameModels = new Models::GameModels(program);
    gameModels->CreatePlaneModel("plane1");
    gameModels->CreatePlaneModel2("plane2");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 Projection = glm::perspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.5f, 150.f);
    //  Pass in the projection matrix to our model so we can pass it to the vertex shader
    gameModels->SetProjectionMatrix(Projection);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
                                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                                glm::vec3(0,0,0), // and looks at the origin
                                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                );
    gameModels->SetViewMatrix(View);

}

int main(int argc, char **argv)
{
 glewInit();

 Init();

    while(!display.isClosed())
    {
        //  Clear background
        display.clear(0,0.05,0.05,1);

        renderScene();

        display.update();
    }


 delete gameModels;
 glDeleteProgram(program);

 return 0;

}
