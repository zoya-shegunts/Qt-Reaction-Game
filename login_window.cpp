#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <fstream>
#include <QMessageBox>
#include <QPainter>
#include "login_window.h"
#include <QKeyEvent>

#include <QDebug>

loginWindow::loginWindow(AccountMode mode, QWidget* parent)
    :QWidget(parent),
    m_mode(mode)
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

    setFixedSize(350, 250);

    createWidgets();
    layoutWidgets();
    connection();
}

// PRIVATE FUNCTIONS

void loginWindow::createWidgets()
{
    m_username = new QLineEdit;
    m_username->setPlaceholderText("Enter username");

    m_password = new QLineEdit;
    m_password->setPlaceholderText("Enter password");
    m_password->setEchoMode(QLineEdit::Password); //hiding characters

    if (m_mode == AccountMode::Login)
    {
        m_login = new QPushButton("Login");
        setWindowTitle("Login");
        m_signup = new QLabel(
            "<a href='#'>I don't have an account</a>", this);
    }
    else
    {
        m_login = new QPushButton("Sign Up");
        setWindowTitle("Sign Up");
        m_signup = new QLabel(
            "<a href='#'>I already have an account</a>", this);
    }

    m_login->setDefault(true);
}

void loginWindow::layoutWidgets()
{
    QLabel* title = new QLabel(m_mode == AccountMode::Login ? "Login" : "Sign Up");
    title->setStyleSheet(
        "color: #730507;"
        "font-family: Phosphate;"
        "font-size: 50px;"
        "font-weight: bold;"
        );

    auto* form = new QFormLayout;
    form->addRow("Username:", m_username);
    form->addRow("Password:", m_password);

    m_signup->setOpenExternalLinks(false);
    m_signup->setAlignment(Qt::AlignCenter);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(form);
    layout->addWidget(m_login);
    layout->addWidget(m_signup);

    layout->setAlignment(title, Qt::AlignCenter);
}

void loginWindow::connection()
{
    connect(m_login, &QPushButton::clicked, this, &loginWindow::processAccount);
    connect(m_signup, &QLabel::linkActivated, this, &loginWindow::close);
}

// PROTECTED

void loginWindow::paintEvent(QPaintEvent* event)
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

// SLOTS

void loginWindow::processAccount()
{
    std::string username = m_username->text().toStdString();
    std::string password = m_password->text().toStdString();


    if (username.empty() || password.empty())
    {
        QMessageBox::warning(this, "Error",
                             "Enter both username and password!");
        return;
    }

    std::ifstream file(
        "/Users/zoyaprivate/Documents/qt/ReactionGame/users.txt");

    if (!file.is_open())
    {
        QMessageBox::warning(this, "Error",
                             "Could not open users file.");
        return;
    }

    std::string savedUsername;
    std::string savedPassword;
    double savedRecord;

    while (file >> savedUsername >> savedPassword >> savedRecord)
    {
        if (savedUsername == username)
        {
            if (m_mode == AccountMode::SignUp)
            {
                QMessageBox::warning(this, "Error",
                                     "Username already exists!");
            }
            else if (savedPassword == password)
            {
                QMessageBox::information(this, "Success",
                                         "Login successful!");
                emit loginSucceeded(QString::fromStdString(username), savedRecord);
                m_username->clear();
                m_password->clear();
                close();
            }
            else
            {
                QMessageBox::warning(this, "Error",
                                     "Wrong password!");
                m_password->clear();
            }

            return;
        }
    }

    file.close();


    if (m_mode == AccountMode::SignUp)
    {
        std::ofstream outFile(
            "/Users/zoyaprivate/Documents/qt/ReactionGame/users.txt",
            std::ios::app
            );

        if (!outFile.is_open())
        {
            QMessageBox::warning(this, "Error",
                                 "Could not open users.txt for writing!");
            return;
        }

        outFile << username << " " << password << " " << 0 << '\n';
        outFile.close();

        if (outFile.fail())
        {
            QMessageBox::warning(this, "Error",
                                 "Writing to users.txt failed!");
            return;
        }

        outFile.close();

        QMessageBox::information(this, "Success",
                                 "Account saved successfully!");

        m_username->clear();
        m_password->clear();
        close();
    }

    else
    {
        QMessageBox::warning(this, "Error", "Username not found!");
    }

    m_username->clear();
    m_password->clear();
}

// ...

void loginWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return)
    {
        processAccount();
        return;
    }

    QWidget::keyPressEvent(event);
}
