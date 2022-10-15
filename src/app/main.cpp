#include <stdafx_qt>

#include "view/main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    g::view::MainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}