// main.cpp (updated)
#include <QApplication>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QSplitter>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStatusBar>
#include <QStyleFactory>
#include <QPalette>
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QInputDialog>
#include <QLineEdit>
#include <QProcess>
#include <QDir>

// Include the NekoLang interpreter function
extern "C" {
    void interpret(char *code);  // Declaration of the interpret function from neko.c

    // Implementation of get_user_input
    char* get_user_input(const char *prompt) {
        QString qPrompt = QString::fromUtf8(prompt);
        bool ok;
        QString input = QInputDialog::getText(nullptr, "Input Required", qPrompt, QLineEdit::Normal, "", &ok);

        if (ok && !input.isEmpty()) {
            // Convert QString to char* and allocate memory
            QByteArray inputBytes = input.toUtf8();
            char *inputCStr = (char *)malloc(inputBytes.size() + 1);
            strcpy(inputCStr, inputBytes.constData());
            return inputCStr;
        } else {
            // Return an empty string to indicate cancellation or empty input
            char *emptyStr = (char *)malloc(1);
            emptyStr[0] = '\0';
            return emptyStr;
        }
    }
}

// Syntax Highlighter for NekoLang
class NekoHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    NekoHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        // Define highlighting rules
        HighlightingRule rule;

        // Keywords
        keywordFormat.setForeground(Qt::blue);
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns = {
            "\\bneko\\b", "\\bpurr\\b", "\\bkitten\\b", "\\bmeow\\b",
            "\\bcat\\b", "\\bcatnap\\b", "\\bcatnip\\b", "\\bwhiskers\\b", "\\bpaws\\b",
            "\\bwindow\\b", "\\bbutton\\b", "\\bfunction\\b", "\\bonClick\\b"
        };
        for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        // Comments
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }

protected:
    void highlightBlock(const QString &text) override {
        for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
};

// Code Editor with auto-completion and automatic indentation
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr) : QPlainTextEdit(parent) {
        QStringList keywords = {"neko", "purr", "kitten", "meow", "cat", "catnap", "catnip", "whiskers", "paws", "window", "button", "function", "onClick"};
        completer = new QCompleter(keywords, this);
        completer->setWidget(this);
        completer->setCompletionMode(QCompleter::PopupCompletion);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        connect(completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    }

protected:
    void keyPressEvent(QKeyEvent *e) override {
        if (completer && completer->popup()->isVisible()) {
            // The following keys are forwarded by the completer to the widget
            switch (e->key()) {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Escape:
                case Qt::Key_Tab:
                case Qt::Key_Backtab:
                    e->ignore();
                    return; // let the completer do default behavior
                default:
                    break;
            }
        }

        bool isShortcut = (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space;
        if (!completer || !isShortcut) // do not process the shortcut when we have a completer
            QPlainTextEdit::keyPressEvent(e);

        const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
        if (!completer || (ctrlOrShift && e->text().isEmpty()))
            return;

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
        bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 1
                            || eow.contains(e->text().right(1)))) {
            completer->popup()->hide();
            return;
        }

        if (completionPrefix != completer->completionPrefix()) {
            completer->setCompletionPrefix(completionPrefix);
            completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(completer->popup()->sizeHintForColumn(0)
                    + completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(cr); // popup it up!
    }

private slots:
    void insertCompletion(const QString& completion) {
        if (completer->widget() != this)
            return;
        QTextCursor tc = textCursor();
        int extra = completion.length() - completer->completionPrefix().length();
        tc.movePosition(QTextCursor::Left);
        tc.movePosition(QTextCursor::EndOfWord);
        tc.insertText(completion.right(extra));
        setTextCursor(tc);
    }

private:
    QString textUnderCursor() const {
        QTextCursor tc = textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        return tc.selectedText();
    }

    QCompleter *completer;
};

// Main Window of the IDE
class NekoMainWindow : public QMainWindow {
    Q_OBJECT
public:
    NekoMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("NekoLang IDE");

        // Create the code editor
        codeEditor = new CodeEditor(this);
        codeEditor->setPlainText("neko {\n    // Write your NekoLang code here\n}");
        NekoHighlighter *highlighter = new NekoHighlighter(codeEditor->document());

        // Create the output console
        outputConsole = new QTextEdit(this);
        outputConsole->setReadOnly(true);

        // Splitter to separate code editor and output console
        QSplitter *splitter = new QSplitter(Qt::Vertical, this);
        splitter->addWidget(codeEditor);
        splitter->addWidget(outputConsole);

        setCentralWidget(splitter);

        // Create actions
        QAction *newAction = new QAction("&New", this);
        QAction *openAction = new QAction("&Open", this);
        QAction *saveAction = new QAction("&Save", this);
        QAction *saveAsAction = new QAction("Save &As", this);
        QAction *runAction = new QAction("&Run", this);
        QAction *exitAction = new QAction("E&xit", this);
        QAction *toggleThemeAction = new QAction("&Toggle Theme", this);
        QAction *docAction = new QAction("&Documentation", this);

        // Set shortcuts
        newAction->setShortcut(QKeySequence("Ctrl+N"));
        openAction->setShortcut(QKeySequence("Ctrl+O"));
        saveAction->setShortcut(QKeySequence("Ctrl+S"));
        runAction->setShortcut(QKeySequence("F5"));
        toggleThemeAction->setShortcut(QKeySequence("Ctrl+T"));

        // Create menu bar
        QMenu *fileMenu = menuBar()->addMenu("&File");
        fileMenu->addAction(newAction);
        fileMenu->addAction(openAction);
        fileMenu->addAction(saveAction);
        fileMenu->addAction(saveAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction);

        QMenu *runMenu = menuBar()->addMenu("&Run");
        runMenu->addAction(runAction);

        QMenu *viewMenu = menuBar()->addMenu("&View");
        viewMenu->addAction(toggleThemeAction);

        QMenu *helpMenu = menuBar()->addMenu("&Help");
        helpMenu->addAction(docAction);

        // Create status bar
        statusBar()->showMessage("Ready");

        // Connect actions
        connect(newAction, &QAction::triggered, this, &NekoMainWindow::newFile);
        connect(openAction, &QAction::triggered, this, &NekoMainWindow::openFile);
        connect(saveAction, &QAction::triggered, this, &NekoMainWindow::saveFile);
        connect(saveAsAction, &QAction::triggered, this, &NekoMainWindow::saveFileAs);
        connect(runAction, &QAction::triggered, this, &NekoMainWindow::runCode);
        connect(exitAction, &QAction::triggered, this, &NekoMainWindow::close);
        connect(toggleThemeAction, &QAction::triggered, this, &NekoMainWindow::toggleTheme);
        connect(docAction, &QAction::triggered, this, &NekoMainWindow::showDocumentation);

        currentFilePath = "";

        // Apply dark mode by default
        applyDarkMode();
    }

private slots:
    void newFile() {
        codeEditor->clear();
        currentFilePath = "";
    }

    void openFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open NekoLang File", "", "NekoLang Files (*.neko);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QTextStream in(&file);
                codeEditor->setPlainText(in.readAll());
                currentFilePath = fileName;
            } else {
                QMessageBox::warning(this, "Error", "Could not open file");
            }
        }
    }

    void saveFile() {
        if (currentFilePath.isEmpty()) {
            saveFileAs();
            return;
        }
        QFile file(currentFilePath);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << codeEditor->toPlainText();
        } else {
            QMessageBox::warning(this, "Error", "Could not save file");
        }
    }

    void saveFileAs() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save NekoLang File", "", "NekoLang Files (*.neko);;All Files (*)");
        if (!fileName.isEmpty()) {
            currentFilePath = fileName;
            saveFile();
        }
    }

    void runCode() {
        QString code = codeEditor->toPlainText();

        // Check if the code contains GUI keywords
        bool isGuiApp = code.contains("window") || code.contains("button");

        // Clear the output console
        outputConsole->clear();

        // Convert QString to char*
        QByteArray codeBytes = code.toUtf8();
        char *codeCStr = codeBytes.data();

        // Redirect stdout to capture interpreter output
        FILE *fp = freopen("output.txt", "w", stdout);
        if (!fp) {
            QMessageBox::warning(this, "Error", "Could not redirect stdout");
            return;
        }

        if (isGuiApp) {
            // Save the code to a temporary file
            QString tempFilePath = QDir::temp().filePath("temp_code.neko");
            QFile tempFile(tempFilePath);
            if (tempFile.open(QFile::WriteOnly | QFile::Text)) {
                QTextStream out(&tempFile);
                out << code;
                tempFile.close();
            } else {
                QMessageBox::warning(this, "Error", "Could not write temporary code file");
                return;
            }

            // Run the interpreter as a separate process
            QProcess *process = new QProcess(this);
            QString interpreterPath = QCoreApplication::applicationDirPath() + "/neko"; // Path to the interpreter executable
            process->setProgram(interpreterPath);
            process->setArguments(QStringList() << tempFilePath);
            process->start();

            if (!process->waitForStarted()) {
                QMessageBox::warning(this, "Error", "Could not start NekoLang interpreter");
                return;
            }

            // Wait for the process to finish
            process->waitForFinished(-1);

            // Read the output
            QString output = process->readAllStandardOutput();
            outputConsole->setPlainText(output);

            // Handle errors
            QString errorOutput = process->readAllStandardError();
            if (!errorOutput.isEmpty()) {
                QMessageBox::warning(this, "Interpreter Error", errorOutput);
            }

            // Clean up
            tempFile.remove();
        } else {
            // For console applications, run directly
            try {
                interpret(codeCStr);
            } catch (...) {
                outputConsole->setPlainText("Error: NekoLang interpreter crashed.");
            }
        }

        // Restore stdout
        fflush(stdout);
#ifdef _WIN32
        freopen("CONOUT$", "w", stdout);
#else
        freopen("/dev/tty", "w", stdout);
#endif

        // Read output from file
        QFile outputFile("output.txt");
        if (outputFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&outputFile);
            outputConsole->setPlainText(outputConsole->toPlainText() + in.readAll());
            outputFile.close();
            outputFile.remove();  // Clean up
        } else {
            QMessageBox::warning(this, "Error", "Could not read output");
        }
    }

    void toggleTheme() {
        static bool darkMode = true;
        darkMode = !darkMode;
        if (darkMode) {
            applyDarkMode();
        } else {
            applyLightMode();
        }
    }

    void showDocumentation() {
        QString documentation = "NekoLang Documentation:\n\n"
                                "Keywords:\n"
                                "  - neko: Start a NekoLang block\n"
                                "  - purr: Print a message\n"
                                "  - kitten: Define a variable\n"
                                "  - meow: User input\n"
                                "  - cat: If statement\n"
                                "  - catnap: Else statement\n"
                                "  - catnip: Elif statement\n"
                                "  - whiskers: While loop\n"
                                "  - paws: For loop\n"
                                "  - window: Create a window\n"
                                "  - button: Create a button\n"
                                "  - function: Define a function\n"
                                "  - onClick: Event handler for clicks\n\n"
                                "Example GUI Application:\n"
                                "  neko {\n"
                                "      window mainWindow \"My App\"\n"
                                "      button myButton \"Click Me\" mainWindow onClick handleClick\n\n"
                                "      function handleClick() {\n"
                                "          purr \"Button was clicked\"\n"
                                "      }\n"
                                "  }";
        QMessageBox::information(this, "NekoLang Documentation", documentation);
    }

private:
    void applyDarkMode() {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(darkPalette);
    }

    void applyLightMode() {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        lightPalette.setColor(QPalette::Base, Qt::white);
        lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::ToolTipBase, Qt::black);
        lightPalette.setColor(QPalette::ToolTipText, Qt::black);
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::ButtonText, Qt::black);
        lightPalette.setColor(QPalette::BrightText, Qt::red);
        lightPalette.setColor(QPalette::Link, QColor(0, 0, 255));
        lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
        lightPalette.setColor(QPalette::HighlightedText, Qt::white);
        qApp->setPalette(lightPalette);
    }

    CodeEditor *codeEditor;
    QTextEdit *outputConsole;
    QString currentFilePath;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    NekoMainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
