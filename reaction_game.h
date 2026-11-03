#ifndef REACTION_GAME_H
#define REACTION_GAME_H
#include <QWidget>
#include <QElapsedTimer>
#include "settings_window.h"
#include "login_window.h"

class QPushButton;
class QLabel;
class TrafficLight;
class QKeyEvent;

enum GameState
{
    waiting,
    playing,
    finished
};

class reactionGame: public QWidget
{
    Q_OBJECT
public:
    explicit reactionGame(QWidget* p = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void createWidgets();
    void makeWidgetsLayout();
    void makeConnection();
    void updateGame(double res);
    void changeRecord();

private slots:
    void gameRules();
    void quitGame();
    void restartGame();
    void startGame();
    void stopGame();

private:
    QLabel* m_title;

    QPushButton* m_rules;
    QLabel* m_info_text;
    QPushButton* m_restart;
    QPushButton* m_quit;

    QPushButton* m_start;
    QPushButton* m_stop;

    TrafficLight* m_trafficLight;
    bool m_green = false;

    QLabel* m_rec;
    double m_record;

    QLabel* m_timerLabel;
    QTimer* m_timer;
    double m_time;
    QElapsedTimer m_elapsedTimer;

    GameState m_state;
    AccountState m_accState = AccountState::loggedout;

    settingsWindow* m_settings;
    QPushButton* m_settings_button;
    loginWindow* m_login = nullptr;
    QString m_currentUsername;
};

#endif // REACTION_GAME_H
