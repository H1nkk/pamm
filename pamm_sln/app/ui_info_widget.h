/********************************************************************************
** Form generated from reading UI file 'info_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFO_WIDGET_H
#define UI_INFO_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_infoWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *infoWidget)
    {
        if (infoWidget->objectName().isEmpty())
            infoWidget->setObjectName("infoWidget");
        infoWidget->setWindowModality(Qt::WindowModality::NonModal);
        infoWidget->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoWidget->sizePolicy().hasHeightForWidth());
        infoWidget->setSizePolicy(sizePolicy);
        infoWidget->setMinimumSize(QSize(800, 600));
        infoWidget->setMaximumSize(QSize(800, 600));
        verticalLayoutWidget = new QWidget(infoWidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 801, 601));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(verticalLayoutWidget);
        textEdit->setObjectName("textEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy1);
        textEdit->setMinimumSize(QSize(160, 120));
        textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	font-size: 20px;\n"
"}"));
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);

        verticalLayout->setStretch(0, 1);

        retranslateUi(infoWidget);

        QMetaObject::connectSlotsByName(infoWidget);
    } // setupUi

    void retranslateUi(QWidget *infoWidget)
    {
        infoWidget->setWindowTitle(QCoreApplication::translate("infoWidget", "Form", nullptr));
        textEdit->setHtml(QCoreApplication::translate("infoWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:20px; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">Supported operations:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">arithmetic operations</span> - <span style=\" font-weight:700;\">+</span>, <span style=\" font-weight:700;\">-</span>, <span style=\" font-weight:700;\">*</span>"
                        "</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">calc(pol, w, x, y, z)</span> - evaluate polynomial &quot;pol&quot; at point (w, x, y, z)</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">derx(pol)</span> - get derivative of polynomial &quot;pol&quot; by x</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">dery(pol)</span> - get derivative of polynomial &quot;pol&quot; by y</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" fon"
                        "t-weight:700;\">derz(pol) </span>- get derivative of polynomial &quot;pol&quot; by z</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">derw(pol) </span>- get derivative of polynomial &quot;pol&quot; by w</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">intx(pol)</span> - get integral of polynomial &quot;pol&quot; by x</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">inty(pol)</span> - get integral of polynomial &quot;pol&quot; by y</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0p"
                        "x; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">intz(pol) </span>- get integral of polynomial &quot;pol&quot; by z</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">intw(pol)</span> - get integral of polynomial &quot;pol&quot; by w</p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">Input examples:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">pol1 = 6x1y3z3w6</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">pol2 = 78 + 9.1x^7y^8w^2</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px"
                        "; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">pol3 = pol1 + pol2</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">pol4 = calc(pol1, 2, 4, 6, 8) + derx(pol2)</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class infoWidget: public Ui_infoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFO_WIDGET_H
