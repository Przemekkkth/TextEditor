#include "fonts.h"
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

Fonts::Fonts(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Fonts");

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;

    m_fontRed = new QSpinBox;
    m_fontRed->setRange(0, 255);
    m_fontRed->setValue(255);
    m_fontGreen = new QSpinBox;
    m_fontGreen->setRange(0, 255);
    m_fontGreen->setValue(255);
    m_fontBlue = new QSpinBox;
    m_fontBlue->setRange(0 ,255);
    m_fontBlue->setValue(255);
    m_fontSize = new QSpinBox;
    m_fontSize->setRange(10, 72);
    m_fontSize->setValue(12);

    m_fontBold = new QComboBox;
    m_fontBold->addItem("None");
    m_fontBold->addItem("Bold");
    m_fontBold->setCurrentIndex(0);
    m_fontItalic = new QComboBox;
    m_fontItalic->addItem( "None");
    m_fontItalic->addItem("Italic");
    m_fontItalic->setCurrentIndex(0);
    m_fontUnderline = new QComboBox;
    m_fontUnderline->addItem( "None");
    m_fontUnderline->addItem("Underline");
    m_fontUnderline->setCurrentIndex(0);
    formLayout->addRow("<h5>Red : </h5>", m_fontRed);
    formLayout->addRow("<h5>Green : </h5>", m_fontGreen);
    formLayout->addRow("<h5>Blue : </h5>", m_fontBlue);
    formLayout->addRow("<h5>Size : </h5>", m_fontSize);



    formLayout->addRow("<h5>Bold : </h5>", m_fontBold);
    formLayout->addRow("<h5>Italic : </h5>", m_fontItalic);
    formLayout->addRow("<h5>Underline : </h5>", m_fontUnderline);

    m_Title = new QLabel("<h1>Fonts</h2>");
    m_Title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_Button = new QPushButton("Close");

    vboxLayout->addWidget(m_Title);
    vboxLayout->addLayout(formLayout);
    vboxLayout->addWidget(m_Button);
    setLayout(vboxLayout);
    setWindowIcon(QIcon(":/WindowIcon/WindowIcon"));

    connect(m_fontRed, SIGNAL(valueChanged(int)), this, SLOT(changedRedFont(int)));
    connect(m_fontGreen, SIGNAL(valueChanged(int)), this, SLOT(changedGreenFont(int)));
    connect(m_fontBlue, SIGNAL(valueChanged(int)), this, SLOT(changedBlueFont(int)));
    connect(m_fontBold, SIGNAL(currentIndexChanged(int)), this, SLOT(changedBold(int)));
    connect(m_fontItalic, SIGNAL(currentIndexChanged(int)), this, SLOT(changedItalic(int)));
    connect(m_fontUnderline, SIGNAL(currentIndexChanged(int)), this, SLOT(changedUnderline(int)));
    connect(m_fontSize, SIGNAL(valueChanged(int)), this, SLOT(changedSizeFont(int)));
    connect(m_Button, SIGNAL(clicked(bool)), this, SLOT(close()));

}

void Fonts::changedRedFont(int val)
{
    emit setRedColor(val);
}

void Fonts::changedGreenFont(int val)
{
    emit setGreenColor(val);
}

void Fonts::changedBlueFont(int val)
{
    emit setBlueColor(val);
}

void Fonts::changedBold(int val)
{
    if(val == 0)
    {
        emit setBold(false);
    }
    else
    {
        emit setBold(true);
    }
}

void Fonts::changedItalic(int val)
{
    if(val == 0)
    {
        emit setItalic(false);
    }
    else
    {
        emit setItalic(true);
    }
}

void Fonts::changedUnderline(int val)
{
    if(val == 0)
    {
        emit setUnderline(false);
    }
    else
    {
        emit setUnderline(true);
    }
}

void Fonts::changedSizeFont(int val)
{
    emit setSizeFont(val);
}

int Fonts::getFontRed()
{
    return m_fontRed->value();
}

int Fonts::getFontGreen()
{
    return m_fontGreen->value();
}

int Fonts::getFontBlue()
{
    return m_fontBlue->value();
}
