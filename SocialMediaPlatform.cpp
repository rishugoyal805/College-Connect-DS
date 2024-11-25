#include "SocialMediaPlatform.h"
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

User::User(const string &uname, const string &pwd, const string &email, const string &bio, bool isPublic)
    : username(uname), password(pwd), email(email), bio(bio), isPublic(isPublic) {}
string User::getUsername() { return username; }
string User::getEmail() { return email; }
string User::getBio() { return bio; }
bool User::isProfilePublic() { return isPublic; }
bool User::validatePassword(const string &pwd) { return password == pwd; }

void User::updateBio(const string &newBio) { bio = newBio; }
void User::updateEmail(const string &newEmail) { email = newEmail; }
void User::updatePassword(const string &newPassword) { password = newPassword; }
void User::updatePrivacy(bool newPrivacy) { isPublic = newPrivacy; }
void User::updateUsername(const string &newUsername) { username = newUsername; }

User *UserManagement::validateUsername(const string &username)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end())
    {
        return it->second.second;
    }
    return nullptr;
}
bool UserManagement::isValidEmail(const string &email)
{
    int atPos = -1;
    int dotPos = -1;
    for (int i = 0; i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            atPos = i;
        }
        else if (email[i] == '.' && atPos != -1)
        {
            dotPos = i;
        }
    }
    if (atPos > 0 && dotPos > atPos + 1 && dotPos < email.length() - 1)
    {
        return true;
    }
    return false;
}
User *UserManagement::signUp()
{
    string username, password, email, bio;
    bool isPublic;
    while (true)
    {
        cout << "Enter username: ";
        cin >> username;
        if (validateUsername(username) == nullptr)
        {
            break;
        }
        else
        {
            cout << "Username already taken! Please try again." << endl;
        }
    }
    cout << "Enter password: ";
    cin >> password;
    while (true)
    {
        cout << "Enter email: ";
        cin >> email;
        if (isValidEmail(email))
        {
            break;
        }
        else
        {
            cout << "Invalid email format! Please try again." << endl;
        }
    }
    cout << "Enter bio: ";
    cin.ignore();
    getline(cin, bio);
    while (true)
    {
        char choice;
        cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
        {
            isPublic = (choice == 'y' || choice == 'Y') ? 1 : 0;
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
        }
    }
    User *newUser = new User(username, password, email, bio, isPublic);
    userCredentials[username] = {password, newUser};
    userProfiles.push_back(newUser);
    return newUser;
}
User *UserManagement::logIn(const string &username, const string &password)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end() && it->second.first == password)
    {
        return it->second.second;
    }
    return nullptr;
}
void UserManagement::editProfile(User *user)
{
    int choice;
    while (true)
    {
        cout << "Edit Profile Menu:" << endl;
        cout << "1. Change Username" << endl;
        cout << "2. Change Bio" << endl;
        cout << "3. Change Email" << endl;
        cout << "4. Change Password" << endl;
        cout << "5. Change Privacy Settings (Public/Private)" << endl;
        cout << "6. Go Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            string newUsername;
            while (true)
            {
                cout << "Enter new username: ";
                cin >> newUsername;
                if (validateUsername(newUsername) == nullptr)
                {
                    string oldUsername = user->getUsername();
                    userCredentials[newUsername] = userCredentials[oldUsername];
                    userCredentials.erase(oldUsername);
                    user->updateUsername(newUsername);
                    cout << "Username updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Username already taken! Please try again." << endl;
                }
            }
        }
        else if (choice == 2)
        {
            string newBio;
            cout << "Enter new bio: ";
            cin.ignore();
            getline(cin, newBio);
            user->updateBio(newBio);
            cout << "Bio updated successfully!" << endl;
        }
        else if (choice == 3)
        {
            string newEmail;
            while (true)
            {
                cout << "Enter new email: ";
                cin >> newEmail;
                if (isValidEmail(newEmail))
                {
                    user->updateEmail(newEmail);
                    cout << "Email updated successfully!" << endl;
                    break;
                }
                cout << "Invalid email format! Please try again." << endl;
            }
        }
        else if (choice == 4)
        {
            string newPassword, confirmPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            cout << "Confirm new password: ";
            cin >> confirmPassword;
            if (newPassword == confirmPassword)
            {
                user->updatePassword(newPassword);
                userCredentials[user->getUsername()].first = newPassword;
                cout << "Password updated successfully!" << endl;
            }
            else
            {
                cout << "Passwords do not match. Try again." << endl;
            }
        }
        else if (choice == 5)
        {
            bool isPublic;
            while (true)
            {
                char choice;
                cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
                {
                    isPublic = (choice == 'y' || choice == 'Y');
                    user->updatePrivacy(isPublic);
                    cout << "Privacy settings updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
                }
            }
        }
        else if (choice == 6)
        {
            break;
        }
        else
        {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}
void UserManagement::displayProfile(User *user)
{
    cout << "Username: " << user->getUsername() << endl;
    cout << "Email: " << user->getEmail() << endl;
    cout << "Bio: " << user->getBio() << endl;
    cout << "Profile Status: " << (user->isProfilePublic() ? "Public" : "Private") << endl;
}
User *UserManagement::findUserByUsername(const string &username)
{
    auto it = userCredentials.find(username);
    return (it != userCredentials.end()) ? it->second.second : nullptr; // Return user if found
}
void UserManagement::displayAllUsers()
{
    for (User *user : userProfiles)
    {
        cout << user->getUsername() << endl;
    }
}
void PostManagement::createPost(User *user, const string &content)
{
    userPosts[user].push_back(content);
    postComments[content] = {};
    cout << "post created successfully" << endl;
}
void PostManagement::addComment(User *user, const std::string &postContent, const std::string &commentContent)
{
    std::cout << "Adding a new comment by user: " << user->getUsername() << std::endl;
    std::cout << "Post content: " << postContent << std::endl;
    std::cout << "Comment content: " << commentContent << std::endl;
    Comment *newComment = new Comment(user, commentContent);
    postComments[postContent].emplace_back(newComment);
    std::cout << "Comment added successfully to post: " << postContent << std::endl;
}
void PostManagement::viewUserPosts(User *user)
{
    auto it = userPosts.find(user);
    if (it != userPosts.end())
    {
        for (const string &post : it->second)
        {
            cout << post << endl;
            interactiveCommentSection(user);
            while (true)
            {
                cout << "Do you want to add a comment to this post? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y' || choice == 'Y')
                {
                    cin.ignore();
                    cout << "Enter your comment: ";
                    string commentContent;
                    getline(cin, commentContent);
                    addComment(user, post, commentContent);
                }
                else if (choice == 'n' || choice == 'N')
                {
                    cout << "Next post:" << endl
                         << endl;
                    break;
                }
                else
                {
                    cout << "Invalid option. Please enter 'y' or 'n'." << endl;
                }
            }
        }
    }
    else
    {
        cout << "No posts found!" << endl;
    }
}
void PostManagement::viewPostComments(const std::string &postContent, User *currentUser)
{
    auto it = postComments.find(postContent);
    if (it != postComments.end())
    {
        std::cout << "Comments for post: " << postContent << std::endl;
        for (auto *comment : it->second)
        {
            comment->displayComment();
            while (true)
            {
                std::cout << "Do you want to add a reply to this comment? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y')
                {
                    std::cin.ignore();
                    std::cout << "Enter your reply: ";
                    std::string replyContent;
                    std::getline(std::cin, replyContent);
                    comment->addReply(currentUser, replyContent);
                    std::cout << "Reply added successfully!" << std::endl;
                }
                else if (choice == 'n' || choice == 'N')
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
                }
            }
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    else
    {
        std::cout << "No comments found for this post." << std::endl;
    }
}
void PostManagement::viewFriendsPosts(User *user, const map<User *, list<User *>> &friends)
{
    auto it = friends.find(user);
    if (it != friends.end())
    {
        for (User *friendUser : it->second)
        {
            cout << "Posts by " << friendUser->getUsername() << ":" << endl;
            auto friendPosts = userPosts.find(friendUser);
            if (friendPosts != userPosts.end())
            {
                for (const string &post : friendPosts->second)
                {
                    cout << post << endl;
                    interactiveCommentSection(friendUser);
                    while (true)
                    {
                        cout << "Do you want to add a comment to this post? (y/n): ";
                        char choice;
                        cin >> choice;
                        if (choice == 'y' || choice == 'Y')
                        {
                            cin.ignore();
                            cout << "Enter your comment: ";
                            string commentContent;
                            getline(cin, commentContent);
                            addComment(user, post, commentContent);
                        }
                        else if (choice == 'n' || choice == 'N')
                        {
                            break;
                        }
                        else
                        {
                            cout << "Invalid option. Please enter 'y' or 'n'." << endl;
                        }
                    }
                }
            }
            else
            {
                cout << "No posts found for this friend!" << endl;
            }
        }
    }
    else
    {
        cout << "No friends found!" << endl;
    }
}
void PostManagement::viewPublicPosts(const map<User *, list<string>> &userPosts, User *currentUser)
{
    for (const auto &pair : userPosts)
    {
        User *user = pair.first;
        if (user != currentUser && user->isProfilePublic())
        {
            cout << "Posts by " << user->getUsername() << " (Public Profile):" << endl;
            for (const string &post : pair.second)
            {
                cout << post << endl;
                interactiveCommentSection(user);
                while (true)
                {
                    cout << "Do you want to add a comment to this post? (y/n): ";
                    char choice;
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y')
                    {
                        cin.ignore();
                        cout << "Enter your comment: ";
                        string commentContent;
                        getline(cin, commentContent);
                        addComment(currentUser, post, commentContent);
                    }
                    else if (choice == 'n' || choice == 'N')
                    {
                        break;
                    }
                    else
                    {
                        cout << "Invalid option. Please enter 'y' or 'n'." << endl;
                    }
                }
            }
        }
    }
}
void Comment::addReply(User *replier, const string &replyContent)
{
    replies.emplace_back(replier, replyContent);
}
void Comment::displayComment(int level)
{
    for (int i = 0; i < level; i++)
    {
        cout << "  ";
    }
    cout << getAuthor() << ": " << getContent() << endl;
    for (auto &reply : replies)
    {
        reply.displayComment(level + 1);
    }
}
void PostManagement::addCommentOrReply(Comment &parentComment, User *currentUser)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter your comment/reply: ";
    string content;
    getline(cin, content);
    parentComment.addReply(currentUser, content);
    cout << "Reply added successfully!" << endl;
}
void PostManagement::interactiveCommentSection(User *currentUser)
{
    string content;
    cout << "Welcome to the Comment Section!\n";
    User *rootUser = currentUser;
    cout << "Enter the main comment: ";
    cin.ignore();
    getline(cin, content);
    Comment rootComment(rootUser, content);
    while (true)
    {
        int choice;
        cout << "\nOptions:\n";
        cout << "1. Add a reply to the main comment\n";
        cout << "2. Add a reply to an existing reply\n";
        cout << "3. Display all comments\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            addCommentOrReply(rootComment, currentUser);
            break;
        case 2:
        {
            int replyIndex;
            cout << "Enter the index of the reply you'd like to respond to: ";
            cin >> replyIndex;
            auto &replies = rootComment.getReplies();
            if (replyIndex <= 0 || replyIndex > static_cast<int>(replies.size()))
            {
                cout << "Invalid reply index. Please try again.\n";
                break;
            }
            auto it = replies.begin();
            for (int i = 1; i < replyIndex; ++i)
            {
                ++it;
            }
            addCommentOrReply(*it, currentUser);
            break;
        }
        case 3:
            cout << "\nDisplaying all comments:\n";
            rootComment.displayComment();
            break;
        case 4:
            cout << "Exiting the Comment Section. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void FriendSystem::addFriend(User *user, User *friendUser)
{
    if (find(friends[user].begin(), friends[user].end(), friendUser) != friends[user].end())
    {
        cout << friendUser->getUsername() << " is already a friend of " << user->getUsername() << ".\n";
        return;
    }
    friends[user].push_back(friendUser);
    friends[friendUser].push_back(user);
    cout << "Friend added: " << user->getUsername() << " and " << friendUser->getUsername() << " are now friends.\n";
}
bool FriendSystem::viewFriends(User *user)
{
    auto &friendList = friends[user];
    if (friendList.empty())
    {
        cout << user->getUsername() << " has no friends.\n";
        return false;
    }
    cout << "Friends of " << user->getUsername() << ": ";
    for (User *friendUser : friendList)
    {
        cout << friendUser->getUsername() << " ";
    }
    cout << "\n";
    return true;
}
void FriendSystem::suggestFriendsBFS(User *user)
{
    map<User *, bool> visited;
    list<User *> queue;
    visited[user] = true;
    queue.push_back(user);
    cout << "Friend suggestions for " << user->getUsername() << " using BFS:\n";
    while (!queue.empty())
    {
        User *current = queue.front();
        queue.pop_front();
        for (User *friendUser : friends[current])
        {
            if (!visited[friendUser])
            {
                visited[friendUser] = true;
                queue.push_back(friendUser);
                if (find(friends[user].begin(), friends[user].end(), friendUser) == friends[user].end())
                {
                    cout << "Suggested: " << friendUser->getUsername() << endl;
                }
            }
        }
    }
    cout << "\n";
}
void FriendSystem::suggestFriendsDFS(User *user)
{
    map<User *, bool> visited;
    map<User *, int> mutualCount;
    dfs(user, visited, mutualCount);
    cout << "Friend suggestions for " << user->getUsername() << " using DFS:\n";
    for (const auto &entry : mutualCount)
    {
        if (entry.first != user && find(friends[user].begin(), friends[user].end(), entry.first) == friends[user].end())
        {
            cout << entry.first->getUsername() << " (Mutual friends: " << entry.second << ")\n";
        }
    }
}
void FriendSystem::dfs(User *user, map<User *, bool> &visited, map<User *, int> &mutualCount)
{
    visited[user] = true;
    cout << "DFS visiting: " << user->getUsername() << endl;
    for (User *friendUser : friends[user])
    {
        mutualCount[friendUser]++;
        if (!visited[friendUser])
        {
            dfs(friendUser, visited, mutualCount);
        }
    }
}
void FriendSystem::displayPendingRequests(User *user)
{
    auto &requests = pendingRequests[user];
    if (requests.empty())
    {
        cout << "No pending friend requests for " << user->getUsername() << ".\n";
        return;
    }
    cout << "Pending friend requests for " << user->getUsername() << ": ";
    for (User *requester : requests)
    {
        cout << requester->getUsername() << " ";
    }
    cout << "\n";
}
void FriendSystem::removeFriend(User *user1, User *user2)
{
    auto &user1Friends = friends[user1];
    auto &user2Friends = friends[user2];
    user1Friends.remove(user2);
    user2Friends.remove(user1);
    cout << "Friend removed: " << user1->getUsername() << " and " << user2->getUsername() << " are no longer friends.\n";
}
void FriendSystem::mutualFriendsCount(User *user1, User *user2)
{
    int count = 0;
    for (User *friendUser : friends[user1])
    {
        if (find(friends[user2].begin(), friends[user2].end(), friendUser) != friends[user2].end())
        {
            count++;
        }
    }
    cout << "Mutual friends between " << user1->getUsername() << " and " << user2->getUsername() << ": " << count << "\n";
}

map<User *, list<User *>> &FriendSystem::getFriendsList()
{
    return friends;
}

void MessagingSystem::sendMessage(User *fromUser, User *toUser, const string &message)
{
    userMessages[toUser].emplace(fromUser, message);
    chatHistory[fromUser].append(fromUser, toUser, message);
    chatHistory[toUser].append(fromUser, toUser, message);
}
void MessagingSystem::viewNewMessages(User *user)
{
    auto it = userMessages.find(user);
    if (it != userMessages.end() && !it->second.empty())
    {
        cout << "New messages for " << user->getUsername() << ":\n";
        while (!it->second.empty())
        {
            auto messagePair = it->second.front();
            User *sender = messagePair.first;
            string message = messagePair.second;
            cout << "From " << sender->getUsername() << ": " << message << endl;
            it->second.pop();
        }
    }
    else
    {
        cout << "No new messages found for " << user->getUsername() << "!" << endl;
    }
}

void MessagingSystem::viewChatHistory(User *recipient, User *friendUser)
{
    cout << "Chat history between " << recipient->getUsername()
         << " and " << friendUser->getUsername() << ":\n";
    auto recipientIt = chatHistory.find(recipient);
    if (recipientIt != chatHistory.end())
    {
        recipientIt->second.display(recipient, friendUser);
    }
    if (recipientIt == chatHistory.end())
    {
        cout << "No chat history found between " << recipient->getUsername()
             << " and " << friendUser->getUsername() << "!" << endl;
    }
}

void MessagingSystem::createGroup(User *currentUser, UserManagement &userManagement, FriendSystem &friendSystem, MessagingSystem &messagingSystem)
{
    bool uHaveFriend = friendSystem.viewFriends(currentUser);
    if (!uHaveFriend)
    {
        cout << "You need at least one friend to create a group!" << endl;
        return;
    }
    string groupName;
    cout << "\nEnter the group name: ";
    cin.ignore();
    getline(cin, groupName);
    string groupId = "G" + to_string(groups.size() + 1);
    set<User *> participants;
    participants.insert(currentUser);
    char addMore;
    do
    {
        string friendUsername;
        cout << "Enter the username of a friend to add to the group: ";
        cin >> friendUsername;
        User *friendUser = userManagement.findUserByUsername(friendUsername);
        if (friendUser && friendUser != currentUser)
        {
            participants.insert(friendUser);
            cout << friendUsername << " has been added to the group!" << endl;
        }
        else
        {
            cout << "Invalid username or you cannot add yourself!" << endl;
        }
        cout << "Do you want to add another friend? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');
    Group newGroup(groupId, groupName);
    newGroup.participants = participants;
    groups[groupId] = newGroup;
    cout << "Group \"" << groupName << "\" created successfully with Group ID: " << groupId << endl;
}

void sendMessageToGroup(User *currentUser, MessagingSystem &messagingSystem)
{
    string groupName;
    cout << "Enter the Group Name: ";
    cin.ignore();
    getline(cin, groupName);
    string message;
    cout << "Enter your message: ";
    getline(cin, message);
    if (messagingSystem.isUserInGroup(groupName, currentUser))
    {
        if (messagingSystem.sendMessageToGroup(currentUser, groupName, message))
        {
            cout << "Message sent to group \"" << groupName << "\"!" << endl;
        }
        else
        {
            cout << "Failed to send message to group \"" << groupName << "\"." << endl;
        }
    }
    else
    {
        cout << "You are not a member of the group \"" << groupName << "\"!" << endl;
    }
}
bool MessagingSystem::sendMessageToGroup(User *fromUser, const string &groupName, const string &message)
{
    auto it = find_if(groups.begin(), groups.end(), [&](const auto &pair)
                      { return pair.second.groupName == groupName; });
    if (it != groups.end())
    {
        Group &group = it->second;
        if (group.isUserInGroup(fromUser))
        {
            group.addMessage(fromUser, message);
            for (User *user : group.participants)
            {
                if (user != fromUser)
                {
                    userMessages[user].push({fromUser, message});
                }
            }
            return true;
        }
        else
        {
            cout << "You are not a member of the group \"" << groupName << "\"!" << endl;
            return false;
        }
    }
    else
    {
        cout << "Group not found!" << endl;
        return false;
    }
}

bool MessagingSystem::isUserInGroup(const string &groupName, User *user)
{
    auto it = find_if(groups.begin(), groups.end(), [&](const auto &pair)
                      { return pair.second.groupName == groupName; });
    if (it != groups.end())
    {
        Group &group = it->second;
        return group.isUserInGroup(user);
    }
    return false;
}

void MessagingSystem::viewGroupChatHistory(const string &groupName, User *currentUser)
{
    const auto &groups = getGroups();
    auto it = find_if(groups.begin(), groups.end(), [&](const auto &pair)
                      { return pair.second.groupName == groupName; });
    if (it != groups.end())
    {
        const Group &group = it->second;
        if (!group.isUserInGroup(currentUser))
        {
            cout << "You are not a member of the group \"" << group.groupName << "\"!" << endl;
            return;
        }
        cout << "Chat history for group \"" << group.groupName << "\":\n";
        group.messageHistory.display2();
    }
    else
    {
        cout << "Group not found!" << endl;
    }
}

bool MessagingSystem::addUserToGroup(const string &groupName, User *user)
{
    auto it = find_if(groups.begin(), groups.end(), [&](const auto &pair)
                      { return pair.second.groupName == groupName; });
    if (it != groups.end())
    {
        Group &group = it->second;
        if (group.isUserInGroup(user))
        {
            cout << "User is already a member of the group \"" << group.groupName << "\"!" << endl;
            return false;
        }
        group.addUser(user);
        cout << "User \"" << user->getUsername() << "\" has been added to the group \"" << group.groupName << "\"!" << endl;
        return true;
    }
    else
    {
        cout << "Group \"" << groupName << "\" not found!" << endl;
        return false;
    }
}

void joinGroup(User *currentUser, MessagingSystem &messagingSystem)
{
    cout << "Available Groups:\n";
    const auto &groups = messagingSystem.getGroups();
    for (const auto &groupPair : groups)
    {
        const Group &group = groupPair.second;
        cout << "Group Name: " << group.groupName << endl;
    }
    string groupName;
    cout << "Enter the Group Name to join: ";
    cin.ignore();
    getline(cin, groupName);
    if (messagingSystem.addUserToGroup(groupName, currentUser))
    {
        cout << "You have joined the group \"" << groupName << "\"!" << endl;
    }
    else
    {
        cout << "Failed to join the group \"" << groupName << "\". It may not exist." << endl;
    }
}

void leaveGroup(User *currentUser, MessagingSystem &messagingSystem)
{
    string groupName;
    cout << "Enter the Group Name to leave: ";
    cin.ignore();
    getline(cin, groupName);
    const auto &groups = messagingSystem.getGroups();
    auto it = find_if(groups.begin(), groups.end(), [&](const auto &pair)
                      { return pair.second.groupName == groupName; });
    if (it != groups.end())
    {
        const string &groupId = it->first;
        if (messagingSystem.removeUserFromGroup(groupId, currentUser))
        {
            cout << "You have left the group \"" << groupName << "\"!" << endl;
        }
        else
        {
            cout << "You are not a member of the group \"" << groupName << "\"!" << endl;
        }
    }
    else
    {
        cout << "Group not found!" << endl;
    }
}

bool MessagingSystem::removeUserFromGroup(const string &groupId, User *user)
{
    auto it = groups.find(groupId);
    if (it != groups.end())
    {
        Group &group = it->second;
        if (group.isUserInGroup(user))
        {
            group.removeUser(user);
            return true;
        }
    }
    return false;
}

void displayHeader()
{
    cout << "************************************************************" << endl;
    cout << "*                    WELCOME TO OUR                        *" << endl;
    cout << "*                    College Connect                       *" << endl;
    cout << "*                                                          *" << endl;
    cout << "* Submitted to:                             Programmed by: *" << endl;
    cout << "* Sherry Garg                                        Rishu *" << endl;
    cout << "* Sangeeta Mittal                             Swayam Gupta *" << endl;
    cout << "*                                             Maanya Gupta *" << endl;
    cout << "*                                         Shambhavi Mishra *" << endl;
    cout << "*                                                          *" << endl;
    cout << "************************************************************" << "\n"
         << endl;
}
void showMenu()
{
    displayHeader();
    cout << "1. Sign Up" << endl;
    cout << "2. Log In" << endl;
    cout << "3. Exit" << endl;
    cout << endl;
}
void showUserMenu()
{
    displayHeader();
    cout << "1. View Profile" << endl;
    cout << "2. Edit Profile" << endl;
    cout << "3. Create Post" << endl;
    cout << "4. View My Posts" << endl;
    cout << "5. View Friends' Posts" << endl;
    cout << "6. View Public Posts" << endl;
    cout << "7. View all users" << endl;
    cout << "8. Add Friend" << endl;
    cout << "9. Messages" << endl;
    cout << "10. Group Messages" << endl;
    cout << "11. Friends" << endl;
    cout << "12. Log Out" << endl;
    cout << endl;
}
void showFriendMenu()
{
    cout << "1. View Friends" << endl;
    cout << "2. Suggest Friends using BFS" << endl;
    cout << "3. Suggest Friends using DFS" << endl;
    cout << "4. View Pending Friend Requests" << endl;
    cout << "5. Remove a Friend" << endl;
    cout << "6. Count Mutual Friends" << endl;
    cout << endl;
}
int main()
{
    UserManagement userManagement;
    PostManagement postManagement;
    FriendSystem friendSystem;
    MessagingSystem messagingSystem;
    User *currentUser = nullptr;
    while (true)
    {
        showMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            currentUser = userManagement.signUp();
            if (currentUser)
            {
                cout << "Sign Up successful! You can now log in." << endl;
            }
        }
        else if (choice == 2)
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            currentUser = userManagement.logIn(username, password);
            if (currentUser)
            {
                cout << "Log In successful!" << endl;
                while (true)
                {
                    showUserMenu();
                    int userChoice;
                    cout << "Enter your choice: ";
                    cin >> userChoice;
                    if (userChoice == 1)
                    {
                        userManagement.displayProfile(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 2)
                    {
                        userManagement.editProfile(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 3)
                    {
                        string content;
                        cout << "Enter your post: ";
                        cin.ignore();
                        getline(cin, content);
                        postManagement.createPost(currentUser, content);
                        sleep(1);
                    }
                    else if (userChoice == 4)
                    {
                        postManagement.viewUserPosts(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 5)
                    {
                        postManagement.viewFriendsPosts(currentUser, friendSystem.getFriendsList());
                        sleep(1);
                    }
                    else if (userChoice == 6)
                    {
                        postManagement.viewPublicPosts(postManagement.userPosts, currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 7)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                    }
                    else if (userChoice == 8)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                        string friendUsername;
                        cout << "Enter friend's username: ";
                        cin >> friendUsername;
                        User *friendUser = userManagement.findUserByUsername(friendUsername);
                        if (friendUser)
                        {
                            friendSystem.addFriend(currentUser, friendUser);
                            cout << "Friend added!" << endl;
                            sleep(1);
                        }
                        else
                        {
                            cout << "User not found!" << endl;
                            sleep(1);
                        }
                    }
                    else if (userChoice == 9)
                    {
                        int messageChoice;
                        string recipientUsername;
                        string message;
                        User *recipient = nullptr;
                        bool uHaveFriend = false;
                        do
                        {
                            cout << "Messaging Menu:\n";
                            cout << "1. View New Messages\n";
                            cout << "2. Send Message\n";
                            cout << "3. View Chat History\n";
                            cout << "0. Go Back\n";
                            cout << "Enter your choice: ";
                            cin >> messageChoice;
                            switch (messageChoice)
                            {
                            case 1:
                                messagingSystem.viewNewMessages(currentUser);
                                sleep(1);
                                break;

                            case 2:
                                uHaveFriend = friendSystem.viewFriends(currentUser);
                                if (uHaveFriend)
                                {
                                    cout << "Enter recipient's username: ";
                                    cin >> recipientUsername;
                                    recipient = userManagement.findUserByUsername(recipientUsername);
                                    if (recipient)
                                    {
                                        cout << "Enter your message: ";
                                        cin.ignore();
                                        getline(cin, message);
                                        messagingSystem.sendMessage(currentUser, recipient, message);
                                        cout << "Message sent!" << endl;
                                    }
                                    else
                                    {
                                        cout << "User not found!" << endl;
                                    }
                                }
                                else
                                {
                                    cout << "No friends found!" << endl;
                                    cout << "You can only message your Friends!" << endl;
                                }
                                sleep(1);
                                break;
                            case 3:
                                uHaveFriend = friendSystem.viewFriends(currentUser);
                                if (uHaveFriend)
                                {
                                    string friendUsername;
                                    cout << "Enter Friend's username: ";
                                    cin >> friendUsername;
                                    User *friendUser = userManagement.findUserByUsername(friendUsername);
                                    if (friendUser)
                                    {
                                        messagingSystem.viewChatHistory(currentUser, friendUser);
                                    }
                                    else
                                    {
                                        cout << "Friend not found!" << endl;
                                    }
                                }
                                else
                                {
                                    cout << "You have no friends to view chat history with!" << endl;
                                }
                                sleep(1);
                                break;
                            case 0:
                                cout << "Exiting messaging menu." << endl;
                                break;
                            default:
                                cout << "Invalid choice! Please try again." << endl;
                                break;
                            }
                        } while (messageChoice != 0);
                    }
                    else if (userChoice == 10)
                    {
                        int groupChoice;
                        do
                        {
                            cout << "Group Messaging Menu:\n";
                            cout << "1. Create Group\n";
                            cout << "2. Send Message to Group\n";
                            cout << "3. View Group Chat History\n";
                            cout << "4. Join Group\n";
                            cout << "5. Leave Group\n";
                            cout << "0. Go Back\n";
                            cout << "Enter your choice: ";
                            cin >> groupChoice;
                            switch (groupChoice)
                            {
                            case 1:
                                messagingSystem.createGroup(currentUser, userManagement, friendSystem, messagingSystem);
                                break;
                            case 2:
                                sendMessageToGroup(currentUser, messagingSystem);
                                break;
                            case 3:
                            {
                                string groupName;
                                cout << "Enter the Group Name to view chat history: ";
                                cin.ignore();
                                getline(cin, groupName);
                                messagingSystem.viewGroupChatHistory(groupName, currentUser);
                                break;
                            }
                            case 4:
                                joinGroup(currentUser, messagingSystem);
                                break;
                            case 5:
                                leaveGroup(currentUser, messagingSystem);
                                break;
                            case 0:
                                cout << "Exiting group messaging menu." << endl;
                                break;
                            default:
                                cout << "Invalid choice! Please try again." << endl;
                                break;
                            }
                        } while (groupChoice != 0);
                    }
                    else if (userChoice == 11)
                    {
                        showFriendMenu();
                        int choice;
                        cin >> choice;
                        if (choice == 1)
                        {
                            if (friendSystem.viewFriends(currentUser))
                            {
                                cout << "Friends displayed successfully!" << endl;
                                sleep(1);
                            }
                        }
                        else if (choice == 2)
                        {
                            friendSystem.suggestFriendsBFS(currentUser);
                            sleep(1);
                        }
                        else if (choice == 3)
                        {
                            friendSystem.suggestFriendsDFS(currentUser);
                            sleep(1);
                        }
                        else if (choice == 4)
                        {
                            friendSystem.displayPendingRequests(currentUser);
                            sleep(1);
                        }
                        else if (choice == 5)
                        {
                            string removeFriendUsername;
                            cout << "Enter the username of the friend you want to remove: ";
                            cin >> removeFriendUsername;
                            User *removeFriend = userManagement.findUserByUsername(removeFriendUsername);
                            if (removeFriend)
                            {
                                friendSystem.removeFriend(currentUser, removeFriend);
                                cout << "Friend removed!" << endl;
                                sleep(2);
                            }
                            else
                            {
                                cout << "User not found!" << endl;
                                sleep(1);
                            }
                        }
                        else if (choice == 6)
                        {
                            string mutualFriendUsername;
                            cout << "Enter the username to find mutual friends with: ";
                            cin >> mutualFriendUsername;
                            User *mutualFriendUser = userManagement.findUserByUsername(mutualFriendUsername);
                            if (mutualFriendUser)
                            {
                                friendSystem.mutualFriendsCount(currentUser, mutualFriendUser);
                                sleep(1);
                            }
                            else
                            {
                                cout << "User not found!" << endl;
                                sleep(1);
                            }
                        }
                    }
                    else if (userChoice == 12)
                    {
                        currentUser = nullptr;
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid username or password!" << endl;
            }
        }
        else if (choice == 3)
        {
            break;
        }
    }
    return 0;
}