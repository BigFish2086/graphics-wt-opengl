#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
// There are some options in the render pipeline that we cannot control via shaders
// such as blending, depth testing and so on
// Since each material could require different options (e.g. transparent materials usually use blending),
// we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's
// pipeline
struct PipelineState {
    // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
    struct {
        bool enabled = false;
        GLenum culledFace = GL_BACK;
        GLenum frontFace = GL_CCW;
    } faceCulling;

    // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
    struct {
        bool enabled = false;
        GLenum function = GL_LEQUAL;
    } depthTesting;

    // This set of pipeline options specifies whether blending will be used or not and how it will be configured
    struct {
        bool enabled = false;
        GLenum equation = GL_FUNC_ADD;
        GLenum sourceFactor = GL_SRC_ALPHA;
        GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
        glm::vec4 constantColor = {0, 0, 0, 0};
    } blending;

    // These options specify the color and depth mask which can be used to
    // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
    glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
    bool depthMask = true;                           // To know how to use it, check glDepthMask

    // This function should set the OpenGL options to the values specified by this structure
    // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call
    // glDisable(GL_CULL_FACE)
    void setup() const {
        // TODO: (Req 3) Write this function
        // faceCulling option
        if (faceCulling.enabled == true) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
        glCullFace(faceCulling.culledFace); // specifies which face to be culled
        // specifies the direction for the front face i.e. clockwise or counter-clockwise
        // to distinguish which face is considered to be the front face, so the culling is done properly
        glFrontFace(faceCulling.frontFace);

        // depthTesting option
        if (depthTesting.enabled == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        // glDepthFunc specifies the function used to compare each incoming pixel depth value with the depth value
        // present in the depth buffer. The comparison is performed only if depth testing is enabled.
        // options are:
        // - GL_NEVER = Never passes.
        // - GL_LESS = Passes if the incoming depth value is less than the stored depth value.
        // - GL_EQUAL = Passes if the incoming depth value is equal to the stored depth value.
        // - GL_LEQUAL = Passes if the incoming depth value is less than or equal to the stored depth value.
        // - GL_GREATER = Passes if the incoming depth value is greater than the stored depth value.
        // - GL_NOTEQUAL = Passes if the incoming depth value is not equal to the stored depth value.
        // - GL_GEQUAL = Passes if the incoming depth value is greater than or equal to the stored depth value.
        // - GL_ALWAYS = Always passes.
        // The initial value of func is GL_LESS. Initially, depth testing is disabled. If depth testing is disabled or
        // if no depth buffer exists, it is as if the depth test always passes.
        glDepthFunc(depthTesting.function);

        // blending option
        if (blending.enabled) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }
        // The blending formula has the following form: (source_factor * source) operation (destination_factor *
        // destination). The operation is specified by the blend equation function. The possible values are:
        // - GL_FUNC_ADD: the operation is "+".
        // - GL_FUNC_SUBTRACT: the operation is "-".
        // - GL_FUNC_REVERSE_SUBTRACT: the operation is "-" but the operands are reversed.
        // - GL_MIN: the operation picks the minimum value among the 2 operands.
        // - GL_MAX: the operation picks the maximum value among the 2 operands.
        glBlendEquation(blending.equation);

        // sfactor
        // - Specifies how the red, green, blue, and alpha source blending factors are computed.
        //   The initial value is GL_ONE.
        // dfactor
        // - Specifies how the red, green, blue, and alpha destination blending factors are computed.
        //   The initial value is GL_ZERO.
        glBlendFunc(blending.sourceFactor, blending.destinationFactor);

        // In case you're using any of the factors that use the constant color, you need to define it via the
        // glBlendColor function.
        glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b,
                     blending.constantColor.a);
    }

    // Given a json object, this function deserializes a PipelineState structure
    void deserialize(const nlohmann::json &data);
};

} // namespace our
