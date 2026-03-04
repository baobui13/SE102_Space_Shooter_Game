# SPACE SHOOTER GAME

**Giảng viên:** Thầy Vũ Tuấn Hải  
**Đơn vị:** Khoa Công nghệ Phần mềm (Faculty of Software Engineering) — Trường Đại học Công nghệ Thông tin, ĐHQG-HCM (UIT)  
**Môn học:** **SE102.Q23 — Nhập môn phát triển game** 

**Sinh viên 1:** Bùi Quốc Bảo  
**MSSV 1:** 23520093

**Sinh viên 2:** Nguyễn Công Tài  
**MSSV 2:** 21522565

**Sinh viên 3:** Nguyễn Hoàng Quân  
**MSSV 3:** 21522494

## NỘI DUNG SƠ LƯỢC

Game lấy ý tưởng từ các tựa game Space Shooter như Chicken Invaders, Galaxy Attack,... cùng những tựa game roguelike cho phép nâng cấp nhân vật như Vampire Survival, Brotato,...
Người chơi dùng chuột hoặc bàn phím điều khiển phi thuyền để né đòn tấn công và bắn hạ kẻ địch. Kẻ địch bay từ ngoài màn hình vào, có thể di chuyển liên tục hoặc xếp theo đội hình và tấn công người chơi bằng cách tiếp cận hoặc tấn công từ xa. Khi bị tiêu diệt, kẻ địch sẽ rơi kinh nghiệm, vật phẩm. Màn chơi kết thúc khi người chơi chịu quá nhiều đòn tấn công và hết máu hoặc khi người chơi tiêu diệt toàn bộ kẻ địch trong màn.
Người chơi lên cấp khi đủ kinh nghiệm, giúp nâng các chỉ số và cơ chế mới. Các vật phẩm sẽ cường hóa người chơi trong một khoảng thời gian sau khi nhặt.

## ĐIỀU KHIỂN

Phi thuyền chỉ có một hướng bắn là từ dưới lên trên. Người chơi có thể dùng chuột và hoặc phím mũi tên để điều khiển phi thuyền di chuyển theo tất cả các hướng nhưng không thể xoay phi thuyền. Nhấp chuột hoặc nút space để bắn. Dùng các phím số để sử dụng kỹ năng. Mỗi kỹ năng sẽ có thời gian hồi chiêu riêng, không thể sử dụng kỹ năng khi chưa hồi chiêu xong.

## NÂNG CẤP 

Kẻ địch bị tiêu diệt sẽ rơi ra kinh nghiệm. Nhặt đủ kinh nghiệm giúp người chơi lên cấp. Mỗi khi lên cấp người chơi được chọn một trong ba nâng cấp bất kỳ thuộc các loại:
- Nâng cấp chỉ số
- Nâng cấp kỹ năng chủ động
- Nâng cấp kỹ năng bị động

## KẺ ĐỊCH 

- Kẻ địch đánh xa: sau khi xuất hiện sẽ chỉ đứng xa và bắn đạn về phía người chơi
- Kẻ địch đánh gần: sau khi xuất hiện sẽ lao về phía người chơi, gấy sát thương khi va chạm
- Boss: kẻ địch đánh xa nhưng to hơn, nhiều máu hơn và có thêm kỹ năng.

## GIAO DIỆN
### MENU
- Nút chọn màn chơi
- Nút chơi
- Nút setting
- Nút Exit

### Setting
- Âm thanh game
- Sound effect
- Background music

### Màn chơi
- Thanh máu
- Thanh kinh nghiệm
- Thanh kỹ năng --> Miêu tả kỹ năng khi di chuột vào
- Bảng chỉ số
- 