#include <iostream>

void display_game(char *game_state, const short &board_size)
{
    for (short i = 0; i < board_size - 1; i++)
    {
        for (short j = 0; j < board_size; j++)
        {
            std::cout << game_state[board_size * i + j] << "|";
        }
        std::cout << "\b " << std::endl;
        for (short j = 0; j < board_size; j++)
        {
            std::cout << "──";
        }
        std::cout << std::endl;
    }
    for (short j = 0; j < board_size; j++)
    {
        std::cout << game_state[board_size * (board_size - 1) + j] << "|";
    }
    std::cout << "\b \n\n";
}

bool game_won(char *game_state, const short &board_size, const short &win_condition, const short &row, const short &column)
{
    short counter = 2;
    short last = (row - 1) * board_size + column - 1;

    // horizintal
    for (short i = -1; i + column > 0; --i)
    {
        if (game_state[last + i] != game_state[last])
            break;
        counter++;
    }
    for (short i = 1; i + column <= board_size; ++i)
    {
        if (game_state[last + i] != game_state[last])
            break;
        counter++;
    }
    if (counter > win_condition)
        return 1;

    // vertical
    counter = 2;
    for (short i = -1; i + row > 0; --i)
    {
        if (game_state[last + i * board_size] != game_state[last])
            break;
        counter++;
    }
    for (short i = 1; i + row <= board_size; ++i)
    {
        if (game_state[last + i * board_size] != game_state[last])
            break;
        counter++;
    }
    if (counter > win_condition)
        return 1;

    // downstairs
    counter = 2;
    for (short i = -1; i + row > 0 && i + column > 0; --i)
    {
        if (game_state[last + i * board_size + i] != game_state[last])
            break;
        counter++;
    }
    for (short i = 1; i + row <= board_size && i + column <= board_size; ++i)
    {
        if (game_state[last + i * board_size + i] != game_state[last])
            break;
        counter++;
    }
    if (counter > win_condition)
        return 1;

    // upstairs
    counter = 2;
    for (short i = -1; i + row > 0 && column - i <= board_size; --i)
    {
        if (game_state[last + i * board_size - i] != game_state[last])
            break;
        counter++;
    }
    for (short i = 1; i + row <= board_size && column - i > 0; ++i)
    {
        if (game_state[last + i * board_size - i] != game_state[last])
            break;
        counter++;
    }
    if (counter > win_condition)
        return 1;

    return 0;
}

int heuristic_value(char *gamestate, const short &board_size, const char &player, const short &win_condition)
{
    return 0;

    char enemy = (player == 'X') ? 'O' : 'X';
    int counter;
    int value = 0;
    int last;

    for (int i = 0; i <board_size; i++)
    {
        counter = 0;
        last = i*board_size;
        for (int j =0; j < board_size; j++)
        {
            if (enemy == gamestate[last+j])
            counter = 0;
            ++counter;
            if (counter >= win_condition)
            value +=2;
        }
    }

    for (int i = 0; i <board_size; i++)
    {
        counter = 0;
        last = i*board_size;
        for (int j =0; j < board_size; j++)
        {
            if (player == gamestate[last+j])
            counter = 0;
            ++counter;
            if (counter >= win_condition)
            value -=1;
        }
        
    }

    return value * ((player == 'X') ? 1 : (-1));
}

int minmax(char *gamestate, const short &board_size, const short &win_condition, const short &depth, const char &player_turn, short move, int alfa, int beta)
{
    if (depth == 0)
    {
        return heuristic_value(gamestate, board_size, player_turn, win_condition);
        // return 0;
    }

    if (player_turn == 'X')
    {
        int gamestate_value = std::numeric_limits<int>::min();
        for (short i = 0; i < board_size; i++)
        {
            for (short j = 0; j < board_size; j++)
            {
                if (gamestate[i * board_size + j] != ' ') // skiping taken moves
                    continue;

                gamestate[i * board_size + j] = 'X';

                if (game_won(gamestate, board_size, win_condition, i + 1, j + 1))
                {
                    gamestate_value = std::numeric_limits<int>::max()-1;
                }
                else if (move == board_size * board_size)
                {
                    gamestate_value = 0;
                }
                else
                {
                    gamestate_value = std::max(gamestate_value, minmax(gamestate, board_size, win_condition, depth - 1, 'O', move + 1, alfa, beta));
                }

                gamestate[i * board_size + j] = ' ';

                alfa = std::max(alfa, gamestate_value);
                if (beta <= alfa)
                    break;
            }
        }
        return gamestate_value;
    }
    else
    {
        int gamestate_value = std::numeric_limits<int>::max();
        for (short i = 0; i < board_size; i++)
        {
            for (short j = 0; j < board_size; j++)
            {
                if (gamestate[i * board_size + j] != ' ') // skiping taken moves
                    continue;

                gamestate[i * board_size + j] = 'O';

                if (game_won(gamestate, board_size, win_condition, i + 1, j + 1))
                {
                    gamestate_value = std::numeric_limits<int>::min()+1;
                }
                else if (move == board_size * board_size)
                {
                    gamestate_value = 0;
                }
                else
                {
                    gamestate_value = std::min(gamestate_value, minmax(gamestate, board_size, win_condition, depth - 1, 'X', move + 1, alfa, beta));
                }

                gamestate[i * board_size + j] = ' ';

                beta = std::min(beta, gamestate_value);
                if (alfa >= beta)
                    break;
            }
        }
        return gamestate_value;
    }
}

int minmax(char *gamestate, const short &board_size, const short &win_condition, const short &depth, const char &player_turn, short move, int alfa, int beta, short &row, short &column)
{
    if (depth == 0)
    {
        return heuristic_value(gamestate, board_size, player_turn, win_condition);
        // return 0;
    }

    if (player_turn == 'X')
    {
        int gamestate_value = std::numeric_limits<int>::min();
        for (short i = 0; i < board_size; i++)
        {
            for (short j = 0; j < board_size; j++)
            {
                if (gamestate[i * board_size + j] != ' ') // skiping taken moves
                    continue;

                gamestate[i * board_size + j] = 'X';

                if (game_won(gamestate, board_size, win_condition, i + 1, j + 1) || move == board_size * board_size)
                {
                    row = i;
                    column = j;
                    gamestate[i * board_size + j] = ' ';
                    return 0;
                }

                int child_value = minmax(gamestate, board_size, win_condition, depth - 1, 'O', move + 1, alfa, beta);
                gamestate[i * board_size + j] = ' ';

                if (gamestate_value < child_value)
                {
                    gamestate_value = child_value;
                    row = i;
                    column = j;
                }

                alfa = std::max(alfa, gamestate_value);
                if (beta <= alfa)
                    break;
            }
        }
        return 0;
    }
    else
    {
        int gamestate_value = std::numeric_limits<int>::max();
        for (short i = 0; i < board_size; i++)
        {
            for (short j = 0; j < board_size; j++)
            {
                if (gamestate[i * board_size + j] != ' ') // skiping taken moves
                    continue;

                gamestate[i * board_size + j] = 'O';

                if (game_won(gamestate, board_size, win_condition, i + 1, j + 1) || move == board_size * board_size)
                {
                    row = i;
                    column = j;
                    gamestate[i * board_size + j] = ' ';
                    return 0;
                }

                int child_value = minmax(gamestate, board_size, win_condition, depth - 1, 'X', move + 1, alfa, beta);
                gamestate[i * board_size + j] = ' ';

                if (gamestate_value > child_value)
                {
                    gamestate_value = child_value;
                    row = i;
                    column = j;
                }

                beta = std::min(beta, gamestate_value);
                if (alfa >= beta)
                    break;
            }
        }
        return 0;
    }
}

int main()
{
    short board_size = 5, column, line, win_condition = 4, move = 0, depth = 10;
    char game_state[board_size][board_size], winner;

    for (short i = 0; i < board_size * board_size; i++)
    {
        game_state[0][i] = ' ';
    }

    system("clear");
    display_game(game_state[0], board_size);

    while (1)
    {
        std::cout << "Give line and column:" << std::endl;

        while (1)
        {
            std::cin >> line >> column;

            if (line < 1 || line > board_size || column < 1 || column > board_size)
            {
                system("clear");
                display_game(game_state[0], board_size);
                std::cout << "Wrong coordinats! Insert correct coordinats:" << std::endl;
                continue;
            }

            if (game_state[line - 1][column - 1] != ' ')
            {
                system("clear");
                display_game(game_state[0], board_size);
                std::cout << "Position taken! Pick another:" << std::endl;
                continue;
            }
            break;
        }

        game_state[line - 1][column - 1] = 'O';
        ++move;
        system("clear");
        display_game(game_state[0], board_size);

        if (game_won(game_state[0], board_size, win_condition, line, column))
        {
            std::cout << "O wins!" << std::endl;
            break;
        }
        if (move == board_size * board_size)
        {
            std::cout << "Draw!" << std::endl;
            break;
        }

        minmax(game_state[0], board_size, win_condition, depth, 'X', move + 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), line, column);
        game_state[line][column] = 'X';
        ++move;
        system("clear");
        display_game(game_state[0], board_size);

        if (game_won(game_state[0], board_size, win_condition, line + 1, column + 1))
        {
            std::cout << "X wins!" << std::endl;
            break;
        }
        if (move == board_size * board_size)
        {
            std::cout << "Draw!" << std::endl;
            break;
        }
    }

    return 0;
}
