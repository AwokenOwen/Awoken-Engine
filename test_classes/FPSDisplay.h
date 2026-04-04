//
// Created by awokenowen on 3/31/26.
//

#pragma once
#include <string>

#include "../Object.h"


class FPSDisplay : public Object{
public:
    FPSDisplay();

    void update() override;
private:
    string fps;
};