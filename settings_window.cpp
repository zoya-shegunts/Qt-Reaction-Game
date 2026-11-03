#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <qDebug>
#include <QPainter>
#include "settings_window.h"
#include "login_window.h"

settingsWindow::settingsWindow(QWidget* p)
    : QWidget(p)
{
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
        "font-size: 15px;"
        "font-weight: bold;"
        "}"
        );
    setWindowTitle("Settings");
    setFixedSize(300, 150);
    createWidgets();
    makeLayout();
    connection();
}

void settingsWindow::createWidgets()
{
    m_settings_label = new QLabel("Settings");
    m_settings_label->setStyleSheet(
        "color: #730507;"
        "font-family: Phosphate;"
        "font-size: 40px;"
        "font-weight: bold;"
        );
    m_login = new QPushButton("Login");
    m_logout = new QPushButton("Logout");
    m_signup = new QPushButton("Sign up");
    m_close = new QPushButton("Cancel");

    m_login_window = new loginWindow(AccountMode::Login);
    m_signup_window = new loginWindow(AccountMode::SignUp);
}

void settingsWindow::makeLayout()
{
    auto* buttons = new QHBoxLayout();
    buttons->addWidget(m_login);
    buttons->addWidget(m_signup);
    buttons->addWidget(m_logout);
    buttons->addWidget(m_close);

    auto* mainLyt = new QVBoxLayout(this);
    mainLyt->addWidget(m_settings_label);
    mainLyt->addLayout(buttons);

    mainLyt->setAlignment(m_settings_label, Qt::AlignCenter);

    m_logout->hide();
    m_close->hide();
}

void settingsWindow::connection()
{
    connect(m_login, &QPushButton::clicked, this, [this]{
        m_login_window->show();
    });

    connect(m_signup, &QPushButton::clicked, this, [this] {
        m_signup_window->show();
    });

    connect(m_login_window, &loginWindow::loginSucceeded, this, &settingsWindow::login);

    connect(m_logout, &QPushButton::clicked, this, &settingsWindow::logout);

    connect(m_close, &QPushButton::clicked, this, &QWidget::close);
}

// PROTECTED

void settingsWindow::paintEvent(QPaintEvent* event)
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

//PRIVATE SLOTS


void settingsWindow::login(const QString& username, double savedRecord)
{
    m_currentUsername = username;
    m_record = savedRecord;

    m_login->hide();
    m_signup->hide();
    m_logout->show();
    m_close->show();
    m_close->setDefault(true);
    m_close->setFocus();

    emit userLoggedIn(username, savedRecord);

    m_settings_label->setText("Welcome, " + username);
    m_settings_label->setStyleSheet(
        "color: #730507;"
        "font-family: Phosphate;"
        "font-size: 40px;"
        "font-weight: bold;"
        );
    m_settings_label->adjustSize();

    setFixedSize(m_settings_label->sizeHint().width() + 100, 150);}

void settingsWindow::logout()
{
    int r = QMessageBox::question(this, "Log Out",
                                  "Do you want to log out from your account?",
                                  QMessageBox::Yes |
                                  QMessageBox::No |
                                  QMessageBox::Cancel,
                                  QMessageBox::Cancel);

    if(r == QMessageBox::Yes)
    {
        m_settings_label->setText("Settings");
        m_settings_label->setStyleSheet(
            "color: #730507;"
            "font-family: Phosphate;"
            "font-size: 40px;"
            "font-weight: bold;"
            );
        setFixedSize(300, 150);

        m_login->show();
        m_signup->show();
        m_logout->hide();
        m_close->hide();

        m_currentUsername.clear();

        emit userLoggedOut();

        close();
    }
}




