import cv2
import mediapipe as mp

mp_hand = mp.solutions.hands
hands = mp_hand.Hands()
mp_drawing_utils = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

cap = cv2.VideoCapture(0)

while True:
    success, img = cap.read()
    if not success:
        break
    result = hands.process(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    if result.multi_hand_landmarks:
        for hand_landmark in result.multi_hand_landmarks:
            mp_drawing_utils.draw_landmarks(
                img,
                hand_landmark,
                mp_hand.HAND_CONNECTIONS,
                mp_drawing_styles.get_default_hand_landmarks_style(),
                mp_drawing_styles.get_default_hand_connections_style()
            )
        for id_landmark, landmark in enumerate(hand_landmark.landmark):
            print(id_landmark, landmark)
            if id_landmark == 0:
                h, w, c = img.shape
                cx_0, cy_0 = int(landmark.x*w), int(landmark.y*h)
            elif id_landmark == 1:
                h, w, c = img.shape
                cx_1, cy_1 = int(landmark.x*w), int(landmark.y*h)
            elif id_landmark == 9:
                h, w, c = img.shape
                cx_9, cy_9 = int(landmark.x * w), int(landmark.y * h)
            elif id_landmark == 17:
                h, w, c = img.shape
                cx_17, cy_17 = int(landmark.x * w), int(landmark.y * h)

        cv2.line(img, (cx_0, cy_0), (cx_9, cy_9), (255, 0, 0), 5)
        cv2.line(img, (cx_1, cy_1), (cx_17, cy_17), (255, 0, 0), 5)



    # print(result.multi_hand_landmarks)
    cv2.imshow("Image", img)
    cv2.waitKey(1)

cap.release()
cv2.destroyAllWindows()
