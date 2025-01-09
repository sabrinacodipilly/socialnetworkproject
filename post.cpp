#include "post.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

//default constructor
Post::Post(){

}

//parameterized constructor
Post::Post(int messageId, int ownerId, string message, int likes, int privacyLevel) {
	messageId_ = messageId;
	ownerId_ = ownerId;
	message_ = message;
	likes_ = likes;
    privLevel = privacyLevel;
}

//returns a string "[message_] liked by [likes_] people"
string Post::toString() {
    string privacyLevel;
    if (privLevel == 2) {
        privacyLevel = "private";
    } else if (privLevel == 1) {
        privacyLevel = "semi-private";
    } else {
        privacyLevel = "public";
    }
    return getMessage() + " Liked by " + to_string(likes_) + " people." + " (" + privacyLevel + ")";
}

//returns messageId_
int Post::getMessageId() {
	return messageId_;
}

//returns ownerId_
int Post::getOwnerId() {
	return ownerId_;
}

//returns message_
string Post::getMessage() {
	return message_;
}

//returns likes_
int Post::getLikes() {
	return likes_;
}

//returns the privacy level
int Post::getPrivLevel() {
    return privLevel;
}

//returns ""
string Post::getAuthor() {
	return "";
}

//returns true
bool Post::getIsPublic() {
	return true;
}

void Post::addLike() {
    likes_++;
}

//IncomingPost class

//default constructor
// IncomingPost::IncomingPost() {

// }

// //parameterized constructor
// IncomingPost::IncomingPost(int messageId, int ownerId, string message, int likes, int privacyLevel, bool isPublic, string author)
//     : Post(messageId, ownerId, message, likes, privacyLevel), author_(author), isPublic_(isPublic) {
// }

// //returns "[author_] wrote[private]: [toString]"
// string IncomingPost::toString() {
// 	string priv = "";
// 	if (!isPublic_) {
// 		priv = " (private)";
// 	}
// 	return author_ + " wrote" + priv + ": " + Post::toString();
// }

// //returns author_
// string IncomingPost::getAuthor() {
// 	return author_;
// }

// //returns isPublic_
// bool IncomingPost::getIsPublic() {
// 	return isPublic_;
// }
