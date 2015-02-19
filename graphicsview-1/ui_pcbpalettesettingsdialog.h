/********************************************************************************
** Form generated from reading UI file 'pcbpalettesettingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PCBPALETTESETTINGSDIALOG_H
#define UI_PCBPALETTESETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_PcbPaletteSettingsDialog
{
public:
    QGridLayout *gridLayout;
    QTableWidget *systemTable;
    QTableWidget *silkscreenTable;
    QDialogButtonBox *buttonBox;
    QTableWidget *mechanicalTable;
    QTableWidget *maskTable;
    QTableWidget *planeTable;
    QTableWidget *otherTable;
    QTableWidget *signalTable;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *PcbPaletteSettingsDialog)
    {
        if (PcbPaletteSettingsDialog->objectName().isEmpty())
            PcbPaletteSettingsDialog->setObjectName(QStringLiteral("PcbPaletteSettingsDialog"));
        PcbPaletteSettingsDialog->resize(994, 583);
        gridLayout = new QGridLayout(PcbPaletteSettingsDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        systemTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (systemTable->columnCount() < 3)
            systemTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        systemTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        systemTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        systemTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        systemTable->setObjectName(QStringLiteral("systemTable"));

        gridLayout->addWidget(systemTable, 2, 3, 2, 1);

        silkscreenTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (silkscreenTable->columnCount() < 3)
            silkscreenTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        silkscreenTable->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        silkscreenTable->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        silkscreenTable->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        silkscreenTable->setObjectName(QStringLiteral("silkscreenTable"));

        gridLayout->addWidget(silkscreenTable, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(PcbPaletteSettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 4);

        mechanicalTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (mechanicalTable->columnCount() < 3)
            mechanicalTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        mechanicalTable->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        mechanicalTable->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        mechanicalTable->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        mechanicalTable->setObjectName(QStringLiteral("mechanicalTable"));

        gridLayout->addWidget(mechanicalTable, 0, 3, 1, 1);

        maskTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (maskTable->columnCount() < 3)
            maskTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        maskTable->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        maskTable->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        maskTable->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        maskTable->setObjectName(QStringLiteral("maskTable"));

        gridLayout->addWidget(maskTable, 2, 0, 1, 1);

        planeTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (planeTable->columnCount() < 3)
            planeTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        planeTable->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        planeTable->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        planeTable->setHorizontalHeaderItem(2, __qtablewidgetitem14);
        planeTable->setObjectName(QStringLiteral("planeTable"));

        gridLayout->addWidget(planeTable, 0, 2, 1, 1);

        otherTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (otherTable->columnCount() < 3)
            otherTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        otherTable->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        otherTable->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        otherTable->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        otherTable->setObjectName(QStringLiteral("otherTable"));

        gridLayout->addWidget(otherTable, 2, 2, 1, 1);

        signalTable = new QTableWidget(PcbPaletteSettingsDialog);
        if (signalTable->columnCount() < 3)
            signalTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        signalTable->setHorizontalHeaderItem(0, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        signalTable->setHorizontalHeaderItem(1, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        signalTable->setHorizontalHeaderItem(2, __qtablewidgetitem20);
        signalTable->setObjectName(QStringLiteral("signalTable"));

        gridLayout->addWidget(signalTable, 0, 0, 1, 1);

        checkBox = new QCheckBox(PcbPaletteSettingsDialog);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setChecked(true);

        gridLayout->addWidget(checkBox, 1, 0, 1, 1);

        checkBox_2 = new QCheckBox(PcbPaletteSettingsDialog);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setChecked(true);

        gridLayout->addWidget(checkBox_2, 1, 2, 1, 1);

        checkBox_3 = new QCheckBox(PcbPaletteSettingsDialog);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setChecked(true);

        gridLayout->addWidget(checkBox_3, 1, 3, 1, 1);

        groupBox = new QGroupBox(PcbPaletteSettingsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 1, 0, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_2->addWidget(pushButton_3, 2, 0, 1, 1);


        gridLayout->addWidget(groupBox, 3, 2, 1, 1);


        retranslateUi(PcbPaletteSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PcbPaletteSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PcbPaletteSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PcbPaletteSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *PcbPaletteSettingsDialog)
    {
        PcbPaletteSettingsDialog->setWindowTitle(QApplication::translate("PcbPaletteSettingsDialog", "Dialog", 0));
        QTableWidgetItem *___qtablewidgetitem = systemTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PcbPaletteSettingsDialog", "System Colors", 0));
        QTableWidgetItem *___qtablewidgetitem1 = systemTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PcbPaletteSettingsDialog", "New Column", 0));
        QTableWidgetItem *___qtablewidgetitem2 = systemTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem3 = silkscreenTable->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("PcbPaletteSettingsDialog", "Silkscreen Layers", 0));
        QTableWidgetItem *___qtablewidgetitem4 = silkscreenTable->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem5 = silkscreenTable->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem6 = mechanicalTable->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("PcbPaletteSettingsDialog", "Mechanical Layers", 0));
        QTableWidgetItem *___qtablewidgetitem7 = mechanicalTable->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem8 = mechanicalTable->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem9 = maskTable->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("PcbPaletteSettingsDialog", "Mask Layers", 0));
        QTableWidgetItem *___qtablewidgetitem10 = maskTable->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem11 = maskTable->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem12 = planeTable->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("PcbPaletteSettingsDialog", "Internal Planes", 0));
        QTableWidgetItem *___qtablewidgetitem13 = planeTable->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem14 = planeTable->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem15 = otherTable->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QApplication::translate("PcbPaletteSettingsDialog", "Other Layers", 0));
        QTableWidgetItem *___qtablewidgetitem16 = otherTable->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem17 = otherTable->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        QTableWidgetItem *___qtablewidgetitem18 = signalTable->horizontalHeaderItem(0);
        ___qtablewidgetitem18->setText(QApplication::translate("PcbPaletteSettingsDialog", "Signal Layers", 0));
        QTableWidgetItem *___qtablewidgetitem19 = signalTable->horizontalHeaderItem(1);
        ___qtablewidgetitem19->setText(QApplication::translate("PcbPaletteSettingsDialog", "Color", 0));
        QTableWidgetItem *___qtablewidgetitem20 = signalTable->horizontalHeaderItem(2);
        ___qtablewidgetitem20->setText(QApplication::translate("PcbPaletteSettingsDialog", "Visible", 0));
        checkBox->setText(QApplication::translate("PcbPaletteSettingsDialog", "Only show layers in layer stack", 0));
        checkBox_2->setText(QApplication::translate("PcbPaletteSettingsDialog", "Only show layers in layer stack", 0));
        checkBox_3->setText(QApplication::translate("PcbPaletteSettingsDialog", "Only show layers in layer stack", 0));
        groupBox->setTitle(QApplication::translate("PcbPaletteSettingsDialog", "Load - Store", 0));
        pushButton->setText(QApplication::translate("PcbPaletteSettingsDialog", "Load from ...", 0));
        pushButton_2->setText(QApplication::translate("PcbPaletteSettingsDialog", "Save to ...", 0));
        pushButton_3->setText(QApplication::translate("PcbPaletteSettingsDialog", "Import Altium 2D Color Profile ...", 0));
    } // retranslateUi

};

namespace Ui {
    class PcbPaletteSettingsDialog: public Ui_PcbPaletteSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PCBPALETTESETTINGSDIALOG_H
