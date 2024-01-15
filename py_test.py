# This Python file uses the following encoding: utf-8

import cv2
import mediapipe as mp
from flask import Flask, jsonify

# 初始化MediaPipe Face Mesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh()

# 初始化摄像头
cap = cv2.VideoCapture(0)  # 0表示默认摄像头，可以根据需要更改

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # 将图像传递给MediaPipe Face Mesh进行检测
    result = face_mesh.process(frame)

    # 如果检测到面部关键点，则进行处理
    if result.multi_face_landmarks:
        for face_landmarks in result.multi_face_landmarks:
            # 在这里处理面部关键点，输出关键点的坐标
            for landmark in face_landmarks.landmark:
                x, y, z = landmark.x, landmark.y, landmark.z
                print(f"Landmark coordinates: X={x}, Y={y}, Z={z}")

    # 在图像上绘制Face Mesh检测结果
    if result.multi_face_landmarks:
        mp.solutions.drawing_utils.draw_landmarks(frame, face_landmarks, mp_face_mesh.FACEMESH_TESSELATION)

    # 显示结果
    cv2.imshow('Face Mesh Detection', frame)

    # 按'q'键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
app = Flask(__name__)

@app.route('/get_landmarks', methods=['GET'])
def get_landmarks():
    # 您获取关键点坐标的现有代码
    landmarks = [(x, y, z) for landmark in face_landmarks.landmark]
    return jsonify({'landmarks': landmarks})
# 释放资源
cap.release()
cv2.destroyAllWindows()

