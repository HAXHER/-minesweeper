#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

class Menu
{
public:
    int chooseDifficulty(sf::RenderWindow& window)
    {
        sf::Font cascadiacode;
        if (!cascadiacode.loadFromFile("C:/WINDOWS/FONTS/CASCADIACODE.TTF"))
        {
            std::cerr << "Error al cargar la fuente" << std::endl;
            return -1;
        }

        sf::Text title("Seleccione la dificultad:", cascadiacode, 25);
        title.setFillColor(sf::Color::White);
        title.setPosition(50.f, 50.f);

        sf::Text easy("1. Fácil", cascadiacode,  20);
        easy.setFillColor(sf::Color::White);
        easy.setPosition(50.f, 100.f);

        sf::Text medium("2. Medio", cascadiacode,  20);
        medium.setFillColor(sf::Color::White);
        medium.setPosition(50.f, 150.f);

        sf::Text hard("3. Difícil", cascadiacode,  20);
        hard.setFillColor(sf::Color::White);
        hard.setPosition(50.f, 200.f);

        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(easy);
        window.draw(medium);
        window.draw(hard);
        window.display();

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return -1;
                }
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (easy.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        return 1;
                    else if (medium.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        return 2;
                    else if (hard.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        return 3;
                }
            }
        }

        return -1;
    }
};



class Grid
{
private:
    int rows;
    int cols;
    int cellSize;
    vector<std::vector<int>> board;
    vector<std::vector<bool>> revealed;
    vector<std::vector<bool>> flagged;

    bool gameOver;
    bool win;

    sf::Texture mineTexture;
    sf::Sprite mineSprite;
    sf::Texture flagTexture;
    sf::Sprite flagSprite;
public:
    Grid(int rows, int cols, int cellSize) : rows(rows), cols(cols), cellSize(cellSize), gameOver(false)
    {
        board.resize(rows, std::vector<int>(cols, 0));
        revealed.resize(rows, std::vector<bool>(cols, false));
        flagged.resize(rows, std::vector<bool>(cols, false));

        if (!mineTexture.loadFromFile("C:\\Users\\Alexa Kaarime\\OneDrive\\Escritorio\\Proyectos de programacion\\Lenguaje C++\\Buscaminas\\imagenes\\minita.png") || !flagTexture.loadFromFile("C:\\Users\\Alexa Kaarime\\OneDrive\\Escritorio\\Proyectos de programacion\\Lenguaje C++\\Buscaminas\\imagenes\\banderita.png")) {
           cerr << "Error al cargar la textura de la mina" << endl;

        }
        mineSprite.setTexture(mineTexture);
        flagSprite.setTexture(flagTexture);
    }


    void placeMines(int mines)
    {
        srand(time(NULL));
        int count = 0;
        while (count < mines)
        {
            int x = rand() % rows;
            int y = rand() % cols;
            if (board[x][y] != -1)
            {
                board[x][y] = -1;
                count++;
            }
        }
    }

    void reveal(int x, int y)
    {
        if (revealed[x][y] || gameOver)
            return;

        revealed[x][y] = true;
        if (board[x][y] == -1)
        {
            gameOver = true;
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
                    if (board[i][j] == -1) {
                        revealed[i][j] = true;
                    }
                }
            }
        }
        else
        {
            int adjacentMines = countAdjacentMines(x, y);
            if (adjacentMines == 0)
            {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols)
                            reveal(nx, ny);
                    }
                }
            }
        }


        int unrevealedCount = 0;
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (!revealed[i][j] && board[i][j] != -1)
                {
                    unrevealedCount++;
                }
            }
        }

        if (unrevealedCount == 0)
        {
            win = true;
        }
    }

    int countAdjacentMines(int x, int y)
    {
        int count = 0;

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && board[nx][ny] == -1)
                    count++;
            }
        }
        return count;
    }

    void draw(sf::RenderWindow& window)
    {
        sf::Font cascadiacode;
        cascadiacode.loadFromFile("C:/WINDOWS/FONTS/CASCADIACODE.TTF");

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (revealed[i][j])
                {
                    if (board[i][j] == -1)
                    {
                        mineSprite.setPosition(j * cellSize, i * cellSize);
                        window.draw(mineSprite);
                    }
                    else
                    {
                        int adjacentMines = countAdjacentMines(i, j);
                        if (adjacentMines > 0)
                        {
                            sf::Text text(std::to_string(adjacentMines), cascadiacode, 20);
                            text.setPosition(j * cellSize + 10, i * cellSize + 10);

                            sf::Color numberCellColor = sf::Color::White;

                            sf::RectangleShape numberCell(sf::Vector2f(cellSize, cellSize));
                            numberCell.setPosition(j * cellSize, i * cellSize);
                            numberCell.setFillColor(numberCellColor);
                            window.draw(numberCell);

                            switch (adjacentMines)
                            {
                            case 1:
                                text.setFillColor(sf::Color::Blue);
                                break;
                            case 2:
                                text.setFillColor(sf::Color::Green);
                                break;
                            case 3:
                                text.setFillColor(sf::Color::Red);
                                break;
                            case 4:
                                text.setFillColor(sf::Color::Cyan);
                                break;
                            case 5:
                                text.setFillColor(sf::Color::Magenta);
                                break;
                            case 6:
                                text.setFillColor(sf::Color::Yellow);
                                break;
                            case 7:
                                text.setFillColor(sf::Color(255, 165, 0));
                                break;
                            case 8:
                                text.setFillColor(sf::Color::Black);
                                break;
                            default:
                                text.setFillColor(sf::Color::Black);
                                break;
                            }
                            window.draw(text);
                        }

                        else
                        {
                            sf::RectangleShape emptySpace(sf::Vector2f(cellSize, cellSize));
                            emptySpace.setPosition(j * cellSize, i * cellSize);
                            emptySpace.setFillColor(sf::Color::White);
                            window.draw(emptySpace);
                        }
                    }
                }
                else if (!gameOver && flagged[i][j])
                {
                    flagSprite.setPosition(j * cellSize, i * cellSize);
                    window.draw(flagSprite);
                }
            }
        }


        for (int i = 0; i <= cols; ++i)
        {
            sf::Vertex line[] =
            {
              sf::Vertex(sf::Vector2f(i * cellSize, 0), sf::Color::Black),
              sf::Vertex(sf::Vector2f(i * cellSize, rows * cellSize), sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);
        }


        for (int j = 0; j <= rows; ++j)
        {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f(0, j * cellSize), sf::Color::Black),
               sf::Vertex(sf::Vector2f(cols * cellSize, j * cellSize), sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);
        }

        if (gameOver) {

            sf::Text gameOverText("Game Over", cascadiacode, 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2, (window.getSize().y - gameOverText.getLocalBounds().height) / 2);
            window.draw(gameOverText);
        }

        if (win)
        {
            sf::Text winText("ARE YOU WINNING SON? YES!", cascadiacode, 50);
            winText.setFillColor(sf::Color::Green);
            winText.setPosition((window.getSize().x - winText.getLocalBounds().width) / 2, (window.getSize().y - winText.getLocalBounds().height) / 2);
            window.draw(winText);
        }
    }

    void handleEvent(sf::Event& event)
    {
        if (gameOver) return;
        if (event.type == sf::Event::MouseButtonPressed)
        {
            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;
            int row = mouseY / cellSize;
            int col = mouseX / cellSize;
            if (row >= 0 && row < rows && col >= 0 && col < cols)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    reveal(row, col);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (!revealed[row][col])
                    {
                        flagged[row][col] = !flagged[row][col];
                    }
                }

            }
        }
    }



  };

    int main()
    {
        const int SIZE = 15;
        /*const int MINES = 10; */
        const int TILE_SIZE = 40;

        sf::RenderWindow window(sf::VideoMode(SIZE * TILE_SIZE, SIZE * TILE_SIZE), "Buscaminas");

        Menu menu;
        int difficulty = menu.chooseDifficulty(window);

        int MINES;

        switch (difficulty)
        {
        case 1:
           
            MINES = 15;

            break;
        case 2:
            
            MINES = 27;
            break;
        case 3:
        
            MINES = 36;
            break;
        default:
            MINES = 10;
            break;
        }

      


        if (difficulty != -1)
        {
            Grid grid( SIZE, SIZE, TILE_SIZE);
            grid.placeMines(MINES);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {

                        window.close();
                    }
                    grid.handleEvent(event);
                }

                window.clear(sf::Color(192, 192, 192));

                grid.draw(window);

                window.display();
            }
        }

        return 0;
    }
