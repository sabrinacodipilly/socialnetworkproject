#include <set>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include "user.h"
#include "post.h"

using namespace std;

//default constructor
User::User() {
	id_ = 0;
	name_ = "unknown";
    privlevel_ = 0;
	year_ = 1999;
	zip_ = 99999;
}

//parameterized constructor
User::User(int id, string name, int privlevel, int year, int zip, set<int> friends) {
	id_ = id;
	name_ = name;
    privlevel_ = privlevel;
	year_ = year;
	zip_ = zip;
	friends_ = friends;
}

//add friend (if not already friends)
void User::addFriend(int id) {
	friends_.insert(id);
}

//delete friend (if they exist)
void User::deleteFriend(int id) {
	friends_.erase(id);
}

//change user's privacy level
void User::changePrivacyLevel(int level) {
    privlevel_ = level;
}

//getter methods
int User::getId() {
	return id_;
}

void User::setName(string& username) {
    name_ = username;
}

string User::getName() {
	return name_;
}

int User::getPrivacyLevel() {
    return privlevel_;
}

int User::getYear() {
	return year_;
}

int User::getZip() {
	return zip_;
}

set<int>& User::getFriends() {
	return friends_;
}

void User::addPost(Post* post) {
	messages_.push_back(post);
}

vector<Post*> User::getPosts() {
	return messages_;
}

//reurn correct number of Posts' strings, where individual Post strings are separated by two newline characters
string User::getPostsString(int howMany) {
    stringstream ss;

	//copy the messages in reverse order so the most recent posts are first
	for(int i = messages_.size() - 1; i >= 0 && howMany > 0; --i) {
		Post* post = messages_[i];

        ss << post->toString() << "\n\n";

		--howMany;
	}

    return ss.str();

}

Post* User::getPost(int messageID) {
    Post* p;
    for (int i = 0; i < messages_.size(); i++) {
        if (messages_[i]->getMessageId() == messageID) {
            p = messages_[i];
        }
    }
    return p;
}

int User::getPostByMessage(string message) {
    for (Post* post : messages_) {
        if (post->getMessage() == message) {
            return post->getMessageId();
        }
    }
    return -1;
}
