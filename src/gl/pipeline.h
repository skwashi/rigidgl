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
    void update(const glm::mat4& viewMatrix);
    void update(const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
    void watchProgram(ShaderProgram& program);
    void watchProgram(ShaderProgram* program);
    void clearPrograms();
    void updateMatrices(ShaderProgram& program) const;
    void updateMatrices(ShaderProgram* program) const;
    void updatePrograms() const;

private:
    std::vector<ShaderProgram*> watchedPrograms;
};

}


#endif
