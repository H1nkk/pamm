#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QLineEdit>
#include <QFont>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QItemDelegate>
#include <QPlainTextEdit>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
#include "ui_main_window.h"
#include "ui_info_widget.h"
#include "table.h"

using namespace std;

bool firstTime = true;

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>

class MyHighlighter : public QSyntaxHighlighter {
public:
    MyHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {

        vector<QString> keywords;

        map<QString, QColor> color;

        keywords.push_back("if"); color["if"] = QColor(215, 177, 238);
        keywords.push_back("then"); color["then"] = QColor(215, 177, 238);
        keywords.push_back("else"); color["else"] = QColor(215, 177, 238);
        keywords.push_back("double"); color["double"] = QColor(84, 135, 166);
        keywords.push_back("integer"); color["integer"] = QColor(84, 135, 166);
        keywords.push_back("program"); color["program"] = QColor(202, 39, 57);
        keywords.push_back("begin"); color["begin"] = QColor(171, 91, 219);
        keywords.push_back("end"); color["end"] = QColor(171, 91, 219);
        keywords.push_back("var"); color["var"] = QColor(171, 91, 219);
        keywords.push_back("const"); color["const"] = QColor(171, 91, 219);
        keywords.push_back("Write"); color["Write"] = QColor(221, 221, 170);
        keywords.push_back("WriteLn"); color["WriteLn"] = QColor(221, 221, 170);
        keywords.push_back("Read"); color["Read"] = QColor(221, 221, 170);

        for (auto keyword : keywords) // keyword highlighting
        {
            QTextCharFormat format;
            format.setForeground(color[keyword]);
            //format.setFontWeight(QFont::Bold);

            HighlightingRule rule;
            rule.pattern = QRegularExpression("\\b" + keyword + "\\b");
            rule.format = format;
            highlightingRules.append(rule);
        }

        // quotes highlighting
        QTextCharFormat singleQuoteFormat;
        singleQuoteFormat.setForeground(QColor(214, 157, 127));

        HighlightingRule rule;
        rule.pattern = QRegularExpression(R"('([^'\\]|\\.)*')"); // одинарные кавычки и всё между ними
        rule.format = singleQuoteFormat;
        highlightingRules.append(rule);

    }

protected:
    void highlightBlock(const QString& text) override {
        for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
};


void refreshRowNumbers() {

}

bool saveExisting(Ui::MainWindow* ui, const QString& curFilename) {
    QFile file(curFilename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Something went wrong during file saving";
        return false;
    }

    QString text = ui->textEdit->toPlainText();

    QTextStream out(&file);

    out << text;

    return true;
}

bool saveNew(Ui::MainWindow* ui, QString& curFilename) {
    QString filename = QFileDialog::getSaveFileName(nullptr, "Choose file to save", "Unnamed.txt", "Text files (*.txt)");

    if (filename.isEmpty()) {
        qDebug() << "No file was chosen";
        return false;
    }

    QFile file(filename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Something went wrong during file saving";
        return false;
    }

    QString text = ui->textEdit->toPlainText();

    QTextStream out(&file);

    out << text;

    curFilename = filename;
    ui->fileNameContainer->setText(curFilename);

    return true;
}

bool onOpenButtonClicked(Ui::MainWindow* ui, QString& curFilename) {
    QString fileContent;

    QString filename = QFileDialog::getOpenFileName(nullptr, "Choose file to open", QString(), "Text files (*.txt)");

    if (filename.isEmpty()) {
        qDebug() << "No file was chosen";
        return false;
    }

    QFile file(filename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream in(&file);

    fileContent = in.readAll();

    file.close();

    ui->textEdit->clear();
    ui->textEdit->setPlainText(fileContent);

    curFilename = filename;
    ui->fileNameContainer->setText(curFilename);
    return true;
}

bool onSaveButtonClicked(Ui::MainWindow* ui, QString& curFilename) {

    if (firstTime)
        return saveNew(ui, curFilename);
    else
        return saveExisting(ui, curFilename);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    Ui::MainWindow ui;
    ui.setupUi(&window);

    QString currentFilename;

    QPlainTextEdit* editor = ui.textEdit;  // если вы заменили QLineEdit на QTextEdit
    new MyHighlighter(editor->document());

    QObject::connect(ui.actionOpen, &QAction::triggered, [&]() {
        if (onOpenButtonClicked(&ui, currentFilename)) {
            qDebug() << currentFilename << "has been opened";

            firstTime = false;
        }

        refreshRowNumbers();
        });

    QObject::connect(ui.actionSave, &QAction::triggered, [&]() {
        if (onSaveButtonClicked(&ui, currentFilename)) {
            qDebug() << currentFilename << "has been saved";

            firstTime = false;
        }

        refreshRowNumbers();
        });

    QObject::connect(ui.actionSave_as, &QAction::triggered, [&]() {
        if (saveNew(&ui, currentFilename)) {
            qDebug() << currentFilename << "has been saved";

            firstTime = false;
        }

        refreshRowNumbers();
        });

    QLineEdit::connect(ui.fileNameContainer, &QLineEdit::textChanged, [&]() {
        ui.fileNameContainer->setToolTip(ui.fileNameContainer->text());
        });

    ui.actionSave->setShortcut(QKeySequence("Ctrl+S"));
    ui.actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    ui.actionSave_as->setShortcut(QKeySequence("Ctrl+Shift+S"));


    window.show();
    return app.exec();
}
