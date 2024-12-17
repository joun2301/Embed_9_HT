![image](https://github.com/user-attachments/assets/a5ef8de0-1b52-43ed-b9d1-81b0d8797c51)# 🎬 Home Sweet Theater

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

## 시스템 구조도
### 🛠 전체 시스템 구조
![image](https://github.com/user-attachments/assets/b9c3aacb-0e41-4cd9-9202-d39e9af012ef)

### 🛠 엑추에이터 별 도식화
![image](https://github.com/user-attachments/assets/edfa6b24-465f-46e5-b46f-08133e9d065e)

---

## 사용 설명서
### 사람 감지 및 TV 제어
![image](https://github.com/user-attachments/assets/03259a41-5f81-40eb-a2ed-63851ec44995)
- ① 초음파 센서로 10cm 이내에 사람 감지
  - 사람 감지 시, TV ON
   사람 미감지 시, TV OFF
- 감지 상태에서 3번 버튼(BTN3)을 통해 수동 제어 가능)


### 조명 제어
![image](https://github.com/user-attachments/assets/90ae9fa9-1c2b-476a-b62e-552464d1c10d)
- ① 조도 센서를 활용한 외부 밝기 감지
- ② 밝기 값을 통한 LED ON/OFF 제어


### 온/습도 감지 및 냉각 제어
![image](https://github.com/user-attachments/assets/297c36b6-dbe6-40b1-b1c2-9215d6a3427f)
- ① 내부 온/습도 감지하여 냉각 제어
- ② 기준 값(습도 80%, 온도 25°C) 초과 시, 냉각 팬 ON


### 스피커 음량 조절
![image](https://github.com/user-attachments/assets/97b4b0db-de89-4ed9-bb6f-3839087dbb85)
- ① 로터리 센서를 활용한 시스템 볼륨 조절(+1 or -1)
- ② 로터리 센서 버튼 활용 시, 음소거 및 음소거 해제 가능
  - 음소거 해제 시, 음소거 이전의 값으로 복귀


### 의자 각도 조절
![image](https://github.com/user-attachments/assets/8da19738-d983-4ce5-8ba2-0b79b1aabfaf)
- ① 1번 버튼(BTN1) : 각도 커짐 || 2번 버튼(BTN2) : 각도 작아짐
- ② 1번 버튼&&2번 버튼 : 초기 각도(기본 의자 등받이 각도)로 회귀


### 블루투스 통신
![image](https://github.com/user-attachments/assets/72e94dd6-536a-4c7b-9cfc-53cd481c1f7a)
- ① 메인 프로세스에서 모든 센서 및 액추에이터 자동 모드 시작
  - 블루투스로 사용자 모드 입력을 받는 Client 프로세스
  - 입력 받은 모드를 적용하는 Server 프로세스(ex. SPEAKER:SET_VOLUME;75)


---

## 🧠 발생한 문제점 및 해결방안
### 권한 요구 문제
![image](https://github.com/user-attachments/assets/850afbc3-35a5-4677-b6fd-61aedc41fc6d)
- 서브 모터를 제외한 기능들은 일반 권한 요구
  - 메인 프로세스 코드 내, posix_spawn의 속성 값 변경만으로 권한 변경 불가
- 서브 모터는 PWM이 적용되어 root 권한 요구
  - 서브 모터 코드 내 setuid, setgid 적용 시, 좀비 프로세스 생성


#### 해결 방법
![image](https://github.com/user-attachments/assets/18e7c389-b2b1-45f1-a17c-b8005620242c)
- posiz_spaw 함수의 실행 대상(두 번째 인자)을 /usr/bin/sudo 경로 지정
- 실행 매개 변수(다섯 번째 인자)를 서브 모터 기능 담당 파일로 설정


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
