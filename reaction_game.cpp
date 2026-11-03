#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QTimer>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QPixmap>
#include <QPainter>
#include <fstream>
#include <QIcon>
#include "reaction_game.h"
#include "traffic_light.h"


reactionGame::reactionGame(QWidget* p)
    :QWidget(p)
{
     setFixedSize(500, 600);

    setStyleSheet(
        "QMessageBox {"
        "background: #FAEFCA;"
        "}"

        "QPushButton {"
        "background-color: #EAC271;"
        "color: #66490F;"
        "font-family: Times;"
        "font-size: 20px;"
        "font-weight: bold;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 5px 15px;"
        "}"

        "QLabel {"
        "color: #66490F;"
        "font-family: Times;"
        "font-size: 20px;"
        "font-weight: bold;"
        "}"
    );


    m_state = waiting;

    createWidgets();
    makeWidgetsLayout();
    makeConnection();
}


// PROTECTED FUNCTIONS


void reactionGame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        if(m_start->isVisible()) startGame();
        else stopGame();
    }
    else if(event->key() == Qt::Key_Backspace) quitGame();
}

void reactionGame::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPixmap background(":/background1.jpg");

    QPixmap scaled = background.scaled(
        size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
        );

    int x = (width() - scaled.width()) / 2;
    int y = (height() - scaled.height()) / 2;

    painter.drawPixmap(x, y, scaled);
}


// PRIVATE FUNCTIONS


void reactionGame::createWidgets()
{
    m_title = new QLabel("REACTION GAME");
    m_title->setStyleSheet(
        "color: #730507;"
        "font-family: Phosphate;"
        "font-size: 60px;"
        "font-weight: bold;"
        );

    m_rules = new QPushButton("Rules");
    m_info_text = new QLabel("To check your reaction press 'Start'");
    m_restart = new QPushButton("Restart");
    m_quit = new QPushButton("Quit");

    m_start = new QPushButton("Start");

    m_stop = new QPushButton("Stop");
    m_stop->hide();

    m_trafficLight = new TrafficLight;

    //record
    m_rec = new QLabel("Record:\n" + QString::number(m_record));

    //timer
    m_timerLabel = new QLabel("0.000");
    m_timer = new QTimer;
    m_timer->setInterval(1);

    m_settings = new settingsWindow;
    m_settings_button = new QPushButton;
    m_settings_button->setIcon(QIcon(":/icon/settings_icon-3.svg"));
    m_settings_button->setIconSize(QSize(28, 28));
}

void reactionGame::makeWidgetsLayout()
{
    QVBoxLayout* mainLyt = new QVBoxLayout(this);
    mainLyt->addWidget(m_title);
    mainLyt->setAlignment(m_title, Qt::AlignHCenter);

    QHBoxLayout* line2Lyt = new QHBoxLayout;
    line2Lyt->addWidget(m_rules);
    line2Lyt->addWidget(m_info_text);
    line2Lyt->addWidget(m_quit);

    mainLyt->addLayout(line2Lyt);

    QVBoxLayout* rightLyt = new QVBoxLayout;
    rightLyt->addWidget(m_restart);
    rightLyt->addWidget(m_rec);
    rightLyt->addStretch();

    QGridLayout* trafficLineLyt = new QGridLayout;

    trafficLineLyt->addWidget(m_settings_button, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    m_settings_button->setFixedSize(40, 40);
    trafficLineLyt->addWidget(m_trafficLight, 0, 1, 3, 1);
    trafficLineLyt->addLayout(rightLyt, 0, 2, 1, 1);


    trafficLineLyt->setColumnStretch(0, 1);
    trafficLineLyt->setColumnStretch(1, 2);
    trafficLineLyt->setColumnStretch(2, 1);

    mainLyt->addLayout(trafficLineLyt);
    mainLyt->addWidget(m_timerLabel);

    QGridLayout* buttonLyt = new QGridLayout;

    buttonLyt->addWidget(m_start, 0, 1, 1, 3);
    buttonLyt->addWidget(m_stop, 0, 1, 1, 3);

    mainLyt->addLayout(buttonLyt);

}

void reactionGame::makeConnection()
{
    //Rules button
    connect(m_rules, SIGNAL(clicked()),this, SLOT(gameRules()));
    //Quit Button
    connect(m_quit, SIGNAL(clicked()), this, SLOT(quitGame()));
    //Restart button
    connect(m_restart, SIGNAL(clicked()), this, SLOT(restartGame()));
    //Start button
    connect(m_start, SIGNAL(clicked()), this, SLOT(startGame()));

    //Timer label
    connect(m_timer, &QTimer::timeout, this, [this]() {
        double result = m_elapsedTimer.elapsed() / 1000.0;

        m_timerLabel->setText(
            QString::number(result, 'f', 3)
            );
    });

    //Stop button
    connect(m_stop, &QPushButton::clicked, this, &reactionGame::stopGame);

    //Settings button
    connect(m_settings_button, &QPushButton::clicked, this, [this]{
        m_settings->show();
    });

    connect(m_settings, &settingsWindow::userLoggedIn,
            this, [this](const QString& username, double savedRecord)
            {
                m_currentUsername = username;
                m_record = savedRecord;
                m_accState = AccountState::loggedin;

                m_rec->setText("Record:\n" + QString::number(m_record));
            });

    connect(m_settings, &settingsWindow::userLoggedOut, this, [this]{
        changeRecord();
        m_currentUsername = "";
        m_record = 0;
        m_accState = AccountState::loggedout;

        m_rec->setText("Record:\n" + QString::number(m_record));
    });
}

void reactionGame::updateGame(double res)
{

    if(m_record == 0 && res == 0) m_rec->setText("Record:\n" + QString::number(m_record));
    else
    {
        if(m_record == 0 || res < m_record)
        {
            m_record = res;
            QMessageBox::information(this,
                                     "Score",
                                     "<b>NEW RECORD!</b><br>"
                                     "Your score: " + QString::number(m_record) + " seconds"
                                     );

            m_rec->setText("Record:\n" + QString::number(m_record));
        }
        else
        {
            if(m_green){
                QMessageBox::information(this,
                                         "Score",
                                         "<b>Your score: <b>" + QString::number(res) + " seconds"
                                         );
            }
        }
    }

    m_timerLabel->setText("0.000");
    m_trafficLight->setColor(grey);
    m_timer->stop();
}

void reactionGame::changeRecord()
{
    std::ifstream inFile(
        "/Users/zoyaprivate/Documents/qt/ReactionGame/users.txt"
        );

    if (!inFile.is_open())
    {
        QMessageBox::warning(this, "Error",
                             "Couldn't open users.txt.");
        return;
    }

    std::vector<User> users;
    User user;
    bool found = false;

    while (inFile >> user.username >> user.password >> user.record)
    {
        if (QString::fromStdString(user.username) == m_currentUsername)
        {
            found = true;

            if (user.record == 0 || m_record < user.record)
                user.record = m_record;
        }

        users.push_back(user);
    }

    inFile.close();

    if (!found)
    {
        QMessageBox::warning(this, "Error",
                             "Couldn't find the logged-in user.");
        return;
    }

    // Rewrite the file with the updated records
    std::ofstream outFile(
        "/Users/zoyaprivate/Documents/qt/ReactionGame/users.txt"
        );

    if (!outFile.is_open())
    {
        QMessageBox::warning(this, "Error",
                             "Couldn't save the records.");
        return;
    }

    for (const User& u : users)
    {
        outFile << u.username << " "
                << u.password << " "
                << u.record << '\n';
    }

    outFile.close();
}


// SLOTS


void reactionGame::gameRules()
{
    QMessageBox::information(
        this,
        "Game Rules",
        "This is a Reaction Game.<br>To start the game, click Start. "
        "You will then see the traffic light changing its colors "
        "in the order:<br>"
        "<font color='red'>red</font> → "
        "<font color='yellow'>yellow</font> → "
        "<font color='green'>green</font> "
        "<br>When the green light comes on, the timer will start, "
        "and you should click the Stop button as quickly as possible "
        "to see how fast your reaction was."
        "<br> "
        "<br>Shorcuts:"
        "<br>Start,Stop - 'space'"
        "<br>Quit - 'delete'"
        );
}

void reactionGame::quitGame()
{
    int r = QMessageBox::question(this, "Quitting game",
                                  "Do you want to quit the game?",
                                  QMessageBox::Yes |
                                  QMessageBox::No |
                                  QMessageBox::Cancel,
                                  QMessageBox::Yes);

    if(r != QMessageBox::Yes) return;
    if(m_accState == AccountState::loggedin) changeRecord();

    close();
}

void reactionGame::restartGame()
{
    int r = QMessageBox::question(this, "Restarting game",
                                  "Do you want to restart the game?\n"
                                  "Your current record will be lost.",
                                  QMessageBox::Yes |
                                  QMessageBox::No |
                                  QMessageBox::Cancel,
                                  QMessageBox::Cancel);

    if(r == QMessageBox::Yes){
        m_record = 0;
        updateGame(0);
        changeRecord();
    }

}

void reactionGame::startGame()
{
    m_start->hide();
    m_stop->show();

    m_info_text->setText("Press 'Stop' when you see green");
    m_green = false;
    m_state = playing;

    m_trafficLight->setColor(red);

    int delay = QRandomGenerator::global()->bounded(1000, 5000);

    QTimer::singleShot(delay, this, [this]() {
        if (m_state == finished) return;

        m_trafficLight->setColor(yellow);

        int delay = QRandomGenerator::global()->bounded(1000, 5000);

        QTimer::singleShot(delay, this, [this]() {
            if (m_state == finished) return;

            m_trafficLight->setColor(green);
            m_green = true;

            m_elapsedTimer.start();
            m_timer->start(1);

        });
    });
}

void reactionGame::stopGame()
{
    double result = m_elapsedTimer.elapsed() / 1000.0;
    m_timer->stop();
    m_state = finished;
    if(!m_green){
        QMessageBox::information(
            this,
            "Pressed too soon",
            "<b>Pressed too soon</b>"
            "<br>You pressed the Stop button before green light came on, so we can't measure the time, "
            "pay more attantion"
        );
        updateGame(m_record);
    }
    else{
        updateGame(result);
    }

    m_stop->hide();
    m_start->show();

    m_info_text->setText("To check your reaction press 'Start'");

}