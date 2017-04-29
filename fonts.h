#ifndef FONTS_H
#define FONTS_H

#include <QDialog>

class QPushButton;
class QSpinBox;
class QLabel;
class QComboBox;


class Fonts : public QDialog
{
    Q_OBJECT
public:
    explicit Fonts(QWidget *parent = 0);
    int getFontRed();
    int getFontGreen();
    int getFontBlue();
signals:
    void setBold(bool);
    void setUnderline(bool);
    void setItalic(bool);
    void setRedColor(int);
    void setGreenColor(int);
    void setBlueColor(int);
    void setSizeFont(int);
public slots:
   void changedBold(int);
   void changedItalic(int);
   void changedUnderline(int);
   void changedRedFont(int);
   void changedGreenFont(int);
   void changedBlueFont(int);
   void changedSizeFont(int);
private:
    QPushButton *m_Button;
    QLabel *m_Title;
    QSpinBox *m_fontSize;
    QSpinBox *m_fontRed;
    QSpinBox *m_fontGreen;
    QSpinBox *m_fontBlue;
    QComboBox *m_fontBold;
    QComboBox *m_fontItalic;
    QComboBox *m_fontUnderline;
};

#endif // FONTS_H
