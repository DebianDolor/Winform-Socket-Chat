
# IRC Socket Chat 

## Các công nghệ được sử dụng:

- Truyền nhận tin nhắn theo giao thức IRC.
- Lập trình socket để trao đổi giữa nhiều máy Windows.
- Sử dụng thread để xử lý được nhiều clients cùng lúc.

## Cấu trúc của chương trình:

- Project gồm 2 chương trình: 1 chương trình đóng vai trò IRC Server và 1 chương trình đóng vai trò IRC Client.

**1, IRC Server:**

- Có giao diện người dùng.
- Standalone Server, mọi Clients kết nối tới server này.
- Xử dụng thread để xử lý được nhiều clients 1 lúc.
- Các lệnh được định nghĩa: 
    - `NICK`: đặt nickname cho 1 user, nếu như nickname đã có thì báo lỗi cho người dùng.
	- `QUIT`: kết thúc phiên làm việc của user. Server thông báo rằng đã có người thoát ra khỏi kênh cho tất cả các user khác đang còn trên kênh đó.
	- `JOIN`: tham gia một room; (mỗi người dùng chỉ được tham gia 1 room tại 1 thời điểm, khi người dùng muốn tham gia một room khác thì họ sẽ thoát ra khỏi room đang có).
	- `PART`: thoát user ra khỏi room hiện tại. Nếu người dùng không đang trong room muốn thoát thì báo lỗi.
	- `LIST`: hiện các room đang tồn tại ở server, và đưa ra số người trong các room đó.
	- `PRIVMSG`: gửi tin nhắn tới user khác hoặc room. Nhận vào nickname của người muốn gửi hoặc tên room. Nếu gửi tới room, tất cả những người đang thuộc room đó (không kể người gửi) đều nhận được tin nhắn. Nếu gửi tới người dùng, chỉ người dùng đó nhận được tin nhắn.
	- `WHO`: lấy thông tin về người dùng đang có trong 1 room.
	- `HELP`: hiện danh sách các lệnh và cách sử dụng
    - `ERR_UNKNOWNCOMMAND`: các lệnh không được định nghĩa.



**2, IRC Client:**

- Có giao diện người dùng.
- Kết nối với IRC Server.
- Cho phép người dùng:
    - Hiển thị menu trợ giúp.
    - Thay đổi nickname.
    - Xem các room hiện có.
    - Thực hiện kết nối tới các room, và xem các thành viên hiện có trong room đó.
    - Nếu server bị closed unexpectedly, thông báo lỗi và thoát.


## Flow của chương trình:

- Người dùng phải chạy file Server.exe trước, sau đó mới chạy được Client.exe 
- Sau khi người dùng mở Client.exe, Server thông báo có client mới kết nối.

- Người dùng chọn tab "Rename" để đổi nickname. 

- Người dùng chọn tab "Group chat" để join 1 room và bắt đâu trò chuyện.
- Server sẽ log ra việc client nào tham gia room nào và nội dung cuộc trò chuyện trên room đó.

- Người dùng chọn tab "Private Chat" để bắt đầu trò chuyện private (chỉ partner được chọn mới nhận được tin nhắn).
- Server cũng log ra nội dung cuộc trò chuyện này.

- Client ngắt kết nối thì server hiển thị thông báo.
- Server ngắt kết nối thì client cũng hiển thị thông báo rồi thoát.















