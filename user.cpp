#include "user.hpp"


// Location management
void User::subscribeTo(const std::string& location) {
    if (!isSubscribed(location)) {
        subscribedLocations.push_back(location);
    }
}

void User::unsubscribeFrom(const std::string& location) {
    auto it = std::find(subscribedLocations.begin(), subscribedLocations.end(), location);
    if (it != subscribedLocations.end()) {
        subscribedLocations.erase(it);
    }
}

bool User::isSubscribed(const std::string& location) const {
    return std::find(subscribedLocations.begin(), subscribedLocations.end(), location) != subscribedLocations.end();
}

std::string User::listSubscribedLocations() const {
    if (subscribedLocations.empty()) return "You are not subscribed to any locations.\n";
    std::string result = "Subscribed locations:\n";
    for (const auto& loc : subscribedLocations)
        result += "- " + loc + "\n";
    return result;
}