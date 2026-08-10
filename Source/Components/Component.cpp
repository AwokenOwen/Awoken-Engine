//
// Created by AwokenOwen on 4/23/26.
//

#include "Component.h"
#include "Object.h"

Component::Component(Object *parent) {
    p_parent = parent;

    p_parent->addStartEvent(this, &Component::start);
    p_parent->addUpdateEvent(this, &Component::update);
    p_parent->addDestroyEvent(this, &Component::destroy);
}

Object * Component::getParent() const {
    return p_parent;
}

bool Component::getActiveState() const {
    return m_activeState;
}

void Component::setActiveState(const bool active) {
    if (m_activeState == active)
        return;
    m_activeState = active;
    getParent()->setComponentActiveState(this, active);
}
