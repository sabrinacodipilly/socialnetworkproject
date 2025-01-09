#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include "post.h"
#include "network.h"

using namespace std;

class Post;

class User {
public:
	//default constructor
	User();

	//parameterized constructor
    User(int id, string name, int privlevel, int year, int zip, set<int> friends);

	//pre: accepts ID of a user to add as a friend
	//post: adds the user into the friends list (if not already friends)
	void addFriend(int id);

	//pre: accepts ID of a user to delete as a friend
	//post: removes the user from the friends list (if in list)
	void deleteFriend(int id);

    //pre: accepts int number of either 0, 1, or 2 that depicts which privacy level the user wants
    //post: changes the user's privacy level
    void changePrivacyLevel(int level);

	//getter
	//post: returns the user's ID
	int getId();

    void setName(string& username);

	//getter
	//post: returns the user's name
	string getName();

    //getter
    //post: returns the user's privacy level
    int getPrivacyLevel();

	//getter
	//post: returns the user's birth year
	int getYear();

	//getter
	//post: returns the user's zipcode
	int getZip();

	//getter
	//post: returns a reference to the user's set of friends
	set<int>& getFriends();

	//add post to messages_
	void addPost(Post* post);

	//returns messages_
	vector<Post*> getPosts();

	//pre: takes in amount posts and if private posts should or shouldn't be included
	//post: returns a string with the most recent howMany posts using toString()
    string getPostsString(int howMany);

    //returns a pointer to Post
    Post* getPost(int messageID);

    //returns the message id of the post if its message is found; else returns -1
    int getPostByMessage(string message);


private:
	int id_;
	string name_;
    int privlevel_;
	int year_;
	int zip_;
	set<int> friends_;
	vector<Post*> messages_;
};

#endif
