#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

// This function should setup the pipeline state and set the shader to be used
void Material::setup() const {
    // TODO: (Req 6) Write this function
    this->pipelineState.setup();
    this->shader->use();
}

// This function read the material data from a json object
void Material::deserialize(const nlohmann::json &data) {
    if (!data.is_object()) {
        return;
    }

    if (data.contains("pipelineState")) {
        pipelineState.deserialize(data["pipelineState"]);
    }
    shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
    transparent = data.value("transparent", false);
}

// This function should call the setup of its parent and
// set the "tint" uniform to the value in the member variable tint
void TintedMaterial::setup() const {
    // TODO: (Req 6) Write this function
    Material::setup();
    this->shader->set("tint", this->tint);
}

// This function read the material data from a json object
void TintedMaterial::deserialize(const nlohmann::json &data) {
    Material::deserialize(data);
    if (!data.is_object())
        return;
    tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

// This function should call the setup of its parent and
// set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
// Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
void TexturedMaterial::setup() const {
    // TODO: (Req 6) Write this function
    TintedMaterial::setup();
    this->shader->set("alphaThreshold", this->alphaThreshold);
    this->texture->bind(GL_TEXTURE0); // this bind the texture to unit number 0
    this->sampler->bind(0);
    this->shader->set("tex", 0);
}

// This function read the material data from a json object
void TexturedMaterial::deserialize(const nlohmann::json &data) {
    TintedMaterial::deserialize(data);
    if (!data.is_object())
        return;
    alphaThreshold = data.value("alphaThreshold", 0.0f);
    texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
    sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
}

void LitMaterial::setup() const {
    Material::setup();
    if (this->albedo) {
        this->albedo->bind(GL_TEXTURE0);
    }
    this->sampler->bind(0);
    this->shader->set("material.albedo", 0);

    if (this->specular) {
        this->specular->bind(GL_TEXTURE1);
    }
    this->sampler->bind(1);
    this->shader->set("material.specular", 1);

    if (this->ambient_occlusion) {
        this->ambient_occlusion->bind(GL_TEXTURE2);
        this->shader->set("material.ambientOcclusionEnable", true);
    } else {
        this->shader->set("material.ambientOcclusionEnable", false);
    }
    this->sampler->bind(2);
    this->shader->set("material.ambient_occlusion", 2);

    if (this->roughness) {
        this->roughness->bind(GL_TEXTURE3);
    }
    this->sampler->bind(3);
    this->shader->set("material.roughness", 3);

    if (this->emissive) {
        this->emissive->bind(GL_TEXTURE4);
    }
    this->sampler->bind(4);
    this->shader->set("material.emissive", 4);
}

// This function read the material data from a json object
void LitMaterial::deserialize(const nlohmann::json &data) {
    if (!data.is_object())
        return;
    Material::deserialize(data);
    sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    //if (data.contains("albedo_texture")) {
        albedo = AssetLoader<Texture2D>::get(data.value("albedo_texture", ""));
   // }
    //albedo_sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    //if (data.contains("specular_texture")) {
        specular = AssetLoader<Texture2D>::get(data.value("specular_texture", ""));
   // }
    //specular_sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    //if (data.contains("ambientOcclusion_texture")) {
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambientOcclusion_texture", ""));
    //}
    //ambient_occlusion_sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    //if (data.contains("roughness_texture")) {
        roughness = AssetLoader<Texture2D>::get(data.value("roughness_texture", ""));
    //}
    //roughness_sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    if (data.contains("emissive_texture")) {
        emissive = AssetLoader<Texture2D>::get(data.value("emissive_texture", ""));
    }
    //emissive_sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
}
} // namespace our
