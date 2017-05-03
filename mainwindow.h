#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>


class QAction;
class QMenu;
class QLabel;
class QPrinter;
class QTextEdit;
class QToolBar;
class QDockWidget;
class QWidget;

class Fonts;
class GoTo;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateAllRecentFiles();
    void updateRecentFiles();
protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    bool eventFilter(QObject *, QEvent *);
private:
    static QStringList recentStringFiles;
    enum { MaxRecentFiles = 5 };
    //Central Widget
    QWidget *m_centralWidget;
    QWidget *m_leftWidget;
    QWidget *m_rightWidget;
    QTextEdit *m_textEdit;
    QDockWidget *m_dockedWidget;
    //Variable to operate on files
    QString m_FileName;
    QString m_PathFile;
    //Actions
    QAction *m_OpenAction, *m_NewAction, *m_NewAppAction, *m_SaveAction, *m_SaveAsAction, *m_PrintAction, *m_RecentFilesAction[MaxRecentFiles], *m_SeparatorAction, *m_openAllRecentFileAction, *m_deleleAllRecentFilesAction, *m_CloseAction, *m_ExitAction;
    QAction *m_UndoAction, *m_RedoAction, *m_CutAction, *m_CopyAction, *m_PasteAction, *m_SelectAllAction;
    QAction *m_FontsAction, *m_FindAction, *m_AboutSoftAction, *m_AboutQtAction;
    QAction *m_AlwaysOnTopAction, *m_FullScreenAction, *m_NormalScreenAction;
    //Menus
    QMenu *m_FileMenu;
    QMenu *m_EditMenu;
    QMenu *m_ToolMenu;
    QMenu *m_SettingsMenu;
    QMenu *m_ViewMenu;
    //ToolBar
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_toolToolBar;
    QToolBar *m_viewToolBar;
    QToolBar *m_level2ToolBar;
    //Data from central widget(textEdit)
    QString m_actualString , m_savedString;
    //Other components
    Fonts *m_FontsDialog;
    GoTo *m_GoTo;
    QPrinter *m_Printer;
    //Labels
    QLabel *m_nameFile;
    QLabel *m_cursorInformation;
    QLabel *m_countRowLabel;
    //Function
    void createActions();
    void createMenus();
    void createContextMenu();
    void createStatusBar();

    void createToolBar();
    void createWidgetsAndSettings();
    void readSettings();
    void writeSettings();


    void newFileAction();
    bool ReadFile(QString);
    void SaveFile(QString);



private slots:
    void newFile();
    void newApp();
    void openFile();
    void saveFile();
    void saveAsFile();
    void openRecentFile();
    void printDocument();
    void openFontDialog();
    void openGoToDialog();

    void openAllRecentFiles();
    void deleteAllRecentFiles();
    void updateStatusBar();
    void updateCounterRows();
    void aboutSoft();


    void setAlwaysOnTop(bool);
    void showFullScreen();
    void showNormalScreen();
    void checkModifity();
    //Font Dialog
    void setRedFont(int);
    void setGreenFont(int);
    void setBlueFont(int);
    void setFontSize(int);
    void setBoldFont(bool);
};

#endif // MAINWINDOW_H

