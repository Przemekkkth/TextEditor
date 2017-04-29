#include "goto.h"
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <QMessageBox>

GoTo::GoTo(QWidget *parent) : QWidget(parent)
{
    m_findEdit = new QLineEdit;
    m_replaceEdit = new QLineEdit;
    m_findButton = new QPushButton;
    m_replaceButton = new QPushButton;
    m_allReplacement = new QCheckBox;
    m_Info = new QCheckBox;

    m_findButton->setText("Find");
    m_replaceButton->setText("Replace");


    m_allReplacement->setText("Replace all");

    m_Info->setText("Information");

    QHBoxLayout *h0boxlayout = new QHBoxLayout;
    h0boxlayout->addWidget(new QLabel("<h5>Find: </h5>"));
    h0boxlayout->addWidget(m_findEdit);
    h0boxlayout->addWidget(m_findButton);
    h0boxlayout->addWidget(m_Info);

    QHBoxLayout *h1boxlayout = new QHBoxLayout;
    h1boxlayout->addWidget(new QLabel("<h5>Replace: </h5>"));
    h1boxlayout->addWidget(m_replaceEdit);
    h1boxlayout->addWidget(m_replaceButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(h0boxlayout);
    mainLayout->addLayout(h1boxlayout);
    mainLayout->addWidget(m_allReplacement);

    m_findButton->setEnabled(false);
    m_replaceButton->setEnabled(false);
    m_isFinded = false;

    setMinimumSize(QSize(300, 140));
    setMaximumSize(QSize(300, 140));
    setLayout(mainLayout);
    setWindowTitle("Go to ...");
    setWindowIcon(QIcon(":/WindowIcon/WindowIcon"));

    connect(m_findEdit, SIGNAL(textChanged(QString)), this, SLOT(checkFind()));
    connect(m_replaceEdit, SIGNAL(textChanged(QString)), this, SLOT(checkReplace()));
    connect(m_findButton, SIGNAL(clicked(bool)), this, SLOT(findSlot()));
    connect(m_replaceButton, SIGNAL(clicked(bool)), this, SLOT(replaceSlot()));

}

void GoTo::setText(const QString &text)
{
    this->text = text;
}


void GoTo::checkFind()
{
    m_isFinded = false;
    if(m_findEdit->text().isEmpty())
    {
        m_findButton->setEnabled(false);
    }
    else
    {
        m_findButton->setEnabled(true);
    }
}

void GoTo::checkReplace()
{
    if(m_replaceEdit->text().isEmpty() || m_isFinded == false)
    {
        m_replaceButton->setEnabled(false);
    }
    else
    {
        m_replaceButton->setEnabled(true);
    }
}

void GoTo::findSlot()
{
    int count = 0;
    int offset = 0;
    text.indexOf(m_findEdit->text()) != -1 ? m_isFinded = true : m_isFinded = false;
    while(text.indexOf(m_findEdit->text(), offset) != -1)
    {
        int oldoffset;
        oldoffset = text.indexOf(m_findEdit->text(), offset);
        offset = oldoffset + m_findEdit->text().size();
        count++;
    }
    if(m_Info->isChecked())
    {
        if(text.indexOf(m_findEdit->text()) != -1)
        {
            QMessageBox::information(this,"Information", "Found : " + m_findEdit->text() + " " + QString::number(count) + " time(s)", QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this,"Information", "No found : " + m_findEdit->text(), QMessageBox::Ok);
        }
    }

}

void GoTo::replaceSlot()
{
    if(m_allReplacement->isChecked())
    {
        int offset = 0 ;
        while(text.indexOf(m_findEdit->text(), offset) != -1)
        {
            int  oldoffset;
            oldoffset = text.indexOf(m_findEdit->text(), offset);
            text.replace(oldoffset,  m_replaceEdit->text().size(), m_replaceEdit->text());
            offset = oldoffset + m_replaceEdit->text().size();
        }


    }
    else
    {
        if(text.indexOf(m_findEdit->text()) != -1)
        {
            text.replace(text.indexOf(m_findEdit->text()), m_replaceEdit->text().size(), m_replaceEdit->text());

        }
    }
    emit replaceClicked(text);
}

QString GoTo::getText() const
{
    return text;
}
