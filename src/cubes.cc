/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <png.h>
#include <AntTweakBar.h>

#include "rutils.h"
#include "gl/mesh.h"
#include "gl/gltexture.h"
#include "gl/cubemap.h"
#include "gl/textureutils.h"
#include "camera.h"
#include "scene/node.h"
#include "scene/meshes.h"
#include "scene/skybox.h"
#include "cubes.h"
#include "glapp.h"
#include "assets.h"

// Font test
#include <stdio.h>
#include <wchar.h>

#include "freetype-gl.h"


using namespace rutils;
using namespace std;
using namespace glm;
using namespace rgl;

static void add_text(VertexBuffer* vertexBuffer, ftgl::texture_font_t * font,
                     const wchar_t * text, glm::vec4 * color, glm::vec2 * pen )
{
    size_t i;
    float r = color->r, g = color->g, b = color->b, a = color->a;
    for( i=0; i<wcslen(text); ++i )
    {
        ftgl::texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
        if( glyph != NULL )
        {
            float kerning =  0.0f;
            if( i > 0)
            {
                kerning = texture_glyph_get_kerning( glyph, text[i-1] );
            }
            pen->x += kerning;
            float x0  = (float)( pen->x + glyph->offset_x );
            float y0  = (float)( pen->y + glyph->offset_y );
            float x1  = (float)( x0 + glyph->width );
            float y1  = (float)( y0 - glyph->height );
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;
            GLuint indices[6] = {0,1,2, 0,2,3};
            float vertices[4 * 9] = { x0,y0,0,  s0,t0,  r,g,b,a ,
                                      x0,y1,0,  s0,t1,  r,g,b,a ,
                                      x1,y1,0,  s1,t1,  r,g,b,a ,
                                      x1,y0,0,  s1,t0,  r,g,b,a };
            vertexBuffer->push(vertices, 4 * 9, indices, 6 );
            pen->x += glyph->advance_x;
        }
    }
}



void Cubes::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);

    camera.init(45.0f, width/ (float) height, 0.1f, 2000.0f);
    camera.moveTo(0, 0, 150);

    // TwInit(TW_OPENGL, NULL);
    // TwWindowSize(width, height);
    // TwBar* bar = TwNewBar("Empty TweakBar");
    // float myVar;
    // TwAddVarRW(bar, "NameOfMyVariable", TW_TYPE_FLOAT, &myVar, "");

    // temp stuff for testing


    cubeMesh = new Mesh();

    cubeMesh->addVertex(vec3(-0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, 0.5f, -0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, -0.5f));

    cubeMesh->addTexCoord(TexCoord(0, 1));
    cubeMesh->addTexCoord(TexCoord(0, 0));
    cubeMesh->addTexCoord(TexCoord(1, 0));
    cubeMesh->addTexCoord(TexCoord(1, 1));

    cubeMesh->addColor(Color(1, 0, 0, 1));
    cubeMesh->addColor(Color(0, 1, 0, 1));
    cubeMesh->addColor(Color(0, 0, 1, 1));

    // cubeMesh->addVertex(vec3(-0.5f, 0.5f, 0.5f));
    // cubeMesh->addVertex(vec3(-0.5f, -0.5f, 0.5f));
    // cubeMesh->addVertex(vec3(0.5f, -0.5f, 0.5f));
    // cubeMesh->addVertex(vec3(0.5f, 0.5f, 0.5f));
    // cubeMesh->addTexCoord(TexCoord(0, 1));
    // cubeMesh->addTexCoord(TexCoord(0, 0));
    // cubeMesh->addTexCoord(TexCoord(1, 0));
    // cubeMesh->addTexCoord(TexCoord(1, 1));

    // cubeMesh->addVertex(vec3(-0.5f, 0.5f, -0.5f));
    // cubeMesh->addVertex(vec3(-0.5f, -0.5f, -0.5f));
    // cubeMesh->addVertex(vec3(0.5f, -0.5f, -0.5f));
    // cubeMesh->addVertex(vec3(0.5f, 0.5f, -0.5f));
    // cubeMesh->addTexCoord(TexCoord(0, 1));
    // cubeMesh->addTexCoord(TexCoord(0, 0));
    // cubeMesh->addTexCoord(TexCoord(1, 0));
    // cubeMesh->addTexCoord(TexCoord(1, 1));

    // cubeMesh->addColor(vec4(-0.5f, 0.5f, 0.5f, 1.f));
    // cubeMesh->addColor(vec4(-0.5f, -0.5f, 0.5f, 1.f));
    // cubeMesh->addColor(vec4(0.5f, -0.5f, 0.5f, 1.f));
    // cubeMesh->addColor(vec4(0.5f, 0.5f, 0.5f, 1.f));
    // cubeMesh->addColor(vec4(-0.5f, 0.5f, -0.5f, 1.f));
    // cubeMesh->addColor(vec4(-0.5f, -0.5f, -0.5f, 1.f));
    // cubeMesh->addColor(vec4(0.5f, -0.5f, -0.5f, 1.f));
    // cubeMesh->addColor(vec4(0.5f, 0.5f, -0.5f, 1.f));

    cubeMesh->addFace(Face(0, 1, 2, 3,
                           0, 1, 2, 3,
                           0, 0, 0, 0));
    cubeMesh->addFace(Face(7, 6, 5, 4,
                           0, 1, 2, 3,
                           0, 0, 0, 0));
    cubeMesh->addFace(Face(4, 5, 1, 0,
                           0, 1, 2, 3,
                           1, 1, 1, 1));
    cubeMesh->addFace(Face(3, 2, 6, 7,
                           0, 1, 2, 3,
                           1, 1, 1, 1));
    cubeMesh->addFace(Face(4, 0, 3, 7,
                           0, 1, 2, 3,
                           2, 2, 2, 2));
    cubeMesh->addFace(Face(1, 5, 6, 2,
                           0, 1, 2, 3,
                           2, 2, 2, 2));

    cubeMesh->computeFaceNormals();
    cubeMesh->computeVertexNormals();
    cubeMesh->useColors();
    cubeMesh->useTextures();

    cubeMesh->bufferFaceData(GL_STATIC_DRAW);

    // load textures
    string texname = assets::TEXTURE_DIR + "stGrid1.png";

    uint wi, he, bpp;
    byte* data;
    loadImage(texname.c_str(), wi, he, bpp, &data);
    GLint intFormat = bpp == 24 ? GL_RGB : GL_RGBA;
    GLenum format = bpp == 24 ? GL_BGR : GL_BGRA;

    GLTexture* tex = new GLTexture(intFormat, format, wi, he, data);    GLTexture* texmip = new GLTexture(intFormat, format, wi, he, data, true);

    string cubemapDir = assets::CUBEMAP_DIR + "RomeChurch/";

    CubeMap* cmap = createCubeMap(
        cubemapDir + "negx.png", cubemapDir + "posx.png",
        cubemapDir + "posy.png", cubemapDir + "negy.png",
        cubemapDir + "negz.png", cubemapDir + "posz.png");

    CubeMap* cm = new CubeMap(intFormat, format, wi,
                              data, data, data, data, data, data, true);

    Mesh* cmesh = new Mesh();
    rgl::createCubeMesh(cmesh);
    cmesh->bufferData();

    Node* pivot = sceneGraph.allocateNode();
    pivot->w = vec3(0, 0.5f, 0);

    string dir = assets::SHADER_DIR;
    string vertSrc = readFile((dir + "normcol.vert").c_str());
    string fragSrc = readFile((dir + "normcol.frag").c_str());

    cprogram.create(vertSrc, fragSrc, VAS_POSNORMCOL);
    pipeline.watchProgram(cprogram);

    tprogram.create(readFile((dir + "normtex.vert").c_str()),
                    readFile((dir + "normtex.frag").c_str()),
                    VAS_POSNORMTEX);
    pipeline.watchProgram(tprogram);

    skybox = new Skybox(cmap);
    pipeline.watchProgram(skybox->getProgram());

    ShaderProgram* cpr = new ShaderProgram() ;
    cpr->create(readFile((dir + "cubemap.vert").c_str()),
                readFile((dir + "cubemap.frag").c_str()),
                VAS_POSNORM);
    pipeline.watchProgram(*cpr);

    Model* model = new Model(sceneGraph.allocateNode(), cubeMesh, tex);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(50, 0, 0);
    model->node->w = vec3(-R_PI/2, 0, 0);
    model->attachProgram(&tprogram);
    scene.addModel(model);

    model = new Model(sceneGraph.allocateNode(), cubeMesh, tex);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(-50, 0, 0);
    model->node->w = vec3(0, R_PI/2, 0);
    model->attachProgram(&tprogram);
    scene.addModel(model);

    Node* boxNode = sceneGraph.allocateNode();
    boxNode->w = vec3(R_PI/8, 0, 0);

    int w = 100, h = 20;
    for (int i = -w / 2; i <= w / 2; i++)
        for (int j = -h / 2; j <= h / 2; j++) {
            model = new Model(sceneGraph.allocateNode(), cmesh, cm);
            model->scale = vec3(1, 1, 1);
            model->moveTo(i * 9, j * 9, 0);
            sceneGraph.attachNode(model->node, boxNode);
            model->node->w = vec3(0, R_PI/2, 0);
            model->attachProgram(cpr);
            scene.addModel(model);
        }
    program = &cprogram;

    Mesh* testMesh = new Mesh();
    bool success = loadObj((assets::MODEL_DIR + "bunny.obj").c_str(), *testMesh);
    if (success) {
        testMesh->bufferData();
        model = new Model(sceneGraph.allocateNode(), testMesh);
        model->scale = vec3(20, 20, 20);
        model->attachProgram(&cprogram);
        scene.addModel(model);
    }


    textVB = new VertexBuffer(VAS_POSTEXCOL);

    textprog = new ShaderProgram() ;
    textprog->create(readFile((dir + "texcol.vert").c_str()),
                readFile((dir + "texcol.frag").c_str()),
                VAS_POSTEXCOL);
    pipeline.watchProgram(*textprog);

    size_t i;
    ftgl::texture_font_t *font = 0;
    ftgl::texture_atlas_t *atlas = ftgl::texture_atlas_new( 512, 512, 1 );
    const char * filename = "assets/fonts/Vera.ttf";
    const wchar_t *text = L"A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
    vec2 pen = vec2(5, 400);
    Color black = Color(1, 1, 1, 1);

    for( i=7; i < 27; ++i)
    {
        font = texture_font_new_from_file( atlas, i, filename );
        pen.x = 5;
        pen.y -= font->height;
        texture_font_load_glyphs( font, text );
        add_text(textVB, font, text, &black, &pen );
        texture_font_delete( font );
    }

    textVB->bind();
    textVB->bufferData(GL_STATIC_DRAW);
    glBindTexture( GL_TEXTURE_2D, atlas->id );
}

void Cubes::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skybox->render();
    glDepthMask(GL_TRUE);
    scene.render();

    textprog->use();
    cubeMesh->render();
    textVB->render();
}


void Cubes::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLApp::keyCallback(window, key, scancode, action, mods);
    // if (key == GLFW_KEY_O && action == GLFW_PRESS) {
    //     cubeMesh->bufferData(!cubeMesh->flatShading, GL_STATIC_DRAW);
    // }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        scene.models[3]->texture->bind();
        if (mipmap) {
            scene.models[3]->texture->setFilter(GL_LINEAR, GL_LINEAR);

        } else {
            scene.models[3]->texture->setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        }
        mipmap = !mipmap;
        if (program == &tprogram)
            program = &cprogram;
        else
            program = &tprogram;
    }

}
