#include <iostream>
#include "User.hpp"
#include "Video.hpp"

void User::printSubscriptions() {
    Node< Video*> * first = subscriptions.first();
    while (first) {
        cout << *(first->getData());
        first = first->getNext();
    }
    cout << std::endl;
}

void User::printFriends() {
    Node< User*> * first = friends.first();
    while (first) {
        cout << *(first->getData());
        first = first->getNext();
    }
    cout << std::endl;
}

ostream& operator<<(ostream& out, const User & user) {
    string st = (user.status == ACTIVE) ? "active" : "suspended";
    out << "username:" << user.username << ",name:" << user.name << ",surname:" << user.surname << ",Status:" << st << endl;
    return out;
}

/* TO-DO: method implementations below */

User::User() {
    status = ACTIVE;
}

User::User(string username, string name, string surname) {
    this->username = username;
    this->name = name;
    this->surname = surname;
}

User::~User() {

}

const string &User::getUsername() const {
    return username;
}

const string &User::getName() const {
    return name;
}

const string &User::getSurname() const {
    return surname;
}

Status User::getStatus() const {
    return status;
}

void User::updateStatus(Status st) {
    status = st;
}

void User::subscribe(Video *video) {

}

void User::unSubscribe(Video *video) {

}

void User::addFriend(User *user) {

}

void User::removeFriend(User *user) {

}

LinkedList<Video *> *User::getSubscriptions() {
    return NULL;
}

LinkedList<User *> *User::getFriends() {
    return NULL;
}

bool User::operator==(const User &rhs) const {
    return false;
}
