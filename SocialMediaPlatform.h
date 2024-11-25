#ifndef SOCIAL_MEDIA_PLATFORM_H
#define SOCIAL_MEDIA_PLATFORM_H
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <set>
#include <unistd.h>
#include <windows.h>
#include <algorithm>
#include <limits>
using namespace std;

class User;
class User
{
private:
    string username;
    string password;
    string email;
    string bio;
    bool isPublic;

public:
    User(const string &uname, const string &pwd, const string &email, const string &bio, bool isPublic);
    string getUsername();
    string getEmail();
    string getBio();
    bool isProfilePublic();
    bool validatePassword(const string &pwd);
    void updateBio(const string &newBio);
    void updateEmail(const string &newEmail);
    void updatePassword(const string &newPassword);
    void updatePrivacy(bool newPrivacy);
    void updateUsername(const string &newUsername);
};
class Comment
{
public:
    string content;
    User *author;
    list<Comment> replies;

public:
    Comment(User *author, const string &content) : author(author), content(content) {}
    string getContent() { return content; }
    string getAuthor() { return author->getUsername(); }
    list<Comment> &getReplies() { return replies; }
    void addReply(User *replier, const string &replyContent);
    void displayComment(int level = 0);
};
class MessageNode
{
public:
    User *sender;
    User *receiver;
    string message;
    MessageNode *prev;
    MessageNode *next;
    MessageNode(User *sender, User *receiver, const string &message)
        : sender(sender), receiver(receiver), message(message), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList
{
private:
    MessageNode *head;
    MessageNode *tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    ~DoublyLinkedList()
    {
        MessageNode *current = head;
        while (current)
        {
            MessageNode *nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
    void append(User *sender, User *receiver, const string &message)
    {
        MessageNode *newNode = new MessageNode(sender, receiver, message);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    void display(User *user1, User *user2) const
    {
        MessageNode *current = head;
        bool foundMessages = false;
        while (current)
        {
            if (current->sender == user1 && current->receiver == user2)
            {
                cout << "To " << user2->getUsername() << ": " << current->message << endl;
                foundMessages = true;
            }
            else if (current->sender == user2 && current->receiver == user1)
            {
                cout << "From " << user2->getUsername() << ": " << current->message << endl;
                foundMessages = true;
            }
            current = current->next;
        }
        if (!foundMessages)
        {
            cout << "No messages found between " << user1->getUsername() << " and " << user2->getUsername() << "!" << endl;
        }
    }
    void display2() const
    {
        MessageNode *current = head;
        if (!current)
        {
            cout << "No messages in this group." << endl;
            return;
        }
        while (current)
        {
            cout << "From " << current->sender->getUsername() << ": " << current->message << endl;
            current = current->next;
        }
    }
    MessageNode *getHead() const
    {
        return head;
    }
};

class Group
{
public:
    string groupId;
    string groupName;
    set<User *> participants;
    DoublyLinkedList messageHistory;
    Group() : groupId(""), groupName("") {}
    Group(const string &groupId, const string &groupName)
        : groupId(groupId), groupName(groupName) {}
    void addUser(User *user)
    {
        participants.insert(user);
    }
    void removeUser(User *user)
    {
        participants.erase(user);
    }
    bool isUserInGroup(User *user) const
    {
        return participants.find(user) != participants.end();
    }
    void addMessage(User *sender, const string &message)
    {
        messageHistory.append(sender, nullptr, message);
    }
};

class UserManagement
{
private:
    unordered_map<string, pair<string, User *>> userCredentials;
    list<User *> userProfiles;

public:
    User *signUp();
    User *logIn(const string &username, const string &password);
    void updateUserProfile(User *user, const string &newBio, const string &newEmail);
    void displayProfile(User *user);
    User *findUserByUsername(const string &username);
    void displayAllUsers();
    void editProfile(User *user);
    User *validateUsername(const string &username);
    bool isValidEmail(const string &email);
    friend class FriendSystem;
    friend class MessagingSystem;
};
class PostManagement
{
public:
    map<User *, list<string>> userPosts;
    map<string, vector<Comment *>> postComments;
    void createPost(User *user, const string &content);
    void viewUserPosts(User *user);
    void viewFriendsPosts(User *user, const map<User *, list<User *>> &friends);
    void viewPublicPosts(const map<User *, list<string>> &userPosts, User *currentUser);
    void addComment(User *user, const string &postContent, const string &commentContent);
    void addReplyToComment(User *user, const string &postContent, Comment *parentComment, const string &replyContent);
    void displayPostWithComments(const string &postContent);
    vector<string> getAllPosts();
    void viewPostComments(const string &postContent, User *currentUser);
    void addCommentOrReply(Comment &parentComment, User *currentUser);
    void interactiveCommentSection(User *currentUser);
};
class FriendSystem
{
private:
    map<User *, list<User *>> friends;
    map<User *, list<User *>> pendingRequests;

public:
    map<User *, list<User *>> &getFriendsList();
    void addFriend(User *user, User *friendUser);
    bool viewFriends(User *user);
    void suggestFriendsBFS(User *user);
    void suggestFriendsDFS(User *user);
    void dfs(User *user, map<User *, bool> &visited, map<User *, int> &mutualCount);
    void displayPendingRequests(User *user);
    void removeFriend(User *user1, User *user2);
    void mutualFriendsCount(User *user1, User *user2);
};
class MessagingSystem
{
private:
    map<User *, queue<pair<User *, string>>> userMessages;
    map<User *, DoublyLinkedList> chatHistory;
    map<string, Group> groups;

public:
    void sendMessage(User *fromUser, User *toUser, const string &message);
    void viewNewMessages(User *user);
    void viewChatHistory(User *recipient, User *friendUser);
    void createGroup(User *currentUser, UserManagement &userManagement, FriendSystem &friendSystem, MessagingSystem &messagingSystem);
    bool sendMessageToGroup(User *fromUser, const string &groupId, const string &message);
    void viewGroupChatHistory(const string &groupName, User *currentUser);
    bool addUserToGroup(const string &groupName, User *user);
    bool removeUserFromGroup(const string &groupId, User *user);
    bool isUserInGroup(const string &groupName, User *user);
    const map<string, Group> &getGroups() const
    {
        return groups;
    }
};
#endif