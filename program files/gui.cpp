// gui.cpp - Qt Widgets front-end for the Vole Machine Simulator (FCAI Cairo University)
// Build with Qt 5/6 Widgets together with: Muhammad.cpp Eslam.cpp Nabil.cpp Abdelrahman.cpp gui.cpp

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QFont>
#include <QString>
#include <QStringList>

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

// Project headers (order matters: they rely on each other's declarations)
#include "Muhammad.h"
#include "Eslam.h"
#include "Abdelrahman.h"
#include "Nabil.h"

// ------------------------------------------------------------------
//  Helpers
// ------------------------------------------------------------------
static QString hx(int v, int width = 2)
{
    return QString::number(v & ((width == 2) ? 0xFF : (width == 4 ? 0xFFFF : 0xF)), 16)
        .toUpper().rightJustified(width, '0');
}

static QString hx1(int v) { return QString::number(v & 0xF, 16).toUpper(); }

// Short comment shown in the "Source & Debugger" panel
static QString shortComment(int hi, int lo)
{
    int op = hi >> 4, r = hi & 0xF, s = lo >> 4, t = lo & 0xF, xy = lo;
    switch (op) {
    case 0x1: return QString("Load reg %1 with mem[%2]").arg(hx1(r), hx(xy));
    case 0x2: return QString("Load reg %1 with %2").arg(hx1(r), hx(xy));
    case 0x3: return (xy == 0xFF) ? QString("Store R%1 to Screen").arg(hx1(r))
                                  : QString("Store R%1 to mem[%2]").arg(hx1(r), hx(xy));
    case 0x4: return QString("Move R%1 -> R%2").arg(hx1(s), hx1(t));
    case 0x5: return QString("Add R%1 + R%2 -> R%3").arg(hx1(s), hx1(t), hx1(r));
    case 0x6: return QString("Add (float) R%1 + R%2 -> R%3").arg(hx1(s), hx1(t), hx1(r));
    case 0x7: return QString("R%1 OR R%2 -> R%3").arg(hx1(s), hx1(t), hx1(r));
    case 0x8: return QString("R%1 AND R%2 -> R%3").arg(hx1(s), hx1(t), hx1(r));
    case 0x9: return QString("R%1 XOR R%2 -> R%3").arg(hx1(s), hx1(t), hx1(r));
    case 0xA: return QString("Rotate R%1 right by %2").arg(hx1(r), hx1(t));
    case 0xB: return QString("Jump to %1 if R%2 == R0").arg(hx(xy), hx1(r));
    case 0xC: return QString("Halt execution");
    default:  return QString("Unknown opcode (halts)");
    }
}

// Long description used by the Output Log
static QString longDescription(int hi, int lo)
{
    int op = hi >> 4, r = hi & 0xF, s = lo >> 4, t = lo & 0xF, xy = lo;
    switch (op) {
    case 0x1: return QString("Load memory value at %1 into Register %2.").arg(hx(xy), hx1(r));
    case 0x2: return QString("Load immediate value %1 into Register %2.").arg(hx(xy), hx1(r));
    case 0x3: return (xy == 0xFF) ? QString("Store Register %1 to the Screen.").arg(hx1(r))
                                  : QString("Store Register %1 into memory at %2.").arg(hx1(r), hx(xy));
    case 0x4: return QString("Move Register %1 into Register %2.").arg(hx1(s), hx1(t));
    case 0x5: return QString("Add R%1 + R%2 (two's complement) into R%3.").arg(hx1(s), hx1(t), hx1(r));
    case 0x6: return QString("Add R%1 + R%2 (floating point) into R%3.").arg(hx1(s), hx1(t), hx1(r));
    case 0x7: return QString("Bitwise OR of R%1 and R%2 into R%3.").arg(hx1(s), hx1(t), hx1(r));
    case 0x8: return QString("Bitwise AND of R%1 and R%2 into R%3.").arg(hx1(s), hx1(t), hx1(r));
    case 0x9: return QString("Bitwise XOR of R%1 and R%2 into R%3.").arg(hx1(s), hx1(t), hx1(r));
    case 0xA: return QString("Rotate Register %1 right by %2 step(s).").arg(hx1(r), hx1(t));
    case 0xB: return QString("Jump to %1 if Register %2 equals Register 0.").arg(hx(xy), hx1(r));
    case 0xC: return QString("Halt execution.");
    default:  return QString("Unknown opcode - machine halted.");
    }
}

// ------------------------------------------------------------------
//  Machine: owns every simulator component and wires them together
// ------------------------------------------------------------------
struct Machine
{
    programCounter      pc;
    InstructionRegister ir;
    Memory              mem;
    registers           reg;
    ALU                 alu;
    Screen              screen;
    CU                  cu;
    CPU                 cpu;
    voleMachine         vm;

    Machine()
        : cu(&pc, &ir, &mem, &reg, &alu, &screen),
          cpu(&reg, &pc, &ir, &alu, &cu),
          vm(&mem, &cpu, &screen)
    {
        pc.set(0);
    }
};

// ------------------------------------------------------------------
//  Main window
// ------------------------------------------------------------------
class MainWindow : public QMainWindow
{
public:
    MainWindow()
    {
        setWindowTitle("Vole Machine Simulator (FCAI Cairo University)");
        m_machine.reset(new Machine());
        buildUi();
        applyStyle();
        setStatus("READY");
        refresh();
    }

private:
    // --- simulator state
    std::unique_ptr<Machine> m_machine;
    int     m_programEnd = 0;      // number of bytes occupied by the loaded program
    int     m_steps = 0;
    bool    m_halted = false;
    QString m_irText = "0000";
    QString m_lastScreenToken;
    QString m_screenText;

    // --- widgets
    QLabel *m_pcValue = nullptr, *m_irValue = nullptr, *m_status = nullptr;
    QLabel *m_regLabels[16] = {};
    QLabel *m_cells[256] = {};
    int     m_cellState[256];
    QTextBrowser   *m_source = nullptr;
    QPlainTextEdit *m_console = nullptr, *m_log = nullptr;

    // ---------------------------------------------------------------
    QFrame* makePanel(const QString& title, QWidget* body)
    {
        auto* panel = new QFrame;
        panel->setObjectName("panel");
        auto* lay = new QVBoxLayout(panel);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->setSpacing(0);

        auto* head = new QLabel(title);
        head->setObjectName("panelTitle");
        lay->addWidget(head);

        auto* line = new QFrame;
        line->setObjectName("panelLine");
        line->setFixedHeight(1);
        lay->addWidget(line);

        body->setParent(panel);
        lay->addWidget(body, 1);
        return panel;
    }

    QPushButton* makeButton(const QString& text, const QString& kind)
    {
        auto* b = new QPushButton(text);
        b->setObjectName(kind);
        b->setCursor(Qt::PointingHandCursor);
        b->setMinimumHeight(34);
        return b;
    }

    QFrame* makeInfoBox(const QString& name, QLabel*& valueLabel)
    {
        auto* box = new QFrame;
        box->setObjectName("infoBox");
        auto* l = new QHBoxLayout(box);
        l->setContentsMargins(14, 6, 14, 6);
        auto* n = new QLabel(name);
        n->setObjectName("infoName");
        valueLabel = new QLabel("[00]");
        valueLabel->setObjectName("infoValue");
        l->addWidget(n);
        l->addStretch(1);
        l->addWidget(valueLabel);
        return box;
    }

    void buildUi()
    {
        // ---------- menu bar
        auto* fileMenu = menuBar()->addMenu("File");
        auto* actLoad  = fileMenu->addAction("Load File");
        fileMenu->addSeparator();
        auto* actExit  = fileMenu->addAction("Exit");

        auto* editMenu = menuBar()->addMenu("Edit");
        auto* actClear = editMenu->addAction("Clear Memory");

        menuBar()->addMenu("View");

        auto* simMenu  = menuBar()->addMenu("Simulator");
        auto* actRun   = simMenu->addAction("Run All");
        auto* actStep  = simMenu->addAction("Single Step");

        menuBar()->addMenu("Tools");

        auto* helpMenu = menuBar()->addMenu("Help");
        auto* actHelp  = helpMenu->addAction("Instruction Set");

        // ---------- central widget
        auto* central = new QWidget;
        setCentralWidget(central);
        auto* root = new QVBoxLayout(central);
        root->setContentsMargins(10, 8, 10, 10);
        root->setSpacing(10);

        // ---------- toolbar row
        auto* bar = new QHBoxLayout;
        bar->setSpacing(8);
        auto* btnLoad  = makeButton("\xF0\x9F\x93\x82  Load File", "btnBlue");
        auto* btnClear = makeButton("\xF0\x9F\xA7\xB9  Clear Memory", "btnBlue");
        auto* sep = new QFrame;
        sep->setObjectName("vsep");
        sep->setFixedWidth(1);
        sep->setMinimumHeight(28);
        auto* btnRun   = makeButton("\xE2\x96\xB6  Run All", "btnGreen");
        auto* btnStep  = makeButton("\xE2\x8F\xAD  Single Step", "btnBlue");
        auto* btnHelp  = makeButton("\xE2\x84\xB9  Help", "btnBlue");
        btnLoad->setText(QString::fromUtf8("\xF0\x9F\x93\x82  Load File"));
        btnClear->setText(QString::fromUtf8("\xF0\x9F\xA7\xB9  Clear Memory"));
        btnRun->setText(QString::fromUtf8("\xE2\x96\xB6  Run All"));
        btnStep->setText(QString::fromUtf8("\xE2\x8F\xAD  Single Step"));
        btnHelp->setText(QString::fromUtf8("\xE2\x84\xB9  Help"));

        m_status = new QLabel("STATUS: READY");
        m_status->setObjectName("status");
        m_status->setAlignment(Qt::AlignCenter);
        m_status->setMinimumHeight(38);
        m_status->setMinimumWidth(190);

        bar->addWidget(btnLoad);
        bar->addWidget(btnClear);
        bar->addSpacing(4);
        bar->addWidget(sep);
        bar->addSpacing(4);
        bar->addWidget(btnRun);
        bar->addWidget(btnStep);
        bar->addWidget(btnHelp);
        bar->addStretch(1);
        bar->addWidget(m_status);
        root->addLayout(bar);

        // ---------- CPU state panel
        auto* cpuBody = new QWidget;
        auto* cpuLay = new QVBoxLayout(cpuBody);
        cpuLay->setContentsMargins(12, 10, 12, 12);
        cpuLay->setSpacing(8);
        cpuLay->addWidget(makeInfoBox("Program Counter (PC)", m_pcValue));
        cpuLay->addWidget(makeInfoBox("Instruction Register (IR)", m_irValue));
        auto* hline = new QFrame;
        hline->setObjectName("panelLine");
        hline->setFixedHeight(1);
        cpuLay->addWidget(hline);

        auto* regGrid = new QGridLayout;
        regGrid->setHorizontalSpacing(8);
        regGrid->setVerticalSpacing(8);
        for (int i = 0; i < 16; ++i) {
            auto* l = new QLabel;
            l->setObjectName("reg");
            l->setAlignment(Qt::AlignCenter);
            l->setMinimumHeight(38);
            m_regLabels[i] = l;
            regGrid->addWidget(l, i % 8, i / 8);
        }
        cpuLay->addLayout(regGrid, 1);

        // ---------- source & debugger
        m_source = new QTextBrowser;
        m_source->setObjectName("source");
        m_source->setFrameShape(QFrame::NoFrame);
        m_source->setOpenLinks(false);

        // ---------- console
        m_console = new QPlainTextEdit;
        m_console->setObjectName("console");
        m_console->setReadOnly(true);
        m_console->setFrameShape(QFrame::NoFrame);

        // ---------- output log
        m_log = new QPlainTextEdit;
        m_log->setObjectName("log");
        m_log->setReadOnly(true);
        m_log->setFrameShape(QFrame::NoFrame);

        // ---------- memory viewer
        auto* memBody = new QWidget;
        auto* memGrid = new QGridLayout(memBody);
        memGrid->setContentsMargins(10, 10, 10, 10);
        memGrid->setSpacing(3);
        for (int c = 0; c < 16; ++c) {
            auto* h = new QLabel(hx1(c));
            h->setObjectName("memHead");
            h->setAlignment(Qt::AlignCenter);
            memGrid->addWidget(h, 0, c + 1);
        }
        for (int r = 0; r < 16; ++r) {
            auto* h = new QLabel(hx1(r));
            h->setObjectName("memHead");
            h->setAlignment(Qt::AlignCenter);
            memGrid->addWidget(h, r + 1, 0);
            for (int c = 0; c < 16; ++c) {
                int idx = r * 16 + c;
                auto* cell = new QLabel("00");
                cell->setAlignment(Qt::AlignCenter);
                cell->setMinimumSize(30, 26);
                cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                m_cells[idx] = cell;
                m_cellState[idx] = -1;
                memGrid->addWidget(cell, r + 1, c + 1);
            }
        }
        for (int i = 0; i < 17; ++i) {
            memGrid->setRowStretch(i, i == 0 ? 0 : 1);
            memGrid->setColumnStretch(i, i == 0 ? 0 : 1);
        }

        // ---------- main grid
        auto* grid = new QGridLayout;
        grid->setHorizontalSpacing(10);
        grid->setVerticalSpacing(10);
        grid->addWidget(makePanel("CPU State", cpuBody),               0, 0, 2, 1);
        grid->addWidget(makePanel("Source & Debugger", m_source),      0, 1);
        grid->addWidget(makePanel("Console & Screen Output", m_console), 1, 1);
        grid->addWidget(makePanel("Output Log", m_log),                2, 0, 1, 2);
        grid->addWidget(makePanel("Main Memory Viewer", memBody),      0, 2, 3, 1);
        grid->setColumnStretch(0, 27);
        grid->setColumnStretch(1, 30);
        grid->setColumnStretch(2, 43);
        grid->setRowStretch(0, 52);
        grid->setRowStretch(1, 30);
        grid->setRowStretch(2, 18);
        root->addLayout(grid, 1);

        // ---------- connections
        auto doLoad  = [this]() { onLoad(); };
        auto doClear = [this]() { onClear(); };
        auto doRun   = [this]() { onRunAll(); };
        auto doStep  = [this]() { onStep(); };
        auto doHelp  = [this]() { onHelp(); };

        connect(btnLoad,  &QPushButton::clicked, this, doLoad);
        connect(btnClear, &QPushButton::clicked, this, doClear);
        connect(btnRun,   &QPushButton::clicked, this, doRun);
        connect(btnStep,  &QPushButton::clicked, this, doStep);
        connect(btnHelp,  &QPushButton::clicked, this, doHelp);
        connect(actLoad,  &QAction::triggered,   this, doLoad);
        connect(actClear, &QAction::triggered,   this, doClear);
        connect(actRun,   &QAction::triggered,   this, doRun);
        connect(actStep,  &QAction::triggered,   this, doStep);
        connect(actHelp,  &QAction::triggered,   this, doHelp);
        connect(actExit,  &QAction::triggered,   this, &QWidget::close);
    }

    // ---------------------------------------------------------------
    void applyStyle()
    {
        qApp->setStyle("Fusion");
        setStyleSheet(R"(
            QMainWindow, QWidget#centralwidget { background: #0a0f17; }
            QWidget { color: #d5dde8; font-family: "Segoe UI", "Inter", "Noto Sans", sans-serif; font-size: 14px; }
            QMainWindow > QWidget { background: #0a0f17; }

            QMenuBar { background: #0d1520; color: #e1e8f0; padding: 3px 6px; font-size: 15px; }
            QMenuBar::item { padding: 5px 12px; background: transparent; }
            QMenuBar::item:selected { background: #1a2a3d; border-radius: 4px; }
            QMenu { background: #101b29; border: 1px solid #243750; color: #e1e8f0; }
            QMenu::item { padding: 6px 26px; }
            QMenu::item:selected { background: #1d3550; }

            QPushButton { border-radius: 6px; padding: 4px 14px; font-size: 17px; }
            QPushButton#btnBlue  { background: #14253a; border: 1px solid #28445f; color: #58b6ff; }
            QPushButton#btnBlue:hover  { background: #1a3350; }
            QPushButton#btnGreen { background: #0f2f2e; border: 1px solid #1f7d62; color: #34e0a1; }
            QPushButton#btnGreen:hover { background: #134240; }
            QPushButton:pressed { background: #0b1a2a; }
            QFrame#vsep { background: #243447; }

            QLabel#status { background: #1b1408; border: 2px solid #f0961c; border-radius: 8px;
                            color: #ffa62b; font-size: 21px; padding: 0 14px; }

            QFrame#panel { background: #0f1722; border: 1px solid #1d2a3b; border-radius: 8px; }
            QLabel#panelTitle { background: transparent; color: #e2e9f2; font-size: 21px; padding: 8px 12px 6px 12px; border: none; }
            QFrame#panelLine { background: #1d2a3b; border: none; }

            QFrame#infoBox { background: #0b1b2d; border: 2px solid #1a9bff; border-radius: 9px; }
            QLabel#infoName { color: #8fd0ff; font-size: 19px; background: transparent; border: none; }
            QLabel#infoValue { color: #8fd0ff; font-size: 19px; background: transparent; border: none;
                               font-family: "Consolas", "Cascadia Mono", "DejaVu Sans Mono", monospace; }

            QLabel#reg { background: #0e1620; border: 1px solid #27374b; border-radius: 4px;
                         color: #dbe3ec; font-size: 21px;
                         font-family: "Consolas", "Cascadia Mono", "DejaVu Sans Mono", monospace; }

            QTextBrowser#source, QPlainTextEdit#console, QPlainTextEdit#log {
                background: #0a1018; border: none; margin: 6px 8px 8px 8px;
                font-family: "Consolas", "Cascadia Mono", "DejaVu Sans Mono", monospace; font-size: 20px; color: #d3dbe6;
            }
            QPlainTextEdit#log { font-size: 19px; }
            QPlainTextEdit#console { font-size: 21px; }

            QLabel#memHead { color: #d9e1ea; font-size: 20px; background: transparent; border: none; }

            QScrollBar:vertical { background: #0a1018; width: 8px; }
            QScrollBar::handle:vertical { background: #2a3b50; border-radius: 4px; min-height: 20px; }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        )");
        QFont mono("Consolas");
        mono.setStyleHint(QFont::Monospace);
        m_source->setFont(mono);
    }

    void setStatus(const QString& s)
    {
        m_status->setText("STATUS: " + s);
        QString color = "#ffa62b", border = "#f0961c", bg = "#1b1408";
        if (s == "READY")        { color = "#58b6ff"; border = "#1a9bff"; bg = "#0b1b2d"; }
        else if (s == "RUNNING") { color = "#34e0a1"; border = "#1f7d62"; bg = "#0f2f2e"; }
        else if (s == "HALTED")  { color = "#ff6b6b"; border = "#c24040"; bg = "#2a1010"; }
        m_status->setStyleSheet(QString("QLabel#status { background:%1; border:2px solid %2; "
                                        "border-radius:8px; color:%3; font-size:21px; padding:0 14px; }")
                                    .arg(bg, border, color));
    }

    void log(const QString& s) { m_log->appendPlainText(s); }

    // ---------------------------------------------------------------
    //  Simulator control
    // ---------------------------------------------------------------
    void resetMachine()
    {
        m_machine.reset(new Machine());
        m_programEnd = 0;
        m_steps = 0;
        m_halted = false;
        m_irText = "0000";
        m_lastScreenToken.clear();
        m_screenText.clear();
        for (int i = 0; i < 256; ++i) m_cellState[i] = -1;
    }

    void onLoad()
    {
        QString path = QFileDialog::getOpenFileName(this, "Load Vole Program", QString(),
                                                    "Text Files (*.txt);;All Files (*)");
        if (path.isEmpty()) return;

        resetMachine();
        m_log->clear();
        m_machine->vm.load_program_from_file(path.toStdString(), 0);

        // find the end of the program (last non-zero byte, rounded up to an even size)
        int last = -1;
        for (int i = 0; i < 256; ++i)
            if (m_machine->mem.read(i) != 0) last = i;
        m_programEnd = (last < 0) ? 0 : ((last / 2) + 1) * 2;

        log(QString("Program Loaded Successfully (%1 instructions).").arg(m_programEnd / 2));
        setStatus("READY");
        refresh();
    }

    void onClear()
    {
        resetMachine();
        m_log->clear();
        log("Memory, registers and screen cleared.");
        setStatus("READY");
        refresh();
    }

    // executes one instruction; returns false if nothing could be executed
    bool stepOnce(bool verbose)
    {
        if (m_halted || m_machine->cu.is_halted()) {
            m_halted = true;
            return false;
        }
        int pc = m_machine->pc.get();
        if (pc < 0 || pc > 254) {
            log(QString("[ERROR] PC (%1) is outside memory. Execution stopped.").arg(pc));
            m_halted = true;
            return false;
        }
        int hi = m_machine->mem.read(pc) & 0xFF;
        int lo = m_machine->mem.read(pc + 1) & 0xFF;
        m_irText = hx(hi) + hx(lo);

        m_machine->cpu.step();
        ++m_steps;

        if (m_machine->cu.is_halted()) m_halted = true;

        log(QString("[STEP %1] Executed %2: %3").arg(m_steps).arg(m_irText, longDescription(hi, lo)));
        if (verbose && !m_halted)
            log(QString("[STEP %1] PC updated to %2. Awaiting next step...")
                    .arg(m_steps).arg(hx(m_machine->pc.get())));
        return true;
    }

    void onStep()
    {
        if (!stepOnce(true)) {
            log("Machine is halted. Load a program or clear memory to start again.");
            setStatus("HALTED");
            return;
        }
        if (m_halted) {
            log("Program halted.");
            setStatus("HALTED");
        } else {
            setStatus("STEPPING");
        }
        refresh();
    }

    void onRunAll()
    {
        if (m_halted) {
            log("Machine is halted. Load a program or clear memory to start again.");
            return;
        }
        setStatus("RUNNING");
        const int maxSteps = 100000;
        int count = 0;
        while (!m_halted && count < maxSteps) {
            if (!stepOnce(false)) break;
            ++count;
        }
        if (!m_halted) log("Stopped: step limit reached (possible infinite loop).");
        else           log("Program halted.");
        setStatus("HALTED");
        refresh();
    }

    void onHelp()
    {
        QMessageBox::information(this, "Vole Machine - Instruction Set",
            "1RXY  LOAD  R with memory[XY]\n"
            "2RXY  LOAD  R with the value XY\n"
            "3RXY  STORE R into memory[XY]   (XY = FF writes to the screen)\n"
            "40RS  MOVE  R(R) -> R(S)\n"
            "5RST  ADD   R(S) + R(T) -> R(R)   (two's complement)\n"
            "6RST  ADD   R(S) + R(T) -> R(R)   (floating point)\n"
            "7RST  OR    8RST  AND    9RST  XOR\n"
            "AR0X  ROTATE R right by X steps\n"
            "BRXY  JUMP  to XY if R(R) == R(0)\n"
            "C000  HALT");
    }

    // ---------------------------------------------------------------
    //  Refreshing the widgets
    // ---------------------------------------------------------------
    void captureScreen()
    {
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        m_machine->screen.display();
        std::cout.rdbuf(old);

        m_screenText = QString::fromStdString(oss.str()).trimmed();
        QStringList toks = m_screenText.split(' ', Qt::SkipEmptyParts);
        m_lastScreenToken = toks.isEmpty() ? QString() : toks.last();
    }

    void refresh()
    {
        captureScreen();

        // PC / IR
        m_pcValue->setText("[" + hx(m_machine->pc.get()) + "]");
        m_irValue->setText("[" + m_irText + "]");

        // registers
        for (int i = 0; i < 16; ++i)
            m_regLabels[i]->setText(QString("R%1:   %2").arg(hx1(i), hx(m_machine->reg.read(i))));

        refreshMemory();
        refreshSource();

        m_console->setPlainText("Screen Output (Mem FF): " +
                                (m_lastScreenToken.isEmpty() ? QString("--") : m_lastScreenToken));
    }

    void refreshMemory()
    {
        static const QString mono = "font-family:'Consolas','Cascadia Mono','DejaVu Sans Mono',monospace;font-size:20px;";
        for (int i = 0; i < 256; ++i) {
            int v = m_machine->mem.read(i) & 0xFF;
            QString text = hx(v);
            int state = 0; // 0 = normal, 1 = program byte, 2 = screen cell (FF)

            if (i < m_programEnd) state = 1;
            if (i == 0xFF && !m_lastScreenToken.isEmpty()) {
                state = 2;
                text = m_lastScreenToken.rightJustified(2, '0');
            }
            int key = state * 100000 + v * 10 + (state == 2 ? text.toInt() % 7 : 0);
            if (m_cellState[i] == key && m_cells[i]->text() == text) continue;
            m_cellState[i] = key;

            QString style;
            if (state == 1)
                style = "background:#0e4660;border:1px solid #1ec8ff;border-radius:4px;color:#c8f6ff;";
            else if (state == 2)
                style = "background:#2a1d0c;border:1px dashed #ff9f1a;border-radius:4px;color:#ffb84d;";
            else
                style = QString("background:#111a26;border:1px solid #1b2837;border-radius:4px;color:%1;")
                            .arg(v ? "#aab7c6" : "#5f6e80");
            m_cells[i]->setStyleSheet(style + mono);
            m_cells[i]->setText(text);
        }
    }

    void refreshSource()
    {
        int pc = m_machine->pc.get();
        QString html =
            "<html><body style=\"background:#0a1018;\">"
            "<table cellspacing='0' cellpadding='3' width='100%' "
            "style=\"font-family:Consolas,'Cascadia Mono','DejaVu Sans Mono',monospace;font-size:20px;\">";
        if (m_programEnd == 0) {
            html += "<tr><td style='color:#5f6e80'>No program loaded.</td></tr>";
        }
        for (int a = 0; a < m_programEnd; a += 2) {
            int hi = m_machine->mem.read(a) & 0xFF;
            int lo = m_machine->mem.read(a + 1) & 0xFF;
            bool cur = (a == pc && !m_halted);
            html += cur ? "<tr bgcolor='#161f2c'>" : "<tr>";
            html += QString("<td width='24' style='color:#d3dbe6'>%1</td>").arg(cur ? "&rarr;" : "");
            html += QString("<td style='color:#6f7d8f'>%1:</td>").arg(hx(a));
            html += QString("<td style='color:#f5c542'>%1%2</td>").arg(hx(hi), hx(lo));
            html += QString("<td style='color:#c3ccd8'>&nbsp;;&nbsp;%1</td>").arg(shortComment(hi, lo).toHtmlEscaped());
            html += "</tr>";
        }
        html += "</table></body></html>";

        int scroll = m_source->verticalScrollBar()->value();
        m_source->setHtml(html);
        m_source->verticalScrollBar()->setValue(scroll);
    }
};

// ------------------------------------------------------------------
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1288, 720);
    w.show();
    return app.exec();
}