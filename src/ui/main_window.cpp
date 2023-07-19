#include <QDockWidget>
#include <QFileDialog>
#include <QListView>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>

#include "ui/main_window.hpp"

#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
#include "project/assets/texture_asset.hpp"
#include "project/object.hpp"
#include "project/project.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include "project/transform_component.hpp"
#include "qspdlog/qspdlog.hpp"
#include "viewport/camera_view.hpp"
// #include "ui/asset_manager.hpp"
#include "common/logger.hpp"
#include "project/asset.hpp"
#include "project/asset_manager.hpp"
#include "project/camera_component.hpp"
#include "ui/scene_view.hpp"
#include "viewport/viewport.hpp"
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

namespace g::ui
{

namespace
{
common::logger_ptr logger = common::get_logger("ui");
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _project { std::make_shared<project::project>("Dummy project") }
    , _scene { project::scene::create("Dummy scene") }
    // , _assetManager { new ui::AssetManager }
    , _viewport { nullptr }
    , _shaderEditor { new QTabWidget }
    , _vertexShaderEditor { new QPlainTextEdit }
    , _fragmentShaderEditor { new QPlainTextEdit }
{
    project::resource_manager::init();
    _viewport = new viewport::Viewport;
    initializeDockWidgets();

    _project->add_scene(_scene);
    setCentralWidget(_viewport);

    auto rootObject = project::object::create("Dummy object");
    _scene->add_object(rootObject);

    auto cameraObject = project::object::create("Camera");
    _scene->add_object(cameraObject);

    cameraObject->add_component<project::camera_component>();
    cameraObject->transform()->set_position({ 10.0f, 10.0f, 10.0f });

    auto rendererComponent =
        rootObject->add_component<project::renderer_component>();

    auto assets = project::asset_manager().load_asset("sample.fbx");
    rendererComponent->set_mesh(
        std::static_pointer_cast<project::assets::mesh>(assets[ "mesh" ]));
    auto material = std::static_pointer_cast<project::assets::material>(
        assets[ "material" ]);
    rendererComponent->set_material(material);

    setMenuBar(new QMenuBar);
    QMenu* sceneMenu = menuBar()->addMenu("Scene");
    sceneMenu->addActions(_sceneWidget->actions());

    qobject_cast<viewport::Viewport*>(_viewport)->onInitialized(
        [ this ]()
        { qobject_cast<viewport::Viewport*>(_viewport)->loadScene(_scene); });

    connect(qobject_cast<SceneView*>(_sceneWidget),
            &SceneView::objectActivated,
            this,
            [ this ](std::shared_ptr<project::object> object)
            {
        logger->info("Object activated: {}", object->name());
        auto cameraComponent =
            object->get_component<project::camera_component>();
        if (cameraComponent)
        {
            viewport::CameraView* view = new viewport::CameraView;
            qobject_cast<viewport::Viewport*>(view)->onInitialized(
                [ this, view ]() {
                qobject_cast<viewport::Viewport*>(view)->loadScene(_scene);
            });

            view->setCamera(cameraComponent);
            view->show();
        }
    });
}

MainWindow::~MainWindow() { project::resource_manager::deinit(); }

void MainWindow::initializeDockWidgets()
{
    initializeProjectExplorerDockWidget();
    initializeLoggerDockWidget();
}

void MainWindow::initializeProjectExplorerDockWidget()
{
    auto dock = new QDockWidget("Scene explorer", this);
    _sceneWidget = new SceneView { _scene };
    dock->setWidget(_sceneWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::initializeLoggerDockWidget()
{
    auto loggerDockWidget = new QDockWidget("Logger", this);

    QSpdLog* qspdlog = new QSpdLog(this);
    qspdlog->setAutoScrollPolicy(
        AutoScrollPolicy::AutoScrollPolicyEnabledIfBottom);
    qspdlog->sink()->set_level(spdlog::level::info);
    // register all loggers into the qspldog widget
    auto& registry = spdlog::details::registry::instance();
    registry.apply_all([ qspdlog ](std::shared_ptr<spdlog::logger> logger)
                       { logger->sinks().push_back(qspdlog->sink()); });

    loggerDockWidget->setWidget(qspdlog);
    addDockWidget(Qt::BottomDockWidgetArea, loggerDockWidget);
}

} // namespace g::ui
