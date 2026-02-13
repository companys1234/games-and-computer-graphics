#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <vector>

// Структура для точки
struct Point {
    float x;
    float y;
};

// Функция для рисования осей координат
void draw_axes() {
    glLineWidth(1);
    glBegin(GL_LINES);

    // Ось X (красная)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);

    // Ось Y (зеленая)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);

    glEnd();
}

// Функция для рисования точек и соединения их линиями
void draw_points_and_lines(const std::vector<Point>& points) {
    if (points.empty()) return;

    // Рисуем соединительные линии
    glLineWidth(2.0f);
    glColor3f(0.3f, 0.6f, 1.0f); // Голубой цвет для линий
    glBegin(GL_LINE_STRIP); // LINE_STRIP соединяет точки последовательно

    for (const auto& point : points) {
        glVertex2f(point.x, point.y);
    }

    glEnd();

    // Рисуем точки поверх линий
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // Белые точки

    for (const auto& point : points) {
        glVertex2f(point.x, point.y);
    }

    glEnd();

    // Добавляем маленькие кружки вокруг точек для лучшей видимости (опционально)
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f); // Черные точки в центре

    for (const auto& point : points) {
        glVertex2f(point.x, point.y);
    }

    glEnd();

    // Вывод координат в консоль
    printf("Points connected with lines:\n");
    for (size_t i = 0; i < points.size(); i++) {
        printf("  Point %zu: (%.2f, %.2f)\n", i, points[i].x, points[i].y);
    }
    printf("\n");
}

// Функция для рисования с замыканием контура (последняя точка соединяется с первой)
void draw_closed_contour(const std::vector<Point>& points) {
    if (points.size() < 3) return; // Для замкнутого контура нужно минимум 3 точки

    // Рисуем замкнутый контур
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.5f, 0.0f); // Оранжевый цвет
    glBegin(GL_LINE_LOOP); // LINE_LOOP автоматически соединяет последнюю точку с первой

    for (const auto& point : points) {
        glVertex2f(point.x, point.y);
    }

    glEnd();

    // Рисуем точки
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (const auto& point : points) {
        glVertex2f(point.x, point.y);
    }

    glEnd();
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 800, "Points Connected with Lines", NULL, NULL);

    if (!window) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Создаем массив точек для визуализации (упорядоченных для соединения)
    std::vector<Point> points = {
        {-0.8f, -0.6f},  // Точка 0
        {-0.5f, 0.2f},   // Точка 1
        {0.0f, 0.8f},    // Точка 2
        {0.3f, 0.4f},    // Точка 3
        {0.7f, 0.1f},    // Точка 4
        {0.5f, -0.3f},   // Точка 5
        {0.1f, -0.5f}   // Точка 6// Точка 7
    };

    // Второй набор точек для демонстрации замкнутого контура
    std::vector<Point> closedPoints = {
        {0.3f, 0.3f},
        {0.6f, 0.0f},
        {0.3f, -0.3f},
        {-0.3f, -0.3f},
        {-0.6f, 0.0f},
        {-0.3f, 0.3f}
    };

    printf("\n=== Points Visualization with Lines ===\n");
    printf("Blue lines: points connected sequentially\n");
    printf("Orange lines: closed contour (last point connects to first)\n");
    printf("White dots: point locations\n\n");

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Настройка проекции
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Рисуем оси координат
        draw_axes();

        // Рисуем первый набор точек с соединительными линиями
        draw_points_and_lines(points);

        // Рисуем второй набор как замкнутый контур (смещаем немного вправо для наглядности)
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f); // Можно изменить позицию для второго набора
        draw_closed_contour(closedPoints);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}