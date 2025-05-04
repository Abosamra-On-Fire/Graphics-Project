#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        pipelineState.setup(); // Setup the pipeline state
        shader->use();         // Activate the shader
    }

    // This function reads the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        Material::setup();         // Call base class setup
        shader->set("tint", tint); // Set the "tint" uniform
    }

    // This function reads the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        TintedMaterial::setup();                       // Call base class setup
        shader->set("alphaThreshold", alphaThreshold); // Set "alphaThreshold"
        glActiveTexture(GL_TEXTURE0);                  // Activate texture unit 0
        if (texture)
            texture->bind(); // Bind texture
        if (sampler)
            sampler->bind(0);  // Bind sampler to unit 0
        shader->set("tex", 0); // Set "tex" uniform to use texture unit 0
    }

    // This function reads the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void litMaterial::setup() const
    {
        TintedMaterial::setup(); // Call base class setup

        glActiveTexture(GL_TEXTURE1); // Activate texture unit 0
        if (textureAlbedo)
        {
            textureAlbedo->bind();
            shader->set("albedoMap", 1);
        }
        glActiveTexture(GL_TEXTURE2); // Activate texture unit 0
        if (textureMetallic)
        {
            textureMetallic->bind();
            shader->set("metallicMap", 2);
        }
        glActiveTexture(GL_TEXTURE3); // Activate texture unit 0
        if (textureAmbientOcclusion)
        {
            textureAmbientOcclusion->bind();
            shader->set("aoMap", 3);
        }
        glActiveTexture(GL_TEXTURE4); // Activate texture unit 0
        if (textureRoughness)
        {
            textureRoughness->bind();
            shader->set("roughnessMap", 4);
        }
        glActiveTexture(GL_TEXTURE5); // Activate texture unit 0
        if (textureNormal)
        {
            textureNormal->bind();
            shader->set("normalMap", 5);
        }
    }

    // This function reads the material data from a json object
    void litMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        textureAlbedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        textureRoughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        textureMetallic = AssetLoader<Texture2D>::get(data.value("metallic", ""));
        textureAmbientOcclusion = AssetLoader<Texture2D>::get(data.value("ambientOcclusion", ""));
    }
}
