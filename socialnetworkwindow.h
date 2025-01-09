#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QStackedWidget.h>
#include <QtWidgets/qtextedit.h>
#include "user.h"
#include "network.h"
#include "post.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    void Display();
    void friendProfilePage();
    void backToMyProfile();
    void loginButtonClick();
    void populateFriendsList(User* u);
    void friendCellClicked(int row, int column);
    void populateSuggestedFriends();
    void addSuggestedFriend(int row, int column);
    void addFriends();
    void checkPrivacyLevel();
    void userChoosePrivLevel();
    void privButtonClick();
    void semiprivButtonClick();
    void publicButtonClick();
    void searchButtonClick();
    void createAccountPage();
    void createAccountButtonClick();
    void removeFriendButtonClick();
    void writePostsButtonClick();
    int postsPrivacy();
    void publishPosts();
    void displayPosts(vector<string> lines);
    void likeButton1Click();
    void likeButton2Click();
    void likeButton3Click();
    void likeButton4Click();
    void likeButton5Click();

private:
    Ui::SocialNetworkWindow *ui;
    QTextEdit *loginTextEdit;
    SocialNetworkWindow* sn;
    User* loggedInUser;
    User* user;
    User* otherUser;
    Network network;
    bool loggedIn;
    bool privLevelChanged;
};
#endif // SOCIALNETWORKWINDOW_H
