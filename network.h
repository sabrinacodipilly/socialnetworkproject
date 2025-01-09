#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include <set>
#include "post.h"
#include "user.h"

using namespace std;

class User;
class Post;

class Network {
public:
	//default constructor
	Network();

	//pre: takes in an ID
	//post: returns a pointer to the corresponding user (if no user with that id, returns a nullptr)
	User* getUser(int id);

	//pre: takes in a pointer to a user
	//post: adds the user to the network
	void addUser(User* user);

	//pre: take in two strings for the names of two users
	//post: adds a friend connection between the two if it doesn't already exist (returns -1 if invalid and returns 0 otherwise)
	int addConnection(string s1, string s2);

	//pre: takes in two strings for the names of two users
	//post: deletes the friend connection between the two if it exists (returns -1 if invalid and 0 otherwise)
	int deleteConnection(string s1, string s2);

	//pre: takes a string for the user's name
	//post: returns the id for that user or -1 if user doesn't exist
	int getId(string name);

	//pre:
	//post: returns the number of users in the network
	int numUsers();

	//pre: takes in a file name
	//post: initializes all of the network's information from a file
	void readUsers(const char* fname);

	//pre: takes in a file name
	//post: writes all of the network's information to a file
	void writeUsers(char* fname);

	//pre: takes in two users' ids
	//post: returns a vector containing the shortest path of vertics starting at user from and ending at user to
	vector<int> shortestPath(int from, int to);

	//pre: takes in a user's id and the distance
	//post: returns the shortest path
	vector<int> distanceUser(int from, int& to, int distance);

	//pre: takes in a user
	//post: returns a list of friend suggestions for the user that contains friend suggestions with a lot of mutual friends
	//post: if no suggestions exist, return empty vector and set score to 0
    vector<int>  suggestFriends(int who);

	//pre: computes the connected components of the network
	//post: ouputs a list of lists, where each list contains the ids of the users in one connected component
	vector<vector<int>> groups();

	//pre: takes in owner id, message, likes, isIncoming, author name, and if it's public or not
	//post: adds a new post to the messages vector of the user whose id is ownerId
    void addPost(int ownerId, string message, int likes, int privacyLevel);

	//pre: takes in owner id, how many posts, and if it's public or not
	//post: makes a call to User::getPostsString and returns the result
    string getPostsString(int ownerId, int howMany);

	//pre: takes in a file name
	//post: reads the posts from a file; returns -1 if file cannot be opened
	int readPosts(const char* fname);

	//pre: takes in a file name
	//post: writes the posts to a file; returns -1 if file cannot be opened
	//		load all of the posts from all users into a single vector of Post pointers and sort the posts by their messageId using STL sort method, then write them in that order
	int writePosts(const char* fname);

    //pre: takes in a string that is the user'name
    //post: searches for the user by their name and returns true if that user exists and false if they don't
    bool getUserByName(string& name);

private:
	vector<User*> users_;
};

#endif
