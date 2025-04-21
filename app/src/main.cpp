#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <QElapsedTimer>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
#include "ui_main_window.h"
#include "ui_info_widget.h"
#include "table.h"
#include "calculator.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    Ui::MainWindow ui;

    ui.setupUi(&window);

    window.show();
    return app.exec();
}
