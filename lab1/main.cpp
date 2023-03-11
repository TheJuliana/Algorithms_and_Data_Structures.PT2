/*Светлаковой УВ 2-ПМИ
Нахождение выпуклой оболочки множества точек
В данной задаче требуется ввести N точек своими координатами (x,y).
Затем требуется определить, существует ли выпуклая оболочка заданного множества точек.
При этом можно использовать:
или алгоритм Грэхема, или алгоритм Джарвиса, или метод «разделяй и властвуй»
*/


//Для определения существования выпуклой оболоки воспользуемся алгоритмом Грэхема
// и найдем минимальную выпуклую оболочку (если сущетсвует минимальная, то существует и любая)
#include <iostream>
#include <vector>
#include <algorithm>
#include <valarray>

struct Point{
    int x;
    int y;
};

//определяет, с какой стороны от вектора АВ находится точка С (при <0 - правая, >0 - левая сторона)
int Rotate(const Point &A, const Point &B, const Point &C) {
    return (B.x-A.x)*(C.y-B.y)-(B.y-A.x)*(C.x-B.x);
}

int dist(const Point &A, const Point &B) {
    return sqrt(pow((B.x-A.x),2)+pow((B.y-A.y),2));
}

bool check(const int &A, const int &B, const int &C, const std::vector<Point> &points) {
    int tmp = Rotate(points[A], points[B], points[C]);
    if (tmp < 0 || tmp == 0 && dist(points[A], points[C]) < dist(points[A], points[B])) return true;
    return false;
}

void PrintMasP(std::vector<Point> &points) {
    std::cout << "Points:" << std::endl;
    for (auto& i : points) {
        std::cout << i.x << " " << i.y << std::endl;
    }
}

void PrintMasI(std::vector<int> &points) {
    std::cout << "Points:" << std::endl;
    for (auto& i : points) {
        std::cout << i << std::endl;
    }
}

int main() {
    std::vector<Point> points;
    Point point;
    while (std::cin) {
        std::cout << "Введите координаты точки через пробел или любой символ для завершения ввода:" << std::endl;
        std::cin >> point.x >> point.y;
        points.push_back(point);
    }
    points.pop_back();

    if (points.size() <= 2) {
        std::cout << "Недостаточно точек для построения выпуклой оболочки" << std::endl;
        return 0;
    }
    //PrintMasP(points); <--для отладки

    std::vector<int> points_indexes; //массив индеков точек в массиве points
    for (int i = 0; i < points.size(); i++) {
        points_indexes.push_back(i);
    }
    //PrintMasI(points_indexes); <--для отладки

    //Находим минимальную точку (самая нижняя левая) и ставим ее на первом месте в массиве индексов
    for(int i = 1; i<points.size(); i++) {
        if ((points[points_indexes[i]].y < points[points_indexes[0]].y) || ((points[points_indexes[i]].y == points[points_indexes[0]].y)&&(points[points_indexes[i]].x < points[points_indexes[0]].y))) {
            std::swap(points_indexes[i], points_indexes[0]);
        }
    }
    //PrintMasI(points_indexes); <--для отладки
    //Сортируем (вставками) по возрастанию полярного угла относительно минимальной точки
    for (int i = 2; i < points.size(); i++) {
        int j=i;
        while ((j>1 && check(points_indexes[0], points_indexes[j - 1], points_indexes[j], points)))  {
            std::swap(points_indexes[j-1], points_indexes[j]);
            j--;
        }
    }


    for (int x : points_indexes) std::cout << x << " ";
    std::cout << std::endl;

    //Удаляем те вершины, в которых выполняется правый поворот
    std::vector<int> stack;
    stack.push_back(points_indexes[0]); //кладем в стек первые две вершины
    stack.push_back(points_indexes[1]);

    //std::cout << "!"<< stack.size() << std::endl; <--для отладки
    for (int i = 2; i < points.size(); i++) {
        //отслеживаем направление поворота с точки зрения последних двух вершин в стеке
        while (stack.size() >= 2 && Rotate(points[stack[stack.size()-2]], points[stack[stack.size()-1]], points[points_indexes[i]])<=0) {
            stack.pop_back();
            //std::cout << "!stack popped "<< stack.size() << std::endl; <--для отладки
        }
        stack.push_back(points_indexes[i]); //поворот положительный - заносим точку в стек
        //std::cout << "!stack pushed"<< stack.size() << std::endl; <--для отладки
    }
    if (stack.size() <= 2) {
        std::cout << "Выпуклой оболочки не существует" << std::endl;
        return 0;
    }
    //std::cout << "!"<< stack.size() << std::endl; <--для отладки
    //PrintMasI(stack); <--для отладки
    //std::cout << "!"<< stack.size() << std::endl; <--для отладки
    std::cout << "Выпуклая оболочка существует и минимальная состоит из точек:" << std::endl;
    for (auto i : stack) {
        std::cout << points[i].x << " " << points[i].y << std::endl;
    }

    return 0;
}
