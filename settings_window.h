#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "QWidget"
#include "login_window.h"

class QLabel;
class QPushButton;

struct User
{
    std::string username;
    std::string password;
    double record;
};

class settingsWindow: public QWidget
{
    Q_OBJECT
public:
    explicit settingsWindow(QWidget* p = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void createWidgets();
    void makeLayout();
    void connection();
    void changeRecord();

signals:
    void userLoggedIn(const QString& username, double savedRecord);
    void userLoggedOut();

public slots:
    void login(const QString& username, double savedRecord);
    void logout();

private:
    QLabel* m_settings_label;
    QString m_currentUsername;
    double m_record;

    QPushButton* m_login;
    QPushButton* m_logout;
    QPushButton* m_signup;
    QPushButton* m_close;

    loginWindow* m_login_window;
    loginWindow* m_signup_window;
};

#endif // SETTINGS_WINDOW_H
