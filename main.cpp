#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <random>
#include <stdio.h>
#include "graph.h"
#include "shortest_path.h"

std::random_device rd;
std::mt19937 gen(rd());

void buildPath(Graph& vert, int unit, int width, int height) {
    //Постройка путей в зависимости от нахождения центра клетки (рассматриваем положение от рамки(зоны)
    //зная, что рамка начинается от левого верхнего края Point(unit, unit),
    //до правого нижнего края Point(unit * (win_width / unit), unit * (win_height/unit))

    //Причем передобавления одних и тех же путей и вершин в методе add_edge не будет, так как проверяется условие, 
    //есть ли там вершины и сами пути или совпадает ли вес ребра прошлый с текущим заносимым.

    for (auto& elem : vert.get_vertices()) {
        //Сверху граница
        if (elem.y - unit * 0.5 == unit) {
            //left
            if (elem.x - unit * 0.5 == unit) {
                vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
                vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
                vert.add_edge(elem, Point(elem.x + unit, elem.y + unit), 1.4);//right-down
            }
            //right
            else if (elem.x + unit * 0.5 == unit * (width / unit)) {
                vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
                vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
                vert.add_edge(elem, Point(elem.x - unit, elem.y + unit), 1.4);//left-down
            }
            else {
                vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
                vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
                vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
                vert.add_edge(elem, Point(elem.x - unit, elem.y + unit), 1.4);//left-down
                vert.add_edge(elem, Point(elem.x + unit, elem.y + unit), 1.4);//right-down

            }
        }
        //Справа граница
        else if (elem.x + unit * 0.5 == unit * (width / unit)) {
            //down
            if (elem.y + unit * 0.5 == unit * (height / unit)) {
                vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
                vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
                vert.add_edge(elem, Point(elem.x - unit, elem.y - unit), 1.4);//left-up
            }
            else {
                vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
                vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
                vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
                vert.add_edge(elem, Point(elem.x - unit, elem.y - unit), 1.4);//left-up
                vert.add_edge(elem, Point(elem.x - unit, elem.y + unit), 1.4);//left-down

            }

        }
        //Снизу граница
        else if (elem.y + unit * 0.5 == unit * (height / unit)) {
            //left
            if (elem.x - unit * 0.5 == unit) {
                vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
                vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
                vert.add_edge(elem, Point(elem.x + unit, elem.y - unit), 1.4);//right-up
            }
            else {
                vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
                vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
                vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
                vert.add_edge(elem, Point(elem.x - unit, elem.y - unit), 1.4);//left-up
                vert.add_edge(elem, Point(elem.x + unit, elem.y - unit), 1.4);//right-up

            }
        }
        //Слева граница
        else if (elem.x - unit * 0.5 == unit) {
            vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
            vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
            vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
            vert.add_edge(elem, Point(elem.x + unit, elem.y - unit), 1.4);//right-up
            vert.add_edge(elem, Point(elem.x + unit, elem.y + unit), 1.4);//right-down

        }
        else {
            vert.add_edge(elem, Point(elem.x, elem.y - unit), 1.0);//up
            vert.add_edge(elem, Point(elem.x, elem.y + unit), 1.0);//down
            vert.add_edge(elem, Point(elem.x + unit, elem.y), 1.0);//right
            vert.add_edge(elem, Point(elem.x - unit, elem.y), 1.0);//left
            vert.add_edge(elem, Point(elem.x + unit, elem.y - unit), 1.4);//right-up
            vert.add_edge(elem, Point(elem.x + unit, elem.y + unit), 1.4);//right-down
            vert.add_edge(elem, Point(elem.x - unit, elem.y - unit), 1.4);//left-up
            vert.add_edge(elem, Point(elem.x - unit, elem.y + unit), 1.4);//left-down

        }


    }
}

std::vector<sf::Vertex> genRectangles(Graph& vertices, int unit, int width, int height, Point start, Point finish, FILE *fout) {

    int amount_of_rectangles = 0.15 * (width/unit)*(height/unit);
    std::uniform_int_distribution <int> rand(0, vertices.get_vertices().size()-1);
    std::vector<sf::Vertex> rects_point;

    for (int i = 0; i < amount_of_rectangles; i++) {

        int random = rand(gen);

        if (vertices.get_vertices()[random] != start && vertices.get_vertices()[random] != finish) {

            //Центр препятствия, от которого формируем края клетки для
            //отрисовки препятствий в массив points_rect.
            Point delete_center = vertices.get_vertices()[random];
            Rectangle rect = Rectangle().atCenter(delete_center, unit, unit);
            std::array<Point, 4> points_rect = rect.getPoints();

            //Избавление от всех дорог к центру клетки-препятствия
            //чтобы никак не пересечь ее, а следовательно, в алгоритме поиска пути "объезжать" препятствия
            vertices.remove_vertex(delete_center); 

            for (int j = 0; j < 4; j++) {
                rects_point.push_back(sf::Vertex(sf::Vector2f(points_rect[j].x, points_rect[j].y)));
            }

            int x_num_rect = (delete_center.x - (unit + unit * 0.5)) / unit + 1;
            int y_num_rect = (delete_center.y - (unit + unit * 0.5)) / unit + 1;
            fprintf(fout, "%d %d\n", x_num_rect, y_num_rect);
        }
    }
    return rects_point;
}

std::vector<sf::Vertex> chooseRectangles(Graph& vertices, int unit, int height, FILE *f) {
    std::vector<Point> delete_points;
    std::vector<sf::Vertex> rects_point;
    int num_x, num_y;

    while (!feof(f)) {
        fscanf(f, "%d%d", &num_x, &num_y);
        //По аналогии получения элемента в двумерном массиве: mas[i*k + j]
        //i номер строки с нуля, j - номер столбца, k - кол-во столбцов
        delete_points.push_back(vertices.get_vertices()[(num_x - 1) * ((height - unit) / unit) + (num_y - 1)]);

    }
    for (auto& elem : delete_points) {
        Rectangle rect = Rectangle().atCenter(elem, unit, unit);
        std::array<Point, 4> points_rect = rect.getPoints();

        //Избавление от всех дорог к центру клетки-препятствия
        //чтобы никак не пересечь ее, а следовательно, в алгоритме поиска пути "объезжать" препятствия
        vertices.remove_vertex(elem);
        for (int j = 0; j < 4; j++) {
            rects_point.push_back(sf::Vertex(sf::Vector2f(points_rect[j].x, points_rect[j].y)));
        }
    }
    return rects_point;
}

std::vector<sf::Vertex> deleteRectangles(Graph &vertices, std::vector<sf::Vertex> pointsOfRects, Rectangle rect, Point deletePoint, int unit, FILE *fout) {
    std::array<Point, 4> points_rect = rect.getPoints();
    vertices.remove_vertex(deletePoint);
    for (int i = 0; i < 4; i++) {
        pointsOfRects.push_back(sf::Vertex(sf::Vector2f(points_rect[i].x, points_rect[i].y)));
    }

    int x_num_rect = (deletePoint.x - (unit + unit * 0.5)) / unit + 1;
    int y_num_rect = (deletePoint.y - (unit + unit * 0.5)) / unit + 1;
    fprintf(fout, "%d %d\n", x_num_rect, y_num_rect);

    return pointsOfRects;
}

sf::VertexArray drawPathMode(std::vector<Point> short_path) {
    sf::VertexArray result_line_path(sf::LineStrip, short_path.size());
    for (int i = 0; i < short_path.size(); i++) {
        result_line_path[i].position = sf::Vector2f(short_path[i].x, short_path[i].y);
        result_line_path[i].color = sf::Color::Red;
    }
    return result_line_path;
}

int main()
{
    
    FILE* f, *fout;
    int win_width = 920, win_height = 920, unit = 40;
    int snum_x, snum_y, fnum_x, fnum_y, mode;
    std::vector<sf::RectangleShape> rects;
    std::vector<sf::Vertex> topleftPoints, points_of_genRectangles;
    std::vector<Point> short_path;
    sf::VertexArray lines(sf::LineStrip, 5);
    Graph vertices;

    fopen_s(&f, "settings.txt", "r");
    fopen_s(&fout, "output.txt", "w");

    fscanf(f, "%d%d", &win_width, &win_height);
    fscanf(f, "%d", &unit);
    fscanf(f, "%d%d", &snum_x, &snum_y);
    fscanf(f, "%d%d", &fnum_x, &fnum_y);
    fscanf(f, "%d", &mode);

    fprintf(fout, "%d %d\n", win_width, win_height);
    fprintf(fout, "%d\n", unit);
    fprintf(fout, "%d %d\n", snum_x, snum_y);
    fprintf(fout, "%d %d\n", fnum_x, fnum_y);
    fprintf(fout, "%d\n", mode);

    sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Robopath");
    
   
    //Занесем крайние точки на основе крайних координат при разбиении на клетки для построения рамки (зоны).
    lines[0].position = sf::Vector2f(unit, unit);
    lines[1].position = sf::Vector2f(unit*(win_width/unit), unit);
    lines[2].position = sf::Vector2f(unit * (win_width / unit), unit * (win_height/unit));
    lines[3].position = sf::Vector2f(unit, unit * (win_height / unit));
    lines[4].position = lines[0].position;

    //Работа с текстом
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text_info("Click Enter. Start(yellow), Finish(blue)", font, unit/2);
    text_info.setFillColor(sf::Color::Green);
    text_info.setStyle(sf::Text::Bold);
    
    sf::Text text_hasnotShortPath("There is not the path", font, unit);
    text_hasnotShortPath.setFillColor(sf::Color::Red);
    text_hasnotShortPath.setStyle(sf::Text::Bold);
    //text_hasnotShortPath.setPosition(sf::Vector2f(win_width / 2, win_height / 2));



    //Формируем центровые точки клеток, на которые мы разбили, в массив вершин.
    for (int y = unit; y <= win_height; y += unit) {
        for (int x = unit; x <= win_width; x += unit) {
            topleftPoints.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Green));
            //Если мы скраю (последняя точка по горизонтали или вертикали,
            //то не формировать прямоугольник, который будет за пределами
            if (x + unit <= win_width && y + unit <= win_height) {
                Rectangle rect(x, y, unit, unit);
                vertices.add_vertex(rect.centerPoint());
                
            }
        }
    }

    Point topLeft(unit + unit / 2, unit + unit / 2);
    //Координаты точек старта и финиша должны лежать на одном из центров клеток, на которые мы разбиваем. 
    Point start(topLeft.x + unit * (snum_x - 1), topLeft.y + unit * (snum_y - 1));
    Point finish(topLeft.x + unit * (fnum_x - 1), topLeft.y + unit * (fnum_y - 1));
    //

    //Отметить центровые клетки старта и финиша для видимости в виде окружностей.
    float radius = 4.f;
    sf::CircleShape start_c(radius), finish_c(radius);
    start_c.setPosition(sf::Vector2f(start.x - radius, start.y - radius));
    finish_c.setPosition(sf::Vector2f(finish.x - radius, finish.y - radius));
    start_c.setFillColor(sf::Color::Yellow);
    finish_c.setFillColor(sf::Color::Blue);


    //Строим дороги для центров клеток, вертикаль/горизонталь = 1.0
    //диагональ = sqrt(2.0) = 1.4.
    buildPath(vertices, unit, win_width, win_height);  

    //Препятсвия расставляем, возвращая массив точек этих препятствий.
    //mode == 0 - случайные препятствия
    //mode == 1 - прописанные препятствия из файла по координатам центральных точек этих препятствий.
    //mode == 2 - левым кликом мыши кликаем для установки препятствий (после кнопки Enter произойдет построение пути и отрисовка).
    if (mode == 0) points_of_genRectangles = genRectangles(vertices, unit, win_width, win_height, start, finish, fout);
    else if (mode == 1) points_of_genRectangles = chooseRectangles(vertices, unit, win_height, f);
    

    //Алгоритм Дейкстры
    short_path = shortest_path(vertices, start, finish, unit);

    bool hasnotpath = false;
    if (short_path == std::vector<Point>()) hasnotpath = true; 

    sf::VertexArray result_line_path(sf::LineStrip, short_path.size());
    result_line_path = drawPathMode(short_path);
    
    
    
    bool draw = false;
 
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                draw = true;

            if (event.type == sf::Event::MouseButtonPressed) {
                //При нажатии левой кнопки мыши считывается положение в точку
                //После для центральных вершин клеток проверяется, пересекается ли точка нажатия с 
                //текущей центральной клеткой. Если да, то удаляется из графа пути к текущей центр. клетке и она сама.
                //В вектор точек препятствий заносятся четыре точки пересекаемой клетки для дальнейшей отрисовки.
                if (event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    Point pos(position.x, position.y - unit);

                    for (auto& center_point : vertices.get_vertices()) {
                        Rectangle currentRect = Rectangle().atCenter(center_point, unit, unit);
                        if (currentRect.centerPoint() != start && currentRect.centerPoint() != finish) {
                            if (currentRect.collidesWith(pos)) {
                                points_of_genRectangles = deleteRectangles(vertices, points_of_genRectangles, currentRect, center_point, unit, fout);
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        window.draw(lines);
        window.draw(&topleftPoints[0], topleftPoints.size(), sf::Points);//Разбиения на клетки (отрисовка цветных краевых точек клеток)
        window.draw(&points_of_genRectangles[0], points_of_genRectangles.size(), sf::Quads);//Препятствия
        window.draw(text_info);
        window.draw(start_c);
        window.draw(finish_c);

        if (mode == 1 || mode == 0) {
            if (!hasnotpath && draw) window.draw(result_line_path);
            else if (hasnotpath) window.draw(text_hasnotShortPath);
        }

        else if (mode == 2 && draw) {
            short_path = shortest_path(vertices, start, finish, unit);
            if (short_path == std::vector<Point>()) hasnotpath = true;

            if (!hasnotpath) {
                result_line_path = drawPathMode(short_path);
                window.draw(result_line_path);
            }
            else window.draw(text_hasnotShortPath);
        }
        window.display();
    }
    fclose(f);
    fclose(fout);
    return 0;
}