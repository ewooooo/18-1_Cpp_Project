#include "stdafx.h"		//2017 사용
#include <windows.h>	//콘솔화면지우기 사용을 위한
#include <iostream>		//입출력
#include <fstream>		//text 파일로 출력
using namespace std;


class datam {					//data 클래스

	int day1;					//날짜(YYMMDD)
	char *mname;				//사용내역
	char *category;				//분류
	int money;					//금액
	int padata;					//수입=1 ,지출=0
	int category_nuber;			//사용분류 고정값
								//1.월급 2.용돈 3.기타 4.식비 5.생활비 6.유흥비 7,여가비용 8.자기관리 9.정기지출

public:
	void set_data(int day_d, char *mname_mn, char *category_c, int money_n, int padata_p, int n);	//데이터 저장함수
	int get_padata() { return padata; };							
	int get_day() { return day1; };									
	char* get_mname() { return mname; };							//클래스 내부변수 반환 함수
	char* get_category() { return category; };						
	int get_money() { return money; };								
	int get_category_nuber() { return category_nuber; };			
	datam *next;													//링크드 리스트 사용
	datam *dat_next() { return next; };
};

void datam::set_data(int day_d, char *mname_mn, char *category_c, int money_n, int padata_p, int n) {

	day1 = day_d;

	int size = strlen(mname_mn)+1;		//인수로 받은 문자열 크기+1
	mname = new char[size];				//받은문자열 크기만큼 공간 생성
	strcpy(mname, mname_mn);			//데이터 복사

	size = strlen(category_c)+1;
	category = new char[size];
	strcpy(category, category_c);

	money = money_n;

	padata = padata_p;
	category_nuber = n;
	//프로그램 종료시 저장할 데이터들이므로 공간을 지우기 않는다
}

class money_control : public datam {			//수입지출에 대해  데이터 입출력 관리

	datam *start;	//링크드 리스트 처음부분
	int catagory_money[6] = { 0,0,0,0,0,0 };		//설정할 예산값
public:
	void data_in(int control);							//데이터 입력함수
	void data_out(int control1, int control2);			//데이터 출력함수
	int data_out3(int control);							//수입 지출별 총계
	int data_categorysum(int control);					//사용 분류별 총계
	void set_category_money(int a[6]) {					//예산 설정 메뉴에서 예산값 저장하기
		for (int icount = 0; icount < 6; icount++) {
			catagory_money[icount] = a[icount];
		}
	}
	int get_catagory_money(int control) { return catagory_money[control]; }		//분류별 예산값 출력
	void load();		//프로그램 실행시 파일에 있는 데이터 불러오기
	void save();		//종료시 파일에 데이터 덮어쓰기
};
void money_control::data_in(int control) {		//데이터 입력함수 , control = 1-수입,0-지출
	int d;					//날짜
	char *mn;				//사용내역
	char *c;				//사용분류
	int m;					//금액
	int c_n;				//분류 고정값설정

	mn = new char[20];		//사용내역 내용
	c = new char[10];		//분류 내용

	cout << endl << "날짜(YYMMDD)/사용내역/분류/금액(원)" << endl << endl;
	cout << "데이터를 띄어쓰기하여 입력, 다음내용입력 엔터, 나가기 0" << endl;
	cout << "------------------------------------------------------------" << endl;
	while (1) {
		cout << "------------------------------------------------------------" << endl;
		cin >> d;
		if (d == 0) {					//나가기 0 종료시 공간정리
			delete mn;			
			delete c;
			
			return;
		}
		else {
			cin >> mn >> c >> m;
			if (d < 100000) {				//날짜 형식 지정
				cout << endl << "날짜를 형식에 맞게 다시 입력해주세요" << endl;
				continue;
			}
			else if (991231 < d) {			//날짜 형식 지정
				cout << endl << "날짜를 형식에 맞게 다시 입력해주세요" << endl;
				continue;
			}
			else {
				if (control == 1) {			//수입 일떄의 분류
					if (!strcmp(c, "월급")) {		//분류를 미리 정해놓은 후 비교
						c_n = 1;					//만약 미리 지정한 분류와 맞지않으면 다시 입력
					}
					else if (!strcmp(c, "용돈")) {
						c_n = 2;
					}
					else if (!strcmp(c, "기타")) {
						c_n = 3;
					}
					else {
						cout << endl << " 분류입력값을 형식에 맞게 다시 입력해주세요" << endl;
						continue;
					}
				}
				else {						//지출 일때의 분류
					if (!strcmp(c, "식비")) {
						c_n = 4;
					}
					else if (!strcmp(c, "생활비")) {
						c_n = 5;
					}
					else if (!strcmp(c, "유흥비")) {
						c_n = 6;
					}
					else if (!strcmp(c, "여가비용")) {
						c_n = 7;
					}
					else if (!strcmp(c, "자기관리")) {
						c_n = 8;
					}
					else if (!strcmp(c, "정기지출")) {
						c_n = 9;
					}
					else {
						cout << endl << " 분류입력값을 형식에 맞게 다시 입력해주세요" << endl;
						continue;
					}
				}
			}
			cout << endl << "입력완료:" << d << " " << mn << " " << c << " " << m << endl;		//입력데이터 확인

			datam *newdata = new datam;						//새로운 데이터 공간 생성
			newdata->set_data(d, mn, c, m, control, c_n);	//새로운 데이터 공간에 입력받은 내용 입력
			newdata->next = start;							//링크드리스트 시작값에 있는 데이터를 다음 공간으로 복사 
			start = newdata;								//링크드리스트 시작값에 방금 받은 데이터 입력
		}

	}
	delete mn;	//공간정리
	delete c;	//공간정리
}

void money_control::data_out(int control1, int control2 = 3) {		//데이터 출력함수 ,control1 = 수입,지출 종류 구분, control2 = 수입,지출 모두 출력시
	cout << "-------------------------------------" << endl;		//control 값이 1=수입, 0=지출
	if (start == 0) {
		cout << "데이터 없음" << endl << endl;		//데이터 입력 받은게 있는지 확인
	}
	else {
		char test[5], test2[5];
		if (control1 == 1) {		//화면에 출력할 값이 수입인지 지출인지를 정함
			strcpy(test, "수입");
		}
		else {
			strcpy(test, "지출");
		}
		if (control2 == 1) {		//수입 지출 모두 출력할 때 사용함
			strcpy(test2, "수입");
		}
		else {
			strcpy(test2, "지출");
		}
		cout << "날짜(YYMMDD)/사용내역/분류/금액(원)" << endl;
		cout << "-------------------------------------" << endl;
		for (datam *datarol = start; datarol != 0; datarol = datarol->dat_next()) {		//저장된 문구 출력

			if (datarol->get_padata() == control1) {				//control1 값이 1=수입 0=지출
				cout << datarol->get_day() << " " << test << " " << datarol->get_mname() << " " << datarol->get_category() << " " << datarol->get_money() <<"원"<< endl;
			}
			if (datarol->get_padata() == control2) {				//수입지출 모두 보여줄때만 사용
				cout << datarol->get_day() << " " << test2 << " " << datarol->get_mname() << " " << datarol->get_category() << " " << datarol->get_money()<<"원" << endl;
			}
		}
	}
	cout << "-------------------------------------" << endl;
	cout << "나가기 : 아무 키 입력 후 엔터:";
	char a;
	cin >> a;

}
int money_control::data_out3(int control) {			//수입 지출 총계 control=1 수입. control=0 지출
	int sum = 0;	//sum

	for (datam *datarol = start; datarol != 0; datarol = datarol->dat_next()) {		//수입지출분류값으로 데이터를 선별하여 더해주는 함수
		if (datarol->get_padata() == control) {
			sum = sum + datarol->get_money();
		}
	}
	return sum;
}
int money_control::data_categorysum(int control) {									//분류별 총계
	int sum = 0;	//sum

	for (datam *datarol = start; datarol != 0; datarol = datarol->dat_next()) {		//데이터에서 분류설정값을 선별하여 더해주는 함수
		if (datarol->get_category_nuber() == control) {
			sum = sum + datarol->get_money();
		}
	}
	return sum;
}


void money_control::save() {							//파일을 txt파일로 저장
	ofstream out;
	out.open("C:Data_file.txt");							
	for (int count = 0; count < 6; count++) {		//예산설정 데이터 파일로 출력
		out << endl << catagory_money[count];
	}
	if (start == 0) {			//입력된 데이터가 없을시 함수종료
		return;
	}
	else {						//링크드리스트 반복하여 파일에 데이터 출력

		for (datam *datarol = start; datarol != 0; datarol = datarol->dat_next()) {

			out << endl << datarol->get_day() << " " << datarol->get_mname() << " " << datarol->get_category() << " " << datarol->get_money() << " " << datarol->get_padata() << " " << datarol->get_category_nuber();

		}

	}
	out.close();

}

void money_control::load() {

	int d;				//날짜
	char *mn;			//사용내역
	char *c;			//사용분류
	int m;				//금액
	int pd;				//수입, 지출 구분
	int c_n;			//분류 고정값설정
	mn = new char[20];	//문자열 입력을 위한 공간 생성
	c = new char[10];	//문자열 입력을 위한 공간 생성

	ifstream in;
	in.open("C:Data_file.txt");
	if (!in.fail()) {		//파일에 저장된 예산값데이터 불러오기 
		for (int count = 0; count < 6; count++) {
			in >> catagory_money[count];			
		}
		while (!in.eof())		//파일에 입력된 데이터값 불러오기
		{
			in >> d >> mn >> c >> m >> pd >> c_n;
			datam *newdata = new datam;					//새로운 데이터 공간 생성
			newdata->set_data(d, mn, c, m, pd, c_n);	//새로운 데이터 공간에 입력받은 내용 입력
			newdata->next = start;						//링크드리스트 시작값에 있는 데이터를 다음 공간으로 복사 
			start = newdata;							//링크드리스트 시작값에 방금 받은 데이터 입력
		}
		in.close();
	}
	delete mn;		//공간 지우기
	delete c;		//공간 지우기
}




//-----------------------------------------------------
//-----------------------------------------------------
class menu : public money_control {		//메인메뉴 컨트롤 클래스
public:
	void menu1();
	void menu2();
	void menu3();
	void menu4();
	void menuout();
	void load_data() { load(); }
	void save_data() { save(); }
};

void menu::menu1()
{
	system("cls");		//콘솔 화면 지우기
	cout << "1.수입 입력" << endl;
	cout << "2.지출 입력" << endl;
	cout << "0.나가기" << endl;
	cout << "=====================================" << endl;
	cout << "메뉴 바로가기 번호 입력:";
	char choose;
	while (1) {
		cin >> choose;
		switch (choose)
		{
		case '1':
			cout << endl << "-------------------------------------" << endl;
			cout << "수입 자료를 입력합니다." << endl << endl;
			cout << "(분류: 월급, 용돈, 기타)" << endl;
			data_in(1);			//수입(1) 입력 함수
			return;
		case '2':
			cout << endl << "-------------------------------------" << endl;
			cout << "지출 자료를 입력합니다." << endl << endl;
			cout << "(분류: 식비, 생활비, 유흥비, 여가비용, 자기관리, 정기지출)" << endl ;
			data_in(0);			//지출(0) 입력 함수
			return;
		case '0':
			return;
		default:
			cout << "번호를 다시 입력하세요:";
		}
	}

}
void menu::menu2() {
	system("cls");		//콘솔 화면 지우기
	cout << "1. 수입보기" << endl;
	cout << "2. 지출보기" << endl;
	cout << "3. 수입지출 모두 보기" << endl;
	cout << "0. 나가기" << endl;
	cout << "=====================================" << endl;
	cout << "메뉴 바로가기 번호 입력:";
	char mn2;
	while (1) {
		cin >> mn2;
		switch (mn2) {
		case '1':
			data_out(1);		//수입(1) 출력 함수
			return;
		case '2':
			data_out(0);		//지출(0) 출력 함수
			return;
		case '3':
			data_out(0, 1);		//수입(1), 지출 출력 함수
			return;
		case '0':
			return;
		default:
			cout << "번호를 다시 입력하세요:";
		}
	}

}

void menu::menu3() {
	system("cls");		//콘솔 화면 지우기
	cout << "총계보기" << endl << endl;
	cout << "수입 총계 : " << data_out3(1) << "원" << endl;
	cout << "지출 총계 : " << data_out3(0) << "원" << endl;
	cout << "잔액 : " << data_out3(1) - data_out3(0) << "원" << endl;
	cout << "-------------------------------------" << endl;
	cout << "분류별 수입" << endl << endl;
	cout << "월급 : " << data_categorysum(1) << "원" << endl;
	cout << "용돈 : " << data_categorysum(2) << "원" << endl;
	cout << "기타 : " << data_categorysum(3) << "원" << endl;
	cout << "-------------------------------------" << endl;
	cout << "분류별 지출" << endl << endl;
	cout << "식비 : " << data_categorysum(4) << "원" << endl;
	cout << "생활비 : " << data_categorysum(5) << "원" << endl;
	cout << "유흥비 : " << data_categorysum(6) << "원" << endl;
	cout << "여가비용 : " << data_categorysum(7) << "원" << endl;
	cout << "자기관리 : " << data_categorysum(8) << "원" << endl;
	cout << "정기지출 : " << data_categorysum(9) << "원" << endl;
	cout << "-------------------------------------" << endl;
	cout << "나가기 : 아무 키 입력 후 엔터:";
	char mn3;
	cin >> mn3;
}

void menu::menu4() {
	system("cls");		//콘솔 화면 지우기
	cout << "4. 월별 예산" << endl << endl;
	cout << "식비 : " << get_catagory_money(0) << endl;
	cout << "생활비 : " << get_catagory_money(1) << endl;
	cout << "유흥비 : " << get_catagory_money(2) << endl;
	cout << "여가비용 : " << get_catagory_money(3) << endl;
	cout << "자기관리 : " << get_catagory_money(4) << endl;
	cout << "정기지출 : " << get_catagory_money(5) << endl;
	cout << "=====================================" << endl;
	int bg[6];
	char c1;
	while(1){
		cout << "(수정 y/나가기 n) :";	
		cin >> c1;
		switch (c1)	{
		case 'y':
			cout<< endl;
			cout << "식비 : ";
			cin >> bg[0];
			cout << "생활비 : ";
			cin >> bg[1];
			cout << "유흥비 : ";
			cin >> bg[2];
			cout << "여가비용 : ";
			cin >> bg[3];
			cout << "자기관리 : ";
			cin >> bg[4];
			cout << "정기지출 : ";
			cin >> bg[5];
			cout << endl;
			set_category_money(bg);
			break;
		case 'n':		//n 입력시 메인화면으로
			return;
		default:
			break;
		}
	}
}


void menu::menuout() {		//프로그램 첫화면.
	system("cls");					//콘솔 화면 지우기
	cout << "< 예산을 이용한 가계부 프로그램>" << endl;
	cout << "-------------------------------------" << endl;
	cout << "보유자금 : " << data_out3(1) - data_out3(0) << "원" << endl;
	cout << "-------------------------------------" << endl;
	cout << "예산별 사용가능 금액" << endl << endl;								//설정한 예산 - 분류별 사용한 금액 = 앞으로 내가 분류별 사용할 수 있는 금액
	cout << "식비 : " << get_catagory_money(0) - data_categorysum(4) << endl;
	cout << "생활비 : " << get_catagory_money(1) - data_categorysum(5) << endl;
	cout << "유흥비 : " << get_catagory_money(2) - data_categorysum(6) << endl;
	cout << "여가비용 : " << get_catagory_money(3) - data_categorysum(7) << endl;
	cout << "자기관리 : " << get_catagory_money(4) - data_categorysum(8) << endl;
	cout << "정기지출 : " << get_catagory_money(5) - data_categorysum(9) << endl;

	cout << "-------------------------------------" << endl;
	cout << "1. 수입 및 지출 내역 입력" << endl;
	cout << "2. 수입 및 지출 내역 보기" << endl;
	cout << "3. 총계 보기" << endl;
	cout << "4. 월별 예산" << endl;
	cout << "5. 프로그램종료" << endl;
	cout << "=====================================" << endl;
	cout << "메뉴 바로가기 번호 입력:";
}

//-----------------------------------------------------
//-----------------------------------------------------
int main() {
	menu mainmenu;
	mainmenu.load_data();		//text파일에서 데이터 불러오기
	char n1;
	mainmenu.menuout();			//프로그램 첫 화면 출력
	while (1) {
		cin >> n1;				//메뉴 선택 입력
		switch (n1) {
		case '1':
			mainmenu.menu1();
			mainmenu.menuout();
			break;
		case '2':
			mainmenu.menu2();
			mainmenu.menuout();
			break;
		case '3':
			mainmenu.menu3();
			mainmenu.menuout();
			break;
		case '4':
			mainmenu.menu4();
			mainmenu.menuout();
			break;
		case '5':
			mainmenu.save_data();		//프로그램 종류시 데이터를 text파일에 저장
			return 0;
		default:
			cout << "번호를 다시 입력하세요:";
			break;
		}
	}
}