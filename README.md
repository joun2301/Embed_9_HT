# 🎬 Home Sweet Theater

> **경험을 극대화하는 나만의 홈시어터**  
> **Home Sweet Theater**는 사용자에게 **몰입감 넘치는 시청 환경**을 제공하는 프로젝트입니다.  
> 집에서도 영화관처럼, 더 편안하고 더 생동감 있게 즐겨보세요!

---

## ✨ 주요 기능

### 🛋 리클라이너
- **서보 모터**로 구현한 움직이는 리클라이너  
- **버튼**을 눌러 체형에 딱 맞는 각도로 조절해보세요!

### 💡 조명 제어 기능
- **조도 센서**: 실내 조도를 감지하여 최적화된 밝기를 찾아냅니다.  
- **LED 조명**: 주변 환경에 따라 조명 밝기를 자동 조절, 편안한 시야 확보!

### ❄ 공기 순환 기능
- **온/습도 센서**: 방 안의 온도와 습도를 측정  
- **쿨러**: 온도 25도 이상 or 습도 80% 이상일 때 자동 작동! 언제나 쾌적한 환경을 유지합니다.

### 🔊 볼륨 조절 기능
- **로터리 센서**로 음량을 직관적으로 조절하세요!  
  원하는 만큼 키우거나 줄여 몰입도를 최대치로 끌어올릴 수 있습니다.

### 🎦 TV 자동 ON/OFF 기능 
- **초음파 센서**: 의자에 사람이 앉아 있는지 감지  
- **디스플레이**: 착석 시 자동으로 디스플레이 ON! 비어있다면 OFF! 전기 낭비 없이 스마트하게.

### 위의 모든 기능을 블루투스로도 사용해보세요!

---

## 🛠 전체 시스템 구조

![image]()

### 엑추에이터 별 도식화
![image]()

---

## 👥 팀 소개

| 이름      | 역할                             | GitHub                                      |
|-----------|----------------------------------|---------------------------------------------|
| 김민준    | 프로젝트 총괄, 회로 설계 및 구성, 소스코드 통합  | [joun2301](https://github.com/joun2301)   |
| 하종찬    | 볼륨 조절, 조명 제어 시스템, 인테리어  | [gkwhdcks9](https://github.com/gkwhdcks9)   |
| 이근탁    | 메인 프로세스, 디스플레이 제어, 블루투스 통신  | [imbmi4](https://github.com/imbmi4)   |
| 최승민    | 볼륨 조절, 조명 제어 시스템, 인테리어  | [c3110y3110](https://github.com/dashboard)   |
| 김대현    | 의자 각도 조절, 인테리어  | [hetrkumt](https://github.com/hetrkumt)   |
---

## 🔧 기술 스택 & 하드웨어
- **Sensor**: Button, Photo Resistor(Cds), Temp/Humidity Sensor(DHT11), Rotary Encoder, Ultrasonic Sensor
- **Actuator**: Servo Motor, LED(LED Bar), Cooler(DC 12V), Display(Raspberry Pi Display V1.1)
- **Hardware**: Raspberry Pi 4 B, UPS Battery Module, Bread Board
- **Software**: gcc, C/C++, VSCode
- **Environment**: Home-like Theater Setup

---
