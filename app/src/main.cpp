#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QLineEdit>
#include <QFont>
#include <QTimer>
#include <QItemDelegate>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
#include "ui_main_window.h"
#include "ui_info_widget.h"
#include "table.h"

using namespace std;

class TableEditor;

int globalRowWidth = 30;
int globalCharacterWidth = 7;
TableEditor* pInputTable;
int pos = 0;

class TableEditor : public QTableWidget 
{
    friend class CustomDelegate;

public:
    TableEditor(QTableWidget* parent) : QTableWidget(parent) {
        //globalCharacterWidth = QFontMetrics(font()).averageCharWidth();
    }
    int cursorColumn = 0;

protected:
    void keyPressEvent(QKeyEvent* event) override 
    {
        qDebug() << "smt is pressed";
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) 
        {
            int newRow = currentRow() + 1;

            insertRow(newRow);

            setCurrentCell(newRow, 0);

            QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth , newRow * globalRowWidth });

            edit(index);

            return;
        }
        else if (event->key() == Qt::Key_Backspace) { // TODO Добавить key_delete
            
            int curRow = currentRow();
            QTableWidgetItem* curItem = currentItem();

            qDebug() << "text: " << curItem->text();

            if (curItem->text() == "") {
                if (currentRow() == 0) return;

                int newRow = max(0, currentRow() - 1);

                removeRow(currentRow());

                setCurrentCell(newRow, 0);
                QTableWidgetItem* pItem = currentItem();
                cursorColumn = pItem->text().length();

                QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, newRow * globalRowWidth });

                edit(index);

                QWidget* editor = indexWidget(index);
                if (!editor) return;

                QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, newRow * globalRowWidth } - visualRect(index).topLeft();

                if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
                {
                    lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
                }
                else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
                {
                    QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                    textEdit->setTextCursor(cursor);
                }



                return;
            }
            else {
                QTableWidget::keyPressEvent(event);
            }

            return;
        }
        else if (event->key() == Qt::Key_Escape) { // TODO пока не работает, доделать. хотя мб это вообще не надо. надо подумать
            editItem(item(currentRow(), 0));

            return;
        }
        else if (event->key() == Qt::Key_Up)
        {
            int newRow = max(0, currentRow() - 1);

            if (currentRow() == newRow) { // when newrow == 0 and currow == 0
                cursorColumn = 0;
            }

            setCurrentCell(newRow, 0);

            QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, newRow * globalRowWidth });

            edit(index);

            QWidget* editor = indexWidget(index);
            if (!editor) return;

            QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, newRow * globalRowWidth } - visualRect(index).topLeft();

            if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
            {
                lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
            }
            else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
            {
                QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                textEdit->setTextCursor(cursor);
            }

            return;
        }
        else if (event->key() == Qt::Key_Down) 
        {
            int newRow = min(rowCount() - 1, currentRow() + 1);

            if (currentRow() == newRow) { // when newrow == rowCount() - 1 and currow == rowCount() - 1

                cursorColumn = currentItem()->text().length(); // TODO Изменить на конец строки, а не 1000
            }

            setCurrentCell(newRow, 0);

            QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, newRow * globalRowWidth });

            edit(index);

            QWidget* editor = indexWidget(index);
            if (!editor) return;

            QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, newRow * globalRowWidth } - visualRect(index).topLeft();

            if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
            {
                lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
            }
            else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
            {
                QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                textEdit->setTextCursor(cursor);
            }
            return;
        }
        else if (event->key() == Qt::Key_Left)
        {
            qDebug() << "left pressed";

            if (cursorColumn > 0) {
                cursorColumn = min(cursorColumn, (int)(currentItem()->text().length()));
                cursorColumn--;

                setCurrentCell(currentRow(), 0);

                QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, currentRow() * globalRowWidth });

                edit(index);

                QWidget* editor = indexWidget(index);
                if (!editor) return;

                QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, currentRow() * globalRowWidth } - visualRect(index).topLeft();

                if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
                {
                    lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
                }
                else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
                {
                    QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                    textEdit->setTextCursor(cursor);
                }
                return;
            }
            else
            {
                bool change = true;
                if (currentRow() == 0) change = false;

                int newRow = max(0, currentRow() - 1);

                setCurrentCell(newRow, 0);
                QTableWidgetItem* pItem = currentItem();
                if (change)
                    cursorColumn = pItem->text().length();

                QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, newRow * globalRowWidth });

                edit(index);

                QWidget* editor = indexWidget(index);
                if (!editor) return;

                QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, newRow * globalRowWidth } - visualRect(index).topLeft();

                if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
                {
                    lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
                }
                else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
                {
                    QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                    textEdit->setTextCursor(cursor);
                }

                return;
            }
        }
        else if (event->key() == Qt::Key_Right)
        {
            qDebug() << "right pressed";

            QTableWidgetItem* pItem = currentItem();
            int len = pItem->text().length();
            qDebug() << len;
            if (cursorColumn < len) {
                cursorColumn++;

                setCurrentCell(currentRow(), 0);

                QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, currentRow() * globalRowWidth });

                edit(index);

                QWidget* editor = indexWidget(index);
                if (!editor) return;

                QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, currentRow() * globalRowWidth } - visualRect(index).topLeft();

                if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
                {
                    lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
                }
                else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
                {
                    QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                    textEdit->setTextCursor(cursor);
                }
                return;
            }
            else {
                bool change = true;
                if (currentRow() == rowCount() - 1) change = false;

                if (change)
                    cursorColumn = 0;

                int newRow = min(currentRow() + 1, rowCount() - 1);
                setCurrentCell(newRow, 0);

                QModelIndex index = indexAt({ cursorColumn * globalCharacterWidth, newRow * globalRowWidth });

                edit(index);

                QWidget* editor = indexWidget(index);
                if (!editor) return;

                QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, newRow * globalRowWidth } - visualRect(index).topLeft();

                if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
                {
                    lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
                }
                else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
                {
                    QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
                    textEdit->setTextCursor(cursor);
                }
                return;

            }
        }
        else 
        {
            QTableWidget::keyPressEvent(event);
        }
    }

    void mousePressEvent(QMouseEvent* event) override 
    {
        QTableWidget::mousePressEvent(event);

        QModelIndex index = indexAt(event->pos());
        if (!index.isValid()) return;

        edit(index);

        QWidget* editor = indexWidget(index);
        if (!editor) return;

        QPoint posInEditor = event->pos() - visualRect(index).topLeft();

        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor)) 
        {
            lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
            cursorColumn = lineEdit->cursorPosition();
            qDebug() << "lineEdit index: " << cursorColumn;

        }
        else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
        {
            QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
            textEdit->setTextCursor(cursor);
            cursorColumn = textEdit->textCursor().position();
            qDebug() << "textEdit index: " << cursorColumn;

        }

        qDebug() << "index: " << event->pos().x() - visualRect(index).topLeft().x() << " " << event->pos().y();

    }

public:

    void update() {
        QTableWidgetItem* curItem = currentItem();
        if (!curItem) {
            qDebug() << "bug";
            return;
        }
        QString text = currentItem()->text();
        qDebug() << "LUFE";

        closePersistentEditor(currentItem());
        editItem(curItem);
        curItem->setText(text);
        /*QModelIndex index = currentIndex();
        if (!index.isValid()) return;

        QString text = currentItem()->text();
        int cursorPos = 0;
        if (QLineEdit* editor = qobject_cast<QLineEdit*>(indexWidget(index))) {
            cursorPos = editor->cursorPosition();
        }

        closePersistentEditor(currentItem());
        edit(index);
        
        QWidget* editor = indexWidget(index);

        QPoint posInEditor = QPoint{ cursorColumn * globalCharacterWidth, currentRow() * globalRowWidth } - visualRect(index).topLeft();

        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
        {
            lineEdit->setCursorPosition(lineEdit->cursorPositionAt(posInEditor));
        }
        else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
        {
            QTextCursor cursor = textEdit->cursorForPosition(posInEditor);
            textEdit->setTextCursor(cursor);
        }
        return;*/
    }
};

class CustomDelegate : public QItemDelegate {
public:
    using QItemDelegate::QItemDelegate;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QWidget* editor = QItemDelegate::createEditor(parent, option, index);

        editor->setFont(QFont("Cascadia Mono"));

        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor)) {
            lineEdit->installEventFilter(const_cast<CustomDelegate*>(this));
        }
        return editor;
    }

    bool eventFilter(QObject* editor, QEvent* event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right ) {
                //QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
                //if (!lineEdit) return false;

                //QString currentText = lineEdit->text();
                //int cursorPos = lineEdit->cursorPosition();

                //// Закрываем редактор
                //closeEditor(lineEdit, QAbstractItemDelegate::NoHint);

                //// Обновляем значение ячейки (иначе будет потеря текста)
                //QTableWidgetItem* item = pInputTable->currentItem();
                //if (item) item->setText(currentText);

                //// Открываем редактор заново
                //QModelIndex index = pInputTable->currentIndex();
                //pInputTable->edit(index);

                //QWidget* newEditor = pInputTable->indexWidget(index);
                //if (QLineEdit* newLineEdit = qobject_cast<QLineEdit*>(newEditor)) {
                //    newLineEdit->setCursorPosition(cursorPos > 0 ? cursorPos - 1 : 0);  // left или right можно скорректировать
                //}

                //return true;


                //pInputTable->update();


                commitData(qobject_cast<QWidget*>(editor));
                closeEditor(qobject_cast<QWidget*>(editor), QAbstractItemDelegate::NoHint);

                qDebug() << "Left or right key pressed in editor!";

                pInputTable->keyPressEvent(keyEvent);
                return true;
            }
            if (keyEvent->key() == Qt::Key_Backspace) {
                qDebug() << "backspace key pressed in editor!";
                pInputTable->keyPressEvent(keyEvent);
            }
        }
        return QItemDelegate::eventFilter(editor, event);
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    Ui::MainWindow ui;
    ui.setupUi(&window);

    pInputTable = new TableEditor(ui.tableWidget);
    ui.verticalLayout->replaceWidget(ui.tableWidget, pInputTable);
    delete ui.tableWidget;
    ui.tableWidget = pInputTable;
    ui.tableWidget->setItemDelegate(new CustomDelegate(ui.tableWidget));

    pInputTable->setColumnCount(1);
    pInputTable->setRowCount(1);
    pInputTable->setColumnWidth(0, 10000);
    pInputTable->setCursor(Qt::IBeamCursor);
    pInputTable->setFont(QFont("Cascadia Mono")); // TODO с этой строкой иногда лагает :) Хотя лаги не из-за нее, а изза "QAbstractItemView::commitData called with an editor that does not belong to this view"
    pInputTable->horizontalHeader()->hide();
    pInputTable->setShowGrid(false);
    pInputTable->setContentsMargins(0, 0, 0, 0);


    window.show();
    return app.exec();
}
