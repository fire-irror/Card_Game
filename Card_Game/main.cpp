#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int arr_cnt = 4;

struct Card {
	RectangleShape sprite;
	int id_i;    //카드 번호
	int id_j;               
	int type;
	int is_clicked;		//현재 뒤집혀진 카드인지
	int is_cleared;		//정답을 맞춘 카드인지
};

void swap_card(struct Card *c1, struct Card *c2) {
	struct Card temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}
int main()
{
	int CARD_W = 150;
	int CARD_H = 200;

	RenderWindow window(VideoMode(1200, 800), "Card Game!");
	window.setFramerateLimit(60);

	Vector2i mouse_pos;
	int flipped_num = 0;     //현재 뒤집혀진 카드의 개수

	long start_time;	//시작시각
	long spent_time;	//현재시각
	long delay_time;	//다시 ?로 뒤집혀지지 않도록 딜레이를 줌 딜레이가 시작되는 시각 

	Texture t[8 + 1];
	t[0].loadFromFile("./resource/images/0.png");	//카드 뒷면
	t[1].loadFromFile("./resource/images/1.png");
	t[2].loadFromFile("./resource/images/2.png");
	t[3].loadFromFile("./resource/images/3.png");
	t[4].loadFromFile("./resource/images/4.png");
	t[5].loadFromFile("./resource/images/5.png");
	t[6].loadFromFile("./resource/images/6.png");
	t[7].loadFromFile("./resource/images/7.png");
	t[8].loadFromFile("./resource/images/8.png");

	//폰트
	Font font;
	font.loadFromFile("c:/Windows/Fonts/Arial.ttf");

	//텍스트
	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct Card compare_card;	//첫번째로 뒤집힌 카드
	struct Card cards[arr_cnt][arr_cnt];
	int n = 0;
	for (int i = 0; i < arr_cnt; i++)
	{
		for (int j = 0; j < arr_cnt; j++)
		{
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setTexture(&t[0]); // 뒤집기 전
			cards[i][j].type = 1 + n / 2;
			cards[i][j].id_i = i;
			cards[i][j].id_j = j;
			cards[i][j].is_clicked = 0;
			cards[i][j].is_cleared = 0;
			n++;
		}
	}

	//카드 100번 섞기
	for (int i = 0; i < 100; i++) {
		swap_card(&cards[rand() % arr_cnt][rand() % arr_cnt], &cards[rand() % arr_cnt][rand() % arr_cnt]);
	}

	//idx에 맞춰서 id값. 위치 조정
	for (int i = 0; i < arr_cnt; i++) {
		for (int j = 0; j < arr_cnt; j++) {
			cards[i][j].id_i = i;
			cards[i][j].id_j = j;
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
		}
	}
	start_time = clock();
	delay_time = start_time;



	while (window.isOpen())
	{
		mouse_pos = Mouse::getPosition(window);
		spent_time = clock() - start_time;

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
				//한 번 누르면 한 번만 적용
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left)
				{
					for (int i = 0; i < arr_cnt; i++)
					{
						for (int j = 0; j < arr_cnt; j++)
						{
							if (flipped_num < 2) {

								//마우스 위치가 cards[i][j]의 위치에 해당한다면?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
								{
									//뒤집혀지지 않은 카드만 뒤집겠다. 
									if (cards[i][j].is_clicked == 0)
									{
										cards[i][j].is_clicked = 1;
										flipped_num++;

										//뒤집혀진 카드가 한 개라면?
										if (flipped_num == 1) {
											compare_card = cards[i][j];
										}
										//두 카드가 같은 종류면?
										else if (flipped_num == 2) {
											if (compare_card.type == cards[i][j].type)
											{
												cards[i][j].is_cleared = 1;
												cards[compare_card.id_i][compare_card.id_j].is_cleared = 1;
											}
											delay_time = spent_time;
										}
										else
										{
											delay_time = spent_time;                                              
										}
									}
								}
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < arr_cnt; i++)
		{
			for (int j = 0; j < arr_cnt; j++)
			{
				//클릭이 된 상태거나 정답을 맞춘 카드라면 
				if (cards[i][j].is_clicked==1 || cards[i][j].is_cleared==1)
				{
					// 그림이 있는 sprite로 변경(카드를 뒤집겠다)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				}
				else {
					//카드는 ?? 상태
					cards[i][j].sprite.setTexture(&t[0]);
				}
			}
		}
		//뒤집힌 카드가 2개라면 
		if (flipped_num == 2)
		{
			//두 카드가 뒤집힌지 1초 이내라면
			if (spent_time - delay_time < 1000) {
			}
			else {
				for (int i = 0; i < arr_cnt; i++)
				{
					for (int j = 0; j < arr_cnt; j++)
					{
						cards[i][j].is_clicked = 0;
						flipped_num = 0;
					}
				}
			}
		}

		sprintf(info, "(%d, %d) / CLICKS : %d\n", mouse_pos.x, mouse_pos.y, spent_time / 1000);
		text.setString(info);

		window.clear(Color::Black);
		for (int i = 0; i < arr_cnt; i++)
		{
			for (int j = 0; j < arr_cnt; j++)
			{
				window.draw(cards[i][j].sprite);
			}
		}

		window.draw(text);

		window.display();
	}

	return 0;
}