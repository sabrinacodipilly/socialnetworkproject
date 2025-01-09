#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>


SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);

    ui->loginButton->show();
    ui->loginTextEdit->show();
    ui->loginLabel->show();
    ui->loginFailedLabel->show();
    ui->stackedWidget->setCurrentIndex(0);

    userChoosePrivLevel();

    network.readUsers("users.txt");

    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::loginButtonClick);

    connect(ui->createAccountButton, &QPushButton::clicked, this, &SocialNetworkWindow::createAccountPage);
    connect(ui->officialCreateAccountButton, &QPushButton::clicked, this, &SocialNetworkWindow::createAccountButtonClick);

    connect(ui->privateButton, &QPushButton::clicked, this, &SocialNetworkWindow::privButtonClick);
    connect(ui->semiPrivateButton, &QPushButton::clicked, this, &SocialNetworkWindow::semiprivButtonClick);
    connect(ui->publicButton, &QPushButton::clicked, this, &SocialNetworkWindow::publicButtonClick);

    network.readPosts("posts.txt");

    connect(ui->friendsTableWidget, &QTableWidget::cellClicked, this, &SocialNetworkWindow::friendCellClicked);

    connect(ui->suggestedFriendsTableWidget, &QTableWidget::cellClicked, this, &SocialNetworkWindow::addSuggestedFriend);

    connect(ui->myprofileButton, &QPushButton::clicked, this, &SocialNetworkWindow::backToMyProfile);

    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::addFriends);

    connect(ui->removeFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::removeFriendButtonClick);

    connect(ui->searchButton, &QPushButton::clicked, this, &SocialNetworkWindow::searchButtonClick);

    connect(ui->writePostButton, &QPushButton::clicked, this, &SocialNetworkWindow::writePostsButtonClick);
    //connect(ui->postPrivacySetting, &QComboBox::currentIndexChanged, this, &SocialNetworkWindow::postsPrivacy);
    connect(ui->publishButton, &QPushButton::clicked, this, &SocialNetworkWindow::publishPosts);

    connect(ui->likeButton1, &QPushButton::clicked, this, &SocialNetworkWindow::likeButton1Click);
    connect(ui->likeButton2, &QPushButton::clicked, this, &SocialNetworkWindow::likeButton2Click);
    connect(ui->likeButton3, &QPushButton::clicked, this, &SocialNetworkWindow::likeButton3Click);
    connect(ui->likeButton4, &QPushButton::clicked, this, &SocialNetworkWindow::likeButton4Click);
    connect(ui->likeButton5, &QPushButton::clicked, this, &SocialNetworkWindow::likeButton5Click);
}

void SocialNetworkWindow::Display(){
    if (privLevelChanged) {
        ui->addFriendButton->hide();
        ui->removeFriendButton->hide();
        ui->viewableLabel->hide();
        ui->searchBar->show();
        ui->searchBar->setPlaceholderText("Search...");
        ui->searchButton->show();
        ui->myprofileButton->hide();
        ui->writePostsTextEdit->hide();
        ui->postPrivacySetting->hide();
        ui->publishButton->hide();

        ui->stackedWidget->setCurrentIndex(3);
        ui->friendsTableWidget->show();
        ui->profileLabel->show();
        ui->suggestedFriendsTableWidget->show();
        ui->addSuggestedFriendLabel->show();
        ui->suggestedFriendsLabel->show();
        ui->profileLabel->setText(QString::fromStdString("My Profile"));
        ui->writePostButton->show();

        //show recent posts
        string recentPosts = network.getPostsString(loggedInUser->getId(), 5);
        stringstream ss(recentPosts);
        string line;
        vector<string> lines;

        while(getline(ss, line)) {
            lines.push_back(line);
        }

        ui->recentPostsLabel1->setText(QString::fromStdString(lines[0]));
        ui->recentPostsLabel1->setWordWrap(true);
        ui->recentPostsLabel2->setText(QString::fromStdString(lines[2]));
        ui->recentPostsLabel2->setWordWrap(true);
        ui->recentPostsLabel3->setText(QString::fromStdString(lines[4]));
        ui->recentPostsLabel3->setWordWrap(true);
        ui->recentPostsLabel4->setText(QString::fromStdString(lines[6]));
        ui->recentPostsLabel4->setWordWrap(true);
        ui->recentPostsLabel5->setText(QString::fromStdString(lines[8]));
        ui->recentPostsLabel5->setWordWrap(true);

        displayPosts(lines);

        populateFriendsList(loggedInUser);
        populateSuggestedFriends();
    }
}

void SocialNetworkWindow::friendProfilePage() {
    checkPrivacyLevel();

    ui->myprofileButton->show();
    ui->profileLabel->show();
    ui->viewableLabel->show();

    ui->suggestedFriendsTableWidget->hide();
    ui->addSuggestedFriendLabel->hide();
    ui->suggestedFriendsLabel->hide();
    ui->writePostButton->hide();
    ui->writePostsTextEdit->hide();
    ui->postPrivacySetting->hide();
    ui->publishButton->hide();

    const set<int>& loggedInFriends = loggedInUser->getFriends();
    bool friends = false;
    for (int friendID : loggedInFriends) {
        if (friendID == otherUser->getId()) {
            friends = true;
            break;
        }
    }
    if (friends) {
        ui->removeFriendButton->show();
        ui->addFriendButton->hide();
    } else {
        ui->addFriendButton->show();
        ui->removeFriendButton->hide();
    }

    populateFriendsList(otherUser);
    ui->profileLabel->setText(QString::fromStdString(otherUser->getName() + "'s profile"));
}

void SocialNetworkWindow::backToMyProfile() {
    loggedIn = true;
    Display();
}

void SocialNetworkWindow::loginButtonClick() {
    QString enteredUser = ui->loginTextEdit->toPlainText();
    std::string username = enteredUser.toStdString();
    if(!network.getUserByName(username)) {
        ui->loginFailedLabel->setText("Login failed :( Please try again.");
        loggedIn = false;
    } else{
        loggedIn = true;
        int loggedInUserId = network.getId(username);
        loggedInUser = network.getUser(loggedInUserId);
    }

    userChoosePrivLevel();

}

void SocialNetworkWindow::populateFriendsList(User* u) {
    int size = u->getFriends().size();

    ui->friendsTableWidget->setRowCount(size);
    ui->friendsTableWidget->setColumnCount(1);


    int row = 0;
    set<int> ids = u->getFriends();

    for(int id : ids) {
        User* friendUser = network.getUser(id);
        if(friendUser != nullptr) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));
            ui->friendsTableWidget->setItem(row,0,item);
            row++;
        }
    }
}

void SocialNetworkWindow::friendCellClicked(int row, int column) {
    loggedIn = false;
    QString friendName = ui->friendsTableWidget->item(row, column)->text();
    if (friendName.toStdString() == loggedInUser->getName()) {
        backToMyProfile();
    } else {
        otherUser = network.getUser(network.getId(friendName.toStdString()));

        friendProfilePage();
    }
}

void SocialNetworkWindow::populateSuggestedFriends() {    
    if (loggedInUser->getFriends().empty()) {
        int min = 0;
        int max = network.numUsers();
        int i = 10;
        int row1 = 0;
        ui->suggestedFriendsTableWidget->setRowCount(i);
        ui->suggestedFriendsTableWidget->setColumnCount(1);
        while (i > 0) {
            int randomNum = min + rand() % max;
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(network.getUser(randomNum)->getName()));
            ui->suggestedFriendsTableWidget->setItem(row1,0,item);
            row1++;
            i--;
        }
    } else {
        vector<int> ids = network.suggestFriends(loggedInUser->getId());
        int size = network.suggestFriends(loggedInUser->getId()).size();
        ui->suggestedFriendsTableWidget->setRowCount(size);
        ui->suggestedFriendsTableWidget->setColumnCount(1);
        int row = 0;
        for(int id : ids) {
            User* friendUser = network.getUser(id);
            if(friendUser != nullptr) {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));
                ui->suggestedFriendsTableWidget->setItem(row,0,item);
                row++;
            }
        }
    }
}

void SocialNetworkWindow::addSuggestedFriend(int row, int column) {
    QString suggestedFriend = ui->suggestedFriendsTableWidget->item(row, column)->text();
    User* sf = network.getUser(network.getId(suggestedFriend.toStdString()));
    loggedInUser->addFriend(sf->getId());
    network.writeUsers("users.txt");
    ui->addSuggestedFriendLabel->setText(QString::fromStdString("Friend added!"));
}

void SocialNetworkWindow::addFriends() {
    loggedInUser->addFriend(otherUser->getId());
    network.writeUsers("users.txt");
}

void SocialNetworkWindow::checkPrivacyLevel() {
    int priv = otherUser->getPrivacyLevel();
    const vector<Post*> posts = otherUser->getPosts();
    const set<int>& loggedInFriends = loggedInUser->getFriends();
    const set<int>& otherUserFriends = otherUser->getFriends();

    //check if otherUser and loggedInUser are friends
    bool friends = false;
    for (int friendID : loggedInFriends) {
        if (friendID == otherUser->getId()) {
            friends = true;
            break;
        }
    }

    //find out how much to loop through posts
    int size;
    if (posts.size() < 5) {
        size = posts.size();
    } else {
        size = 5;
    }

    //check privacy levels of posts
    stringstream ss;
    for (int i = 0; i < size; i++) {
        if (posts[i]->getPrivLevel() == 2) {
            //private so only friends can see
            if (friends) {
                ss << posts[i]->toString() << "\n\n";
            }
        } else if (posts[i]->getPrivLevel() == 1) {
            //semipriv so only friends or friends of friends can see
            if (friends) {
                ss << posts[i]->toString() << "\n\n";
            } else {
                bool found = false;
                for (int friendID : loggedInFriends) {
                    for (int potentialFriend : otherUserFriends) {
                        if (friendID == potentialFriend) {
                            ss << posts[i]->toString() << "\n\n";
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                }
            }
        } else {
            //public so anyone can see
            ss << posts[i]->toString() << "\n\n";
        }
    }

    //string recentPosts = network.getPostsString(loggedInUser->getId(), 5, false);
    ui->stackedWidget->setCurrentIndex(3);

    //adding the recent posts
    string line;
    vector<string> lines;

    while(getline(ss, line)) {
        lines.push_back(line);
    }

    if (!lines.empty()) {
        ui->recentPostsLabel1->setText(QString::fromStdString(lines[0]));
        ui->recentPostsLabel1->setWordWrap(true);
        ui->recentPostsLabel2->setText(QString::fromStdString(lines[2]));
        ui->recentPostsLabel2->setWordWrap(true);
        ui->recentPostsLabel3->setText(QString::fromStdString(lines[4]));
        ui->recentPostsLabel3->setWordWrap(true);
        ui->recentPostsLabel4->setText(QString::fromStdString(lines[6]));
        ui->recentPostsLabel4->setWordWrap(true);
        ui->recentPostsLabel5->setText(QString::fromStdString(lines[8]));
        ui->recentPostsLabel5->setWordWrap(true);
    }

    displayPosts(lines);

    //check privacy levels of accounts
    if (priv == 2) {
        ui->friendsTableWidget->hide();
        ui->viewableLabel->setText("Private Account (not friends)");

        //checks if loggedInUser is friends with otherUser
        if (friends) {
            ui->friendsTableWidget->show();
            ui->viewableLabel->setText("Private Account (You are friends)");
        }
    } else if (priv == 1) {
        //check if loggedInUser and otherUser are friends, if not then checks if loggedInUser is a friend of
        // a friend

        ui->friendsTableWidget->hide();
        ui->viewableLabel->setText("Semi-Private Account (not friends or friends of friends)");

        if (friends) {
            ui->friendsTableWidget->show();
            ui->viewableLabel->setText("Semi-Private Account (You are friends)");
        } else {
            for (int friendID : loggedInFriends) {
                for (int potentialFriend : otherUserFriends) {
                    if (friendID == potentialFriend) {
                        ui->friendsTableWidget->show();
                        ui->viewableLabel->setText("Semi-Private Account (You are friends of friends)");
                        break;
                    }
                }
            }
        }
    } else {
        //otherUser is public so their profile can be seen by anyone
        ui->friendsTableWidget->show();
        ui->viewableLabel->setText("Public Account");
    }
}

void SocialNetworkWindow::userChoosePrivLevel() {
    //the page the user sees after they login to choose which privacy setting they want
    if (loggedIn) {
        privLevelChanged = false;
        ui->stackedWidget->setCurrentIndex(2);
    }
}

//if the user wishes to change their account to private
void SocialNetworkWindow::privButtonClick() {
    loggedInUser->changePrivacyLevel(2);
    network.writeUsers("users.txt");
    privLevelChanged = true;
    Display();
}

//if the user wishes to change their account to semi-private
void SocialNetworkWindow::semiprivButtonClick() {
    loggedInUser->changePrivacyLevel(1);
    network.writeUsers("users.txt");
    privLevelChanged = true;
    Display();
}

//if the user wishes to change their account to public
void SocialNetworkWindow::publicButtonClick() {
    loggedInUser->changePrivacyLevel(0);
    network.writeUsers("users.txt");
    privLevelChanged = true;
    Display();
}

void SocialNetworkWindow::searchButtonClick() {
    QString searchedUser = ui->searchBar->toPlainText();
    std::string username = searchedUser.toStdString();
    if(!network.getUserByName(username)) {
        ui->searchBar->setPlaceholderText("User not found");
        ui->searchBar->setText("");
    } else{
        int otherUserId = network.getId(username);
        otherUser = network.getUser(otherUserId);
        friendProfilePage();
        ui->searchBar->setPlaceholderText("Search...");
        ui->searchBar->setText("");
    }
}

void SocialNetworkWindow::createAccountPage() {
    ui->stackedWidget->setCurrentIndex(1);
}

void SocialNetworkWindow::createAccountButtonClick() {
    QString getName = ui->createName->toPlainText();
    std::string newName = getName.toStdString();

    QString getYear = ui->createYear->toPlainText();
    int newYear = getYear.toInt();

    QString getZip = ui->createZip->toPlainText();
    int newZip = getZip.toInt();

    set<int> friends;

    User* newUser = new User(network.numUsers(), newName, 0, newYear, newZip, friends);
    network.addUser(newUser);
    loggedInUser = network.getUser(newUser->getId());;
    network.writeUsers("users.txt");
    loggedIn = true;
    userChoosePrivLevel();
}

void SocialNetworkWindow::removeFriendButtonClick() {
    network.deleteConnection(loggedInUser->getName(), otherUser->getName());
    network.writeUsers("users.txt");
}

void SocialNetworkWindow::writePostsButtonClick() {
    ui->writePostsTextEdit->show();
    ui->postPrivacySetting->show();
    ui->publishButton->show();

    ui->recentPostsLabel1->hide();
    ui->recentPostsLabel2->hide();
    ui->recentPostsLabel3->hide();
    ui->recentPostsLabel4->hide();
    ui->recentPostsLabel5->hide();
    ui->writePostButton->hide();

    ui->likeButton1->hide();
    ui->likeButton2->hide();
    ui->likeButton3->hide();
    ui->likeButton4->hide();
    ui->likeButton5->hide();
}

int SocialNetworkWindow::postsPrivacy() {
    int postPriv;
    QString text = ui->postPrivacySetting->currentText();
    if (text == "Public") {
        postPriv = 0;
    } else if (text == "Semi-Private") {
        postPriv = 1;
    } else {
        postPriv = 2;
    }
    return postPriv;
}

void SocialNetworkWindow::publishPosts() {
    QString enteredPost = ui->writePostsTextEdit->toPlainText();
    std::string newPost = enteredPost.toStdString();

    network.addPost(loggedInUser->getId(), newPost, 0, postsPrivacy());
    network.writePosts("posts.txt");
    ui->writePostsTextEdit->hide();
    ui->postPrivacySetting->hide();
    ui->publishButton->hide();

    ui->recentPostsLabel1->show();
    ui->recentPostsLabel2->show();
    ui->recentPostsLabel3->show();
    ui->recentPostsLabel4->show();
    ui->recentPostsLabel5->show();
    ui->writePostButton->show();

    ui->likeButton1->show();
    ui->likeButton2->show();
    ui->likeButton3->show();
    ui->likeButton4->show();
    ui->likeButton5->show();
}

void SocialNetworkWindow::displayPosts(vector<string> lines) {
    if (lines.size() == 2) {
        ui->recentPostsLabel1->show();
        ui->likeButton1->show();

        ui->recentPostsLabel2->hide();
        ui->likeButton2->hide();
        ui->recentPostsLabel3->hide();
        ui->likeButton3->hide();
        ui->recentPostsLabel4->hide();
        ui->likeButton4->hide();
        ui->recentPostsLabel5->hide();
        ui->likeButton5->hide();
    } else if (lines.size() == 4) {
        ui->recentPostsLabel1->show();
        ui->likeButton1->show();
        ui->recentPostsLabel2->show();
        ui->likeButton2->show();

        ui->recentPostsLabel3->hide();
        ui->likeButton3->hide();
        ui->recentPostsLabel4->hide();
        ui->likeButton4->hide();
        ui->recentPostsLabel5->hide();
        ui->likeButton5->hide();
    } else if (lines.size() == 6) {
        ui->recentPostsLabel1->show();
        ui->likeButton1->show();
        ui->recentPostsLabel2->show();
        ui->likeButton2->show();
        ui->recentPostsLabel3->show();
        ui->likeButton3->show();

        ui->recentPostsLabel4->hide();
        ui->likeButton4->hide();
        ui->recentPostsLabel5->hide();
        ui->likeButton5->hide();
    } else if (lines.size() == 8) {
        ui->recentPostsLabel1->show();
        ui->likeButton1->show();
        ui->recentPostsLabel2->show();
        ui->likeButton2->show();
        ui->recentPostsLabel3->show();
        ui->likeButton3->show();
        ui->recentPostsLabel4->show();
        ui->likeButton4->show();

        ui->recentPostsLabel5->hide();
        ui->likeButton5->hide();
    } else if (lines.size() == 10) {
        ui->recentPostsLabel1->show();
        ui->likeButton1->show();
        ui->recentPostsLabel2->show();
        ui->likeButton2->show();
        ui->recentPostsLabel3->show();
        ui->likeButton3->show();
        ui->recentPostsLabel4->show();
        ui->likeButton4->show();
        ui->recentPostsLabel5->show();
        ui->likeButton5->show();
    } else {
        ui->recentPostsLabel1->hide();
        ui->likeButton1->hide();
        ui->recentPostsLabel2->hide();
        ui->likeButton2->hide();
        ui->recentPostsLabel3->hide();
        ui->likeButton3->hide();
        ui->recentPostsLabel4->hide();
        ui->likeButton4->hide();
        ui->recentPostsLabel5->hide();
        ui->likeButton5->hide();
    }
}

void SocialNetworkWindow::likeButton1Click() {
    QString postx = ui->recentPostsLabel1->text();
    std::string postxx = postx.toStdString();
    const char letterStop = 'L';
    int position = postxx.find(letterStop);
    string recentPost1 = postxx.substr(0, position-1);
    int messageId1 = otherUser->getPostByMessage(recentPost1);
    Post* post1 = otherUser->getPost(messageId1);

    post1->addLike();
    network.writePosts("posts.txt");
    checkPrivacyLevel();
}

void SocialNetworkWindow::likeButton2Click() {
    QString postx = ui->recentPostsLabel2->text();
    std::string postxx = postx.toStdString();
    const char letterStop = 'L';
    int position = postxx.find(letterStop);
    string recentPost2 = postxx.substr(0, position-1);
    int messageId2 = otherUser->getPostByMessage(recentPost2);
    Post* post2 = otherUser->getPost(messageId2);

    post2->addLike();
    network.writePosts("posts.txt");
    checkPrivacyLevel();
}

void SocialNetworkWindow::likeButton3Click() {
    QString postx = ui->recentPostsLabel3->text();
    std::string postxx = postx.toStdString();
    const char letterStop = 'L';
    int position = postxx.find(letterStop);
    string recentPost3 = postxx.substr(0, position-1);
    int messageId3 = otherUser->getPostByMessage(recentPost3);
    Post* post3 = otherUser->getPost(messageId3);

    post3->addLike();
    network.writePosts("posts.txt");
    checkPrivacyLevel();
}

void SocialNetworkWindow::likeButton4Click() {
    QString postx = ui->recentPostsLabel4->text();
    std::string postxx = postx.toStdString();
    const char letterStop = 'L';
    int position = postxx.find(letterStop);
    string recentPost4 = postxx.substr(0, position-1);
    int messageId4 = otherUser->getPostByMessage(recentPost4);
    Post* post4 = otherUser->getPost(messageId4);

    post4->addLike();
    network.writePosts("posts.txt");
    checkPrivacyLevel();
}

void SocialNetworkWindow::likeButton5Click() {
    QString postx = ui->recentPostsLabel5->text();
    std::string postxx = postx.toStdString();
    const char letterStop = 'L';
    int position = postxx.find(letterStop);
    string recentPost5 = postxx.substr(0, position-1);
    int messageId5 = otherUser->getPostByMessage(recentPost5);
    Post* post5 = otherUser->getPost(messageId5);

    post5->addLike();
    network.writePosts("posts.txt");
    checkPrivacyLevel();
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}
