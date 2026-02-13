#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <list>
void draw_lines(float alpha) {
	glPushMatrix();
	glRotatef(alpha, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Выбираем матрицу модели (обычно это делается один раз при инициализации)
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Сбрасываем изменения от предыдущего кадра

    // 2. Настраиваем масштаб (X увеличим в 0.5 раза, Y в 0.5 раза)
    // Если поставить 100.0f, ваши точки x=10 уйдут далеко за границы видимости
    //glScalef(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glColor3f(1, 0, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glColor3f(0, 1, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glColor3f(0, 1, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();
	glPopMatrix();

}


// Весь код ниже должен быть ВНУТРИ функции
void display() {


    // 3. Начинаем рисование
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex2f(-0.5f, -0.5f);
    glVertex2f(10.0f, 0.0f);
    glVertex2f(20.0f,0.0f);
    glVertex2f(30.0f, 0.8f);
    glEnd();
}

int main() {
    // 1. Инициализация
    if (!glfwInit()) return -1;
    std::list<int> numbers = { 10, 20, 30, 40 };

    // 2. Создание окна (вот здесь объявляется 'window')
    GLFWwindow* window = glfwCreateWindow(1000, 1500, "My Window", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // 3. Сделать контекст окна текущим
    glfwMakeContextCurrent(window);

    // Главный цикл
    while (!glfwWindowShouldClose(window)) { // Теперь 'window' объявлен и виден
        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // ВАШ КОД ОТРИСОВКИ
        glLineWidth(2);
        //display();
        draw_lines(90);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}