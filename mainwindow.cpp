#include "mainwindow.h"
#include "fonts.h"
#include "goto.h"

#include <QObject>
#include <QEvent>
#include <QMimeData>
#include <QTextEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QCloseEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QStatusBar>
#include <QToolBar>
#include <QSettings>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QApplication>
#include <QLabel>
#include <QDateTime>
#include <QDropEvent>
#include <QDragEnterEvent>



QStringList MainWindow::recentStringFiles;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_textEdit = new QTextEdit(this);
    m_textEdit->setAcceptDrops(false);
    setAcceptDrops(true);
    m_savedString = m_actualString = m_textEdit->toPlainText();
    m_FontsDialog = new Fonts;
    m_GoTo = new GoTo;
    m_Printer = new QPrinter;
    m_FileName = "untitled";
    m_PathFile  = "";
    m_cursorInformation = new QLabel;
    m_nameFile = new QLabel;
    //filters
    m_cursorInformation->installEventFilter(this);

    setCentralWidget(m_textEdit);
    setWindowModified(false);
    setMinimumSize(640, 480);
    setWindowTitle(tr("%1[*] - text editor").arg(m_FileName));
    setWindowIcon(QIcon(":/WindowIcon/WindowIcon"));
    setAttribute(Qt::WA_DeleteOnClose);
   // setContextMenuPolicy(Qt:);

    createActions();
    createMenus();
    createContextMenu();
    createStatusBar();
    createToolBar();
   // statusBar()->showMessage(tr("Text Editor"),2000);
    readSettings();

    connect(m_textEdit, SIGNAL(textChanged()), this, SLOT(updateStatusBar()));

    connect(m_NewAction, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(m_NewAppAction, SIGNAL(triggered(bool)), this, SLOT(newApp()));
    connect(m_OpenAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(m_SaveAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(m_SaveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAsFile()));
    connect(m_PrintAction, SIGNAL(triggered(bool)), this, SLOT(printDocument()));

    for(int i = 0 ; i < MaxRecentFiles ; ++i)
    {
        connect(m_RecentFilesAction[i], SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
    }
    connect(m_openAllRecentFileAction, SIGNAL(triggered(bool)), this, SLOT(openAllRecentFiles()));
    connect(m_deleleAllRecentFilesAction, SIGNAL(triggered(bool)), this, SLOT(deleteAllRecentFiles()));

    connect(m_CloseAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(m_ExitAction, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));

    connect(m_UndoAction, SIGNAL(triggered(bool)), m_textEdit, SLOT(undo()));
    connect(m_RedoAction, SIGNAL(triggered(bool)), m_textEdit, SLOT(redo()));
    connect(m_CutAction, SIGNAL(triggered(bool)), m_textEdit, SLOT(cut()));
    connect(m_CopyAction, SIGNAL(triggered(bool)), m_textEdit, SLOT(copy()));
    connect(m_PasteAction, SIGNAL(triggered(bool)), m_textEdit, SLOT(paste()));

    connect(m_FontsAction, SIGNAL(triggered(bool)), this, SLOT(openFontDialog()));
    connect(m_FindAction, SIGNAL(triggered(bool)), this, SLOT(openGoToDialog()));
    connect(m_AboutSoftAction, SIGNAL(triggered(bool)), this, SLOT(aboutSoft()));
    connect(m_AboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    connect(m_textEdit, SIGNAL(textChanged()), this, SLOT(checkModifity()));


    // Font Dialog
    connect(m_FontsDialog, SIGNAL(setItalic(bool)), m_textEdit, SLOT(setFontItalic(bool)));
    connect(m_FontsDialog, SIGNAL(setUnderline(bool)), m_textEdit, SLOT(setFontUnderline(bool)));
    connect(m_FontsDialog, SIGNAL(setBold(bool)), this, SLOT(setBoldFont(bool)));
    connect(m_FontsDialog, SIGNAL(setRedColor(int)), this, SLOT(setRedFont(int)));
    connect(m_FontsDialog, SIGNAL(setBlueColor(int)), this, SLOT(setBlueFont(int)));
    connect(m_FontsDialog, SIGNAL(setGreenColor(int)), this, SLOT(setGreenFont(int)));
    connect(m_FontsDialog, SIGNAL(setSizeFont(int)), this, SLOT(setFontSize(int)));
    //View
    connect(m_AlwaysOnTopAction, SIGNAL(toggled(bool)), this, SLOT(setAlwaysOnTop(bool)));
    connect(m_FullScreenAction, SIGNAL(triggered(bool)), this, SLOT(showFullScreen()));
    connect(m_NormalScreenAction, SIGNAL(triggered(bool)), this, SLOT(showNormalScreen()));


}

MainWindow::~MainWindow()
{

}
void MainWindow::closeEvent(QCloseEvent *e)
{
    int answer;
    if(isWindowModified())
    {
        answer = QMessageBox::information(this, tr("Information"), tr("The document has been unsaved data\nDo you want to close ? \n"), QMessageBox::Yes | QMessageBox::No);

        if(answer == QMessageBox::Yes)
        {
            writeSettings();
            e->accept();
        }
        else if(answer == QMessageBox::No)
        {
            e->ignore();
        }
    }
    else
    {
        writeSettings();
        e->accept();
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
        close();
}

void MainWindow::createStatusBar()
{


    statusBar()->addWidget(m_nameFile);
    statusBar()->addWidget(m_cursorInformation);
}

void MainWindow::createActions()
{
    m_NewAction = new QAction(tr("New"), this);
    m_NewAction->setShortcut(QKeySequence::New);
    m_NewAction->setStatusTip(tr("New file..."));
    m_NewAction->setIcon(QIcon(":/File/New"));

    m_NewAppAction = new QAction(tr("New app"), this);
    m_NewAppAction->setStatusTip(tr("New App"));

    m_OpenAction = new QAction(tr("Open"), this);
    m_OpenAction->setShortcut(QKeySequence::Open);
    m_OpenAction->setStatusTip(tr("Open file..."));
    m_OpenAction->setIcon(QIcon(":/File/png/File/open-file.png"));

    m_SaveAction = new QAction(tr("Save"), this);
    m_SaveAction->setShortcut(QKeySequence::Save);
    m_SaveAction->setStatusTip(tr("Save ") + m_FileName);
    m_SaveAction->setIcon(QIcon(":/File/Save"));

    m_SaveAsAction = new QAction(tr("Save as"), this);
    m_SaveAsAction->setShortcut(QKeySequence::SaveAs);
    m_SaveAsAction->setStatusTip(tr("Save a new file"));
    m_SaveAsAction->setIcon(QIcon(":/File/Save_As"));

    m_PrintAction = new QAction(tr("Print"), this);
    m_PrintAction->setShortcut(QKeySequence::Print);
    m_PrintAction->setIcon(QIcon(":/File/png/File/print.png"));
    m_PrintAction->setStatusTip(tr("Print a document"));

    m_SeparatorAction = new QAction(this);
    m_SeparatorAction = menuBar()->addSeparator();
    m_SeparatorAction->setVisible(false);
    for(int i = 0 ; i < MaxRecentFiles ; ++i)
    {
        m_RecentFilesAction[i] = new QAction(this);
        m_RecentFilesAction[i]->setVisible(false);
    }

    m_openAllRecentFileAction = new QAction(this);
    m_openAllRecentFileAction->setText(tr("Open All Recent Files"));
    m_openAllRecentFileAction->setIcon(QIcon(":/File/png/File/openAll.png"));

    m_deleleAllRecentFilesAction = new QAction(this);
    m_deleleAllRecentFilesAction->setText(tr("Delete List of Recent Files"));
    m_deleleAllRecentFilesAction->setIcon(QIcon(":/File/png/File/delete.png"));

    m_CloseAction = new QAction(tr("Close"), this);
    m_CloseAction->setStatusTip(tr("Close window"));
    m_CloseAction->setShortcut(QKeySequence::Close);

    m_ExitAction = new QAction(tr("Exit"), this);
    m_ExitAction->setShortcut(QKeySequence::ExactMatch);
    m_ExitAction->setStatusTip(tr("Close app"));
    m_ExitAction->setIcon(QIcon(":/File/Exit"));
    //
    m_UndoAction = new QAction(tr("Undo"), this);
    m_UndoAction->setShortcut(QKeySequence::Undo);
    m_UndoAction->setStatusTip(tr("Undo action"));
    m_UndoAction->setIcon(QIcon(":/Edit/Undo"));
    m_RedoAction = new QAction(tr("Redo"), this);
    m_RedoAction->setShortcut(QKeySequence::Redo);
    m_RedoAction->setStatusTip(tr("Redo action"));
    m_RedoAction->setIcon(QIcon(":/Edit/Redo"));
    m_CopyAction = new QAction(tr("Copy"), this);
    m_CopyAction->setShortcut(QKeySequence::Copy);
    m_CopyAction->setStatusTip(tr("Copy text"));
    m_CopyAction->setIcon(QIcon(":/Edit/Copy"));
    m_CutAction = new QAction(tr("Cut"), this);
    m_CutAction->setShortcut(QKeySequence::Cut);
    m_CutAction->setStatusTip(tr("Cut text"));
    m_CutAction->setIcon(QIcon(":/Edit/Cut"));
    m_PasteAction = new QAction(tr("Paste"), this);
    m_PasteAction->setShortcut(QKeySequence::Paste);
    m_PasteAction->setStatusTip(tr("Paste text"));
    m_PasteAction->setIcon(QIcon(":/Edit/Paste"));
    m_SelectAllAction = new QAction(tr("Select All"), this);
    m_SelectAllAction->setShortcut(QKeySequence::SelectAll);
    m_SelectAllAction->setStatusTip("Select All");
    m_SelectAllAction->setIcon(QIcon(":/Edit/Select_All"));
    //
    m_FontsAction = new QAction(QIcon(":/Tool/png/Tool/font.png"),tr("Fonts"), this);
    m_FindAction = new QAction(QIcon(":/Tool/png/Tool/find.png"),tr("Find"), this);
    m_AboutSoftAction = new QAction(tr("About Software"), this);
    m_AboutQtAction = new QAction(tr("About Qt"), this);
    //
    m_AlwaysOnTopAction = new QAction(tr("Always on top"),this);
    m_AlwaysOnTopAction->setCheckable(true);
    m_AlwaysOnTopAction->setChecked(true);
    m_FullScreenAction = new QAction(QIcon(":/View/png/View/fullscreen.png"), tr("Full Screen"), this);
    m_NormalScreenAction = new QAction(QIcon(":/View/png/View/normalscreen.png"),tr("Normal Screen"), this);

}

void MainWindow::createMenus()
{
    m_FileMenu = new QMenu(this);
    m_FileMenu = menuBar()->addMenu(tr("File"));
    m_FileMenu->addAction(m_NewAction);
    m_FileMenu->addAction(m_NewAppAction);
    m_FileMenu->addAction(m_OpenAction);
    m_FileMenu->addAction(m_SaveAction);
    m_FileMenu->addAction(m_SaveAsAction);
    m_FileMenu->addAction(m_PrintAction);
    m_FileMenu->addAction(m_SeparatorAction);
    for(int i = 0 ; i < MaxRecentFiles; ++i)
    {
        m_FileMenu->addAction(m_RecentFilesAction[i]);
    }
    m_FileMenu->addSeparator();
    m_FileMenu->addAction(m_openAllRecentFileAction);
    m_FileMenu->addAction(m_deleleAllRecentFilesAction);
    m_FileMenu->addAction(m_CloseAction);
    m_FileMenu->addAction(m_ExitAction);
    //
    m_EditMenu = new QMenu(this);
    m_EditMenu = menuBar()->addMenu(tr("Edit"));
    m_EditMenu->addAction(m_UndoAction);
    m_EditMenu->addAction(m_RedoAction);
    m_EditMenu->addAction(m_CopyAction);
    m_EditMenu->addAction(m_CutAction);
    m_EditMenu->addAction(m_PasteAction);
    m_EditMenu->addAction(m_SelectAllAction);
    //
    m_ViewMenu = new QMenu(this);
    m_ViewMenu = menuBar()->addMenu(tr("View"));
    m_ViewMenu->addAction(m_AlwaysOnTopAction);
    m_ViewMenu->addAction(m_FullScreenAction);
    m_ViewMenu->addAction(m_NormalScreenAction);
    //
    m_ToolMenu = new QMenu(this);
    m_ToolMenu = menuBar()->addMenu(tr("Tools"));
    m_ToolMenu->addAction(m_FontsAction);
    m_ToolMenu->addAction(m_FindAction);
    //
    m_SettingsMenu = new QMenu(this);
    m_SettingsMenu = menuBar()->addMenu(tr("Settings"));
    m_SettingsMenu->addAction(m_AboutSoftAction);
    m_SettingsMenu->addAction(m_AboutQtAction);


    //connect(m_textEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowModified(bool)) );
    connect(m_textEdit, SIGNAL(copyAvailable(bool)), m_CutAction, SLOT(setEnabled(bool)));

}

void MainWindow::newFile()
{
    if(!isWindowModified())
    {
        newFileAction();

    }
    else
    {
        int answer;
        answer = QMessageBox::information(this, tr("Information"), tr("Do you want to save ?"), QMessageBox::Yes | QMessageBox::Default,
                                          QMessageBox::No,
                                          QMessageBox::Cancel | QMessageBox::Escape);
        if(answer == QMessageBox::Cancel || answer == QMessageBox::Escape)
        {
            return;
        }
        else if(answer == QMessageBox::No)
        {
            newFileAction();
        }
        else
        {
            if(m_FileName == "untitled")
            {
                saveAsFile();
            }
            else
            {
                saveFile();
            }
        }
    }
}
void MainWindow::newApp()
{
    MainWindow *instance = new MainWindow;
    instance->show();
}


void MainWindow::openFile()
{
    int answer = 0;
    if(isWindowModified())
    {
        answer = QMessageBox::information(this, tr("Information"), tr("Do you wana to save ?"), QMessageBox::Yes, QMessageBox::No);
    }
    if(answer == QMessageBox::Yes)
    {
        saveAsFile();
    }

    m_PathFile = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), "All types (*.*) ;; Normal text (*.txt) ;; Assembly language (*.asm) ;; Unix script (*.bash; *.sh; *.bsh; *.csh) ;;"
                                                                                          "Batch file(*.bat; *.cmd; *.nt) ;; C source file(*.c) ;; CMake file(*.cmake) ;; C++ soure file (*.h ; *.hpp ; *.hxx; *.cpp; *.cxx ; *.cc) ;;"
                                                                                          "C# source file (*.cs) ;; Cascade Style Sheet File (*.css) ;; Hyper Text Markup Language(*.html; *.htm; *.shtml; *.shtm; *.xhtml; *.xht ; *.hta ) ;;"
                                                                                          "MS ini file (*.ini; *.inf; *.reg; *.url; *.wer) ;; Java source file (*.java) ;;  Java Script file (*.js; *.jsm) ;; JSON file (*.json);;"
                                                                                          "PHP Hypertext Preprocesor file (*.php; *.php3; *.php4; *.php5; *.phpt; *.phtml) ;; Window Resoure file (*.rc) ;; Ruby file(*.rb ; *.rbw) ;;"
                                                                                          "eXtensible Markup Language file (*.xml ; *.xaml; *.xsl ; *.xsd; *.xul; *.kml; *.svg; *.mxml; *.wsdl; *.xlf; *.xlif; *.sxbl; *.sitemap; *.gml; *.gpx; *.plist)");
    ReadFile(m_PathFile);

}

void MainWindow::saveFile()
{
    if(m_FileName == "untitled")
    {
        saveAsFile();
    }

    QFile fileToSave(m_PathFile);
    if(fileToSave.open(QIODevice::WriteOnly))
    {
        return;
    }
    QTextStream stream(&fileToSave);

    stream << m_textEdit->toPlainText();

    m_savedString = m_actualString = m_textEdit->toPlainText();
    setWindowModified(false);
    setWindowTitle(tr("%1[*] - text editor").arg(m_FileName));

    fileToSave.close();



    statusBar()->showMessage(tr("Saved : %1").arg(m_FileName), 2000);
}

void MainWindow::saveAsFile()
{
    m_PathFile = QFileDialog::getSaveFileName(this, tr("Save As..."), QDir::currentPath(), "All types (*.*) ;; Normal text (*.txt) ;; Assembly language (*.asm) ;; Unix script (*.bash; *.sh; *.bsh; *.csh) ;;"
                                                                                           "Batch file(*.bat; *.cmd; *.nt) ;; C source file(*.c) ;; CMake file(*.cmake) ;; C++ soure file (*.h ; *.hpp ; *.hxx; *.cpp; *.cxx ; *.cc) ;;"
                                                                                           "C# source file (*.cs) ;; Cascade Style Sheet File (*.css) ;; Hyper Text Markup Language(*.html; *.htm; *.shtml; *.shtm; *.xhtml; *.xht ; *.hta ) ;;"
                                                                                           "MS ini file (*.ini; *.inf; *.reg; *.url; *.wer) ;; Java source file (*.java) ;;  Java Script file (*.js; *.jsm) ;; JSON file (*.json);;"
                                                                                           "PHP Hypertext Preprocesor file (*.php; *.php3; *.php4; *.php5; *.phpt; *.phtml) ;; Window Resoure file (*.rc) ;; Ruby file(*.rb ; *.rbw) ;;"
                                                                                           "eXtensible Markup Language file (*.xml ; *.xaml; *.xsl ; *.xsd; *.xul; *.kml; *.svg; *.mxml; *.wsdl; *.xlf; *.xlif; *.sxbl; *.sitemap; *.gml; *.gpx; *.plist)");
    if(m_PathFile.isEmpty())
    {
        return;
    }
    QFile fileToSave(m_PathFile);


    QTextStream stream(&fileToSave);

    m_textEdit->setPlainText(stream.readAll());

    m_savedString = m_actualString = m_textEdit->toPlainText();
    setWindowModified(false);
    setWindowTitle(tr("%1[*] - text editor").arg(m_FileName));
    fileToSave.close();
    updateRecentFiles();
    statusBar()->showMessage("Saved : " + m_FileName, 2000);
}

void MainWindow::printDocument()
{
    QPrintDialog printDialog(m_Printer, this);
    if(printDialog.exec()){
        m_textEdit->print(m_Printer);
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {

        int answer = 0;
        if(isWindowModified())
        {
            answer = QMessageBox::information(this, tr("Information"), tr("Do you wana to save ?"), QMessageBox::Yes, QMessageBox::No);
        }
        if(answer == QMessageBox::Yes)
        {
            saveAsFile();
        }

        m_PathFile = action->data().toString();
        ReadFile(m_PathFile);

    }
}

void MainWindow::openFontDialog()
{
    m_FontsDialog->exec();

}

void MainWindow::checkModifity()
{
    m_actualString = m_textEdit->toPlainText();
    if(m_actualString != m_savedString)
    {
        setWindowModified(true);
    }
    else
    {
        setWindowModified(false);
    }
}

void MainWindow::createContextMenu()
{
    m_textEdit->addAction(m_NewAction);
    m_textEdit->addAction(m_OpenAction);
    m_textEdit->addAction(m_SaveAction);
    m_textEdit->addAction(m_FontsAction);
    m_textEdit->addAction(m_FindAction);


    m_textEdit->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBar()
{
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_NewAction);
    m_fileToolBar->addAction(m_OpenAction);
    m_fileToolBar->addAction(m_SaveAction);
    m_fileToolBar->addAction(m_PrintAction);
    m_fileToolBar->addAction(m_ExitAction);

    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_UndoAction);
    m_editToolBar->addAction(m_RedoAction);

    m_viewToolBar = addToolBar(tr("View"));
    m_viewToolBar->addAction(m_FullScreenAction);
    m_viewToolBar->addAction(m_NormalScreenAction);

    m_toolToolBar = addToolBar(tr("Tool"));
    m_toolToolBar->addAction(m_FontsAction);
    m_toolToolBar->addAction(m_FindAction);



    addToolBarBreak();

    m_level2ToolBar = addToolBar(tr("RecentFiles"));

    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        m_level2ToolBar->addAction(m_RecentFilesAction[i]);
    }
}

void MainWindow::readSettings()
{
    QSettings settings("Badaruk Soft", "Text Editor");
    recentStringFiles = settings.value("RecentFiles").toStringList();
    updateRecentFiles();
    updateStatusBar();
}

void MainWindow::writeSettings()
{
    QSettings settings("Badaruk Soft", "Text Editor");
    settings.setValue("RecentFiles", recentStringFiles);
}

void MainWindow::updateRecentFiles()
{
    if(!m_PathFile.isEmpty()){
        if(recentStringFiles.indexOf(m_PathFile) == -1)
        recentStringFiles.push_front(m_PathFile);
    }

    if(recentStringFiles.size() > MaxRecentFiles)
    {
        recentStringFiles.pop_back();
    }


    for(int i = 0 ; i < MaxRecentFiles; ++i)
    {
        if(i < recentStringFiles.size())
        {
        m_RecentFilesAction[i]->setVisible(true);
        m_RecentFilesAction[i]->setData(recentStringFiles.at(i));
        m_RecentFilesAction[i]->setText(QFileInfo(recentStringFiles.at(i)).fileName());
        }
        else
        {
        m_RecentFilesAction[i]->setVisible(false);
        }
    }
    if(!recentStringFiles.isEmpty())
        m_SeparatorAction->setVisible(true);
}

void MainWindow::setRedFont(int val)
{
    m_textEdit->setTextColor(QColor(val, m_FontsDialog->getFontGreen(), m_FontsDialog->getFontBlue()));
}

void MainWindow::setGreenFont(int val)
{
    m_textEdit->setTextColor(QColor(m_FontsDialog->getFontRed(), val, m_FontsDialog->getFontBlue()));
}

void MainWindow::setBlueFont(int val)
{
    m_textEdit->setTextColor(QColor(m_FontsDialog->getFontRed(), m_FontsDialog->getFontGreen(), val));
}

void MainWindow::newFileAction()
{
    m_textEdit->clear();
    m_savedString = m_actualString = m_textEdit->toPlainText();
    m_FileName = "untitled";
    setWindowTitle(tr("%1[*] - text editor").arg(m_FileName));
    setWindowModified(false);
}

void MainWindow::openGoToDialog()
{
    connect(m_GoTo, SIGNAL(replaceClicked(QString)), m_textEdit, SLOT(setText(QString)));
    if(m_GoTo->isHidden()){
    m_GoTo->show();
    m_GoTo->setText(m_textEdit->toPlainText());

    }
    else
    {
        m_GoTo->activateWindow();
    }
}

void MainWindow::updateAllRecentFiles()
{
    foreach(QWidget *win, QApplication::topLevelWidgets())
    {
        MainWindow *mainwindow = qobject_cast<MainWindow*>(win);
        mainwindow->updateRecentFiles();
    }
}

bool MainWindow::ReadFile(QString str)
{
    if(str.isEmpty())
    {
        return false;
    }

    QFile fileToOpen(str);

    if(!fileToOpen.open(QIODevice::ReadOnly))
    {
        return false;
    }
    updateRecentFiles();
    QTextStream stream(&fileToOpen);

    m_textEdit->setPlainText(stream.readAll());

    QFileInfo fileInfo(str);
    m_FileName = fileInfo.fileName();

    m_savedString = m_actualString = m_textEdit->toPlainText();
    setWindowModified(false);
    setWindowTitle(tr("%1[*] - text editor").arg(m_FileName));

    fileToOpen.close();

    statusBar()->showMessage(tr("Opened : %1").arg(m_FileName), 2000);
    updateStatusBar();

    return true;
}

void MainWindow::openAllRecentFiles()
{
   // QMessageBox::information(0, "Tekst", QString::number(recentStringFiles.size()), QMessageBox::Ok);
    foreach(QString fileName, recentStringFiles)
    {
        MainWindow* mainWindow = new MainWindow;
        mainWindow->ReadFile(fileName);
        QMessageBox::information(0, "Tekst", fileName, QMessageBox::Ok);
        mainWindow->updateRecentFiles();
        mainWindow->show();

    }
    recentStringFiles.clear();

}

void MainWindow::deleteAllRecentFiles()
{

    recentStringFiles.clear();
    updateRecentFiles();
}

void MainWindow::aboutSoft()
{
    QMessageBox::about(this, tr("About program"), tr("<h2>About TextEditor 0.9</h2>"
                                "<p>TextEditor &copy; 2017 GPL"
                                "<p>TextEditor is a small appliation that "
                                "demonstrates Qt's Widgets\n"
                                                     "<p>Icons: <a href=\"http://www.flaticon.com/\" > www.flaticon.com</a>"));
}

void MainWindow::updateStatusBar()
{


  //  statusBar()->showMessage(tr("%1\tCols:%2\tRows:%3\tLrngth: %4").arg(m_FileName).arg(m_textEdit->textCursor().blockNumber()).arg(m_textEdit->toPlainText().contains("\n")).arg(m_textEdit->textCursor().position()));
   m_cursorInformation->setText(tr("Cols:%1\tRows:%2\tLength: %3").arg(m_textEdit->textCursor().columnNumber()).arg(m_textEdit->textCursor().blockNumber()).arg(m_textEdit->textCursor().position()));
   m_nameFile->setText(tr("%1").arg(m_FileName));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if(obj == m_cursorInformation)
    {
        if(evt->type() == QEvent::MouseButtonDblClick)
        {
            QMouseEvent *event = static_cast<QMouseEvent*>(evt);
            if(event->button() == Qt::LeftButton)
            {
            /*    QMessageBox::about(this, "Summary", tr("Full file path: %1\n"
                                                       "Created: %2\n"
                                                       "LastModified: %3\n"
                                                       "LastRead: %4\n").arg(m_PathFile).arg(QFileInfo(m_PathFile).created().toString().arg(QFileInfo(m_PathFile).lastModified().toString()).arg(QFileInfo(m_PathFile).lastRead().toString()))
                                                        );*/
          /*   QMessageBox::about(this, "Summary", tr("Full file path: %1 \n"
                                                       "Created: %2 \n"
                                                       "LastModified: %3 \n"
                                                       "LastRead: %4 \n").arg(m_PathFile, QFileInfo(m_PathFile).created().toString() ,QFileInfo(m_PathFile).lastModified().toString(), QFileInfo(m_PathFile).lastRead().toString())
                                                                       );*/
                if(m_PathFile.isEmpty()){
                    QMessageBox::about(this, "Summary", tr("Full file path: untitled \n"
                                                                           "Created: 00:00:00 \n"
                                                                           "LastModified: 00:00:00 \n"
                                                                           "LastRead: 00:00:00 \n"))
                                                                                           ;

                }
                else
                {
                    QMessageBox::about(this, "Summary", tr("Full file path: %1 \n"
                                                                           "Created: %2 \n"
                                                                           "LastModified: %3 \n"
                                                                           "LastRead: %4 \n").arg(m_PathFile, QFileInfo(m_PathFile).created().toString() ,QFileInfo(m_PathFile).lastModified().toString(), QFileInfo(m_PathFile).lastRead().toString())
                                                                                           );
                }

            }
        }
    }
   return QMainWindow::eventFilter(obj, evt);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasFormat("text/uri-list"))
    {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl>urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();

    ReadFile(fileName);
}

void MainWindow::setAlwaysOnTop(bool flag)
{
    if(flag)
    {
        setWindowModality(Qt::WindowModal);
    }
    else
    {
        setWindowModality(Qt::NonModal);
    }
}

void MainWindow::showFullScreen()
{
    QMainWindow::showFullScreen();
}

void MainWindow::showNormalScreen()
{
    QMainWindow::showNormal();
}

void MainWindow::setFontSize(int a)
{
    QFont font("MS Shell Dlg", a);
    m_textEdit->setFont(font);
}

void MainWindow::setBoldFont(bool a)
{
    if(a)
    {
        m_textEdit->setFontWeight(99);
    }
    else
    {
        m_textEdit->setFontWeight(50);
    }
}
