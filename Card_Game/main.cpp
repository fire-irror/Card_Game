#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int S = 4;    //카드 행 열

struct Card {
    RectangleShape sprite;
    int id;    //카드 번호

}; 

int main(void) {

    int CARD_W = 150;
    int CARD_H = 200;

    Texture t[8 + 1];
    t[0].loadFromFile("./resource/images/0.png");
    t[1].loadFromFile("./resource/images/1.png");
    t[2].loadFromFile("./resource/images/2.png");
    t[3].loadFromFile("./resource/images/3.png");
    t[4].loadFromFile("./resource/images/4.png");
    t[5].loadFromFile("./resource/images/5.png");
    t[6].loadFromFile("./resource/images/6.png");
    t[7].loadFromFile("./resource/images/7.png");
    t[8].loadFromFile("./resource/images/8.png");

    RenderWindow window(VideoMode(1200, 800), "Card_Game");// 화면 크기, 제목
    window.setFramerateLimit(60);// 60초마다 한 번씩 돌아간다

    // f : 소수
    // Vector2i : 정수
    Vector2i mouse_pos;// 마우스 좌표
    int click_cnt = 0;    //마우스 누른 횟수
    int n = 0;

    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");

    Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(Color::White);
    text.setPosition(0, 0);
    char info[40];

    struct Card cards[S][S];
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
            cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
            cards[i][j].sprite.setTexture(&t[1+n/2]);
            n++;
        }
    }

    while (window.isOpen())
    {
        mouse_pos = Mouse::getPosition(window);// 마우스 좌표 실시간으로 받기

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left) {
                    click_cnt++;
                }
            }
        }
        sprintf(info, "(%d, %d) click %d\n"
            , mouse_pos.x, mouse_pos.y, click_cnt);// 마우스 x값 y값 출력
        text.setString(info);

        window.clear(Color::Black);
        for (int i = 0; i < S; i++) {
            for (int j = 0; j < S; j++) {
                window.draw(cards[i][j].sprite);
            }
        }
        window.draw(text);

        window.display();
    }

    return 0;
}