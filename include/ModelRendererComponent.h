//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "ResourceManager.h"

struct Mesh;
class ModelRendererComponent : public Component{
    friend class ResourceManager;
public:
    /**
     * @brief Constructor to set the parent of the component
     *
     * @param parent The parent object of the component
     */
    explicit ModelRendererComponent(Object* parent) : Component(parent){}

private:
    /**
     * @brief Function that runs the first frame it is initiated
     */
    void start() override;
    /**
     * @brief Function that runs once every frame as long as it's active
     */
    void update() override;
    /**
     * @brief Function that runs once when the component is set active
     */
    void enable() override{}
    /**
     * @brief Function that runs once when the component is set inactive
     */
    void disable() override{}
    /**
     * @brief Function called when the component is being destroyed
     */
    void destroy() override;

    /**
     * @brief Function that convert the component into a JSON object
     *
     * @return JSON object that holds all the data of the Component
     */
    nlohmann::json toJson() override;
    /**
     * @brief Takes a JSON object and fills in all the data needed to initialize the component
     *
     * @param j JSON object that holds all the data for the Component
     */
    void fromJson(nlohmann::json j) override;

    /**
     * @brief Helper function to load default uniforms into the onto the material
     *
     * @param mat The material getting loaded with uniforms
     */
    void defaultDynamicUniformLoader(Material mat) const;

    /**
     * @brief The path to the model being used
     */
    std::string m_modelPath{"assets/defaultAssets/Models/cube.fbx"};
    /**
     * @brief A list of material paths the model needs
     */
    std::vector<std::string> m_materialPaths{"assets/defaultAssets/Materials/default.json"};

    /**
     * @brief The loaded model
     */
    Model m_model{};
    /**
     * @brief List of loaded materials
     */
    std::vector<Material> m_materials{};
};
