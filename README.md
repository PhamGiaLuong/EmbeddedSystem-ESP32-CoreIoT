# Mô phỏng Thiết bị IoT (MQTT Client)

Dự án này mô phỏng một thiết bị IoT kết nối đến Core IoT/ThingsBoard sử dụng giao thức MQTT. Chương trình gửi dữ liệu cảm biến (nhiệt độ, độ ẩm,...) định kỳ và nhận lệnh điều khiển từ server.

## Yêu cầu hệ thống

- Python 3.x

## Hướng dẫn Cài đặt & Chạy (Local Development)

Vì thư mục môi trường ảo không được lưu trên Git để tối ưu dung lượng, bạn vui lòng thực hiện các bước sau để thiết lập môi trường chạy lần đầu tiên.

### 1. Khởi tạo môi trường ảo (Virtual Environment)

Tại thư mục gốc của dự án, mở terminal và chạy lệnh sau để tạo một môi trường ảo mới tên là `venv`:

```bash
python -m venv venv
```

### 2\. Kích hoạt môi trường ảo

Bạn cần kích hoạt môi trường này để cài đặt thư viện mà không ảnh hưởng đến Python gốc của máy.

- **Windows:**

  ```bash
  .\venv\Scripts\activate
  ```

- **macOS / Linux:**

  ```bash
  source venv/bin/activate
  ```

_(Khi kích hoạt thành công, dấu nhắc lệnh sẽ hiện tiền tố `(venv)`)._

### 3\. Cài đặt thư viện

Dự án yêu cầu chính xác phiên bản `paho-mqtt` 1.6.1 để tương thích với mã nguồn hiện tại. Sau khi kích hoạt môi trường ảo, chạy lệnh:

```bash
pip install paho-mqtt==1.6.1
```

### 4\. Cấu hình kết nối

Mở file `thingsboard.py` và cập nhật các thông số kết nối MQTT nếu cần (Token, Username):

```python
BROKER_ADDRESS = "app.coreiot.io"
ACCESS_TOKEN = "Vid2-IoT_Device"
ACCESS_USERNAME = "Vid2-IoT_Device"
```

### 5\. Chạy chương trình

Sau khi cài đặt xong, có thể chạy code bằng lệnh:

```bash
python thingsboard.py
```

Để dừng chương trình, nhấn tổ hợp phím `Ctrl + C` trong terminal.

### 6\. Thoát môi trường ảo

Khi làm việc xong, để tắt môi trường ảo, chỉ cần gõ:

```bash
deactivate
```
