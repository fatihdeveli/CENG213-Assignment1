#include "hw3.h"


using namespace std;

AccessControl::AccessControl(int table1Size, int table2Size) {
    regUsers.resize((unsigned long) table1Size, pair<string, string>(EMPTY, EMPTY));
    regUserNum = 0;
    activeUsers.resize((unsigned long) table2Size, EMPTY);
    activeUserNum = 0;
}

AccessControl::~AccessControl() {}

int AccessControl::addUser(string username, string pass) {
    // Adds a user to regUsers list if there is no entry with the same username.

    if (isRegistered(username)) return 0; // User already registered.

    insert(username, pass); // Insert the new user

    return 1;
}

int AccessControl::addUsers(string filePath) {
    int count = 0; // Number of users successfully added
    ifstream file;
    file.open(filePath.c_str());
    string username, password;
    while (file >> username >> password) {
        count += addUser(username, password);
    }
    file.close();
    return count;
}

int AccessControl::delUser(string username, vector<string> &oldPasswords) {
    // Delete the user with the given username, if exists.

    if (!isRegistered(username)) return 0;

    int size = (int) regUsers.size();
    for (int i = 0; i < size; i++) { // Find all entries with different passwords.
        int hash = hashFunction(username, size, i);
        if (regUsers[hash].first == username) {
            oldPasswords.push_back(regUsers[hash].second);
            regUsers[hash].first = DELETED;
            regUsers[hash].second = DELETED;
        }
        else if (regUsers[hash].first == EMPTY) { // There are no more entries.
            break;
        }
        // Else, found another user, increment i and continue.
    }
    regUserNum--;
    return 1;
}

int AccessControl::changePass(string username, string oldpass,
                              string newpass) {
    // User must be registered.
    // Given new password must not be the same as old password.
    if (!isRegistered(username, oldpass) || newpass == oldpass) return 0;

    insert(username, newpass); // Insert an entry with the new password.
    return 1;
}

int AccessControl::login(string username, string pass) {
    if (!isRegistered(username, pass)) return 0; // User must be registered.

    // User must not be already logged in.
    int size = (int) activeUsers.size();
    for (int i = 0; i < size; i++) {
        int hash = hashFunction(username, size, i);
        if (activeUsers[hash] == username)
            return 0; // Found the user in the table, already logged in.
        if (regUsers[hash].first == EMPTY)
            break; // There is no such record.
        // If the hashed area is deleted or there is another record, increment
        // the hash level and search again.
    }

    // Entry exists, make sure given password is not an old one.
    for (int i = 0;;i++) {
        int hash = hashFunction(username, (int) regUsers.size(), i);
        if (regUsers[hash].first == username && regUsers[hash].second == pass) {
            // Found the entry, make sure it is the most recent password.
            int nextHash = hashFunction(username, (int) regUsers.size(), i+1);
            if (regUsers[nextHash].first == username && regUsers[nextHash].second != pass)
                return 0; // It is not the most recent password.
            break; // Else, user can login.
        }
    }

    // Add the entry
    for (int i = 0; i < (int) activeUsers.size(); i++) {
        int hash = hashFunction(username, (int) activeUsers.size(), i);
        if (activeUsers[hash] == EMPTY || activeUsers[hash] == DELETED) {
            activeUsers[hash] = username;
            break;
        }
    }
    activeUserNum++;

    // Check if the load factor exceeded.
    if ((double)(activeUserNum) / activeUsers.size() > MAX_LOAD_FACTOR) {
        // Resize the table and insert the existing elements again

        // Find the new size, the first prime larger than 2*size
        unsigned long oldSize = activeUsers.size();
        unsigned long newSize = 2 * oldSize + 1;
        while(true) {
            bool isPrime = true;
            for (int i = 2; i <= (int) newSize/2; i++) {
                if (newSize % i == 0)
                    isPrime = false;
            }
            if (isPrime)
                break;
            else
                newSize++;
        }

        // Create a new table to reinsert the old entries, with the new size.
        vector<string> newTable(newSize, EMPTY);
        // Keep track of users moved to the new table.
        vector<string> checkedUsers;

        for (int i = 0; i < (int) oldSize; i++) { // Go through the old table
            string temp = activeUsers[i];
            if (temp != EMPTY && temp != DELETED && !find(checkedUsers, temp)) {
                // If the entry at the current position is not EMPTY, not DELETED, and it
                // was not processed yet, find all entries with username "temp" and
                // rehash them to the new table.

                for (int j = 0, k = 0; ;j++) {
                    // j: hash variable for old table, k: hash variable for new table
                    int oldHash = hashFunction(temp, (int) oldSize, j);

                    if (activeUsers[oldHash] == temp) { // Found an entry to rehash
                        int newHash;
                        do { // Find the available position on the new table.
                            newHash = hashFunction(temp, (int) newSize, k++);
                        } while (newTable[newHash] != EMPTY);
                        newTable[newHash] = activeUsers[oldHash];
                    }
                    else if (activeUsers[oldHash] == EMPTY) {
                        break; // There are no more entries with username "temp"
                    } // Else, some other entry was found, continue.
                }
                // Rehashed all entries of the user, entries with its username should
                // not be processed again.
                checkedUsers.push_back(temp);
            }
        }
        activeUsers = newTable; // Replace the old table with the new one.
    }
    return 1;
}

int AccessControl::logout(string username) {
    int size = (int) activeUsers.size();
    for (int i = 0; i < size; i++) {
        int hash = hashFunction(username, size, i);
        if (activeUsers[hash] == EMPTY)
            return 0;
        if (activeUsers[hash] == username) {
            activeUsers[hash] = DELETED;
            activeUserNum--;
            return 1;
        }
        // If the field is deleted or there is any other entry, increment i and calculate
        // hash again.
    }
    return 0;
}

float AccessControl::printActiveUsers() {
    int size = (int) activeUsers.size();
    for (int i = 0; i < size; i++)
        cout << activeUsers[i] << endl;
    return (float) activeUserNum/size;
}

float AccessControl::printPasswords() {
    int size = (int) regUsers.size();
    for (int i = 0; i < size; i++)
        cout << regUsers[i].first << " " << regUsers[i].second << endl;
    return (float) regUserNum/size;
}

void AccessControl::insert(string username, string password) {
    // Inserts a record to regUsers table. If necessary, increases the table size and
    // rehashes the old entries.

    // Insert the element
    for (int i = 0;;i++) {
        int hash = hashFunction(username, (int) regUsers.size(), i);
        //cout << "found hash " << hash << endl;
        if (regUsers[hash].first == EMPTY || regUsers[hash].first == DELETED) {
            regUsers[hash] = pair<string, string>(username, password);
            break;
        }
    }

    // Check if the load factor exceeded.
    if ((double)(regUserNum+1) / regUsers.size() > MAX_LOAD_FACTOR) {
        // Resize the table and insert the existing elements again

        // Find the new size, the first prime larger than 2*size
        unsigned long oldSize = regUsers.size();
        unsigned long newSize = 2 * oldSize + 1;
        while(true) {
            bool isPrime = true;
            for (int i = 2; i <= (int) newSize/2; i++) {
                if (newSize % i == 0)
                    isPrime = false;
            }
            if (isPrime)
                break;
            else
                newSize++;
        }

        // Create a new table to reinsert the old entries
        vector< pair<string, string> > newTable(newSize, pair<string, string>(EMPTY, EMPTY));
        // Keep track of users moved to the new table.
        vector<string> checkedUsers;

        for (int i = 0; i < (int) oldSize; i++) { // Go through the old table
            string temp = regUsers[i].first;
            if (temp != EMPTY && temp != DELETED && !find(checkedUsers, temp)) {
                // Find all entries with username "temp" and rehash them to the new table.

                for (int j = 0, k = 0; j < (int) regUsers.size() ;j++) {
                    int oldHash = hashFunction(temp, (int) oldSize, j);

                    if (regUsers[oldHash].first == temp) { // Found an entry to rehash
                        int newHash;
                        do { // Find the available position on the new table.
                            newHash = hashFunction(temp, (int) newSize, k++);
                        } while (newTable[newHash].first != EMPTY);
                        newTable[newHash] = regUsers[oldHash];
                    } else if (regUsers[oldHash].first == EMPTY) {
                        // There are no more entries with username "temp"
                        break;
                    } // Else, some other entry was found, continue.
                }
                checkedUsers.push_back(temp); // Rehashed all entries of the user.
            }
        }
        regUsers = newTable; // Replace the old table with the new one.
    }
    regUserNum++;
}


bool AccessControl::isRegistered(string username) {
    // Checks if the user with given username exists in the hash table regUsers.
    int size = (int) regUsers.size();
    for (int i = 0; i < size; i++) {
        int hash = hashFunction(username, size, i);
        if (regUsers[hash].first == username)
            return true; // Found
        if (regUsers[hash].first == EMPTY)
            break;
        // If the hashed area is deleted or there is another record, increment
        // the hash level and search again.
    }
    return false;
}

bool AccessControl::isRegistered(std::string username, std::string password) {
    // Checks if an entry with given username AND password exists in the hash table regUsers.
    int size = (int) regUsers.size();
    for (int i = 0; i < size; i++) {
        int hash = hashFunction(username, size, i);
        if (regUsers[hash].first == username && regUsers[hash].second == password)
            return true; // Found
        if (regUsers[hash].first == EMPTY)
            break;
        // If the hashed area is deleted or there is another record, increment
        // the hash level and search again.
    }
    return false;
}

// Check for existence of an element in a vector<string>.
bool AccessControl::find(vector<string> &list, string &element) {
    long size = list.size();
    for (int i = 0; i < size; i++) {
        if (list[i] == element)
            return true;
    }
    return false;
}
