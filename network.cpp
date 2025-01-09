#include "network.h"
#include "user.h"
#include "post.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

//default constructor
Network::Network() {

}

//takes id and returns pointer to corresponding user; if no user, return nullptr
User* Network::getUser(int id) {
	//checks that user id is valid
	if (id >= 0 && id < users_.size()) {
		return users_[id];
	}
	//if not valid
	return nullptr;
}

//adds user to database
void Network::addUser(User* user) {
	users_.push_back(user);
}

//adds a friend connection between two users (if doesn't exist)
int Network::addConnection(string s1, string s2) {
	int id1 = getId(s1);
	int id2 = getId(s2);

	//checks that ids are valid
	if(id1 == -1 || id2 == -1)
		return -1;

	//checks if they already are friends
	User* user1 = users_[id1];
	set<int>& friends1 = user1->getFriends();

	for (int f : friends1) {
		if (f == id2)
			return -1;
	}

	//adds a connection
	users_[id1]->addFriend(id2);
	users_[id2]->addFriend(id1);
	return 0;
}

//deletes friend connection between two users (if it exists)
int Network::deleteConnection(string s1, string s2) {
	int id1 = getId(s1);
	int id2 = getId(s2);

	//checks that ids are valid
	if(id1 == -1 || id2 == -1) {
		return -1;
	}

	//deletes a connection
	users_[id1]->deleteFriend(id2);
	users_[id2]->deleteFriend(id1);
	return 0;
}

//returns id of user (if user exists)
int Network::getId(string name) {
	//finds the id based on the user's name
	for (size_t i = 0; i <users_.size(); i++) {
		if (users_[i]->getName() == name) {
			return users_[i]->getId();
		}
	}
	return -1;
}

//returns number of users in the network
int Network::numUsers() {
	return users_.size();
}

//initializes all of the network's information from a file
void Network::readUsers(const char* fname){
	ifstream infile(fname);

	string myline;

	//number of users
	getline(infile, myline);
	int numUsers = stoi(myline);

	//loop through each user
	for (int i = 0; i < numUsers; i++) {
		//read the user ID
		getline(infile, myline);
		int id = stoi(myline);

		//read name
		getline(infile, myline);
		string name = myline.substr(1);

        //read privacy level
        getline(infile, myline);
        int privLevel = stoi(myline);

		//read year
		getline(infile, myline);
		int year = stoi(myline);

		//read zipcode
		getline(infile, myline);
		int zip = stoi(myline);

		//read list of friends
		getline(infile, myline);
		int fID;
		set<int> friends;
		stringstream ss(myline);
        if (!myline.empty()) {
            while (ss >> fID) {
                friends.insert(fID);
            }
        } else {
            friends = {};
        }

        User* newUser = new User(id, name, privLevel, year, zip, friends);
        addUser(newUser);
	}
}

//writes all of the network's information to a file
void Network::writeUsers(char* fname) {
	ofstream myfile;
	myfile.open(fname);

	//write number of users
	myfile << users_.size() << endl;

	//loop through each user
	for (User* user : users_) {

		//write id
		myfile << user->getId() << endl;

		//write name
		myfile << "\t" << user->getName() << endl;

        //write privacy level
        myfile << "\t" << user->getPrivacyLevel() << endl;

		//write birth year
		myfile << "\t" << user->getYear() << endl;

		//write zipcode
		myfile << "\t" << user->getZip() << endl;

		//write list of friends
		set<int>& friends = user->getFriends();
        myfile << "\t";
        // for (int fID : friends) {
        //     myfile << fID << " ";
        // }
        // myfile << endl;

        if (friends.empty()) {
            myfile << "\n\n";
        } else {
            for (int fID : friends) {
                myfile << fID << " ";
            }
            myfile << endl;
        }
	}
	myfile.close();
}

//finds shortest path between two users
vector<int> Network::shortestPath(int from, int to) {
	queue<int> q;
	vector<bool> visited (users_.size(), 0);
	vector<int> prev (users_.size(), -1);

	visited[from] = true;
	q.push(from);

	while (q.size() > 0) {
		int cur = q.front();
		q.pop();

		User* user = users_[cur];

		for (auto neighbor : user->getFriends()) {
			if (!visited[neighbor]) {
				prev[neighbor] = cur;
				visited[neighbor] = true;
				q.push(neighbor);

				if (neighbor == to)
					break;
			}
		}
	}

	vector<int> output;
	int cur = to;
	while (cur != -1) {
		output.push_back(cur);
		cur = prev[cur];
	}
	reverse(output.begin(), output.end());
	return output;
}

vector<int> Network::distanceUser(int from, int& to, int distance) {
	queue<int> q;
	vector<bool> visited(users_.size(), 0);
	vector<int> dist(users_.size(), -1);
	vector<int> prev(users_.size(), -1);

	dist[from] = 0;
	visited[from] = true;
	q.push(from);
	while(q.size() > 0) {
		int cur = q.front();
		q.pop();

		User* user = users_[cur];

		for (int neighbor : user->getFriends()) {
			if (!visited[neighbor]) {
				dist[neighbor] = dist[cur] + 1;
				prev[neighbor] = cur;
				visited[neighbor] = true;
				q.push(neighbor);
				if (dist[neighbor] == distance) {
					to = neighbor;
					vector<int> path;
					for (int at = neighbor; at != -1; at = prev[at]) {
						path.push_back(at);
					}
					reverse(path.begin(), path.end());
					return path;
				}
			}
		}
	}
	to = -1;
	return {};
}

vector<int> Network::suggestFriends(int who) {
	vector<int> scores(users_.size(), 0);
	const set<int>& friendsOfWho = users_[who]->getFriends();
    int score = 0;

	//loop through all of user's friends to find friends of friends
	for (int friendID : friendsOfWho) {
		const set<int>& friendsOfFriend = users_[friendID]->getFriends();
		for (int potentialFriend : friendsOfFriend) {
			if (potentialFriend != who && !(friendsOfWho.count(potentialFriend))) {
				scores[potentialFriend]++;
				score = max(score, scores[potentialFriend]);
			}
		}
	}

	//collect all users with the highest score
	vector<int> suggestions;
	for (int i = 0; i < scores.size(); i++) {
        if (scores[i] == score && score > 0) {
			suggestions.push_back(i);
		}
	}

	//if no potiental friends found, set score to 0
	if (suggestions.empty()) {
		score = 0;
	}

	return suggestions;
}

vector<vector<int>> Network::groups() {
	vector<vector<int>> allComponents;
	vector<bool> visited(users_.size(), false);

	for (int userId = 0; userId < users_.size(); ++userId) {
		if (!visited[userId]) {
			vector<int> component;
			stack<int> stck;
			stck.push(userId);

			while (!stck.empty()) {
				int currentUser = stck.top();
				stck.pop();

				if (!visited[currentUser]) {
					visited[currentUser] = true;
					component.push_back(currentUser);

					//add all unvisited friends of current user to stack
					User* user = users_[currentUser];
					for (int friendID : user->getFriends()) {
						if (!visited[friendID]) {
							stck.push(friendID);
						}
					}
				}
			}

			//add this component to the list of all components
			allComponents.push_back(component);
		}
	}

	return allComponents;
}

void Network::addPost(int ownerId, string message, int likes, int privacyLevel) {
	User* user = getUser(ownerId);
	int messageId = 0;

	int mi = 0;
	for (int i = 0; i < users_.size(); i++) {
		for (int k = 0; k < users_[i]->getPosts().size(); k++) {
			mi++;
		}
	}

	messageId = mi;

	Post* post;
    // if (isIncoming) {
 //        post = new IncomingPost(messageId, ownerId, message, likes, privacyLevel, isPublic, authorName);
    // } else {
 //        post = new Post(messageId, ownerId, message, likes, privacyLevel);
    // }

    post = new Post(messageId, ownerId, message, likes, privacyLevel);
	user->addPost(post);
}

string Network::getPostsString(int ownerId, int howMany) {
    return users_[ownerId]->getPostsString(howMany);
}

int Network::readPosts(const char* fname) {
	ifstream infile(fname);

	if (!infile.is_open()) {
		return -1;
	}

	string myline;

	//number of posts
	int numPosts;
	getline(infile, myline);
	numPosts = stoi(myline);

	//loop through each post
	for(int i = 0; i < numPosts; ++i) {
		//get message id
		string imessageid;
		int messageId;
		getline(infile, imessageid);
		messageId = stoi(imessageid);

		//get message text
		string imessagetext;
		getline(infile, imessagetext);
		string messageText = imessagetext.substr(1);

		//get owner id
		string iownerid;
		int ownerId;
		getline(infile, iownerid);
		iownerid = iownerid.substr(1);
		ownerId = stoi(iownerid);

		//get likes
		string ilikes;
		int likes;
		getline(infile, ilikes);
		ilikes = ilikes.substr(1);
		likes = stoi(ilikes);

        //privacy level
        //bool isPublic = false;
        int privacyLevel;
        //bool isIncoming;
        //string author;

        //string iauthor;
		string ipublic;
		getline(infile, ipublic);

        string status = ipublic.substr(1);
        if (status == "public") {
            //isPublic = true;
            privacyLevel = 0;
        } else if (status == "semi-private") {
            privacyLevel = 1;
        } else {
            privacyLevel = 2;
        }
        // getline(infile, iauthor);
        // if (!iauthor.empty()) {
        //     isIncoming = true;
        //     author = iauthor;
        // } else {
        //     isIncoming = false;
        //     author = "";
        // }

        addPost(ownerId, messageText, likes, privacyLevel);
	}
	infile.close();
	return 0;
}

bool compare(Post* a, Post* b) {
	return a->getMessageId() < b->getMessageId();
}

int Network::writePosts(const char* fname) {
	vector<Post*> messages;

	for (int i = 0; i <users_.size(); i++) {
		for (Post* p : users_[i]->getPosts()) {
			messages.push_back(p);
		}
	}

	ofstream outfile(fname);

	if(!outfile.is_open()) {
		return -1;
	}

	//write number of posts
	outfile << messages.size() << endl;

	sort(messages.begin(), messages.end(), compare);

	for(Post* post : messages) {
		outfile << post->getMessageId() << endl;
		outfile << "\t" <<  post->getMessage() << endl;
		outfile << "\t" << post->getOwnerId() << endl;
		outfile << "\t" << post->getLikes() << endl;
        if (post->getPrivLevel() == 0) {
            outfile << "\t" << "public" << endl;
        } else if (post->getPrivLevel() == 1) {
            outfile << "\t" << "semi-private" << endl;
        } else {
            outfile << "\t" << "private" << endl;
        }

        // if (post->getAuthor() != "") {
        // 	outfile << post->getAuthor() << endl;
        // } else {
        // 	outfile << "\n\n";
        // }
	}
	outfile.close();
	return 0;
}

bool Network::getUserByName(std::string& name) {
    for(User* user : users_) {
        if(user->getName() == name) {
            return true;
        }
    }
    return false;
}
