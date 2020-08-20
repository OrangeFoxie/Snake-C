#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<string.h>
#include<time.h>


int character[1000];	// Chiều dài tối đa của rắn
int length = 2;	
int fruit;				// Thức ăn của rắn
int score;				// Điểm của người chơi
clock_t t;				// 
int s_e;
int vx;					// Trục X
int vy;					// Trục Y
int speed = 10;			// Tốc độ di chuyển của rắn

enum state { // trạng thái các thành phần trong game
	MENU = 0, INGAME, SETTING, GUIDING, INFORMATION, GAMEOVER, EXIT
	// danh sách các hàm trong trò chơi
	// menu: bảng danh sách lúc vào giao diện
	// ingame: hàm xử lí các sự kiện trong game
	// setting: hàm cài đặt
	// guiding: hàm chưa có chức năng
	// information: mục thông tin về demo
	// gameover: hàm xử lí khi kết thúc trò chơi
	// exit: thoát trò chơi
} state; // trả về state

int Random(int a, int b)
{
	return a + rand() % (b - a + 1);	//
}

void gotoxy(int x, int y)	// Di chuyển con trỏ đến vị trí trong màn hình
{
	static HANDLE  h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x,y };
	SetConsoleCursorPosition(h, c);	// cột dọc: h, hàng ngang: c
}
void SetColor(WORD color)	// Màu kí tự khi hiển thị
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;	// mã màu
	wAttributes &= 0xfff0; wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void information() {	// mục thông tin game
	system("cls");
	SetColor(4);	// màu kí tự
	gotoxy(10, 8);	// vị trí của dòng 1
	printf("DEMO RAN SAN MOI"); // dòng 1
	gotoxy(8, 9);	// vị trí của dòng 2
	printf("Thanh vien nhom"); // dòng 2
	gotoxy(3, 10);	// vị trí của dòng 3
	printf("MSSV");	// dòng 3
	gotoxy(27, 11); // vị trí của dòng 4
	SetColor(14);	
	printf("Testing");	// dòng 4
	getchar();		// nhận kí tự từ bàn phím khi chọn các chức năng
	state = MENU;	// hiển thị menu chọn chức năng
}
void setting() {	// mục tuỳ chỉnh
	system("cls");	// làm mới màn hình
	gotoxy(0, 9);	// vị trí text sát viền trái màn hình
	printf("an cac phim up down tren ban phim de tang giam toc do, nhan enter de tro ve menu");
	gotoxy(30, 11);	// vị trí mục điều chỉnh tốc độ rắn
	printf("toc do hien tai : %d", speed); // mục điều chỉnh tốc độ rắn
	int key;		// biến nhận phản hồi từ bàn phím
	while (state == SETTING) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
				// nút trái
			case 75:
				if (speed > 0) {

					speed -= 1; // Giảm tốc độ 1 đon vị
					gotoxy(30, 11);
					printf("toc do hien tai : %d   ", speed);
				}
				if (speed == 0) { // giảm xuống dưới 1 thì về 100

					speed = 100; // Giảm tốc độ 1 đon vị
					gotoxy(30, 11);
					printf("toc do hien tai : %d   ", speed);
				}
				break;
				// nút phải
			case  77:
				if (speed < 101) {	

					speed += 1; // tăng tốc độ lên 1 đơn vị
					gotoxy(30, 11);
					printf("toc do hien tai : %d   ", speed);
				}
				if (speed == 101){	// tăng đến 100 thì quay về 1

					speed = 1;
					gotoxy(30, 11);
					printf("toc do hien tai : %d   ", speed);
				}
				break;
			case 13: // nút enter
				state = MENU;
				break;
			}
		}
	}
}

// Ve khung vien cho game
void vekhung() {
	int i = 0;	// chiều ngang
	int j = 0;	// chiều cao
	SetColor(3); // màu kí tự
	for (i; i < 79; i++) {	// Viền trên, 79 là chiều dài khung
		printf("-");
	}
	printf("\n");
	i = 0;
	for (i; i < 23; i++) {	// 23 là số dòng, chiều cao của khung

		for (j; j < 79; j++) {	// căn chỉnh chiều dài của của khung để đánh dấu chiều cao
			if (j == 0)printf("#");	// Viền trái
			else if (j == 78) printf("#\n");	// Viền phải
			else printf(" "); // Chỉ ghi kí tự # ở vị trí 0 và 78, các vị trí trống là vùng nằm trong hộp để rắn chạy
		}
		j = 0;
	}
	i = 0;
	for (i; i < 79; i++) { // Viền dưới
		printf("-");
	}
}
// Thông báo khi trò chơi kết thúc
void gameover() {
	SetColor(4); // màu kí tự
	system("cls");	// làm mới màn hình
	gotoxy(30, 9);
	printf("....Game over....");
	gotoxy(15, 11);
	printf("Choi duoc %d diem, an phim bat ki de ve menu", score);
	gotoxy(27, 13);
	getchar();
	state = MENU;
}

// Exit 
/*
void exitGame() {
	state = EXIT;
	exit(0);
}*/

void menu() {
	SetColor(14);
	system("cls");
	gotoxy(25, 3);
	printf("DEMO RAN SAN MOI");
	gotoxy(30, 5);
	SetColor(4);
	printf("1. vao game"); // 5
	gotoxy(30, 6);
	printf("2. tuy chinh");// 6
	gotoxy(30, 7);
	printf("3. thong tin");// 7
/*	gotoxy(30, 8);
	printf("4. exit");	*/   // 8

	int choice = 1; // lựa chọn hiện tại
	int oldChoice = 1; // lựa chọn cũ
	int next = 1;	// lựa chọn tiếp theo
	int key;		// kí tự được nhập từ bàn phím
	SetColor(14);
	gotoxy(29, 5);
	printf("%c", 16);
	while (state == MENU) {
		if (_kbhit()) {
			key = _getch();
			if (key == 80) {
				choice++;
				if (choice == 4) choice = 1;
			}
			if (key == 72) {
				choice--;
				if (choice == 0) choice = 3;
			}
			switch (choice) {	// cập nhật - hiển thị vị trí con trỏ chọn chức năng
			case 1:
				if (oldChoice != choice) {

					gotoxy(29, 6);
					printf(" ");
					gotoxy(29, 7);
					printf(" ");
					oldChoice = 1;

					gotoxy(29, 5);
					SetColor(14);
					printf("%c", 16);
				}
				break;
			case 2:
				if (oldChoice != choice) {

					gotoxy(29, 5);
					printf(" ");
					gotoxy(29, 7);
					printf(" ");
					gotoxy(29, 6);
					SetColor(14);
					printf("%c", 16);
					oldChoice = 2;
				}
				break;
			case 3:
				if (oldChoice != choice) {


					gotoxy(29, 6);
					printf(" ");
					gotoxy(29, 5);
					printf(" ");
					gotoxy(29, 7);
					SetColor(14);
					printf("%c", 16);

					oldChoice = 3;
				}
				break;
/*			case 4:
				if (oldChoice != choice) {


					gotoxy(29, 6);
					printf(" ");
					gotoxy(29, 7);
					printf(" ");
					oldChoice = 4;

					gotoxy(29, 8);
					SetColor(14);
					printf("%c", 16);

				}
				break;*/
			}
			if (key == 13) {
				switch (choice) {
				case 1:	// chọn chức năng vào game
					state = INGAME;
					break;
				case 2:	// chọn chức năng tuỳ chỉnh
					state = SETTING;
					break;
				case 3:	// chọn chức năng thông tin game
					state = INFORMATION;
					break;
				/*case 4:
					state = EXIT;
					break;*/

				}
			}

		}

	}

}


void initgame() {
	system("cls");
	s_e = 120 - speed;
	t = 0;
	vx = 1;	// vị trí bắt đầu của rắn theo chiều dọc
	vy = 0; // Vị trí bắt đầu của rắn theo chiều ngang
	score = 0; // Điểm của người chơi
	length = 2;	// Chiều dài cơ bản của rắn lúc bắt đầu trò chơi
	character[0] = 11510;
	character[1] = 11410;
	character[2] = 11310;
	character[3] = 11210;	// Vị trí các phần của rắn lúc bắt đầu
	character[4] = 11110;
	character[5] = 11010;
	character[6] = 10910;
	character[7] = 10810;
	character[8] = 10710;
	character[9] = 10610;	
	fruit = 15010;			// vị trí đồ ăn lúc bắt đầu game
}
void creatfruit() {	// hàm tạo đồ ăn
	int notok = 1;
	while (notok) {
		notok = 0;
		fruit = 10000 + Random(1, 77) * 100 + Random(1, 23);	// Vị trí random khi tạo đồ ăn
		int i = 0;
		for (i; i < length; i++) {
			if (character[i] == fruit) {	// cộng dồn đồ ăn vào thân rắn cho dài ra
				notok = 1;	// trả về điều kiện bắt đầu vòng lặp while
			}
		}
	}
}
int getx(int a) {
	return (a - 10000) / 100;	// vị trí theo chiều ngang
}
int gety(int a) {	// vị trí theo chiều dọc
	return a % 100;
}
void drawscore() {	// hiển thị bảng điểm -- bên phải hộp rắn

	// Chữ "score"
	gotoxy(79, 3);
	printf("s");
	gotoxy(79, 4);
	printf("c");
	gotoxy(79, 5);
	printf("o");
	gotoxy(79, 6);
	printf("r");
	gotoxy(79, 7);
	printf("e");

	// hàng điểm số
	gotoxy(79, 9);
	printf("0");
	gotoxy(80, 9);
	printf("0");
	gotoxy(81, 9);
	printf("0");
	gotoxy(82, 9);
	printf("0");
	gotoxy(83, 9);
	printf("0");


	/*gotoxy(79, 9);
	printf("0");
	gotoxy(79, 10);
	printf("0");
	gotoxy(79, 11);
	printf("0");
	gotoxy(79, 12);
	printf("0");
	gotoxy(79, 13);
	printf("0");*/


}
void addscore() {	// Hàm cộng điểm khi rắn ăn mồi
	SetColor(14);
	gotoxy(79, 9); //(cột, dòng)
	printf("%d", (score % 100000) / 10000);
	gotoxy(80, 9);
	printf("%d", (score % 10000) / 1000);
	gotoxy(81, 9);
	printf("%d", (score % 1000) / 100);
	gotoxy(82, 9);
	printf("%d", (score % 100) / 10);
	gotoxy(83, 9);
	printf("%d", score % 10);


	/*gotoxy(79, 9); //(cột, dòng)
	printf("%d", (score % 100000) / 10000);
	gotoxy(79, 10);
	printf("%d", (score % 10000) / 1000);
	gotoxy(79, 11);
	printf("%d", (score % 1000) / 100);
	gotoxy(79, 12);
	printf("%d", (score % 100) / 10);
	gotoxy(79, 13);
	printf("%d", score % 10);*/


}
void ingame() {	// hàm xử lí sự kiện trong game
	int key;
	int i;
	initgame();	// khởi chạy game
	vekhung();	// tạo hộp nhốt rắn

	drawscore();	// tạo bảng điểm
	state = INGAME;
	SetColor(13);
	gotoxy(getx(fruit), gety(fruit));	// tạo đồ ăn
	printf("3");	// hiển thị đồ ăn lần đầu bên trong hộp rắn nơi rắn bò
	SetColor(12);
	gotoxy(getx(character[0]), gety(character[0]));	// cập nhật di chuyển của rắn
	printf("#");	
	i = 1;
	SetColor(2);
	for (i; i < length; i++) {
		gotoxy(getx(character[i]), gety(character[i]));
		printf("o");
	}
	while (state == INGAME) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
				// nút lên
			case 72:
				if (vy == 0) { // đi lên theo chiều cao
					vx = 0;
					vy = -1;
				}
				break;
				// nút xuống
			case 80:
				if (vy == 0) { // đi xuống theo chiều cao
					vx = 0;
					vy = 1;
				}
				break;
				// nút trái
			case 75:
				if (vx == 0) { // đi qua trái
					vy = 0;
					vx = -1;
				}
				break;
				// nút phải
			case 77:
				if (vx == 0) { // đi qua phải
					vy = 0;
					vx = 1;
				}
				break;
			}
		}


		if ((clock() - t) >= s_e) {
			if (character[length - 1] != 0) {
				gotoxy(getx(character[length - 1]), gety(character[length - 1]));
				printf(" ");	// Những chỗ rắn bò qua rồi thì sẽ quay lại thành khoảng trống
			}
			i = length - 1;
			for (i; i > 0; i--) {
				character[i] = character[i - 1];	// xoá phần thân cũ khi rắn bò qua
			}
			SetColor(2);
			gotoxy(getx(character[0]), gety(character[0]));	// vị trí thân rắn
			printf("0"); // hình dạng thân rắn
			character[0] += vx * 100; // cập nhật bước di chuyển của rắn theo chiều ngang
			character[0] += vy;		  // cập nhật bước di chuyển của rắn theo chiều dọc
			if (character[0] == fruit) {	// đầu rắn mà chạm mồi thì...
				length += 1;				// chiều dài tăng 1
				score += (speed / 10 + 5);	// điểm tăng = tốc độ /10 + 5
				addscore();					// cập nhật điểm lên bảng điểm
				creatfruit();				// tạo đồ ăn mới
				SetColor(13);				
				gotoxy(getx(fruit), gety(fruit));	// cập nhật vị trí đồ ăn
				printf("0"); // hiển thị hình dạng thức ăn lần 2 về sau
			}
			if ((vx > 0) && (getx(character[0]) == 78)) {
				character[0] -= 7700;
			}
			if ((vx < 0) && (getx(character[0]) == 0)) {
				character[0] += 7700;
			}
			if ((vy > 0) && (gety(character[0]) == 24)) {
				character[0] -= 23;
			}
			if ((vy < 0) && (gety(character[0]) == 0)) {
				character[0] += 23;
			}
			SetColor(12); // màu đỏ
			gotoxy(getx(character[0]), gety(character[0]));	// cập nhật vị trí đầu rắn
			printf("+"); // hình dạng đầu rắn
			i = 1;
			for (i; i < length; i++) {
				if (character[i] == character[0]) {	// Nếu rắn lỡ cắn trúng bản thân thì...
					system("cls");	// cập nhật màn hình
					state = GAMEOVER;	// hiển thị màn hình GAME OVER
				}
			}
			t = clock();	// cập nhật timing trong game
		}

	}
}

int main() {
	state = MENU;	// lúc khởi động vào game thì màn hình đăng nhập xuất hiện
	menu();			// hiện màn hình menu chọn chức năng

	while (1) {		// while để duy trì chức năng 
		switch (state) {	// state = phím chọn chức năng
		case MENU:		// vào menu chức năng
			menu();
			break;
		case INGAME:	// vào game
			ingame();
			break;
		case GAMEOVER:	// game kết thúc
			gameover();
			break;
		case INFORMATION:	// thông tin game
			information();
			break;
		case  SETTING:	// tuỳ chỉnh
			setting();
			break;
		}

	}
	return 0;
}

