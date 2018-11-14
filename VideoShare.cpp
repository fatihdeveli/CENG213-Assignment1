#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */

VideoShare::VideoShare() {

}

VideoShare::~VideoShare() {
    users.clear();
    videos.clear();
}

void VideoShare::createUser(const string &userName, const string &name, const string &surname) {
    User* user = new User(userName, name, surname);
    users.insertNode(users.getHead(), *user);

}

void VideoShare::loadUsers(const string &fileName) {
    ifstream usersFile;
    string line;
    usersFile.open(fileName.c_str());
    while (usersFile.good()) { // Read a line
        getline(usersFile, line);cout << "1" << endl;
        stringstream ss(line);cout << "2" << endl;
        string username, name, surname;cout << "3" << endl;
        // Read 3 fields
        getline(ss, username, ';'); // Username
        if (username.empty()) continue; // A user must have a username
        getline(ss, name, ';'); // Name
        getline(ss, surname, ';'); // Surname
        User newUser(username, name, surname);
        users.insertNode(users.getHead(), newUser);
    }
    usersFile.close();

}

void VideoShare::createVideo(const string &title, const string &genre) {
    Video* video = new Video(title, genre);
    videos.insertNode(videos.findPrev(*video), *video);
}

void VideoShare::loadVideos(const string &fileName) {
    ifstream videosFile;
    string line;
    videosFile.open(fileName.c_str());
    while (videosFile.good() && getline(videosFile, line)) { // Read a line
        stringstream ss(line);
        string title, genre;
        // Read 2 fields
        getline(ss, title, ';'); // Title
        if (title.empty()) continue; // A video must have a title
        getline(ss, genre, ';'); // Genre
        Video newVideo(title, genre);
        videos.insertNode(videos.getHead(), newVideo);
    }
    videosFile.close();
}

void VideoShare::addFriendship(const string &userName1, const string &userName2) {
    if (userName1.empty() || userName2.empty()) return;
    Node <User>* userNode1 = users.findNode(userName1);
    Node <User>* userNode2 = users.findNode(userName2);
    if (!userNode1 || !userNode2) return;
    User* user1 = userNode1->getDataPtr();
    User* user2 = userNode2->getDataPtr();
    user1->addFriend(user2);
    user2->addFriend(user1);
}

void VideoShare::removeFriendship(const string &userName1, const string &userName2) {

}

void VideoShare::updateUserStatus(const string &userName, Status newStatus) {

}

void VideoShare::subscribe(const string &userName, const string &videoTitle) {

}

void VideoShare::unSubscribe(const string &userName, const string &videoTitle) {

}

void VideoShare::deleteUser(const string &userName) {

}

void VideoShare::sortUserSubscriptions(const string &userName) {

}

void VideoShare::printUserSubscriptions(const string &userName) {

}

void VideoShare::printFriendsOfUser(const string &userName) {

}

void VideoShare::printCommonSubscriptions(const string &userName1, const string &userName2) {

}

void VideoShare::printFriendSubscriptions(const string &userName) {

}

bool VideoShare::isConnected(const string &userName1, const string &userName2) {
    return false;
}

