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
    // users.clear() was removed from here
}

void VideoShare::createUser(const string &userName, const string &name, const string &surname) {
    User* user = new User(userName, name, surname);
    users.insertNode(users.getHead(), *user);

}

void VideoShare::loadUsers(const string &fileName) {
    ifstream usersFile;
    string line;
    usersFile.open(fileName.c_str());
    while (usersFile.good() && getline(usersFile, line)) { // Read a line
        stringstream ss(line);
        string username, name, surname;
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
    User user1 = User(userName1);
    User user2 = User(userName2);
    Node<User>* userNode1 = users.findNode(user1); // Pointer to the node in the friends list
    Node<User>* userNode2 = users.findNode(user2); //
    User* user1Ptr = userNode1->getDataPtr(); // Pointer to the user (of the node) in the friends list
    User* user2Ptr = userNode2->getDataPtr(); //
    userNode1->getDataPtr()->removeFriend(user2Ptr); // Remove user2 from user1's friend list
    userNode2->getDataPtr()->removeFriend(user1Ptr); // Remove user1 from user2's friend list
}

void VideoShare::updateUserStatus(const string &userName, Status newStatus) {
    User user = User(userName);
    Node<User>* userNode1 = users.findNode(user); // Pointer to the node in the friends list
    userNode1->getDataPtr()->updateStatus(newStatus);
}

void VideoShare::subscribe(const string &userName, const string &videoTitle) {
    Node <User>* userNode = users.findNode(User(userName));
    Node <Video>* videoNode = videos.findNode(Video(videoTitle));
    User* userPtr = userNode->getDataPtr();
    if (userPtr->getStatus() == ACTIVE) {
        Video* videoPtr = videoNode->getDataPtr();
        userPtr->subscribe(videoPtr);
    }
}

void VideoShare::unSubscribe(const string &userName, const string &videoTitle) {
    Node<User>* userNode = users.findNode(User(userName));
    Node<Video>* videoNode = videos.findNode(Video(videoTitle));
    User* userPtr = userNode->getDataPtr();
    if (userPtr->getStatus() == ACTIVE) {
        Video* videoPtr = videoNode->getDataPtr();
        userPtr->unSubscribe(videoPtr);
    }
}

void VideoShare::deleteUser(const string &userName) {
    User user = User(userName);
    Node<User>* userNode = users.findNode(user);
    if (!userNode) return;
    User* userPtr = userNode->getDataPtr();

    // Remove all friendships of the user
    for (Node<User> *temp = users.first(); temp; temp = temp->getNext()) {
        temp->getDataPtr()->removeFriend(userPtr);
    }
    users.deleteNode(users.findPrev(*userPtr));
}

void VideoShare::sortUserSubscriptions(const string &userName) {
    Node<User>* userNode = users.findNode(User(userName));
    User* user = userNode->getDataPtr();
    LinkedList<Video*>* userSubs = user->getSubscriptions();

     // Bubble sort
    bool sorted = false;
    int n = (int)userSubs->getLength();
    for (int i = 0; (i < n-1) && !sorted; i++) {
        sorted = true;
        for (int j = 1; j <= n-i-1; j++) {
            // if (list[j−1] > list[j])
            if (nodeAtIndex(userSubs, j-1)->getData()->getTitle() > nodeAtIndex(userSubs, j)->getData()->getTitle()) {
                userSubs->swap(j-1, j);
                sorted = false;
            }
        }
    }
}

void VideoShare::printUserSubscriptions(const string &userName) {
    User user = User(userName);
    Node<User>* userNode = users.findNode(user);
    userNode->getData().printSubscriptions();
}

void VideoShare::printFriendsOfUser(const string &userName) {
    User user = User(userName);
    Node<User>* tempNode = users.findNode(user);
    tempNode->getData().printFriends();
}

void VideoShare::printCommonSubscriptions(const string &userName1, const string &userName2) {
    User* user1 = users.findNode(User(userName1))->getDataPtr();
    User* user2 = users.findNode(User(userName2))->getDataPtr();
    Node<Video*>* user1Video = user1->getSubscriptions()->getHead()->getNext();
    Node<Video*>* user2Video = user2->getSubscriptions()->getHead()->getNext();

    while (user1Video && user2Video) {
        if (user1Video->getData()->getTitle() == user2Video->getData()->getTitle()) { // Common subscription
            cout << *(user1Video->getData());
            user1Video = user1Video->getNext();
            user2Video = user2Video->getNext();
        }
        else if (user1Video->getData()->getTitle() < user2Video->getData()->getTitle()) {
            user1Video = user1Video->getNext();
        }

        else if (user1Video->getData()->getTitle() > user2Video->getData()->getTitle()) {
            user2Video = user2Video->getNext();
        }
        else cout << "Something went wrong." << endl;
    }

}

void VideoShare::printFriendSubscriptions(const string &userName) {
    
}

bool VideoShare::isConnected(const string &userName1, const string &userName2) {
    return false;
}

template<class T>
Node<T *> *VideoShare::nodeAtIndex(const LinkedList<T*>* list, int index) {

    Node<T*>* node = list->getHead()->getNext();
    // Find the node with index

    for (int i = 0; i < index; i++) {
        node = node->getNext();
    }
    return node;
}

bool VideoShare::isSubscribed(Node<User*>* user, const Video* video) {
    LinkedList<Video*>* userSubscriptions = user->getData()->getSubscriptions();

    bool subscribed = false;
    Node<Video*> *temp = userSubscriptions->getHead()->getNext();
    while (temp) { // Search for the specific video in subscriptions
        if (temp->getData()->getTitle() == video->getTitle())
            subscribed = true;
        temp = temp->getNext();
    }

    return subscribed;
}

template<class T>
bool VideoShare::contains(LinkedList<T> *list, Node<T> &n) {
    Node<T> *temp = list->first();
    while (temp) {
        if (temp->getData() == n.getData())
            return true;
        temp = temp->getNext();
    }
    return false;
}
