#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
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
void consoleEnterHandler(Ui::MainWindow*);

class ConsoleTextEdit : public QTextEdit 
{
    Q_OBJECT

public:
    using QTextEdit::QTextEdit;
protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        {
            consoleEnterHandler(&ui);

            qDebug() << "enter pressed in console";
            QTextEdit::keyPressEvent(event);
            return;
        }
        QTextEdit::keyPressEvent(event);
    }
};

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

        QColor lineColor = QColor(55,55,55);

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

void consoleEnterHandler(Ui::MainWindow* ui) // TODO
{

}

void run(Ui::MainWindow* ui) // TODO
{
    std::string str = ui->textEdit->toPlainText().toUtf8().constData();

    ExecutionDriver driver;
    auto res = driver.execute(str);

    if (std::holds_alternative<SyntaxError>(res))
    {
        // hightlignt error
        std::cout << "Syntax error:" << std::get<SyntaxError>(res).message << std::endl;
    }
    else if (std::holds_alternative<std::string>(res))
    {
        std::cout << "Runtime error: " << std::get<std::string>(res) << std::endl;
    }
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
    QWidget* oldConsole = ui.consoleContainer;
    QWidget* parent = oldConsole->parentWidget();
    oldConsole->hide();
    oldConsole->deleteLater();
    ConsoleTextEdit* customConsole = new ConsoleTextEdit(parent);
    customConsole->setObjectName("consoleContainer");
    QLayout* layout = parent->layout();
    if (layout)
    {
        layout->addWidget(customConsole);
    }
    ui.consoleContainer = customConsole;
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

    //QObject::connect(ui.textEdit, &QPlainTextEdit::cursorPositionChanged, [&]() { // CRASHES
    //    highlightCurrentLine(&ui);
    //    });

    ui.actionSave->setShortcut(QKeySequence("Ctrl+S"));
    ui.actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    ui.actionSave_as->setShortcut(QKeySequence("Ctrl+Shift+S"));
    
    window.show();
    return app.exec();
}

#include "main.moc"
