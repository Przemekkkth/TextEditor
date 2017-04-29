#ifndef GOTO_H
#define GOTO_H

#include <QWidget>

class QPushButton;
class QLineEdit;
class QCheckBox;

class GoTo : public QWidget
{
    Q_OBJECT
public:
    explicit GoTo(QWidget *parent = 0);
    void setText(const QString &);
    QString getText() const;
signals:
    void replaceClicked(QString replacedText);
public slots:

private slots:
    void checkFind();
    void checkReplace();
    void findSlot();
    void replaceSlot();
private:
    QPushButton *m_findButton, *m_replaceButton;
    QLineEdit *m_findEdit, *m_replaceEdit;
    QCheckBox *m_Info, *m_allReplacement;

    QString text;
    bool m_isFinded;
};

#endif // GOTO_H
