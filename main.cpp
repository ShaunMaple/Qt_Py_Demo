#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include "D:\Anaconda3\envs\py37\include\Python.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //设置环境变量
    Py_SetPythonHome((const wchar_t *)(L"D:/Anaconda3/envs/py37"));

    //初始化python
    //初始化python模块
    Py_Initialize();
    if ( !Py_IsInitialized() ){return -1;}

    //导入py_test.py模块
    //此处需要替换python文件名，不需要py
    PyObject* pModule = PyImport_ImportModule("py_test");
    if (!pModule) {
        qDebug()<<"Cant open python file!";
        return -1;
       }


    // 创建QProcess对象
    QProcess pythonProcess;

    // 设置要运行的Python脚本路径
    QString pythonScriptPath = "py_test.py";

    // 启动Python脚本
    pythonProcess.start("python", QStringList() << pythonScriptPath);

    // 初始化坐标和计数器
    double totalX = 0.0, totalY = 0.0, totalZ = 0.0;
    int count = 0;

    // 连接readyReadStandardOutput信号到槽函数，以实时处理Python脚本的输出
    QObject::connect(&pythonProcess, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray output = pythonProcess.readAllStandardOutput();
        QTextStream stream(output);
        QString line;
        while (stream.readLineInto(&line)) {
            // 在这里处理每一行输出，将坐标信息添加到计数器中
            QStringList coordinates = line.split(" ");
            if (coordinates.size() == 3) {
                totalX += coordinates[0].toDouble();
                totalY += coordinates[1].toDouble();
                totalZ += coordinates[2].toDouble();
                count++;
            }
        }
    });

    // 等待Python脚本执行完毕
    pythonProcess.waitForFinished();

    // 输出坐标的平均值
    if (count > 0) {
        double averageX = totalX / count;
        double averageY = totalY / count;
        double averageZ = totalZ / count;
        qDebug() << "Average Coordinates:" << "X=" << averageX << "Y=" << averageY << "Z=" << averageZ;
    } else {
        qDebug() << "No coordinates received from Python.";
    }

    return a.exec();
}
