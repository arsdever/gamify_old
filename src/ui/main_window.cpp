#include <QFileDialog>
#include <QListView>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <stdafx_qt>

#include "ui/main_window.hpp"

#include "project/assets/mesh_asset.hpp"
#include "project/mesh_component.hpp"
#include "project/object.hpp"
#include "project/project.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include "qspdlog/qspdlog.hpp"
// #include "ui/asset_manager.hpp"
#include "project/asset.hpp"
#include "project/asset_manager.hpp"
#include "ui/scene_view.hpp"
#include "viewport/viewport.hpp"
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

namespace g::ui
{

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

    // create a cube
    auto meshComponent = rootObject->add_component<project::mesh_component>();
    std::vector<std::array<float, 3>> vertexCoordinates {
        { -0.1f, -0.1f, -0.1f }, // 0
        { 0.1f, -0.1f, -0.1f },  // 1
        { 0.1f, 0.1f, -0.1f },   // 2
        { -0.1f, 0.1f, -0.1f },  // 3
        { -0.1f, -0.1f, 0.1f },  // 4
        { 0.1f, -0.1f, 0.1f },   // 5
        { 0.1f, 0.1f, 0.1f },    // 6
        { -0.1f, 0.1f, 0.1f }    // 7
    };
    std::vector<unsigned> vertexIndices {
        0, 1, 2, 2, 3, 0, // front
        1, 5, 6, 6, 2, 1, // right
        5, 4, 7, 7, 6, 5, // back
        4, 0, 3, 3, 7, 4, // left
        3, 2, 6, 6, 7, 3, // top
        4, 5, 1, 1, 0, 4  // bottom
    };
    meshComponent->set_vertex_coordinates(vertexCoordinates);
    meshComponent->set_vertex_indices(vertexIndices);

    auto rendererComponent =
        rootObject->add_component<project::renderer_component>();

    auto asset = project::asset_manager().load_asset("sample.fbx");
    rendererComponent->set_mesh(
        std::static_pointer_cast<project::assets::mesh>(asset));

    setMenuBar(new QMenuBar);
    QMenu* sceneMenu = menuBar()->addMenu("Scene");
    sceneMenu->addActions(_sceneWidget->actions());

    qobject_cast<viewport::Viewport*>(_viewport)->onInitialized(
        [ this ]()
        { qobject_cast<viewport::Viewport*>(_viewport)->loadScene(_scene); });
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
