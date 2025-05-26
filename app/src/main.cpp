#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
#include <qapplication>
#include <qpushbutton>
#include <QObject>
#include <QProcess>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QLineEdit>
#include <QFont>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QItemDelegate>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextEdit>
#include <QMetaType>
#include "ui_main_window.h"
#include "table.h"
#include "execution_driver.h"


using namespace std;

bool firstTime = true;
Ui::MainWindow ui;
QString fullErr;

std::optional<SyntaxError> inputCheck(std::string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        char ch = input[i];
        if (ch < -1 || ch > 255) return SyntaxError{ (unsigned long long)i, "Unsupported symbol" };
    }
    return std::nullopt;
}

std::vector<size_t> getLineStartIndices(const std::string& text)
{
    std::vector<size_t> indices;
    indices.push_back(0);
    for (size_t i = 0; i < text.size(); ++i)
    {
        if (text[i] == '\n' && i + 1 < text.size())
        {
            indices.push_back(i + 1);
        }
    }
    return indices;
}

std::pair<size_t, size_t> getTextPosition(const std::vector<size_t>& lineStarts, size_t charIndex)
{
    auto it = std::upper_bound(lineStarts.begin(), lineStarts.end(), charIndex);
    if (it == lineStarts.begin())
    {
        return { 0, charIndex };
    }

    size_t lineNumber = static_cast<size_t>(it - lineStarts.begin() - 1);
    size_t columnNumber = charIndex - lineStarts[lineNumber];
    return { lineNumber, columnNumber };
}

class MyHighlighter : public QSyntaxHighlighter
{
public:
    MyHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent)
    {
        vector<QString> keywords;

        map<QString, QColor> color;

        // numbers highlighting
        QTextCharFormat numbersFormat;
        numbersFormat.setForeground(QColor(184, 215, 161));

        HighlightingRule numbersRule;
        numbersRule.pattern = QRegularExpression(R"(\b\d+(\.\d+)?\b)"); // numbers
        numbersRule.format = numbersFormat;
        highlightingRules.append(numbersRule);

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

        keywords.push_back("and"); color["and"] = QColor(37, 99, 187);
        keywords.push_back("or"); color["or"] = QColor(37, 99, 187);
        keywords.push_back("not"); color["not"] = QColor(37, 99, 187);

        keywords.push_back("mod"); color["mod"] = QColor(254, 122, 116);
        keywords.push_back("div"); color["div"] = QColor(254, 122, 116);

        // keywords highlighting
        for (auto keyword : keywords)
        {
            QTextCharFormat format;
            format.setForeground(color[keyword]);

            HighlightingRule rule;
            rule.pattern = QRegularExpression("\\b" + keyword + "\\b");
            rule.format = format;
            highlightingRules.append(rule);
        }

        // quotes highlighting
        QTextCharFormat singleQuoteFormat;
        singleQuoteFormat.setForeground(QColor(214, 157, 127));

        HighlightingRule quotesRule;
        quotesRule.pattern = QRegularExpression(R"('([^'\\]|\\.)*')"); // одинарные кавычки и всё между ними
        quotesRule.format = singleQuoteFormat;
        highlightingRules.append(quotesRule);
    }

protected:
    void highlightBlock(const QString& text) override
    {
        for (const HighlightingRule& rule : qAsConst(highlightingRules))
        {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext())
            {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
};

void highlightCurrentLine(Ui::MainWindow* ui)
{
    QPlainTextEdit* editor = ui->textEdit;
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!editor->isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(55, 55, 55);

        QTextCharFormat format;
        format.setBackground(lineColor);

        selection.format = format;
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        if (!editor->textCursor().isNull())
        {
            selection.cursor = editor->textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
    }

    ui->textEdit->setExtraSelections(extraSelections);
}

void run(Ui::MainWindow* ui)
{
    ui->consoleContainer->clear();

    std::string code = ui->textEdit->toPlainText().toUtf8().constData();

    std::vector<size_t> lineStarts = getLineStartIndices(code);

    auto checkRes = inputCheck(code);
    if (checkRes.has_value())
    {
        auto pos = getTextPosition(lineStarts, checkRes.value().pos);
        ui->consoleContainer->insertPlainText(
            QString::fromStdString("Invalid symbol at pos ("
                + to_string(pos.first + 1) + ":" + to_string(pos.second + 1) + ")")
        );
        return;
    }


    QProcess* process = new QProcess();
    fullErr.clear();

    QProcess::connect(process, &QProcess::readyReadStandardError, [=]()
        {
            fullErr.push_back(process->readAllStandardError());
        });

    QProcess::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int code, QProcess::ExitStatus status)
        {
            stringstream fullSS(fullErr.toUtf8().constData());

            std::string err;
            while (std::getline(fullSS, err))
            {
                stringstream ss(err);

                std::string errCode;
                ss >> errCode;

                if (errCode == "SYNTAX_ERROR")
                {
                    size_t pos; ss >> pos;
                    auto pos2d = getTextPosition(lineStarts, pos);

                    std::string rest;
                    std::getline(ss, rest);
                    std::string msg = "Syntax error at (" + to_string(pos2d.first + 1) + ":"
                        + to_string(pos2d.second + 1) + "): " + rest;
                    ui->consoleContainer->insertPlainText(QString::fromStdString(msg));
                } else
                {
                    ui->consoleContainer->insertPlainText(QString::fromStdString(err));
                }
            }

            qDebug() << "Process finished with code:" << code;
        });

    QProcess::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        process, &QProcess::deleteLater);

    process->setProcessChannelMode(QProcess::ProcessChannelMode::SeparateChannels);
    process->start("compiler_app_pamm.exe");
    process->write(QByteArray::fromStdString(code));
    process->closeWriteChannel();
}

void changeRowScrollBar(Ui::MainWindow* ui)
{
    ui->rowNumbersContainer->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
}

void changeMainTextScrollBar(Ui::MainWindow* ui)
{
    ui->textEdit->verticalScrollBar()->setValue(ui->rowNumbersContainer->verticalScrollBar()->value());
}

void refreshRowNumbers(Ui::MainWindow* ui)
{
    int rows = ui->textEdit->toPlainText().count("\n") + 1;
    QString result;
    for (int i = 1; i <= rows; i++)
    {
        result += to_string(i);
        if (i != rows)
            result += '\n';
    }
    ui->rowNumbersContainer->setPlainText(result);
}

bool saveExisting(Ui::MainWindow* ui, const QString& curFilename)
{
    QFile file(curFilename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Something went wrong during file saving";
        return false;
    }

    QString text = ui->textEdit->toPlainText();

    QTextStream out(&file);

    out << text;

    return true;
}

bool saveNew(Ui::MainWindow* ui, QString& curFilename)
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Choose file to save", "Unnamed.txt", "Text files (*.txt)");

    if (filename.isEmpty())
    {
        qDebug() << "No file was chosen";
        return false;
    }

    QFile file(filename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
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

bool onOpenButtonClicked(Ui::MainWindow* ui, QString& curFilename)
{
    QString fileContent;

    QString filename = QFileDialog::getOpenFileName(nullptr, "Choose file to open", QString(), "Text files (*.txt)");

    if (filename.isEmpty())
    {
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

bool onSaveButtonClicked(Ui::MainWindow* ui, QString& curFilename)
{

    if (firstTime)
        return saveNew(ui, curFilename);
    else
        return saveExisting(ui, curFilename);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    ui.setupUi(&window);

    // change console container to custom one
    ui.consoleContainer->setReadOnly(true);
    // end of changing console container to custom one

    QString currentFilename;
    QPlainTextEdit* editor = ui.textEdit;
    new MyHighlighter(editor->document());

    QObject::connect(ui.actionOpen, &QAction::triggered, [&]()
        {
            if (onOpenButtonClicked(&ui, currentFilename))
            {
                qDebug() << currentFilename << "has been opened";

                firstTime = false;
            }

            refreshRowNumbers(&ui);
        });

    QObject::connect(ui.actionSave, &QAction::triggered, [&]()
        {
            if (onSaveButtonClicked(&ui, currentFilename))
            {
                qDebug() << currentFilename << "has been saved";

                firstTime = false;
            }

            refreshRowNumbers(&ui);
        });

    QObject::connect(ui.actionSave_as, &QAction::triggered, [&]()
        {
            if (saveNew(&ui, currentFilename))
            {
                qDebug() << currentFilename << "has been saved";

                firstTime = false;
            }

            refreshRowNumbers(&ui);
        });

    QObject::connect(ui.runButton, &QPushButton::clicked, [&]()
        {
            run(&ui);
        });

    QObject::connect(ui.fileNameContainer, &QLineEdit::textChanged, [&]()
        {
            ui.fileNameContainer->setToolTip(ui.fileNameContainer->text());
        });

    QObject::connect(ui.textEdit, &QPlainTextEdit::textChanged, [&]()
        {
            refreshRowNumbers(&ui);
        });

    QObject::connect(ui.textEdit->verticalScrollBar(), &QScrollBar::valueChanged, [&]()
        {
            changeRowScrollBar(&ui);
        });

    QObject::connect(ui.rowNumbersContainer->verticalScrollBar(), &QScrollBar::valueChanged, [&]()
        {
            changeMainTextScrollBar(&ui);
        });

    ui.actionSave->setShortcut(QKeySequence("Ctrl+S"));
    ui.actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    ui.actionSave_as->setShortcut(QKeySequence("Ctrl+Shift+S"));

    window.show();
    return app.exec();
}

#include "main.moc"
