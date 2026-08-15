//
// Created by AwokenOwen on 4/23/26.
//

#include "Component.h"
#include "Object.h"

Component::Component(Object *parent) {
    // Set parent
    p_parent = parent;

    // Add this to the start, and update events
    p_parent->addStartEvent(this, &Component::start);
    p_parent->addUpdateEvent(this, &Component::update);
}

Object * Component::getParent() const {
    // Pure return of the parent object
    return p_parent;
}

bool Component::getActiveState() const {
    // Pure return of active state
    return m_activeState;
}

void Component::setActiveState(const bool active) {
    // If the two active states are the same, no need to change anything
    if (m_activeState == active)
        return;
    // Set the new active state
    m_activeState = active;
    // If the new active state is true
    if (m_activeState) {
        // add themselves to the enable and update event
        getParent()->addEnableEvent(this, &Component::enable);
        getParent()->addUpdateEvent(this, &Component::update);
    // if the new active state is false
    }else {
        // add themselves to the disable event and remove themselves from the update event
        getParent()->addDisableEvent(this, &Component::enable);
        getParent()->removeUpdateEvent(this, &Component::update);
    }
}
