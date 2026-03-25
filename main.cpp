#include "GameManager.h"
#include "WorldManager.h"
#include "WindowManager.h"
#include "GuiObject.h"
#include "include/cef_app.h"
#include <filesystem>

#include "Material.h"
#include "MeshRenderer.h"

class AwokenApp : public CefApp {
    IMPLEMENT_REFCOUNTING(AwokenApp);
};

int main(const int argc, char* argv[]) {
    CefMainArgs args(argc, argv);
    CefRefPtr<AwokenApp> pApp = new AwokenApp();
    int exit_code = CefExecuteProcess(args, pApp, nullptr);
    if (exit_code >= 0) {
        return exit_code;  // This IS the subprocess, exit immediately
    }

    CefSettings settings;

    // FIX THIS OMG YOU NEED TO FIX THIS
    std::filesystem::path cefLibDir = "/home/awokenowen/GitHub/CEF_Adventures/CEF/lib/linux";

    CefString(&settings.resources_dir_path) = cefLibDir.string();
    CefString(&settings.locales_dir_path)   = (cefLibDir / "locales").string();
    CefString(&settings.cache_path) = (cefLibDir / "cache").string();

    CefInitialize(args, settings, pApp, nullptr);

    Game.initialize();
    World.getActiveScene()->loadDefaultSkybox();

    auto* a = new GuiObject(vec2(0,0.25f), vec2(0.25f, 0.65f));
    a->getComponent<MeshRenderer>()->material->anchorPoint = UIAnchorPoints::LEFT;

    auto* b = new GuiObject(vec2(0,0.25f), vec2(0.25f, 0.65f));
    b->getComponent<MeshRenderer>()->material->anchorPoint = UIAnchorPoints::RIGHT;

    auto* c = new GuiObject(vec2(0,0), vec2(1.0f, 0.3f));
    c->getComponent<MeshRenderer>()->material->anchorPoint = UIAnchorPoints::BOTTOM;

    auto* d = new GuiObject(vec2(0,0), vec2(1.0f, 0.05f));
    d->getComponent<MeshRenderer>()->material->anchorPoint = UIAnchorPoints::TOP;

    auto* e = new GuiObject(vec2(0.0f,0.25f), vec2(0.5f, 0.65f));
    e->getComponent<MeshRenderer>()->material->anchorPoint = UIAnchorPoints::CENTER;

    Game.run();
    Game.terminate();
}
