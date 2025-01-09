#ifndef POST_H
#define POST_H

#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "user.h"
#include "network.h"

using namespace std;

class User;
class Network;

class Post {
public:
	//default constructor
	Post();

	//parameterized constructor
    Post(int messageId, int ownerId, string message, int likes, int privacyLevel);

	//returns a string relating message to likes
	virtual string toString();

	//getter method - returns the message id
	int getMessageId();

	//getter method - returns the owner's id
	int getOwnerId();

    //getter method - returns the message
	string getMessage();

	//getter method - returns the number of users who have liked the post
	int getLikes();

    //getter method - returns the privacy level
    int getPrivLevel();

	//virtual function - returns ""
	virtual string getAuthor();

	//virtual function - returns true
	virtual bool getIsPublic();

    //add a like to a post
    void addLike();

private:
    int messageId_;
	int ownerId_;
	string message_;
	int likes_;
    int privLevel;
};

// class IncomingPost : public Post {
// public:
// 	//default constructor
// 	IncomingPost();

// 	//parameterized constructor
//     IncomingPost(int messageId, int ownerId, string message, int likes, int privacyLevel, bool isPublic, string author);

// 	//writes a message relating author_ to isPublic_
// 	string toString();

// 	//getter method - returns author_
// 	string getAuthor();

// 	//getter method - returns isPublic_
// 	bool getIsPublic();

// private:
// 	string author_;
//     bool isPublic_;
// };

#endif
