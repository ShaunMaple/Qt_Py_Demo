#include <QCoreApplication>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 创建QProcess对象
    QProcess pythonProcess;

    // 设置要运行的Python脚本路径
    QString pythonScriptPath = "py_test.py";

    // 启动Python脚本
    pythonProcess.start("python", QStringList() << pythonScriptPath);

    QTcpSocket socket;
    socket.connectToHost("your_python_server_ip", 5000);  // 请替换为您的Python服务器的IP和端口

    if (socket.waitForConnected())
    {
        // 发送请求
        socket.write("GET /get_landmarks HTTP/1.1\r\n\r\n");
        socket.waitForBytesWritten();

        // 等待并接收响应
        socket.waitForReadyRead();
        QByteArray response = socket.readAll();

        // 解析JSON响应
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray landmarksArray = jsonObject["landmarks"].toArray();

        // 处理关键点坐标
        for (const QJsonValue &landmark : landmarksArray)
        {
            double x = landmark.toArray()[0].toDouble();
            double y = landmark.toArray()[1].toDouble();
            double z = landmark.toArray()[2].toDouble();

            // 在这里使用 x、y、z 处理关键点坐标
        }
    }

    return a.exec();
}
