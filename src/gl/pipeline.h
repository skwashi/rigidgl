/**
 * @author Jonas Ransjö
 */

#ifndef PIPELINE_H
#define PIPELINE_H

#include <vector>
#include <glm/glm.hpp>

#include "shaderprogram.h"

namespace rgl {

class Pipeline
{
public:
    glm::mat4 projMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projViewMatrix;

    void update();
    void update(glm::mat4 viewMatrix);
    void update(glm::mat4 projMatrix, glm::mat4 viewMatrix);
    void watchProgram(ShaderProgram& program);
    void clearPrograms();
    void updateMatrices(ShaderProgram& program) const;
    void updatePrograms();

private:
    std::vector<ShaderProgram*> watchedPrograms;
};

}


#endif
