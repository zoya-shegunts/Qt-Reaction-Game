#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

enum class AccountMode
{
    Login,
    SignUp
};

enum class AccountState
{
    loggedin,
    loggedout
};

class loginWindow : public QWidget
{
    Q_OBJECT
public:
    loginWindow(AccountMode mode, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void loginSucceeded(const QString& username, double record);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    void createWidgets();
    void layoutWidgets();
    void connection();

private slots:
    void processAccount();

private:
    QLineEdit* m_username;
    QLineEdit* m_password;

    QPushButton* m_login;
    QLabel* m_signup;

    AccountMode m_mode;
    //AccountState m_state = AccountState::loggedout;
};

#endif // LOGIN_WINDOW_H
