#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define mapW 10
#define mapH 10

typedef struct {
    bool mine;
    bool flag;
    bool open;
    int maround;
} Cell;

Cell map[mapH][mapW];
int mines = 20;
int closedCells = mapW * mapH;
bool gameOver = false;
bool firstClick = true;
float cellSize = 0.1f;
GLFWwindow* window;

const float colors[9][3] = {
    {0.0, 0.0, 1.0},    // 1 - синий
    {0.0, 0.5, 0.0},    // 2 - зеленый
    {1.0, 0.0, 0.0},    // 3 - красный
    {0.0, 0.0, 0.5},    // 4 - темно-синий
    {0.5, 0.0, 0.0},    // 5 - темно-красный
    {0.0, 0.5, 0.5},    // 6 - бирюзовый
    {0.0, 0.0, 0.0},    // 7 - черный
    {0.5, 0.5, 0.5},    // 8 - серый
    {0.7, 0.7, 0.7}     // 0 - светло-серый
};

void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void drawMine(float x, float y) {
    // Центр мины
    glColor3f(0, 0, 0);
    drawCircle(x + cellSize / 2, y + cellSize / 2, 0.03, 20);

    // Лучи мины
    glBegin(GL_LINES);
    for (int i = 0; i < 360; i += 45) {
        float angle = i * 3.14159f / 180;
        glVertex2f(x + cellSize / 2 + 0.03 * cos(angle),
            y + cellSize / 2 + 0.03 * sin(angle));
        glVertex2f(x + cellSize / 2 + 0.04 * cos(angle),
            y + cellSize / 2 + 0.04 * sin(angle));
    }
    glEnd();
}

void drawFlag(float x, float y) {
    // Флагшток
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(x + cellSize / 2, y + cellSize / 4);
    glVertex2f(x + cellSize / 2, y + 3 * cellSize / 4);
    glEnd();

    // Флаг
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + cellSize / 2, y + 3 * cellSize / 4);
    glVertex2f(x + 3 * cellSize / 4, y + cellSize / 2);
    glVertex2f(x + cellSize / 2, y + cellSize / 2);
    glEnd();
}

void drawNumber(int num, float x, float y) {
    if (num == 0) return;

    glColor3fv(colors[num - 1]);

    // Простая векторная отрисовка цифр
    float size = cellSize * 0.6;
    float left = x + cellSize / 2 - size / 2;
    float bottom = y + cellSize / 2 - size / 2;

    glLineWidth(2.0);
    switch (num) {
    case 1:
        glBegin(GL_LINES);
        glVertex2f(left + size / 2, bottom);
        glVertex2f(left + size / 2, bottom + size);
        glEnd();
        break;
    case 2:
        glBegin(GL_LINE_STRIP);
        glVertex2f(left, bottom);
        glVertex2f(left + size, bottom);
        glVertex2f(left + size, bottom + size / 2);
        glVertex2f(left, bottom + size / 2);
        glVertex2f(left, bottom + size);
        glVertex2f(left + size, bottom + size);
        glEnd();
        break;
    case 3:
        glBegin(GL_LINE_STRIP);
        glVertex2f(left, bottom);
        glVertex2f(left + size, bottom);
        glVertex2f(left + size, bottom + size / 2);
        glVertex2f(left, bottom + size / 2);
        glVertex2f(left + size, bottom + size / 2);
        glVertex2f(left + size, bottom + size);
        glVertex2f(left, bottom + size);
        glEnd();
        break;
    default:
        // Для остальных цифр рисуем кружок
        drawCircle(x + cellSize / 2, y + cellSize / 2, 0.02, 10);
    }
    glLineWidth(1.0);
}

void drawCell(int x, int y) {
    float px = x * cellSize;
    float py = y * cellSize;

    if (!map[x][y].open) {
        // Закрытая ячейка
        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_QUADS);
        glVertex2f(px, py);
        glVertex2f(px + cellSize, py);
        glVertex2f(px + cellSize, py + cellSize);
        glVertex2f(px, py + cellSize);
        glEnd();

        // Обводка для 3D эффекта
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(px, py);
        glVertex2f(px + cellSize, py);
        glVertex2f(px, py);
        glVertex2f(px, py + cellSize);
        glEnd();

        glColor3f(0.4, 0.4, 0.4);
        glBegin(GL_LINES);
        glVertex2f(px + cellSize, py);
        glVertex2f(px + cellSize, py + cellSize);
        glVertex2f(px, py + cellSize);
        glVertex2f(px + cellSize, py + cellSize);
        glEnd();

        if (map[x][y].flag) {
            drawFlag(px, py);
        }
    }
    else {
        // Открытая ячейка
        glColor3f(0.9, 0.9, 0.9);
        glBegin(GL_QUADS);
        glVertex2f(px, py);
        glVertex2f(px + cellSize, py);
        glVertex2f(px + cellSize, py + cellSize);
        glVertex2f(px, py + cellSize);
        glEnd();

        // Обводка открытой ячейки
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(px, py);
        glVertex2f(px + cellSize, py);
        glVertex2f(px + cellSize, py + cellSize);
        glVertex2f(px, py + cellSize);
        glEnd();

        if (map[x][y].mine) {
            drawMine(px, py);
        }
        else if (map[x][y].maround > 0) {
            drawNumber(map[x][y].maround, px, py);
        }
    }
}

void calculateMinesAround() {
    for (int x = 0; x < mapH; x++) {
        for (int y = 0; y < mapW; y++) {
            if (!map[x][y].mine) {
                int count = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < mapH && ny >= 0 && ny < mapW) {
                            if (map[nx][ny].mine) count++;
                        }
                    }
                }
                map[x][y].maround = count;
            }
        }
    }
}

void game_new() {
    for (int x = 0; x < mapH; x++) {
        for (int y = 0; y < mapW; y++) {
            map[x][y].mine = false;
            map[x][y].flag = false;
            map[x][y].open = false;
            map[x][y].maround = 0;
        }
    }

    mines = 20;
    closedCells = mapW * mapH;
    gameOver = false;
    firstClick = true;
}

void placeMines(int avoidX, int avoidY) {
    srand(time(NULL));

    for (int i = 0; i < mines; i++) {
        int x = rand() % mapH;
        int y = rand() % mapW;

        bool tooClose = false;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (x == avoidX + dx && y == avoidY + dy) {
                    tooClose = true;
                }
            }
        }

        if (map[x][y].mine || (x == avoidX && y == avoidY) || tooClose) {
            i--;
        }
        else {
            map[x][y].mine = true;
        }
    }
    calculateMinesAround();
}

void openCell(int x, int y) {
    if (x < 0 || x >= mapH || y < 0 || y >= mapW) return;
    if (map[x][y].open || map[x][y].flag) return;

    map[x][y].open = true;
    closedCells--;

    if (map[x][y].mine) {
        gameOver = true;
        for (int i = 0; i < mapH; i++) {
            for (int j = 0; j < mapW; j++) {
                if (map[i][j].mine) map[i][j].open = true;
            }
        }
        return;
    }

    if (map[x][y].maround == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) {
                    openCell(x + dx, y + dy);
                }
            }
        }
    }
}

void toggleFlag(int x, int y) {
    if (map[x][y].open) return;
    map[x][y].flag = !map[x][y].flag;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int x = 0; x < mapH; x++) {
        for (int y = 0; y < mapW; y++) {
            drawCell(x, y);
        }
    }

    // Отображение текста через OpenGL примитивы
    glColor3f(0, 0, 0);
    float textY = 1.05;

    // Отображение статуса простыми линиями
    if (gameOver) {
        // Рисуем "GAME OVER"
        glBegin(GL_LINES);
        // Буква G
        glVertex2f(0.05, textY); glVertex2f(0.08, textY);
        glVertex2f(0.05, textY); glVertex2f(0.05, textY - 0.03);
        glVertex2f(0.05, textY - 0.03); glVertex2f(0.08, textY - 0.03);
        glVertex2f(0.08, textY - 0.03); glVertex2f(0.08, textY - 0.015);
        glVertex2f(0.08, textY - 0.015); glVertex2f(0.065, textY - 0.015);
        glEnd();
    }
    else if (closedCells == mines) {
        // Рисуем "YOU WIN!"
        glBegin(GL_LINES);
        // Буква W
        glVertex2f(0.05, textY); glVertex2f(0.055, textY - 0.03);
        glVertex2f(0.055, textY - 0.03); glVertex2f(0.06, textY - 0.015);
        glVertex2f(0.06, textY - 0.015); glVertex2f(0.065, textY - 0.03);
        glVertex2f(0.065, textY - 0.03); glVertex2f(0.07, textY);
        glEnd();
    }
    else {
        // Рисуем счетчик мин
        glBegin(GL_LINES);
        // Цифра количества мин
        int temp = mines;
        float xPos = 0.05;
        while (temp > 0) {
            int digit = temp % 10;
            // Простая цифра 1
            if (digit == 1) {
                glVertex2f(xPos + 0.005, textY);
                glVertex2f(xPos + 0.005, textY - 0.03);
            }
            xPos += 0.02;
            temp /= 10;
        }
        glEnd();
    }

    glfwSwapBuffers(window);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && !gameOver && closedCells > mines) {
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Конвертация координат мыши в OpenGL координаты
        float glX = (mx / windowWidth) * 2.0f - 1.0f;
        float glY = 1.0f - (my / windowHeight) * 2.0f;  // Инвертируем Y

        // Конвертация в координаты клеток
        // Наша сетка от 0 до 1 по X и от 0 до 1 по Y (игровое поле)
        float gridX = (glX + 1.0f) / 2.0f;  // Из [-1,1] в [0,1]
        float gridY = (glY + 1.0f) / 2.0f * (1.0f / 1.1f);  // Учитываем что поле занимает не весь экран

        int cellX = (int)(gridX / cellSize);
        int cellY = (int)(gridY / cellSize);

        printf("Mouse: %.2f, %.2f -> Grid: %.2f, %.2f -> Cell: %d, %d\n",
            glX, glY, gridX, gridY, cellX, cellY);

        if (cellX >= 0 && cellX < mapH && cellY >= 0 && cellY < mapW) {
            if (firstClick) {
                placeMines(cellX, cellY);
                firstClick = false;
            }

            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                openCell(cellX, cellY);
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                toggleFlag(cellX, cellY);
            }
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            game_new();
        }
        else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Ошибка инициализации GLFW\n");
        return -1;
    }

    window = glfwCreateWindow(500, 550, "Minesweeper", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Ошибка создания окна\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.95, 0.95, 0.95, 1.0);
    glOrtho(0, 1, 0, 1.1, -1, 1);

    game_new();

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}