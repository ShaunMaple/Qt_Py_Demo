#include "mainwindow.h"
#include <QProcess>
#include "ui_mainwindow.h"
#include <qDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the button click to the slot
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(startFaceMeshDetection()));
     qDebug() << "1" ;
}

void MainWindow::startFaceMeshDetection()
{
    // Path to the Python script
    QString pythonScriptPath = "py_test.py";
    qDebug() << "2" ;
    // Create a QProcess to run the Python script
    QProcess *pythonProcess = new QProcess(this);
    qDebug() << "3" ;
    // Set the command to run (python and the script path)
    pythonProcess->start("python", QStringList() << pythonScriptPath);
    qDebug() << "4" ;
    // Connect signals and slots to handle output if needed
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::handlePythonOutput);
    connect(pythonProcess, &QProcess::readyReadStandardError, this, &MainWindow::handlePythonError);
    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::handlePythonFinished);
    qDebug() << "5" ;
}

void MainWindow::handlePythonOutput()
{
    // Handle standard output from the Python script
    QString output = pythonProcess->readAllStandardOutput();
    qDebug() << "Python Output:" << output;
}

void MainWindow::handlePythonError()
{
    // Handle standard error from the Python script
    QString error = pythonProcess->readAllStandardError();
    qDebug() << "Python Error:" << error;
}

void MainWindow::handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // Handle Python script finishing
    qDebug() << "Python script finished with exit code:" << exitCode << " Exit status:" << exitStatus;
}

MainWindow::~MainWindow()
{
    delete ui;
}

