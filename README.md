# 🚀 Arduino_Code

> Bộ sưu tập **code Arduino** cho học tập & thực hành: **cảm biến, động cơ, LED và module phổ biến**. Phù hợp cho người mới bắt đầu đến nâng cao.

---

## 📂 Cấu trúc thư mục

```
Arduino_Code/
├── Cam_Bien/     # Code các loại cảm biến
├── Dong_Co/      # Điều khiển các loại động cơ
├── Led/          # LED đơn, LED RGB, LED hiệu ứng
├── Module/       # Các module mở rộng
└── README.md
```

Arduino_Code/
├── Cam_Bien/     # Code các loại cảm biến (siêu âm, DHT, gas, ánh sáng, line, ...)
├── Dong_Co/      # Điều khiển động cơ (DC, Servo, Stepper, L298N, ...)
├── Led/          # LED đơn, LED RGB, LED hiệu ứng
├── Module/       # Các module mở rộng (IR, Relay, ESP, ...)
└── README.md

````

---

## 📖 Giải thích chi tiết nội dung

### 🔌 Module (3 module chính)

#### 1️⃣ Module WiFi (ESP8266 / ESP32)
- Kết nối Arduino với mạng WiFi
- Truyền & nhận dữ liệu qua Web Server / HTTP / MQTT
- Ứng dụng: điều khiển thiết bị từ xa, IoT, nhà thông minh

#### 2️⃣ Module Hồng ngoại (IR Remote)
- Nhận tín hiệu từ remote hồng ngoại
- Giải mã nút bấm để điều khiển LED, động cơ, servo
- Ứng dụng: điều khiển không dây khoảng cách ngắn

#### 3️⃣ Module Bluetooth (HC-05 / HC-06)
- Giao tiếp không dây với điện thoại hoặc máy tính
- Điều khiển Arduino bằng app Bluetooth
- Ứng dụng: xe điều khiển, robot, truyền dữ liệu đơn giản

---

### ⚙️ Động cơ (3 loại)

#### 1️⃣ Động cơ DC
- Điều khiển tốc độ bằng PWM
- Đảo chiều quay với L298N / L293D
- Ứng dụng: quạt, xe robot, băng chuyền

#### 2️⃣ Động cơ bước (Stepper)
- Quay theo từng bước chính xác
- Điều khiển vị trí & góc quay
- Ứng dụng: CNC mini, robot, cơ cấu chính xác

#### 3️⃣ Động cơ Servo
- Điều khiển góc quay (0–180°)
- Dễ sử dụng với thư viện Servo
- Ứng dụng: cánh tay robot, cửa tự động

---

### 🌡️ Cảm biến (6 loại phổ biến)

#### 1️⃣ Cảm biến siêu âm (HC-SR04)
- Đo khoảng cách bằng sóng siêu âm
- Ứng dụng: tránh vật cản, đo mực nước

#### 2️⃣ Cảm biến nhiệt độ & độ ẩm (DHT11 / DHT22)
- Đo nhiệt độ và độ ẩm môi trường
- Ứng dụng: trạm thời tiết, nhà thông minh

#### 3️⃣ Cảm biến khí gas (MQ-x)
- Phát hiện rò rỉ gas, khói
- Ứng dụng: cảnh báo an toàn

#### 4️⃣ Cảm biến ánh sáng (LDR)
- Đo cường độ ánh sáng
- Ứng dụng: đèn tự động bật/tắt

#### 5️⃣ Cảm biến dò line (IR Line Sensor)
- Phát hiện vạch đen/trắng
- Ứng dụng: xe dò line

#### 6️⃣ Joystick (X–Y–Button)
- Nhập dữ liệu điều khiển 2 trục
- Ứng dụng: điều khiển robot, game

---

## 🧰 Yêu cầu
- Arduino IDE (khuyến nghị bản mới nhất)
- Driver CH340 / CP210x (tùy board)
- Các thư viện phổ biến:
  - `IRremote`
  - `Servo`
  - `Stepper`
  - `DHT`

> 💡 Mỗi thư mục con thường có file `.ino` độc lập – chỉ cần mở và nạp.

---

## ▶️ Cách sử dụng nhanh
1. Clone repo:
```bash
git clone https://github.com/<your-username>/Arduino_Code.git
````

2. Mở Arduino IDE → `File` → `Open`
3. Chọn file `.ino` trong thư mục bạn cần
4. Chọn **Board** & **COM Port**
5. Nhấn **Upload** 🚀

---

## 📌 Ví dụ nội dung

* 🔹 Cảm biến siêu âm đo khoảng cách
* 🔹 Điều khiển động cơ DC bằng L298N
* 🔹 Servo quay theo remote IR
* 🔹 Đọc joystick / gas / ánh sáng
* 🔹 ESP8266 điều khiển qua WiFi

---

> *Happy coding with Arduino! 🔌🤖*
