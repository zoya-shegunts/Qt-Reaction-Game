#include <QApplication>
#include "reaction_game.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    reactionGame* game = new reactionGame;
    game->show();

    app.exec();
}