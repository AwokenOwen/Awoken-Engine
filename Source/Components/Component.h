//
// Created by AwokenOwen on 4/23/26.
//

#pragma once
#include <nlohmann/json.hpp>

class Object;
class Component {
    friend class Object;
public:
    /**
     * @brief Constructor necessary for all components
     *
     * @param parent The object that the component is attached to
     */
    explicit Component(Object* parent);
    virtual ~Component() = default;

    /**
     * @brief Getter for the parent object this component is attached to
     *
     * @return Pointer to the Parent Object
     */
    [[nodiscard]] Object* getParent() const;

    /**
     * @brief Getter for the active state of the Component
     *
     * @return The active state of the component
     */
    [[nodiscard]] bool getActiveState() const;
    /**
     * @brief Sets the active state and prepares the components enable/disable functions to be called
     *
     * @param active
     */
    virtual void setActiveState(bool active);

private:
    /**
     * @brief Called on the first frame this component exists
     */
    virtual void start() = 0;
    /**
     * @brief Called every frame it's active
     */
    virtual void update() = 0;
    /**
     * @brief Called on the first frame it's enabled
     */
    virtual void enable() = 0;
    /**
     * @brief Called on the last frame after being set to disabled
     */
    virtual void disable() = 0;
    /**
     * @brief Called when the parent objected is destroyed
     */
    virtual void destroy() {
        delete this;
    }

    /**
     * @brief Converts all necessary properties into JSON for storing
     *
     * @return The JSON of the component
     */
    virtual nlohmann::json toJson() = 0;

    Object* p_parent{};

    bool m_activeState{};
};
