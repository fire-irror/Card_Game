#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int arr_cnt = 4;

struct Card {
	RectangleShape sprite;
	int id_i;    //ī�� ��ȣ
	int id_j;               
	int type;
	int is_clicked;		//���� �������� ī������
	int is_cleared;		//������ ���� ī������
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
	int flipped_num = 0;     //���� �������� ī���� ����

	long start_time;	//���۽ð�
	long spent_time;	//����ð�
	long delay_time;	//�ٽ� ?�� ���������� �ʵ��� �����̸� �� �����̰� ���۵Ǵ� �ð� 

	Texture t[8 + 1];
	t[0].loadFromFile("./resource/images/0.png");	//ī�� �޸�
	t[1].loadFromFile("./resource/images/1.png");
	t[2].loadFromFile("./resource/images/2.png");
	t[3].loadFromFile("./resource/images/3.png");
	t[4].loadFromFile("./resource/images/4.png");
	t[5].loadFromFile("./resource/images/5.png");
	t[6].loadFromFile("./resource/images/6.png");
	t[7].loadFromFile("./resource/images/7.png");
	t[8].loadFromFile("./resource/images/8.png");

	//��Ʈ
	Font font;
	font.loadFromFile("c:/Windows/Fonts/Arial.ttf");

	//�ؽ�Ʈ
	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct Card compare_card;	//ù��°�� ������ ī��
	struct Card cards[arr_cnt][arr_cnt];
	int n = 0;
	for (int i = 0; i < arr_cnt; i++)
	{
		for (int j = 0; j < arr_cnt; j++)
		{
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setTexture(&t[0]); // ������ ��
			cards[i][j].type = 1 + n / 2;
			cards[i][j].id_i = i;
			cards[i][j].id_j = j;
			cards[i][j].is_clicked = 0;
			cards[i][j].is_cleared = 0;
			n++;
		}
	}

	//ī�� 100�� ����
	for (int i = 0; i < 100; i++) {
		swap_card(&cards[rand() % arr_cnt][rand() % arr_cnt], &cards[rand() % arr_cnt][rand() % arr_cnt]);
	}

	//idx�� ���缭 id��. ��ġ ����
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
				//�� �� ������ �� ���� ����
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left)
				{
					for (int i = 0; i < arr_cnt; i++)
					{
						for (int j = 0; j < arr_cnt; j++)
						{
							if (flipped_num < 2) {

								//���콺 ��ġ�� cards[i][j]�� ��ġ�� �ش��Ѵٸ�?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
								{
									//���������� ���� ī�常 �����ڴ�. 
									if (cards[i][j].is_clicked == 0)
									{
										cards[i][j].is_clicked = 1;
										flipped_num++;

										//�������� ī�尡 �� �����?
										if (flipped_num == 1) {
											compare_card = cards[i][j];
										}
										//�� ī�尡 ���� ������?
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
				//Ŭ���� �� ���°ų� ������ ���� ī���� 
				if (cards[i][j].is_clicked==1 || cards[i][j].is_cleared==1)
				{
					// �׸��� �ִ� sprite�� ����(ī�带 �����ڴ�)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				}
				else {
					//ī��� ?? ����
					cards[i][j].sprite.setTexture(&t[0]);
				}
			}
		}
		//������ ī�尡 2����� 
		if (flipped_num == 2)
		{
			//�� ī�尡 �������� 1�� �̳����
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